//
// Created by phoenixflower on 7/24/20.
//

#ifndef LANDESSDEVCORE_SYSTEMLOAD_HPP
#define LANDESSDEVCORE_SYSTEMLOAD_HPP
#include "Definitions/Integer.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "Examples/ReflectionDemoTypes.h"
#include "RowBackingStore.h"
#include "Async/Thread.h"
#include "BasicDelimeterSeperatedFile.h"
namespace LD
{

    namespace Detail
    {
        template<typename T>
        struct GenerateSystemLoadRepresentation
        {

        };

        template<typename T>
        struct GenerateCoreLayout
        {

        };

        template<LD::UInteger ... Indices>
        struct GenerateSystemLoadRepresentation<LD::IntegerSequence<LD::UInteger ,Indices...>>
        {
            using type = LD::CT::TypeList<LD::CPUPackageMetric,LD::ContextSwitchMetric,LD::InterruptServiceMetric,LD::BootUpTimeMetric,LD::ProcessesMetric,LD::ProcessesRunningMetric,LD::ProcessesBlockedMetric,LD::SoftIRQMetric,LD::CPUCoreMetric<Indices>...>;
            //using type = LD::CT::TypeList<LD::CPUPackageMetric,LD::ContextSwitchMetric,LD::InterruptServiceMetric,LD::BootUpTimeMetric,LD::ProcessesMetric,LD::ProcessesRunningMetric,LD::ProcessesBlockedMetric,LD::SoftIRQMetric,LD::CPUCoreMetric<Indices>...>;
        };

        template<LD::UInteger ... Indices>
        struct GenerateCoreLayout<LD::IntegerSequence<LD::UInteger ,Indices...>>
        {
            using type = LD::CT::TypeList<LD::CPUCoreMetric<Indices>...>;
            //using type = LD::CT::TypeList<LD::CPUPackageMetric,LD::ContextSwitchMetric,LD::InterruptServiceMetric,LD::BootUpTimeMetric,LD::ProcessesMetric,LD::ProcessesRunningMetric,LD::ProcessesBlockedMetric,LD::SoftIRQMetric,LD::CPUCoreMetric<Indices>...>;
        };
    }
    template<LD::UInteger NumberOfCores>
    using GenerateSystemRepresentation = typename LD::Detail::GenerateSystemLoadRepresentation<LD::MakeIndexSequence_T<10>>::type;
    template<LD::UInteger NumberOfCores>
    using GenerateCoreLayout = typename LD::Detail::GenerateCoreLayout<LD::MakeIndexSequence_T<NumberOfCores>>::type;
}

namespace LD
{
    template<LD::UInteger MaxCoreCount, typename Obj ,auto CoreCount, class = void>
    class BasicSystemLoad;


