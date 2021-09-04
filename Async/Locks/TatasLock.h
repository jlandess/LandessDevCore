//
//  TatasLock.h
//  DataStructures
//
//  Created by James Landess on 7/9/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_TatasLock_h
#define DataStructures_TatasLock_h


#include <Primitives/Threads/Atomic/Atomic.h>
namespace PDP
{

    class TatasLock
    {
        
        //std::atomic<bool> locked; /* TODO can std::atomic_flag be used? */
        PDP::Atomic<bool> CurrentLock = {false};
    public:
        
        TatasLock() //: locked(false)
        
        {
                
            
        };
        
        TatasLock(TatasLock&) = delete; /* TODO? */
        
        bool TryLock()
        
        {
            
            if(IsLocked()) return false;
                
            return !CurrentLock.exchange(true,PDP::AcquireRelease);
            //return !locked.exchange(true, std::memory_order_acq_rel);
                
            
        }
        
        void UnLock()
        
        {
            CurrentLock.store(false,PDP::Release);
            //locked.store(false, std::memory_order_release);
            
        }
            
        
        bool IsLocked()
        {
            
            //return locked.load(std::memory_order_acquire);
            
            
            return CurrentLock.load(LD::Acquire);
            
        }
            
            
        
        void Lock()
        {
            
            while(!TryLock())
            {
                
                LD::Yield();
                
            }
            
        }
        
    };
}
#endif
