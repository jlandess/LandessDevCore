//
// Created by phoenixflower on 11/5/21.
//
#include <iostream>
#include "Async/Scheduler.hpp"
#include "Async/Then.hpp"
#include "Async/SyncWait.hpp"
#include "Async/Just.hpp"
#include "Async/WhenAll.hpp"
#include "Async/WhenAny.hpp"
#include "Async/Execute.hpp"
#include "Async/Let.hpp"
#include "Async/ScheduleAfter.hpp"
#include "Async/SharedState.hpp"
#include "Async/Thread.h"
namespace LD
{
    namespace Example
    {
        namespace Async
        {

            auto NewWorker(LD::Async::Scheduler & scheduler)
            {
                auto function =  [&](auto p)
                {

                    auto function = [p = std::move(p)]() mutable
                    {
                        p.set_value();
                    };

                    scheduler << function;
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<void>>{function};
            }
        }
        extern void AsyncExample() noexcept
        {
            LD::Async::Scheduler scheduler{8};
            //scheduler << [](){std::cout << "thread is executing function" << "\n";};



            auto asyncF1 = LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ return  7;});
            LD::Second<double> abc = 79_ms;
            LD::Async::Then(LD::Async::ScheduleAfter(abc,LD::Example::Async::NewWorker(scheduler)),[](LD::Second<double> timeElapsed ){ return 7;});
            LD::Async::SyncWait(LD::Async::Then(LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){}),[](){std::cout << "and then" << "\n";}));
            //std::cout << "LetValue Example: " << LD::Async::SyncWait(LD::Async::LetValue(LD::Example::Async::NewWorker((scheduler),7,[](int){ return 8;})) << "\n";
            auto asyncF2 = LD::Async::Then(asyncF1,[](int number){ return 7*number;});
            auto asyncF12 = LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ });
            auto syncWaitJust1 = LD::Async::Then(LD::Async::Then(LD::Example::Async::NewWorker(scheduler),LD::Async::Just(6)),[](int a){ return std::sin(a);});
            std::cout << "LawlRawr: " << LD::Async::SyncWait(syncWaitJust1) << "\n";
            LD::Async::WhenAll(LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ return  7;}),
                               LD::Async::Then(asyncF1,[](int number){ return 7*number;}));
            auto whenAllAreDone = LD::Async::WhenAll(
                    LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ return  7;}),
                    LD::Async::Then(asyncF1,[](int number){ return 7*number;})
            );
            auto whenOneIsDone =  LD::Async::WhenAny(
                    LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ return  7;}),
                    LD::Async::Then(LD::Example::Async::NewWorker(scheduler),[](){ return  7.0;})
            );

            LD::Async::Execute(asyncF12);
            LD::Tuple<int,int > res = LD::Async::SyncWait(whenAllAreDone);
            std::cout << "Number: " << LD::Async::SyncWait(asyncF2) << "\n";
            std::cout << LD::Get<0>(res) << " , " << LD::Get<1>(res) << "\n";

            LD::Channel<LD::ImmutableString<256>> nameChannel;
            LD::Channel<LD::UInteger> mIntegers;


        }
    }
}