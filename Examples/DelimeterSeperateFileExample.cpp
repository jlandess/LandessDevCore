//
// Created by phoenixflower on 6/27/20.
//
#include "DelimeterSeperatedFileExample.h"
#include "TypeTraits/IntegerSequence.hpp"
#include "ReflectionDemoTypes.h"
#include "IO/RowBackingStore.h"
#include "IO/BasicDelimeterSeperatedFile.h"
#include <iostream>
#include <unistd.h>
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
namespace Example
{
    void DelimeterSeperatedFileExample()
    {

        using ObjectPack = LD::GenerateSystemRepresentation<16>;
        using ObjectPackVariant = LD::Rebind<ObjectPack,LD::Variant>;

        using Type = LD::GenerateSystemRepresentation<10>;
        //LD::GenerateSystemRepresentation<8>{};
        using stuffings = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),LD::Pyramid>;



        LD::CPUCoreMetric<7>::GetClassNameTypeString();


        LD::RowBackingStore backingStore{"/proc/stat"};
        LD::SpaceSpeerateValueFile<LD::RowBackingStore> file{backingStore};

        LD::UInteger previous_idle_time=0, previous_total_time=0;
        LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> it{previous_idle_time,previous_total_time};// = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{},previous_idle_time,previous_total_time);


        LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> end = file.End(ObjectPack {},previous_idle_time,previous_total_time);




        for (LD::UInteger i = 0; i < 10; ++i)
        {
            for (auto it = file.Begin(ObjectPack {},previous_idle_time,previous_total_time); it != end; ++it)
            {

                auto onCPUPackageMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::CPUPackageMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {

                    std::cout << "found package " << std::endl;
                    LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(context));
                    LD::UInteger & previous_idle_time = LD::Get(LD::Get<2>(context));
                    LD::UInteger & previous_total_time = LD::Get(LD::Get<3>(context));
                    LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

                    //std::cout << "Previous Idle Time = " << previous_idle_time << std::endl;
                    //std::cout << "Previous Total Time = " << previous_idle_time << std::endl;
                    const LD::UInteger idle_time = metric.Idle();
                    //std::cout << "Idle Time : " << idle_time << std::endl;
                    const double idle_time_delta = idle_time - previous_idle_time;
                    const double total_time_delta = total_time - previous_total_time;
                    const double utilization = (idle_time_delta!=total_time_delta)*(100.0 * (1.0 - idle_time_delta / total_time_delta));
                    std::cout << "Utilizaiton : " <<  utilization << std::endl;
                    //std::cout << "Sum : " << total_time << std::endl;
                    LD::Float idlePercentage = (metric.Idle()*100)/total_time;
                    //std::cout << "Usage: " << ((sum-metric.Idle())*100.0)/sum << std::endl;

                    previous_idle_time = idle_time;
                    previous_total_time = total_time;
                };

                //called when we find an object that matches LD::CPUCoreMetric<LD::UInteger>
                auto onCPUCoreMetric = [](auto  && stuff) noexcept -> LD::Enable_If_T<LD::Require<LD::IsCPUCore<LD::GetType<decltype(LD::GetQueryType(stuff))>>>,void>
                {
                    using CoreType = decltype(LD::GetQueryType(stuff));
                    using Underlyingcore = LD::GetType<CoreType >;
                    constexpr static const LD::UInteger Core = Underlyingcore::Core;
                    std::cout << "found core " << Core <<  std::endl;

                    /*
                    if constexpr(LD::IsCPUCore<Type>)
                    {
                        using CoreType = decltype(LD::GetQueryType(stuff));
                        using Underlyingcore = typename CoreType::type;
                        constexpr static const LD::UInteger Core = Underlyingcore::Core;
                        std::cout << "found core " << Core <<  std::endl;
                    }
                     */

                };

                //called when we get a LD::CPUPackageMetric Object
                auto onContextSwitchMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ContextSwitchMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                auto onInterruptServiceMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::InterruptServiceMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                auto onBootUpTimeMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::BootUpTimeMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                auto onProcessMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                auto onProcessRunningMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesRunningMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                auto onProcessBlockedMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesBlockedMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                //LD::SoftIRQMetric

                auto onSoftIRQMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::SoftIRQMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
                {


                };

                //called if we get a Database Error, such as getting a row with no associated objectyghtytynonneaew WQQq
                auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::UInteger&,LD::UInteger&> & context) noexcept
                {

                    //std::cout << "found core 0 " << std::endl;
                };


                LD::Match(*it,
                          onCPUPackageMetric,
                          onDatabaseError,
                          onCPUCoreMetric,
                          onContextSwitchMetric,
                          onInterruptServiceMetric,
                          onBootUpTimeMetric,
                          onProcessMetric,
                          onProcessRunningMetric,
                          onProcessBlockedMetric,
                          onSoftIRQMetric);
                /*
                auto onCPUPackageMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::CPUPackageMetric,LD::UInteger&,LD::UInteger&> & context)
                {

                    std::cout << "found package " << std::endl;
                    LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(context));
                    LD::UInteger & previous_idle_time = LD::Get(LD::Get<2>(context));
                    LD::UInteger & previous_total_time = LD::Get(LD::Get<3>(context));
                    LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

                    //std::cout << "Previous Idle Time = " << previous_idle_time << std::endl;
                    //std::cout << "Previous Total Time = " << previous_idle_time << std::endl;
                    const LD::UInteger idle_time = metric.Idle();
                    //std::cout << "Idle Time : " << idle_time << std::endl;
                    const double idle_time_delta = idle_time - previous_idle_time;
                    const double total_time_delta = total_time - previous_total_time;
                    const double utilization = (idle_time_delta!=total_time_delta)*(100.0 * (1.0 - idle_time_delta / total_time_delta));
                    //std::cout << "Utilizaiton : " <<  utilization << std::endl;
                    //std::cout << "Sum : " << total_time << std::endl;
                    LD::Float idlePercentage = (metric.Idle()*100)/total_time;
                    //std::cout << "Usage: " << ((sum-metric.Idle())*100.0)/sum << std::endl;

                    previous_idle_time = idle_time;
                    previous_total_time = total_time;
                };

                auto onCPUCore0 = [](const LD::Context<const LD::DatabaseTransactionResult,LD::CPUCoreMetric<0>,LD::UInteger&,LD::UInteger&> & stuff) noexcept
                {
                    std::cout << "found core 0" << std::endl;
                };
                auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::UInteger&,LD::UInteger&> & context)
                {

                    //std::cout << "found core 0 " << std::endl;
                };
                 */


                //LD::Match(*it,onCPUPackageMetric,onDatabaseError,onCPUCore0);
                // sleep(1);
            }
            sleep(2);
        }
    }
}