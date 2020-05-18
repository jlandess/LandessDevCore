//
//  Semaphore.h
//  DataStructures
//
//  Created by James Landess on 7/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Semaphore_h
#define DataStructures_Semaphore_h

#include <Primitives/Threads/Atomic/AtomicUnderlyingCore.h>
#if TURF_TARGET_WIN32
//#define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_Win32.h"
//#define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_Win32


namespace turf
{
    
    class Semaphore
    {
    private:
        HANDLE m_sem;
        
    public:
        Semaphore()
        {
            m_sem = CreateSemaphore(NULL, 0, INT32_MAX, NULL);
        }
        
        ~Semaphore()
        {
            CloseHandle(m_sem);
        }
        
        void wait()
        {
            WaitForSingleObject(m_sem, INFINITE);
        }
        
        void signal(ureg count = 1) {
            ReleaseSemaphore(m_sem, count, NULL);
        }
    };
    
} // namespace turf
#elif TURF_KERNEL_MACH
//#define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_Mach.h"
//#define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_Mach

#include <mach/mach.h>

namespace turf
{
    
    class Semaphore
    {
    private:
        semaphore_t m_semaphore;
        
    public:
        Semaphore()
        {
            semaphore_create(mach_task_self(), &m_semaphore, SYNC_POLICY_FIFO, 0);
        }
        
        ~Semaphore()
        {
            semaphore_destroy(mach_task_self(), m_semaphore);
        }
        
        void wait()
        {
            semaphore_wait(m_semaphore);
        }
        
        void signal(ureg count = 1)
        {
            while (count-- > 0)
                semaphore_signal(m_semaphore);
        }
    };
    
} // namespace turf
#elif TURF_TARGET_POSIX
//#define TURF_IMPL_SEMAPHORE_PATH "impl/Semaphore_POSIX.h"
//#define TURF_IMPL_SEMAPHORE_TYPE turf::Semaphore_POSIX

#if TURF_KERNEL_MACH
// Mach can't use POSIX semaphores due to http://lists.apple.com/archives/darwin-kernel/2009/Apr/msg00010.html
#error "Can't use unnamed POSIX semaphores on Mach."
#endif
#include <semaphore.h>
#include <errno.h>

namespace turf
{
    
    class Semaphore
    {
    private:
        sem_t m_sem;
        
    public:
        Semaphore()
        {
            sem_init(&m_sem, 0, 0);
        }
        
        ~Semaphore()
        {
            sem_destroy(&m_sem);
        }
        
        void wait()
        {
            // http://stackoverflow.com/questions/2013181/gdb-causes-sem-wait-to-fail-with-eintr-error
            int rc;
            do
            {
                rc = sem_wait(&m_sem);
            } while (rc == -1 && errno == EINTR);
        }
        
        void signal(ureg count = 1)
        {
            while (count-- > 0)
                sem_post(&m_sem);
        }
    };
    
} // namespace turf


#else
#define TURF_IMPL_SEMAPHORE_PATH "*** Unable to select a default Semaphore implementation ***"
#endif
#endif
