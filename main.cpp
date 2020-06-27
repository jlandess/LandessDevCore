#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Chrono/Timer.h"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Algorithms/FuzzySearch.h"
#include "IO/RowBackingStore.h"
#include "IO/BasicDelimeterSeperatedFile.h"

int main()
{

    LD::RowBackingStore backingStore{"/proc/stat"};
    LD::SpaceSpeerateValueFile<LD::RowBackingStore> file{backingStore};

    LD::UInteger previous_idle_time=0, previous_total_time=0;
    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<LD::Variant<LD::CPUPackageMetric,LD::CPUCoreMetric<0>>(LD::UInteger&,LD::UInteger&)> it{previous_idle_time,previous_total_time};// = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{},previous_idle_time,previous_total_time);


    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<LD::Variant<LD::CPUPackageMetric,LD::CPUCoreMetric<0>>(LD::UInteger&,LD::UInteger&)> end = file.End(LD::CT::TypeList<LD::CPUPackageMetric,LD::CPUCoreMetric<0>>{},previous_idle_time,previous_total_time);





    std::cout << LD::CPUCoreMetric<0>::GetClassName() << std::endl;
    for (LD::UInteger i = 0; i < 10; ++i)
    {
        for (auto it = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric,LD::CPUCoreMetric<0>>{},previous_idle_time,previous_total_time); it != end; ++it)
        {

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


            LD::Match(*it,onCPUPackageMetric,onDatabaseError,onCPUCore0);
           // sleep(1);
        }
        //sleep(2);
    }



    //auto bar = LD::ImmutableString<9>{'_'};
    //auto edge = LD::ToImmutableString("abc");
    //std::cout << (bar+edge).Data() << std::endl;
    //std::cout << edge.Data() << std::endl;
    //std::cout << bar.GetSize() << std::endl;
    //std::cout << edge.GetSize() << std::endl;
    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();

    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();

    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}