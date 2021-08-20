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

#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#if !defined(_TTHREAD_PLATFORM_DEFINED_)
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define _TTHREAD_WIN32_
#else
#define _TTHREAD_POSIX_
#endif
#define _TTHREAD_PLATFORM_DEFINED_
#endif


// Platform specific includes
#if defined(_TTHREAD_WIN32_)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define __UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef __UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef __UNDEF_LEAN_AND_MEAN
#endif
#else
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#endif

#if defined(_TTHREAD_POSIX_)
#include <unistd.h>
#include <map>
#elif defined(_TTHREAD_WIN32_)
#include <process.h>
#endif

#include <Memory/AutomaticReferenceCounting.h>
#include "Definitions/Integer.hpp"
//#include "Definitions/Common.hpp"
//#include <tuple>
#include "Exceptions/OutOfThreadsException.h"
#include "Async/Promise.h"
#include "Async/Commitment.h"
#include "Async/Commitment.h"
#include "Primitives/General/PackagedTask.h"
#include "Async/ThreadAffinity.h"
#include "Primitives/General/Pair.h"

#ifdef _WIN32
//Windows threads.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define __UNDEF_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifdef __UNDEF_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef __UNDEF_LEAN_AND_MEAN
#endif
#else
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#endif

// Do we have a fully featured C++11 compiler?
#if (__cplusplus > 199711L) || (defined(__STDCXX_VERSION__) && (__STDCXX_VERSION__ >= 201001L))
#define _TTHREAD_CPP11_
#endif

// ...at least partial C++11?
#if defined(_TTHREAD_CPP11_) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__GXX_EXPERIMENTAL_CPP0X__)
#define _TTHREAD_CPP11_PARTIAL_
#endif

// Macro for disabling assignments of objects.
#ifdef _TTHREAD_CPP11_PARTIAL_
#define _TTHREAD_DISABLE_ASSIGNMENT(name) \
name(const name&) = delete; \
name& operator=(const name&) = delete;
#else
#define _TTHREAD_DISABLE_ASSIGNMENT(name) \
name(const name&); \
name& operator=(const name&);
#endif

/// @def thread_local
/// Thread local storage keyword.
/// A variable that is declared with the @c thread_local keyword makes the
/// value of the variable local to each thread (known as thread-local storage,
/// or TLS). Example usage:
/// @code
/// // This variable is local to each thread.
/// thread_local int variable;
/// @endcode
/// @note The @c thread_local keyword is a macro that maps to the corresponding
/// compiler directive (e.g. @c __declspec(thread)). While the C++11 standard
/// allows for non-trivial types (e.g. classes with constructors and
/// destructors) to be declared with the @c thread_local keyword, most pre-C++11
/// compilers only allow for trivial types (e.g. @c int). So, to guarantee
/// portable code, only use trivial types for thread local storage.
/// @note This directive is currently not supported on Mac OS X (it will give
/// a compiler error), since compile-time TLS is not supported in the Mac OS X
/// executable format. Also, some older versions of MinGW (before GCC 4.x) do
/// not support this directive.
/// @hideinitializer

#if !defined(_TTHREAD_CPP11_) && !defined(thread_local)
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#define thread_local __thread
#else
#define thread_local __declspec(thread)
#endif
#endif

#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#define thread_local_helper __thread
#else
#define thread_local_helper __declspec(thread)
#endif
//Simple portable thread library.
namespace LD
{
    // Related to <ratio> - minimal to be able to support chrono.
    typedef long long __intmax_t;
    
    /// Minimal implementation of the @c ratio class. This class provides enough
    /// functionality to implement some basic @c chrono classes.

    /*
    template <__intmax_t N, __intmax_t D = 1> class Ratio {
    public:
        static double _as_double() { return double(N) / double(D); }
    };
     */
    

    class Mutex
    {
    public:
        /// Constructor.
        Mutex()
#if defined(_WIN32)
        : mAlreadyLocked(false)
#endif
        {
#if defined(_TTHREAD_WIN32_)
            InitializeCriticalSection(&mHandle);
#else
            pthread_mutex_init(&mHandle, NULL);
#endif
        }
        
        /// Destructor.
        ~Mutex()
        {
#if defined(_WIN32)
            DeleteCriticalSection(&mHandle);
#else
            pthread_mutex_destroy(&mHandle);
#endif
        }
        
