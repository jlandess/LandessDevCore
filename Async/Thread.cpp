/*
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#include "Thread.h"

namespace PDP
{
    LD::ThreadAffinity Thread::GetThreadAffinity()
    {
#if defined(_WIN32)
        int CPUInfo[4];
        __cpuid(CPUInfo,1);
        if(CPUInfo[3] & (1 << 9)) == 0) return -1;

        return (unsigned)CPUInfo[1] >> 24;
#else
        return sched_getcpu();
#endif
    }
    PDP::UInteger Thread::GetHardwareConcurrency()
        {
#if defined(_WIN32)
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            return (int) si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
            return (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
            return (int) sysconf(_SC_NPROC_ONLN);
#else
            
            // The standard requires this function to return zero if the number of
            // hardware cores could not be determined.
            return 0;
#endif
        }
#if defined(_WIN32)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif
        
#if defined(_WIN32)
        condition_variable::condition_variable() : mWaitersCount(0)
        {
            mEvents[_CONDITION_EVENT_ONE] = CreateEvent(NULL, FALSE, FALSE, NULL);
            mEvents[_CONDITION_EVENT_ALL] = CreateEvent(NULL, TRUE, FALSE, NULL);
            InitializeCriticalSection(&mWaitersCountLock);
        }
#endif
        
#if defined(_WIN32)
        condition_variable::~condition_variable()
        {
            CloseHandle(mEvents[_CONDITION_EVENT_ONE]);
            CloseHandle(mEvents[_CONDITION_EVENT_ALL]);
            DeleteCriticalSection(&mWaitersCountLock);
        }
#endif
        
#if defined(_WIN32)
        void condition_variable::_wait()
        {
            // Wait for either event to become signaled due to notify_one() or
            // notify_all() being called
            int result = WaitForMultipleObjects(2, mEvents, FALSE, INFINITE);
            
            // Check if we are the last waiter
            EnterCriticalSection(&mWaitersCountLock);
            -- mWaitersCount;
            bool lastWaiter = (result == (WAIT_OBJECT_0 + _CONDITION_EVENT_ALL)) &&
            (mWaitersCount == 0);
            LeaveCriticalSection(&mWaitersCountLock);
            
            // If we are the last waiter to be notified to stop waiting, reset the event
            if(lastWaiter)
                ResetEvent(mEvents[_CONDITION_EVENT_ALL]);
        }
#endif
        
#if defined(_WIN32)
        void condition_variable::NotifyOne()
        {
            // Are there any waiters?
            EnterCriticalSection(&mWaitersCountLock);
            bool haveWaiters = (mWaitersCount > 0);
            LeaveCriticalSection(&mWaitersCountLock);
            
            // If we have any waiting threads, send them a signal
            if(haveWaiters)
                SetEvent(mEvents[_CONDITION_EVENT_ONE]);
        }
#endif
        
#if defined(_WIN32)
        void condition_variable::NotifyAll()
        {
            // Are there any waiters?
            EnterCriticalSection(&mWaitersCountLock);
            bool haveWaiters = (mWaitersCount > 0);
            LeaveCriticalSection(&mWaitersCountLock);
            
            // If we have any waiting threads, send them a signal
            if(haveWaiters)
                SetEvent(mEvents[_CONDITION_EVENT_ALL]);
        }
#endif
    /*
        namespace Detail
        {
            MultiThreading::Detail::Thread::Thread():Initialized(false),CurrentTask(nullptr)
            {
                
            }
            
            MultiThreading::Detail::Thread::Thread(Task * task, const bool & executeOnConstruction):Initialized(executeOnConstruction)
            {
                if (executeOnConstruction == true)
                {
                    this->StartExecution();
                }
            }
            
            MultiThreading::Detail::Thread::~Thread()
            {
                CutWaitForThreads(&this->ThreadHandle, 1);
            }
            
            void MultiThreading::Detail::Thread::SetFunction(Task * task)
            {
                
            }
            
            void Thread::operator()(Task * task)
            {
                if (this->Initialized == false)
                {
                    this->Initialized = true;
                    this->StartExecution();
                }
            }
            
            void Thread::Start()
            {
                if (this->Initialized == false)
                {
                    if (this->CurrentTask == nullptr)
                    {
                        exit(0);
                    }
                    this->Initialized = true;
                    this->StartExecution();
                }
            }
            
            void Thread::StartExecution()
            {
                
                this->ThreadHandle = CutStartThread([](void* data)->CUT_RETURN_TYPE
                {
                    Task * currentTask = (Task*)data;
                    
                    try
                    {
                        (*currentTask)();
                    } catch (...)
                    {
                        
                        abort();
                    }
                    
                    
                    delete currentTask;
                    
                    return nullptr;
                }, this->CurrentTask);
                
                if(!this->ThreadHandle)
                {
                    throw PDP::MultiThreading::OutOfThreadsException("out of threads",-1);
                }
            }
            

            
            PDP::UInteger Thread::HardwareConcurrency()
            {
#if defined(_WIN32)
                SYSTEM_INFO si;
                GetSystemInfo(&si);
                return (int) si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
                return (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
                return (int) sysconf(_SC_NPROC_ONLN);
#else
                // The standard requires this function to return zero if the number of
                // hardware cores could not be determined.
                return 0;
#endif
            }
            
            Thread::Identification Thread::GetID(const CUTThread &aHandle)
            {
     
                //static Mutex idMapLock;
                //static std::map<pthread_t, unsigned long int> idMap;
                //static unsigned long int idCount(1);
                
                //LockGuard<Mutex> guard(idMapLock);
                //if(idMap.find(aHandle) == idMap.end())
                    //idMap[aHandle] = idCount ++;
                //return Thread::id(idMap[aHandle]);
     
                //return Thread::Identification(0);
            }
            Thread::Identification Thread::GetIdentification() const
            {
#if defined(_WIN32)
           return id((unsigned long int) mWin32ThreadID);
#else
                return Thread::GetID(this->ThreadHandle);
#endif
                /*
                if(!joinable())
                    return id();
#if defined(_TTHREAD_WIN32_)
                return id((unsigned long int) mWin32ThreadID);
#elif defined(_TTHREAD_POSIX_)
                return _pthread_t_to_ID(ThreadHandle);
#endif
     
                return Thread::Identification(0);
            }
        }
        
        */



