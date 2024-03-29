//
// Created by phoenixflower on 9/19/21.
//

#ifndef LANDESSDEVCORE_SCHEDULEAFTER_HPP
#define LANDESSDEVCORE_SCHEDULEAFTER_HPP
#include "Core/Unit.hpp"
#include "Definitions/TimeExtension.hpp"
#include "Let.hpp"
#include "Chrono/Timer.h"
namespace LD
{
    namespace Async
    {
        template<typename T, typename Context>
        auto ScheduleAfter(LD::Second<T> time, Context context) noexcept
        {
            //return LD::Async::Then(LD::Async::LetValue(LD::Second<T>{time},context),[](LD::Second<T> time){ LD::Usleep(time); return 0;});
            return LD::Async::LetValue(context,LD::Second<T>{time},[](LD::Second<T> time)
            {
                LD::Timer timer;
                timer.Start();
                LD::Usleep(time);
                timer.Stop();
                return timer.Time();
            });
        }

        template<typename T, typename Context, typename Task>
        auto ScheduleAfter(Context context,LD::Second<T> time, Task task) noexcept
        {
            //return LD::Async::Then(LD::Async::LetValue(LD::Second<T>{time},context),[](LD::Second<T> time){ LD::Usleep(time); return 0;});
            return LD::Async::Then(LD::Async::LetValue(context,LD::Second<T>{time},[](LD::Second<T> time)
            {
                LD::Timer timer;
                timer.Start();
                LD::Usleep(time);
                timer.Stop();
                return timer.Time();
            }),task);
        }
    }
}
#endif //LANDESSDEVCORE_SCHEDULEAFTER_HPP