        /// Lock the mutex.
        /// The method will block the calling thread until a lock on the mutex can
        /// be obtained. The mutex remains locked until @c unlock() is called.
        /// @see lock_guard
        inline void Lock()
        {
#if defined(_WIN32)
            EnterCriticalSection(&mHandle);
            while(mAlreadyLocked) Sleep(1000); // Simulate deadlock...
            mAlreadyLocked = true;
#else
            pthread_mutex_lock(&mHandle);
#endif
        }
        
        /// Try to lock the mutex.
        /// The method will try to lock the mutex. If it fails, the function will
        /// return immediately (non-blocking).
        /// @return @c true if the lock was acquired, or @c false if the lock could
        /// not be acquired.
        inline bool TryLock()
        {
#if defined(_WIN32)
            bool ret = (TryEnterCriticalSection(&mHandle) ? true : false);
            if(ret && mAlreadyLocked)
            {
                LeaveCriticalSection(&mHandle);
                ret = false;
            }
            return ret;
#else
            return (pthread_mutex_trylock(&mHandle) == 0) ? true : false;
#endif
        }
        
        /// Unlock the mutex.
        /// If any threads are waiting for the lock on this mutex, one of them will
        /// be unblocked.
        inline void UnLock()
        {
#if defined(_WIN32)
            mAlreadyLocked = false;
            LeaveCriticalSection(&mHandle);
#else
            pthread_mutex_unlock(&mHandle);
#endif
        }
        
        _TTHREAD_DISABLE_ASSIGNMENT(Mutex)
        
    private:
#if defined(_WIN32)
        CRITICAL_SECTION mHandle;
        bool mAlreadyLocked;
#else
        pthread_mutex_t mHandle;
#endif
        
        friend class ConditionVariable;
    };
    
    
    class RecursiveMutex
    {
    public:
        /// Constructor.
        RecursiveMutex()
        {
#if defined(_WIN32)
            InitializeCriticalSection(&mHandle);
#else
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mHandle, &attr);
#endif
        }
        
        /// Destructor.
        ~RecursiveMutex()
        {
#if defined(_WIN32)
            DeleteCriticalSection(&mHandle);
#else
            pthread_mutex_destroy(&mHandle);
#endif
        }
        
        /// Lock the mutex.
        /// The method will block the calling thread until a lock on the mutex can
        /// be obtained. The mutex remains locked until @c unlock() is called.
        /// @see lock_guard
        inline void Lock()
        {
#if defined(_WIN32)
            EnterCriticalSection(&mHandle);
#else
            pthread_mutex_lock(&mHandle);
#endif
        }
        
        /// Try to lock the mutex.
        /// The method will try to lock the mutex. If it fails, the function will
        /// return immediately (non-blocking).
        /// @return @c true if the lock was acquired, or @c false if the lock could
        /// not be acquired.
        inline bool TryLock()
        {
#if defined(_WIN32)
            return TryEnterCriticalSection(&mHandle) ? true : false;
#else
            return (pthread_mutex_trylock(&mHandle) == 0) ? true : false;
#endif
        }
        
        /// Unlock the mutex.
        /// If any threads are waiting for the lock on this mutex, one of them will
        /// be unblocked.
        inline void UnLock()
        {
#if defined(_WIN32)
            LeaveCriticalSection(&mHandle);
#else
            pthread_mutex_unlock(&mHandle);
#endif
        }
        
        _TTHREAD_DISABLE_ASSIGNMENT(RecursiveMutex)
        
    private:
#if defined(_WIN32)
        CRITICAL_SECTION mHandle;
#else
        pthread_mutex_t mHandle;
#endif
        
