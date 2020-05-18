//////////////////////////////////////////////////////////////////////////////
// Timer.cpp
// =========
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system
//////////////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
namespace LD
{
    Timer::Timer()
    {
#ifdef WIN32
        QueryPerformanceFrequency(&frequency);
            startCount.QuadPart = 0;
            endCount.QuadPart = 0;
#else
        startCount.tv_sec = startCount.tv_usec = 0;
        endCount.tv_sec = endCount.tv_usec = 0;
#endif

        stopped = 0;
        //startTimeInMicroSec = 0;
        //endTimeInMicroSec = 0;
    }



    ///////////////////////////////////////////////////////////////////////////////
    // distructor
    ///////////////////////////////////////////////////////////////////////////////
    Timer::~Timer()
    {
    }



    ///////////////////////////////////////////////////////////////////////////////
    // start timer.
    // startCount will be set at this point.
    ///////////////////////////////////////////////////////////////////////////////
    void Timer::Start()
    {
        stopped = 0; // reset stop flag
#ifdef WIN32
        QueryPerformanceCounter(&startCount);
#else
        gettimeofday(&startCount, NULL);
#endif
    }



    ///////////////////////////////////////////////////////////////////////////////
    // stop the timer.
    // endCount will be set at this point.
    ///////////////////////////////////////////////////////////////////////////////
    void Timer::Stop()
    {
        stopped = 1; // set timer stopped flag

#ifdef WIN32
        QueryPerformanceCounter(&endCount);
#else
        gettimeofday(&endCount, NULL);
#endif
    }



    ///////////////////////////////////////////////////////////////////////////////
    // compute elapsed time in micro-second resolution.
    // other getElapsedTime will call this first, then convert to correspond resolution.
    ///////////////////////////////////////////////////////////////////////////////
    const PDP::Float Timer::GetElapsedTimeInMicroSec() const
    {
#ifdef WIN32
        if(!stopped)
                QueryPerformanceCounter(&endCount);

            startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
            endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
        if(!stopped)
            gettimeofday(&endCount, NULL);

        PDP::UInteger startTime = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
        PDP::UInteger endTime = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

        return endTime - startTime;
    }



    ///////////////////////////////////////////////////////////////////////////////
    // divide elapsedTimeInMicroSec by 1000
    ///////////////////////////////////////////////////////////////////////////////
    const PDP::Float Timer::GetElapsedTimeInMilliSec() const
    {
        return this->GetElapsedTimeInSec() * 1000;
    }



    ///////////////////////////////////////////////////////////////////////////////
    // divide elapsedTimeInMicroSec by 1000000
    ///////////////////////////////////////////////////////////////////////////////
    const PDP::Float Timer::GetElapsedTimeInSec() const
    {
        return this->GetElapsedTimeInMicroSec() * 0.000001;
    }

     LD::MicroSecond<LD::Float> Timer::Time() const noexcept
    {
        return LD::MicroSecond<LD::Float>{LD::Float {this->GetElapsedTimeInMicroSec()}};
    }


    ///////////////////////////////////////////////////////////////////////////////
    // same as getElapsedTimeInSec()
    ///////////////////////////////////////////////////////////////////////////////
    const PDP::Float Timer::GetElapsedTime() const
    {
        return this->GetElapsedTimeInSec();
    }

    const bool Timer::HasStarted() const
    {

        return stopped == 0;
    }
}
