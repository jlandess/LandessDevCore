//
// Created by phoenixflower on 11/14/20.
//

#ifndef LANDESSDEVCORE_BENCHMARK_HPP
#define LANDESSDEVCORE_BENCHMARK_HPP
#include "Definitions/Float.hpp"
#include "Definitions/Integer.hpp"
#include "Core/Unit.hpp"
#include "Chrono/Timer.h"
namespace LD
{
    template<typename String>
    class Benchmark
    {
    private:
        LD::UInteger mNumberOfRuns;
        LD::Second<LD::Float> mTimeElapsed;
        String mName;
    public:
        constexpr Benchmark(const String & name, LD::UInteger mNumberOfRuns, LD::Second<LD::Float> mTimeElapsed) noexcept :mName{name},mNumberOfRuns{mNumberOfRuns},mTimeElapsed{mTimeElapsed}
        {

        }

        const String & Name() const noexcept
        {
            return this->mName;
        }

        LD::UInteger NumberOfRuns() const noexcept
        {
            return this->mNumberOfRuns;
        }

        LD::Second<LD::Float> TimeElapsed() const noexcept
        {
            return this->mTimeElapsed;
        }
    };

    template <typename String> Benchmark(const String &, LD::UInteger mNumberOfRuns, LD::Second<LD::Float> mTimeElapsed) -> Benchmark<String>;


    template<typename F, typename ... B>
    LD::Milisecond<LD::Float> TimedExecution(LD::Timer & timer, F && functorObject, B && ... objects) noexcept
    {
        timer.Start();
        functorObject(LD::Forward<B>(objects)...);
        timer.Stop();
        LD::Milisecond<LD::Float> returnable;
        returnable = timer.Time();
        return returnable;
    }
}
#endif //LANDESSDEVCORE_BENCHMARK_HPP