        friend class ConditionVariable;
    };
    
    
    template <class T>
    class LockGuard
    {
    public:
        typedef T mutex_type;
        
        LockGuard() : mMutex(0) {}
        
        /// The constructor locks the mutex.
        explicit LockGuard(mutex_type &aMutex)
        {
            mMutex = &aMutex;
            mMutex->Lock();
        }
        
        /// The destructor unlocks the mutex.
        ~LockGuard()
        {
            if(mMutex)
                mMutex->UnLock();
        }
        
    private:
        mutex_type * mMutex;
    };
    
    
    
    
    template <class T>
    class ScopeLock
    {
    public:
        typedef T mutex_type;
        
        ScopeLock() : mMutex(0) {}
        
        /// The constructor locks the mutex.
        explicit ScopeLock(mutex_type &aMutex)
        {
            mMutex = &aMutex;
            mMutex->Lock();
        }
        
        explicit ScopeLock(mutex_type * aMutex)
        {
            mMutex = aMutex;
            mMutex->Lock();
        }
        
        /// The destructor unlocks the mutex.
        ~ScopeLock()
        {
            if(mMutex)
                mMutex->UnLock();
        }
        
    private:
        mutex_type * mMutex;
    };
    
    
    template<typename T,typename Object>
    class MappedObject
    {
    private:
        Object * CurrentObject;
        T * CurrentLock;
    public:
        
        inline MappedObject();
        
        inline MappedObject(Object * currentObject, T * currentLock);
        
        
        inline ~MappedObject();
        
        
        Object * operator -> ();
        const Object * operator ->() const;
        
        
        Object & operator * ();
        const Object & operator * () const;
    };
    
    template<typename T, typename Object>
    MappedObject<T, Object>::MappedObject():CurrentObject(nullptr),CurrentLock(nullptr)
    {
        
    }
    
    template<typename T, typename Object>
    MappedObject<T, Object>::MappedObject(Object * currentObject, T * currentLock):CurrentObject(currentObject),CurrentLock(currentLock)
    {
        CurrentLock->Lock();
    }
    
    template<typename T, typename Object>
    MappedObject<T, Object>::~MappedObject()
    {
        CurrentLock->UnLock();
    }
    
    template<typename T, typename Object>
    Object * MappedObject<T, Object>::operator -> ()
    {
        return this->CurrentObject;
    }
    
    
    template<typename T, typename Object>
    const Object * MappedObject<T, Object>::operator -> () const
    {
        return this->CurrentObject;
    }
    
    template<typename T, typename Object>
    Object & MappedObject<T, Object>::operator * ()
    {
        return *(this->CurrentObject);
    }
    
    
    template<typename T, typename Object>
    const Object & MappedObject<T, Object>::operator * () const
    {
        return *(this->CurrentObject);
    }
    
    
    
    
    
    class ConditionVariable
    {
    public:
        /// Constructor.
#if defined(_WIN32)
        ConditionVariable();
#else
        ConditionVariable()
        {
            pthread_cond_init(&mHandle, NULL);
        }
#endif
        
        /// Destructor.
#if defined(_WIN32)
        ~ConditionVariable();
#else
        ~ConditionVariable()
        {
            pthread_cond_destroy(&mHandle);
        }
#endif
        
        /// Wait for the condition.
        /// The function will block the calling thread until the condition variable
        /// is woken by @c notify_one(), @c notify_all() or a spurious wake up.
        /// @param[in] aMutex A mutex that will be unlocked when the wait operation
        ///   starts, an locked again as soon as the wait operation is finished.
        template <class _mutexT>
        inline void Wait(_mutexT &aMutex)
        {
#if defined(_WIN32)
            // Increment number of waiters
            EnterCriticalSection(&mWaitersCountLock);
            ++ mWaitersCount;
            LeaveCriticalSection(&mWaitersCountLock);
            
            // Release the mutex while waiting for the condition (will decrease
            // the number of waiters when done)...
            aMutex.unlock();
            _wait();
            aMutex.lock();
#else
            pthread_cond_wait(&mHandle, &aMutex.mHandle);
#endif
        }
        
        /// Notify one thread that is waiting for the condition.
        /// If at least one thread is blocked waiting for this condition variable,
        /// one will be woken up.
        /// @note Only threads that started waiting prior to this call will be
        /// woken up.
#if defined(_WIN32)
        void NotifyOne();
#else
        inline void NotifyOne()
        {
            pthread_cond_signal(&mHandle);
        }
#endif
        
        /// Notify all threads that are waiting for the condition.
        /// All threads that are blocked waiting for this condition variable will
        /// be woken up.
        /// @note Only threads that started waiting prior to this call will be
        /// woken up.
#if defined(_WIN32)
        void NotifyAll();
#else
        inline void NotifyAll()
        {
            pthread_cond_broadcast(&mHandle);
        }
#endif
        
        _TTHREAD_DISABLE_ASSIGNMENT(ConditionVariable)
        
    private:
#if defined(_WIN32)
        void _wait();
        HANDLE mEvents[2];                  ///< Signal and broadcast event HANDLEs.
        unsigned int mWaitersCount;         ///< Count of the number of waiters.
        CRITICAL_SECTION mWaitersCountLock; ///< Serialize access to mWaitersCount.
#else
        pthread_cond_t mHandle;
#endif
    };
    
    