#if _WIN32
        //Create thread
        CUTThread CutStartThread(CUT_THREADROUTINE func, void *data)
        {
            return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, data, 0, NULL);
        }
        
        //Wait for thread to finish
        void CutEndThread(CUTThread thread)
        {
            WaitForSingleObject(thread, INFINITE);
            CloseHandle(thread);
        }
        
        //Destroy thread
        void CutDestroyThread(CUTThread thread)
        {
            TerminateThread(thread, 0);
            CloseHandle(thread);
        }
        
        //Wait for multiple threads
        void CutWaitForThreads(const CUTThread *threads, int num)
        {
            WaitForMultipleObjects(num, threads, true, INFINITE);
            
            for (int i = 0; i < num; i++)
            {
                CloseHandle(threads[i]);
            }
        }
        
        //Create barrier.
        CUTBarrier CutCreateBarrier(int releaseCount)
        {
            CUTBarrier barrier;
            
            InitializeCriticalSection(&barrier.criticalSection);
            barrier.barrierEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("BarrierEvent"));
            barrier.count = 0;
            barrier.releaseCount = releaseCount;
            
            return barrier;
        }
        
        //Increment barrier. (excution continues)
        void CutIncrementBarrier(CUTBarrier *barrier)
        {
            int myBarrierCount;
            EnterCriticalSection(&barrier->criticalSection);
            myBarrierCount = ++barrier->count;
            LeaveCriticalSection(&barrier->criticalSection);
            
            if (myBarrierCount >= barrier->releaseCount)
            {
                SetEvent(barrier->barrierEvent);
            }
        }
        
        //Wait for barrier release.
        void CutWaitForBarrier(CUTBarrier *barrier)
        {
            WaitForSingleObject(barrier->barrierEvent, INFINITE);
        }
        
        //Destory barrier
        void CutDestroyBarrier(CUTBarrier *barrier)
        {
            
        }
        
        
#else
        //Create thread
        CUTThread CutStartThread(CUT_THREADROUTINE func, void *data)
        {
            pthread_t thread;
            pthread_create(&thread, NULL, func, data);
            return thread;
        }
        
        //Wait for thread to finish
        void CutEndThread(CUTThread thread)
        {
            pthread_join(thread, NULL);
        }
        
        //Destroy thread
        void CutDestroyThread(CUTThread thread)
        {
            pthread_cancel(thread);
        }
        
        //Wait for multiple threads
    void CutWaitForThreads(const CUTThread *threads, const PDP::UInteger & num)
        {
            for (PDP::UInteger  i = 0; i < num; i++)
            {
                CutEndThread(threads[i]);
            }
        }
        
        
        //Create barrier.
    CUTBarrier CutCreateBarrier(const PDP::UInteger & releaseCount)
        {
            CUTBarrier barrier;
            
            barrier.count = 0;
            barrier.releaseCount = releaseCount;
            
            pthread_mutex_init(&barrier.mutex, 0);
            pthread_cond_init(&barrier.conditionVariable,0);
            
            
            return barrier;
        }
        
        //Increment barrier. (excution continues)
        void CutIncrementBarrier(CUTBarrier *barrier)
        {
            PDP::UInteger myBarrierCount;
            pthread_mutex_lock(&barrier->mutex);
            myBarrierCount = ++barrier->count;
            pthread_mutex_unlock(&barrier->mutex);
            
            if (myBarrierCount >=barrier->releaseCount)
            {
                pthread_cond_signal(&barrier->conditionVariable);
            }
        }
        
        //Wait for barrier release.
        void CutWaitForBarrier(CUTBarrier *barrier)
        {
            pthread_mutex_lock(&barrier->mutex);
            
            while (barrier->count < barrier->releaseCount)
            {
                pthread_cond_wait(&barrier->conditionVariable, &barrier->mutex);
            }
            
            pthread_mutex_unlock(&barrier->mutex);
        }
        
        //Destory barrier
        void CutDestroyBarrier(CUTBarrier *barrier)
        {
            pthread_mutex_destroy(&barrier->mutex);
            pthread_cond_destroy(&barrier->conditionVariable);
        }
        
#endif
}


