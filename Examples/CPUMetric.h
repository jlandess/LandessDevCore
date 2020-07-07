//
// Created by phoenixflower on 7/3/20.
//

#ifndef LANDESSDEVCORE_CPUMETRIC_H
#define LANDESSDEVCORE_CPUMETRIC_H
#include "Definitions/Integer.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "ReflectionDemoTypes.h"
#include "Async/Thread.h"
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

    class CPUMetric
    {
    private:
        using CoreDefinition = LD::Rebind<LD::GenerateCoreLayout<64>,LD::Variant>;
        LD::CPUPackageMetric mPackageMetric;
        LD::StaticArray<CoreDefinition,2> mCore;
        //LD::Rebind<LD::GenerateCoreLayout<64>,LD::Variant> mCore;
        LD::UInteger mThreadCount;
    public:
        inline CPUMetric() noexcept:mThreadCount{0}
        {
            this->mThreadCount = LD::Thread::GetHardwareConcurrency();
        }

        constexpr LD::Float operator[](const LD::UInteger & core) noexcept
        {
            LD::UInteger index = core%2;

            return {};
        }
    };
}


#endif //LANDESSDEVCORE_CPUMETRIC_H