#if _WIN32
    
    typedef HANDLE CUTThread;
    typedef unsigned(WINAPI *CUT_THREADROUTINE)(void *);
    
    struct CUTBarrier
    {
        CRITICAL_SECTION criticalSection;
        HANDLE barrierEvent;
        PDP::UInteger releaseCount;
        PDP::UInteger count;
    };
    
#define CUT_THREADPROC unsigned WINAPI
#define  CUT_THREADEND return 0
    
#else
    //POSIX threads.
#define CUT_RETURN_TYPE void *
    
    
    
    
    
    typedef pthread_t CUTThread;
    typedef void *(*CUT_THREADROUTINE)(void *);
#define CUT_THREADPROC void*
#define  CUT_THREADEND return 0
    
    struct CUTBarrier
    {
        pthread_mutex_t mutex;
        pthread_cond_t conditionVariable;
        PDP::UInteger  releaseCount;
        PDP::UInteger count;
    };
    
#endif
    
    
    inline void Yield()
    {
#if defined(_WIN32)
        //Sleep(0);
        
        SwitchToThread();//windows equivalient of sched_yield()
#else
        sched_yield();
#endif
    }
    
    
    
#ifdef __cplusplus
    extern "C"
    {
#endif
        
        //Create thread.
        extern CUTThread CutStartThread(CUT_THREADROUTINE, void *data);
        
        //Wait for thread to finish.
        extern void CutEndThread(CUTThread thread);
        
        //Destroy thread.
        extern void CutDestroyThread(CUTThread thread);
        
        //Wait for multiple threads.
        extern void CutWaitForThreads(const CUTThread *threads, const PDP::UInteger & num);
        
        //Create barrier.
        CUTBarrier CutCreateBarrier(const PDP::UInteger & releaseCount);
        
        //Increment barrier. (excution continues)
        extern void CutIncrementBarrier(CUTBarrier *barrier);
        
        //Wait for barrier release.
        extern void CutWaitForBarrier(CUTBarrier *barrier);
        
        //Destory barrier
        extern void CutDestroyBarrier(CUTBarrier *barrier);
        
        
#ifdef __cplusplus
    } //extern "C"
