//
//  SpinLock.h
//  CoreData
//
//  Created by James Landess on 8/13/15.
//  Copyright (c) 2015 LandessDev. All rights reserved.
//

#ifndef CoreData_SpinLock_h
#define CoreData_SpinLock_h
#include "Async/Atomic/Atomic.h"
namespace PDP
{
    //namespace MultiThreading
    //{
        /**
         @addtogroup Multithreading
         @{
         
         */
        
        /**
         @brief Conventionally mutexes and semaphores require the help of the Kernel from the operating system, and this can be a very expensive process depending on what you're doing.  Spin Locks simply uses up a few CPU cycles instead of accessing the kernel.  Depending on the situation this can be more optimal.  I would attempt to state a general case where a Spin Lock is more optimal however; this, can only be found out experimentally on a case by case basis.
         */
        class SpinLock
        {
        public:
            void Lock()
            {
                while(CurrentLock.exchange(true, PDP::Acquire)){}
                
                //while(lck.test_and_set(std::memory_order_acquire)){}
            }
            
            
            const bool TryLock()
            {
                //bool test_and_set(memory_order __m = memory_order_seq_cst) _NOEXCEPT
                //{return __c11_atomic_exchange(&__a_, true, __m);}
                //CurrentLock.store(, )
                
                
                return CurrentLock.exchange(true, PDP::Acquire);
            }
            
            void Unlock()
            {
                //{__c11_atomic_store(&__a_, false, __m);}
                //memory_order_seq_cst
                CurrentLock.store(false, PDP::Release);
                //lck.clear(std::memory_order_release);
            }
            
        private:
            //std::atomic_flag lck = ATOMIC_FLAG_INIT;
            PDP::Atomic<bool> CurrentLock = {false};
        };
    
    
    class ScopedSpinLock
    {
    private:
        SpinLock * CurrentLock;
    public:
        ScopedSpinLock(SpinLock * spinLock):CurrentLock(spinLock)
        {
            CurrentLock->Lock();
        }
        
        ~ScopedSpinLock()
        {
            this->CurrentLock->Unlock();
        }
    };
    
    
    
        /**
         @}
         */
    //}

}
#endif
