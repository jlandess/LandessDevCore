//
//  RWSpinLock.h
//  DataStructures
//
//  Created by James Landess on 7/15/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_RWSpinLock_h
#define DataStructures_RWSpinLock_h

#include <Primitives/Threads/Atomic/Atomic.h>
#include <Primitives/Threads/Thread.h>

#if defined(__GNUC__) && __GNUC__ >= 4
#define LIKELY(x)   (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define LIKELY(x)   (x)
#define UNLIKELY(x) (x)
#endif


class RWSpinLock
{
    
    enum : int32_t { READER = 4, UPGRADED = 2, WRITER = 1 };
    
private:
    
    turf::Atomic<int32_t> bits_;
public:
    
    
    RWSpinLock() : bits_(0) {}
    
    RWSpinLock(RWSpinLock const&) = delete;
    
    
    RWSpinLock& operator=(RWSpinLock const&) = delete;
    
    
    
    // Lockable Concept
    void Lock()
    {
        int count = 0;
        while (!LIKELY(TryLock()))
        {
            
            if (++count > 1000) PDP::Yield();;
        }
    }
    
    // Writer is responsible for clearing up both the UPGRADED and WRITER bits.
    void UnLock()
    {
        static_assert(READER > WRITER + UPGRADED, "wrong bits!");
        bits_.fetchAnd(~(WRITER | UPGRADED), turf::Release);
    }
    
    // SharedLockable Concept
    void LockShared()
    {
        int count = 0;
        while (!LIKELY(TryLockShared()))
        {
            if (++count > 1000) PDP::Yield();;
        }
    }
    
    void UnLockShared()
    {
        bits_.fetchAdd(-READER, turf::Release);
    }
    
    // Downgrade the lock from writer status to reader status.
    void UnlockAndLockShared()
    {
        bits_.fetchAdd(READER, turf::Acquire);
        UnLock();
    }
    
    // UpgradeLockable Concept
    void LockUpgrade()
    {
        int count = 0;
        while (!TryLockUpgrade())
        {
            if (++count > 1000) PDP::Yield();
        }
    }
    
    void UnLockUpgrade()
    {
        
        bits_.fetchAdd(-UPGRADED, turf::AcquireRelease);
    }
    
    // unlock upgrade and try to acquire write lock
    void UnLockUpgradeAndLock()
    {
        int64_t count = 0;
        while (!TryUnlockUpgradeAndLock())
        {
            if (++count > 1000) PDP::Yield();
        }
    }
    
    // unlock upgrade and read lock atomically
    void UnlockUpgradeAndLockShared()
    {
        bits_.fetchAnd(READER - UPGRADED, turf::AcquireRelease);
    }
    
    // write unlock and upgrade lock atomically
    void UnlockAndLockUpgrade()
    {
        // need to do it in two steps here -- as the UPGRADED bit might be OR-ed at
        // the same time when other threads are trying do try_lock_upgrade().
        bits_.fetchOr(UPGRADED, turf::Acquire);
        bits_.fetchAnd(-WRITER, turf::Release);
    }
    
    
    // Attempt to acquire writer permission. Return false if we didn't get it.
    bool TryLock()
    {
        int32_t expect = 0;
        return bits_.compareExchangeStrong(expect, WRITER,
                                           turf::AcquireRelease);
    }
    
    // Try to get reader permission on the lock. This can fail if we
    // find out someone is a writer or upgrader.
    // Setting the UPGRADED bit would allow a writer-to-be to indicate
    // its intention to write and block any new readers while waiting
    // for existing readers to finish and release their read locks. This
    // helps avoid starving writers (promoted from upgraders).
    bool TryLockShared()
    {
        // fetch_add is considerably (100%) faster than compare_exchange,
        // so here we are optimizing for the common (lock success) case.
        int32_t value = bits_.fetchAnd(READER, turf::Acquire);
        
        
        if (UNLIKELY(value & (WRITER|UPGRADED)))
        {
            bits_.fetchAnd(-READER, turf::Release);
            return false;
        }
        return true;
    }
    
    // try to unlock upgrade and write lock atomically
    bool TryUnlockUpgradeAndLock()
    {
        int32_t expect = UPGRADED;
        return bits_.compareExchangeStrong(expect, WRITER,
                                           turf::AcquireRelease);
    }
    
    // try to acquire an upgradable lock.
    bool TryLockUpgrade()
    {
        int32_t value = bits_.fetchOr(UPGRADED, turf::Acquire);
        
        // Note: when failed, we cannot flip the UPGRADED bit back,
        // as in this case there is either another upgrade lock or a write lock.
        // If it's a write lock, the bit will get cleared up when that lock's done
        // with unlock().
        return ((value & (UPGRADED | WRITER)) == 0);
    }
};


#undef LIKELY
#undef UNLIKELY
#endif
