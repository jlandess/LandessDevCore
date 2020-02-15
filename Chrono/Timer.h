//
//  Timer.h
//  DataStructures
//
//  Created by James Landess on 10/23/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#include <Definitions/Float.h>
#include <Definitions/Integer.h>

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

namespace LD
{
    class Timer
    {
    public:
        Timer();                                    // default constructor
        ~Timer();                                   // default destructor

        void   Start();                             // start timer
        void   Stop();                              // stop the timer
        const PDP::Float GetElapsedTime() const;                    // get elapsed time in second
        const PDP::Float GetElapsedTimeInSec() const;               // get elapsed time in second (same as getElapsedTime)
        const PDP::Float GetElapsedTimeInMilliSec() const;          // get elapsed time in milli-second
        const PDP::Float GetElapsedTimeInMicroSec() const;          // get elapsed time in micro-second
        const bool HasStarted() const;


    protected:


    private:
        //PDP::Float startTimeInMicroSec;                 // starting time in micro-second
        //PDP::Float endTimeInMicroSec;                   // ending time in micro-second
        // stop flag
#ifdef WIN32
        mutable LARGE_INTEGER frequency;                    // ticks per second
            mutable LARGE_INTEGER startCount;                   //
            mutable LARGE_INTEGER endCount;                     //
#else
        mutable timeval startCount;                         //
        mutable timeval endCount;                           //
#endif
        bool    stopped;
    };
}
namespace PDP
{
    namespace DataStructures
    {

    }

}


#endif // TIMER_H_DEF