#endif
    
    
    class ThreadIdentification
    {
    public:
        
        /// Default constructor.
        
        /// The default constructed ID is that of thread without a thread of
        
        /// execution.
        
        ThreadIdentification() : mId(0) {};
        
        
        ThreadIdentification(unsigned long int aId) : mId(aId) {};
        
        
        ThreadIdentification(const ThreadIdentification& aId) : mId(aId.mId) {};
        
        
        inline ThreadIdentification & operator=(const ThreadIdentification &aId)
        
        {
            
            mId = aId.mId;
            
            return *this;
            
        }
        
        
        inline friend bool operator==(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId == aId2.mId);
            
        }
        
        
        inline friend bool operator!=(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId != aId2.mId);
            
        }
        
        
        inline friend bool operator<=(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId <= aId2.mId);
            
        }
        
        
        inline friend bool operator<(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId < aId2.mId);
            
        }
        
        
        inline friend bool operator>=(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId >= aId2.mId);
            
        }
        
        
        inline friend bool operator>(const ThreadIdentification &aId1, const ThreadIdentification &aId2)
        
        {
            
            return (aId1.mId > aId2.mId);
            
        }
        
        
    private:
        
        unsigned long int mId;
        
    };
    
    
    class ThreadTaskResource: public PDP::Memory::AutomaticReferencedCountingObject
    {
    private:
        PDP::Task * CurrentTask;
        LD::CUTThread CurrentThread;
    public:
        static ThreadIdentification _pthread_t_to_ID(const pthread_t &aHandle)
        {
            static Mutex idMapLock;
            static std::map<pthread_t, unsigned long int> idMap;
            static unsigned long int idCount(1);
            
            LockGuard<Mutex> guard(idMapLock);
            if(idMap.find(aHandle) == idMap.end())
                idMap[aHandle] = idCount ++;
            return ThreadIdentification(idMap[aHandle]);
        }
        LD::ThreadIdentification GetIdentification() const
        {
            //if(!joinable())
                //return PDP::UInteger();
#if defined(_TTHREAD_WIN32_)
            return PDP::ThreadIdentification((unsigned long int) CurrentThread);
#elif defined(_TTHREAD_POSIX_)
            return _pthread_t_to_ID(CurrentThread);
#endif
        }
        
        ThreadTaskResource(PDP::Task * resource):CurrentTask(resource)
        {
            
        }
        
        
        static void * ThreadFunction(void * data)
        {
            ThreadTaskResource * resource = (ThreadTaskResource*)data;
            resource->CurrentTask->Run();
            return nullptr;
        }
        
        ~ThreadTaskResource()
        {
            LD::CutWaitForThreads(&CurrentThread, 1);
            delete CurrentTask;
            
        }
        
        void Start()
        {
            auto function = [](void * data)->void *
            {
                ThreadTaskResource * resource = (ThreadTaskResource*)data;
                resource->CurrentTask->Run();
                return nullptr;
            };
            
            
            LD::CutStartThread(&LD::ThreadTaskResource::ThreadFunction, this);
        }
    };
    class Thread
    {
    private:
        PDP::Memory::AutomaticReferenceCounter<ThreadTaskResource> CurrentTask;
    public:
        
        static LD::ThreadAffinity GetThreadAffinity();
        static PDP::UInteger GetHardwareConcurrency();
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @return PDP::Promise - The promise made between the thread that will actually perform the work and the calling thread expecting the work to be fufilled
         @brief The methodology of this function consists of performing a task on another thread and waiting on the calling thread for a singular item to be returned at some point
         */
        template<typename T, template<typename> class Functor, typename ... Args>
        inline LD::Promise<T> EnqueueWithPromise(const  Functor<T(Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @return PDP::Promise - The promise made between the thread that will actually perform the work and the calling thread expecting the work to be fufilled
         @brief The methodology of this function consists of performing a task on another thread and waiting on the calling thread for a singular item to be returned at some point.  This function is an objected oriented spin on the vanilla version of this function which accepts functor objects.
         @warning The object will be copied to the thread which will be executing the workload.  There will be no reference to it from the calling thread.
         */
        template<typename T, typename Object, typename ... Args>
        inline LD::Promise<T> EnqueueWithPromise(const Object & object, T (Object::*MemberPointer)(Args...) const , const LD::Detail::Decay_T<Args> & ... arguements);
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @return PDP::Promise - The promise made between the thread that will actually perform the work and the calling thread expecting the work to be fufilled
         @brief The methodology of this function consists of performing a task on another thread and waiting on the calling thread for a singular item to be returned at some point.  This function is an objected oriented spin on the vanilla version of this function which accepts functor objects.
         @warning The object will be copied to the thread which will be executing the workload.  There will be no reference to it from the calling thread.
         */
        template<typename T, typename Object, typename ... Args>
        inline LD::Promise<T> EnqueueWithPromise(Object & object, T (Object::*MemberPointer)(Args...)  , const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread.  An example of this would be downloading a file and streaming back the progress indicators of the download
         */
        template<typename T, template<typename> class Functor, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment(const  Functor<void(PDP::Commitment<T>, Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread.  An example of this would be downloading a file and streaming back the progress indicators of the download
         */
        template<typename T, typename U, typename Object, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment(const Object & object, T (Object::*MemberPointer)(PDP::Commitment<U>,Args...) const , const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread.  An example of this would be downloading a file and streaming back the progress indicators of the download
         */
        template<typename T, typename U, typename Object, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment( Object & object, T (Object::*MemberPointer)(PDP::Commitment<U>,Args...)  , const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread while also returning back a singular object of interest.  Once more an example can be seen when downloading a file.  Where you might be interested in the data of the file which may be best seen as a string whereas the progress that would be streamed would probably be fully realized as a double
         */
        template<typename T, typename U, template<typename> class Functor, typename ... Args>
        inline LD::Pair<LD::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment(const  Functor<T(PDP::Commitment<U>, Args...)> & delegate, const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread while also returning back a singular object of interest.  Once more an example can be seen when downloading a file.  Where you might be interested in the data of the file which may be best seen as a string whereas the progress that would be streamed would probably be fully realized as a double
         */
        template<typename T, typename U, typename Object, typename ... Args>
        inline LD::Pair<LD::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment(const Object & object, T (Object::*MemberPointer)(PDP::Commitment<U>,Args...) const , const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread while also returning back a singular object of interest.  Once more an example can be seen when downloading a file.  Where you might be interested in the data of the file which may be best seen as a string whereas the progress that would be streamed would probably be fully realized as a double
         */
        template<typename T, typename U, typename Object, typename ... Args>
        inline LD::Pair<LD::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment( Object & object, T (Object::*MemberPointer)(PDP::Commitment<U>,Args...)  , const LD::Detail::Decay_T<Args> & ... arguements);
        
        
        
        
        
        inline LD::ThreadIdentification GetIdentification() const;
        
    };
    
}



#endif //MULTITHREADING_H