    template<LD::UInteger MaxCoreCount, typename Obj, auto CoreCount>
    class BasicSystemLoad<MaxCoreCount,Obj,CoreCount,LD::Enable_If_T<
            LD::Require<
                    true
            >>>
    {
    public:
        //using SystemLayout = LD::GenerateSystemRepresentation<MaxCoreCount>;
        using MetricGlob = LD::Tuple<LD::UInteger,LD::UInteger,LD::Float>;
        using SpannedMetricGlob = LD::Span<MetricGlob>;
    private:
        //index of always contains the self-reported package metrics from the underlying kernel
        //indices [1,1+Concurrency) contains the metrics from each core also reported by the kernel
        LD::StaticArray<MetricGlob ,MaxCoreCount+1> mCores;
        //MetricGlob mCores[MaxCoreCount];
        LD::RowBackingStore backingStore{"/proc/stat"};
    public:
        BasicSystemLoad() noexcept
        {
            //this->mBackingStore = LD::RowBackingStore{"/proc/stat"};
            for(LD::UInteger n = 0;n<(MaxCoreCount);++n)
            {
                this->mCores[n] = {0,0,0.0};
            }
            //(*this)();
        }

        /**
         * @brief On Linux, this function should only be called be called no more frequently than every 1s, as this is when the kernel
         * will update the appriopriate files to determine the load of the CPU
         */
        void operator()() noexcept
        {

            using SystemLayout = LD::GenerateSystemRepresentation<16>;
            using MetricGlob = LD::Tuple<LD::UInteger,LD::UInteger,LD::Float>;
            using SpannedMetricGlob = LD::Span<MetricGlob>;

            LD::SpaceSpeerateValueFile<LD::RowBackingStore> file{backingStore};
            LD::UInteger threadCount = LD::Thread::GetHardwareConcurrency();
            //MetricGlob cores[16];
            //for(LD::UInteger n = 0;n<1;++n)
            //{
            //cores[n] = {0,0,0.0};
            //}
            SpannedMetricGlob coreDeltasSpan{this->mCores.GetData(),threadCount+1};
            auto onError = [](const LD::Context<LD::TransactionError,SpannedMetricGlob> & ) noexcept
            {
                //std::cout << "an error occured " << std::endl;

            };
            auto onCPUPackageMetric = [](const LD::Context<LD::TransactionResult,LD::CPUPackageMetric,SpannedMetricGlob> & transaction) noexcept
            {

                SpannedMetricGlob & delta = LD::Get(LD::Get<2>(transaction));
                //the package info will always be stored in the first index
                LD::UInteger & previous_idle_time = LD::Get<0>(delta[0]);
                LD::UInteger & previous_total_time = LD::Get<1>(delta[0]);
                LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(transaction));
                LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();
                const LD::UInteger  idle_time = metric.Idle();
                const double idle_time_delta = idle_time - previous_idle_time;
                const double total_time_delta = total_time - previous_total_time;

                if (total_time_delta != 0)
                {
                    LD::Get<2>(delta[0]) = (100.0 * (1.0 - idle_time_delta / total_time_delta));

                    LD::Get<0>(delta[0]) = idle_time;
                    LD::Get<1>(delta[0]) = total_time;
                }

            };
            auto onCPUCoreMetric = [](auto  && stuff) noexcept -> LD::Enable_If_T<LD::Require<LD::IsCPUCore<LD::GetType<decltype(LD::GetQueryType(stuff))>>>,void>
            {
                auto & metric = LD::Get(LD::Get<1>(LD::Forward<decltype(stuff)>(stuff)));
                LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

                SpannedMetricGlob & delta = LD::Get(LD::Get<2>(LD::Forward<decltype(stuff)>(stuff)));
                LD::UInteger & previous_idle_time = LD::Get<0>(delta[metric.Core()+1]);
                LD::UInteger & previous_total_time = LD::Get<1>(delta[metric.Core()+1]);


                const LD::UInteger  idle_time = metric.Idle();
                const LD::UInteger idle_time_delta = idle_time - previous_idle_time;
                const LD::UInteger total_time_delta = total_time - previous_total_time;

                if (total_time_delta != 0)
                {
                    LD::Get<2>(delta[metric.Core()+1]) = (100.0 * (1.0 - double(idle_time_delta) / double(total_time_delta)));

                    LD::Get<0>(delta[metric.Core()+1]) = idle_time;
                    LD::Get<1>(delta[metric.Core()+1]) = total_time;
                }

            };

            auto onContexSwitchMetric = [](const LD::Context<LD::TransactionResult,LD::ContextSwitchMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onInteruptServiceMetric = [](const LD::Context<LD::TransactionResult,LD::InterruptServiceMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onBootUpTimeMetric = [](const LD::Context<LD::TransactionResult,LD::BootUpTimeMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onProcessMetric = [](const LD::Context<LD::TransactionResult,LD::ProcessesMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onProcessesRunningMetric = [](const LD::Context<LD::TransactionResult,LD::ProcessesRunningMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onProcessesBlockedMetric = [](const LD::Context<LD::TransactionResult,LD::ProcessesBlockedMetric,SpannedMetricGlob> & transaction) noexcept
            {

            };

            auto onSoftIRQMetric = [](const LD::Context<LD::TransactionResult,LD::SoftIRQMetric,SpannedMetricGlob> & transaction) noexcept
            {
            };

            auto end = file.End(SystemLayout{},SpannedMetricGlob {coreDeltasSpan});
            for(auto it = file.Begin(SystemLayout{},SpannedMetricGlob{coreDeltasSpan});it!=end;++it)
            {
                LD::Match(*it,
                          onError,
                          onCPUPackageMetric,
                          onCPUCoreMetric,
                          onContexSwitchMetric,
                          onInteruptServiceMetric,
                          onBootUpTimeMetric,
                          onProcessMetric,
                          onProcessesRunningMetric,
                          onProcessesBlockedMetric,
                          onSoftIRQMetric);
            }
        }

        LD::Float Load(const LD::Variant<LD::TransactionError,LD::UInteger> & core = {}) noexcept
        {

            auto onPackageRequest = [](const LD::TransactionError & ) noexcept
            {
                return LD::Pair<bool,LD::UInteger>{false,0};
            };

            auto onCoreRequest = [](const LD::UInteger & coreNumber) noexcept
            {
                return LD::Pair<bool,LD::UInteger>{true,coreNumber};
            };

            LD::Pair<bool,LD::UInteger> requestPredicate = LD::Match(core,onPackageRequest,onCoreRequest);

            LD::QueryResult<LD::UInteger(LD::StaticArray<MetricGlob ,MaxCoreCount+1>&)> generatePredicates[2];
            generatePredicates[0] = LD::MakeContext(LD::TransactionError{},this->mCores);
            generatePredicates[1] = LD::MakeContext(LD::TransactionResult{},LD::UInteger {requestPredicate.GetSecond()},this->mCores);

            auto onPackage = [](const LD::Context<LD::TransactionError,LD::StaticArray<MetricGlob ,MaxCoreCount+1>&> & context) noexcept
            {
                LD::StaticArray<MetricGlob,MaxCoreCount+1> & metrics = LD::Get(LD::Get<1>(context));
                LD::Float loadOnPackage = LD::Get(LD::Get<2>(metrics[0]));
                return loadOnPackage;
            };

            auto onCore = [](const LD::Context<LD::TransactionResult,LD::UInteger,LD::StaticArray<MetricGlob ,MaxCoreCount+1>&> & context) noexcept
            {
                LD::StaticArray<MetricGlob,MaxCoreCount+1> & metrics = LD::Get(LD::Get<2>(context));
                const LD::UInteger & coreNumber = LD::Get(LD::Get<1>(context));
                LD::Float loadOnCore = LD::Get(LD::Get<2>(metrics[coreNumber+1]));
                return loadOnCore;
            };
            return LD::Match(generatePredicates[requestPredicate.GetFirst()],onPackage,onCore);
        }
    };


    template<LD::UInteger MaxCoreCount>
    using SystemLoad = LD::BasicSystemLoad<MaxCoreCount,void,&LD::Thread::GetHardwareConcurrency>;

}
#endif //LANDESSDEVCORE_SYSTEMLOAD_HPP
