//
//  RWLock.h
//  DataStructures
//
//  Created by James Landess on 7/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_RWLock_h
#define DataStructures_RWLock_h

#include "Async/Atomic/AtomicUnderlyingCore.h"

// Choose default implementation if not already configured by turf_userconfig.h:
#if !defined(TURF_IMPL_RWLOCK_PATH)
// FIXME: Implement RWLock_CPP11/14, RWLock_Boost
#if TURF_TARGET_WIN32
//#define TURF_IMPL_RWLOCK_PATH "impl/RWLock_Win32.h"
//#define TURF_IMPL_RWLOCK_TYPE turf::RWLock_Win32

namespace turf
{
    
    class RWLock
    {
    private:
        SRWLOCK m_rwLock;
        
    public:
        RWLock()
        {
            InitializeSRWLock(&m_rwLock);
        }
        
        ~RWLock()
        {
            // SRW locks do not need to be explicitly destroyed.
        }
        
        void lockExclusive()
        {
            AcquireSRWLockExclusive(&m_rwLock);
        }
        
        void unlockExclusive()
        {
            ReleaseSRWLockExclusive(&m_rwLock);
        }
        
        void lockShared()
        {
            AcquireSRWLockShared(&m_rwLock);
        }
        
        void unlockShared()
        {
            ReleaseSRWLockShared(&m_rwLock);
        }
    };
    
} // namespace turf
#elif TURF_TARGET_POSIX


#include <pthread.h>

namespace turf
{
    
    class RWLock
    {
    private:
        pthread_rwlock_t m_rwLock;
        
    public:
        RWLock()
        {
            pthread_rwlock_init(&m_rwLock, NULL);
        }
        
        ~RWLock()
        {
            pthread_rwlock_destroy(&m_rwLock);
        }
        
        void lockExclusive()
        {
            pthread_rwlock_wrlock(&m_rwLock);
        }
        
        void unlockExclusive()
        {
            pthread_rwlock_unlock(&m_rwLock);
        }
        
        void lockShared()
        {
            pthread_rwlock_rdlock(&m_rwLock);
        }
        
        void unlockShared()
        {
            pthread_rwlock_unlock(&m_rwLock);
        }
    };
    
} // namespace turf

//#define TURF_IMPL_RWLOCK_PATH "impl/RWLock_POSIX.h"
//#define TURF_IMPL_RWLOCK_TYPE turf::RWLock_POSIX
#else
//#define TURF_IMPL_RWLOCK_PATH "*** Unable to select a default RWLock implementation ***"
#endif
#endif
namespace turf
{
    
    //typedef TURF_IMPL_RWLOCK_TYPE RWLock;
    
    //---------------------------------------------------------
    // SharedLockGuard
    //---------------------------------------------------------
    template <typename LockType> class SharedLockGuard
    {
    private:
        LockType& m_lock;
        
    public:
        SharedLockGuard(LockType& lock) : m_lock(lock)
        {
            m_lock.lockShared();
        }
        
        ~SharedLockGuard() {
            m_lock.unlockShared();
        }
    };
    
    //---------------------------------------------------------
    // ExclusiveLockGuard
    //---------------------------------------------------------
    template <typename LockType> class ExclusiveLockGuard
    {
    private:
        LockType& m_lock;
        
    public:
        ExclusiveLockGuard(LockType& lock) : m_lock(lock)
        {
            m_lock.lockExclusive();
        }
        
        ~ExclusiveLockGuard() {
            m_lock.unlockExclusive();
        }
    };
    
} // namespace turf
#endif
