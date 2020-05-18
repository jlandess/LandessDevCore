//
//  MultiThreadingSandbox.h
//  DataStructures
//
//  Created by James Landess on 9/17/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef __DataStructures__THREADPOOL__
#define __DataStructures__THREADPOOL__

#include <Primitives/General/Array.h>
#include <Memory/AutomaticReferenceCounting.h>
#include <Primitives/Threads/Promise.h>
#include <Primitives/Threads/Commitment.h>
#include <Primitives/General/PackagedTask.h>
#include <Primitives/Threads/Thread.h>
#include <Primitives/MVC/Event.h>
#include <Definitions/TimeExtension.h>
#include <Primitives/Threads/Atomic/Atomic.h>
#include <Memory/shared_ptr.hpp>
namespace PDP
{
    class ThreadPoolTaskResource: public PDP::Memory::AutomaticReferencedCountingObject
    {
    private:
        PDP::Task * CurrentTask;
    public:
        
        ThreadPoolTaskResource(PDP::Task * currentTask):CurrentTask(currentTask)
        {
            
        }
        ~ThreadPoolTaskResource()
        {
            
            delete CurrentTask;
        }
        void Run()
        {
            
            CurrentTask->Run();
        }
    };
    
    
    
    class ThreadPoolThreadResource
    {
    private:
        PDP::CUTThread Thread;
        PDP::UInteger Identification;
    public:
        
        
        inline ThreadPoolThreadResource(const PDP::CUTThread & thread, const PDP::UInteger & identification):Thread(thread),Identification(identification)
        {
            
        }
        
        inline ThreadPoolThreadResource():Identification(0)
        {
            
        }
        
        inline ~ThreadPoolThreadResource()
        {
            if (Identification != 0)
            {
                PDP::CutWaitForThreads(&Thread, 1);
            }
            
        }
        
        inline const PDP::CUTThread & GetThread() const
        {
            return this->Thread;
        }
        
        
        inline const PDP::UInteger & GetIdentification() const
        {
            return this->Identification;
        }
        
        
        inline const bool operator > (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification > threadResource.Identification;
        }
        
        inline const bool operator < (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification < threadResource.Identification;
        }
        
        inline const bool operator == (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification == threadResource.Identification;
        }
        
        inline const bool operator >= (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification >= threadResource.Identification;
        }
        
        inline const bool operator <= (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification <= threadResource.Identification;
        }
        
        inline const bool operator != (const ThreadPoolThreadResource & threadResource) const
        {
            return this->Identification != threadResource.Identification;
        }
    };
    
    
    class ThreadPool;
    
    class LocalThreadPoolContext
    {
    private:
        PDP::ElementReference<ThreadPool> CurrentInstance;
        PDP::ElementReference<moodycamel::ConcurrentQueue<PDP::Memory::AutomaticReferenceCounter<ThreadPoolTaskResource>>> QueueReference;
        PDP::UInteger ThreadID;
        
    public:
        
        inline LocalThreadPoolContext(const PDP::ElementReference<ThreadPool> & currentInstance, PDP::ElementReference<moodycamel::ConcurrentQueue<PDP::Memory::AutomaticReferenceCounter<ThreadPoolTaskResource>>> & queue, const PDP::UInteger & identification):CurrentInstance(currentInstance),QueueReference(queue),ThreadID(identification)
        {
            
        }
        
        
        
        inline const PDP::ElementReference<ThreadPool> & GetThreadPoolCurrentInstance() const
        {
            return CurrentInstance;
        }
        
        inline const PDP::ElementReference<moodycamel::ConcurrentQueue<PDP::Memory::AutomaticReferenceCounter<ThreadPoolTaskResource>>> & GetQueue() const
        {
            return QueueReference;
        }
        
        
        inline const PDP::UInteger & GetThreadID() const
        {
            return ThreadID;
        }
    };
    
    class LocalThreadPoolPolicy
    {
    private:
        
    public:
        
        inline const bool OnThreadPoolEvent(const PDP::EventMetaData & eventMetaData, const PDP::ElementReference<ThreadPool> & instance, const PDP::UInteger & threadID);
    };
    
    class ThreadPool
    {
    public:
        
    private:
        struct ThreadPoolCommunicationPacket
        {
            ThreadPool * CurrentInstance;
            PDP::UInteger Index;
        };
        
        
        
        PDP::DataStructures::Array<PDP::CUTThread> Threads;
        
        PDP::Atomic<PDP::UInteger> RequestedNumberOfThreads;
        
        moodycamel::ConcurrentQueue<PDP::SharedPointer<ThreadPoolTaskResource>> * Tasks;
        
        PDP::Atomic<PDP::UInteger> WorkingStatus;
        
        PDP::SpinLock CurrentSpinLock;
        
        
        inline void SetupUpThread(const PDP::UInteger & index)
        {
            
        }
        
        static void * InternalThreadFunction(void * data)
        {
            ThreadPool * currentPool = (ThreadPool*)data;
            
            while (currentPool->GetWorkingStatus() == true)
            {
                //std::cout << currentInstance->GetWorkLoad() << std::endl;
                //the task will deallocate on its own when all references have been let go of it.
                PDP::SharedPointer<ThreadPoolTaskResource> possibleTask;
                
                if (currentPool->Tasks->try_dequeue(possibleTask))
                {
                    possibleTask->Run();
                    //currentPool->CompletedTask();
                    
                    
                }
                
                //sleep for a reasonable amount of time - this will be replaced with a finite state machine to more accurately determine when and how the thread should sleep
                usleep(PDP::Second<PDP::Float>(0.01666666667));
                
            }
            
            return nullptr;
        }
        
        inline void LazilySetupThreads()
        {
            
            if (Tasks == nullptr)
            {
                Tasks = new moodycamel::ConcurrentQueue<PDP::SharedPointer<ThreadPoolTaskResource>>();
            }
            
            
            if (Threads.GetSize() == 0 && RequestedNumberOfThreads.load(PDP::AcquireRelease) > 0)
            {
                //Threads.Reserve(RequestedNumberOfThreads);
                //std::cout << "Amount of requested threads: " << RequestedNumberOfThreads.load(turf::AcquireRelease) << std::endl;
                for (PDP::UInteger n = 0; n<RequestedNumberOfThreads.load(PDP::AcquireRelease); ++n )
                {
                    /*
                     auto function = [](void * data)->void *
                     {
                     
                     
                     
                     ThreadPool * currentPool = (ThreadPool*)data;
                     
                     while (currentPool->GetWorkingStatus() == true)
                     {
                     //std::cout << currentInstance->GetWorkLoad() << std::endl;
                     //the task will deallocate on its own when all references have been let go of it.
                     PDP::Memory::AutomaticReferenceCounter<ThreadPoolTaskResource> possibleTask;
                     
                     if (currentPool->Tasks->try_dequeue(possibleTask))
                     {
                     possibleTask->Run();
                     //currentPool->CompletedTask();
                     
                     
                     }
                     
                     //sleep for a reasonable amount of time - this will be replaced with a finite state machine to more accurately determine when and how the thread should sleep
                     usleep(PDP::Second<LD::Float>(0.01666666667));
                     
                     }
                     
                     return nullptr;
                     };
                     */
                    
                    
                    PDP::CUTThread thread = PDP::CutStartThread(&ThreadPool::InternalThreadFunction, this);
                    
                    
                    this->Threads.PushBack(thread);
                    
                    
                }
            }
        }
        
        void RemoveThreadByIndex(const PDP::UInteger & index);
        
        
        
    public:
        
        
        inline ThreadPool(const PDP::UInteger & numberOfThread = PDP::Thread::GetHardwareConcurrency(), const PDP::UInteger & numberOfPriorities = 1);
        
        inline ~ThreadPool();

        
        
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
        inline PDP::Promise<T> EnqueueWithPromise(const  Functor<T(Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements);
        
        
        
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
        inline PDP::Promise<T> EnqueueWithPromise(const Object & object, T (Object::*MemberPointer)(Args...) const , const PDP::Decay_T<Args> & ... arguements);
        
        
        
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
        inline PDP::Promise<T> EnqueueWithPromise(Object & object, T (Object::*MemberPointer)(Args...)  , const PDP::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Functor - The type of functor which will be invoked upon the thread asked to do work.  It can be any class which falls under the classification of functor, such as std::function, PDP::Lambda, and PDP::LightWeightDelegate
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param delegate - The actual instansiation of the functor object you chose, the compiler will deduce your functor's class type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread.  An example of this would be downloading a file and streaming back the progress indicators of the download
         */
        template<typename T, template<typename> class Functor, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment(const  Functor<void(PDP::Commitment<T>, Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and waiting on the calling thread for a singular item to be returned at some point.  This function is an objected oriented spin on the vanilla version of this function which accepts functor objects.
         @warning The object will be copied to the thread which will be executing the workload.  There will be no reference to it from the calling thread.
         */
        template<typename T, typename Object, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment(const Object & object, T (Object::*MemberPointer)(PDP::Commitment<T>,Args...) const ,  const PDP::Decay_T<Args> & ... arguements);
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and waiting on the calling thread for a singular item to be returned at some point.  This function is an objected oriented spin on the vanilla version of this function which accepts functor objects.
         @warning The object will be copied to the thread which will be executing the workload.  There will be no reference to it from the calling thread.
         */
        template<typename T, typename Object, typename ... Args>
        inline PDP::Commitment<T> EnqueueWithCommitment(Object & object, T (Object::*MemberPointer)(PDP::Commitment<T>,Args...) ,  const PDP::Decay_T<Args> & ... arguements);
        
        
        
        /**
         @tparam T - The expected return typed to be communicated back from the thread that was asked to do the work to the calling thread with the concept of a PDP::Promise
         @tparam Object - The type of object you want to push onto the called thread to execute the given member function
         @tparam Args - The deduced template arguements to help accomodate any type of function signature which can be passed in.  This allows arguements to be passed in along with the enquing of the function unto the thread that will actually do the work
         @param object - The actual instansiation of the  object you chose, the compiler will deduce you class's type upon utilizing the function (EnqueueWithPromise is technically a family of functions as opposed to one singular implementation).
         @param MemberPointer - The member function you want the object to call on the thread which will actually execute the workload
         @param arguements - The arguements that match the function signature of the passed in functor arguement.  Perfect forwarding isn't required because of the compile time effectiveness of PDP::Decay_T.  Normally it would be very important to retain if the functor passed in took arguements in the form of const T &, const T, T, T& etc; however, because the function is being called on another thread all of the arguements are copied anyway to an underlying PDP::PackagedTask which prepares the functor to be called on another thread with the arguements.
         @brief The methodology of this function consists of performing a task on another thread and streaming data back to the calling thread while also returning back a singular object of interest.  Once more an example can be seen when downloading a file.  Where you might be interested in the data of the file which may be best seen as a string whereas the progress that would be streamed would probably be fully realized as a double
         */
        template<typename T, typename U, template<typename> class Functor, typename ... Args>
        inline PDP::DataStructures::Pair<PDP::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment(const  Functor<T(PDP::Commitment<U>, Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements);
        
        
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
        inline PDP::DataStructures::Pair<PDP::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment(const Object & object, T (Object::*MemberPointer)(PDP::Commitment<T>,Args...) const ,   const PDP::Decay_T<Args> & ... arguements);
        
        
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
        inline PDP::DataStructures::Pair<PDP::Promise<T>,PDP::Commitment<U>> EnqueueWithPromiseAndCommitment(Object & object, T (Object::*MemberPointer)(PDP::Commitment<T>,Args...) ,   const PDP::Decay_T<Args> & ... arguements);
        
        
        /**
         @brief Adding a thread does not necessarily immediately add a thread to the system immediately (in the default configuration this assumption is true), it may take a few cycles to add the thread to the threadpool and to fully utilize it.
         */
        inline void AddThread();
        
        /**
         @brief Removing a thread does not necessarily immediately remove a thread immediately.  It will be removed when the threadpool can rectify the numerical despairity (the rate at which this happens may change based on the threadpool configuration)
         */
        inline void RemoveThread();
        
        /**
         @brief Setting the amount of threads does not necessarily immediately set the amount of threads in current use immediately.  It will be removed when the threadpool can rectify the numerical despairity (the rate at which this happens may change based on the threadpool configuration)
         */
        inline void SetAmountOfThreads(const PDP::UInteger & requestedNumberOfThreads);
        
        
        /**
         @param metaData - The Application meta data that will be passed in through the Application run loop
         @brief Although this function will never be directly called nor is it a necessary function for all applications.  When this threadpool is utilized within the PDP threadpool system will
         allow it to manage it's threads dynamically and release/create threads as workload situations change throughout the lifetime of the application.  This will also allow the state machine in each thread to be altered
         in real time.
         @todo Add a class which allows you to tune how the Threadpool manages the threads or to ask it to not manage them at all and stay with a static number.  Possible implementation of a response function.
         Allowing developers to specify under certain work loads how many threads should be active.  Possible response function for the internal state machine for each thread.
         */
        inline void Tick(const PDP::EventMetaData & metaData);
        
        
        
        
        inline const PDP::UInteger  GetAmountOfActiveThreads()
        {
            PDP::UInteger amount;
            CurrentSpinLock.Lock();
            {
                amount = this->Threads.GetSize();
            }
            CurrentSpinLock.Unlock();
            
            return amount;
        }
        
        inline const PDP::UInteger GetAmountOfRequestedThreads() const
        {
            
            
            return RequestedNumberOfThreads.load(PDP::AcquireRelease);;
        }
        
        
        const bool GetWorkingStatus() const
        {
            
            return WorkingStatus.load(PDP::AcquireRelease);
        }
        
    };
    
    
    inline ThreadPool::ThreadPool(const PDP::UInteger & numberOfThread, const PDP::UInteger & numberOfPriorities):Tasks(nullptr)
    {
        
        RequestedNumberOfThreads.store(numberOfThread,PDP::AcquireRelease);
        
        WorkingStatus.store(true,PDP::AcquireRelease);
        
    }
    
    inline ThreadPool::~ThreadPool()
    {
        
        RequestedNumberOfThreads.store(0,PDP::AcquireRelease);
        
        WorkingStatus.store(false,PDP::AcquireRelease);
        
        PDP::CutWaitForThreads(Threads.GetBuffer(), Threads.GetSize());
        
    }
    
    
    
    void ThreadPool::Tick(const PDP::EventMetaData &metaData)
    {
        //application stub - do nothing for now
        
    }
    
    
    
    template<typename T, template<typename> class Functor, typename ... Args>
    inline PDP::Promise<T> ThreadPool::EnqueueWithPromise(const  Functor<T(Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements)
    {
        LazilySetupThreads();//utilize lazy instansiation to only get the threads up and running when they actually need to be
        //this could be improved upon if a designated "load" was setup so that only certain tasks when grouped up together run together - this would ensure fantastic multi-core IPC performance
        
        
        PDP::Promise<T> promise;
        
        PDP::PackagedTask<T(Args...),Functor> currentPackagedTask;
        
        //set the required information
        currentPackagedTask.SetArguements(arguements...);//perfect forwarding isn't required because I'm making the gaurantee that every single arguement will be in the form const T & item (this is due to the flexibility of PDP::Detay_T
        currentPackagedTask.SetDelegate(delegate);
        currentPackagedTask.SetPromise(promise);
        
        
        //putting it in an AutomaticReferenceCounter allows the allocated packaged task to deallocate when it needs to across threads.  As PDP::Memory::AutomaticReferenceCounter is an atomically based data structure
        PDP::SharedPointer<ThreadPoolTaskResource> newlyMadeTask = PDP::SharedPointer<ThreadPoolTaskResource>(new ThreadPoolTaskResource(new  PDP::PackagedTask<T(Args...),Functor>(currentPackagedTask)));
        
        
        
        
        Tasks->enqueue(newlyMadeTask);//add the task to the queue which will be executed at some point in the future
        return promise;
    }
    
    //commitment
    template<typename T, template<typename> class Functor, typename ... Args>
    inline PDP::Commitment<T> ThreadPool::EnqueueWithCommitment(const  Functor<void(PDP::Commitment<T>  , Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements)
    {
        LazilySetupThreads();
        
        PDP::Commitment<T> currentCommitment;
        
        PDP::PackagedTask<void(PDP::Commitment<T> ,Args...),Functor> currentPackagedTask;
        
        
        currentPackagedTask.SetArguements(arguements...);//perfect forwarding isn't required because I'm making the gaurantee that every single arguement will be in the form const T & item (this is due to the flexibility of PDP::Detay_T
        currentPackagedTask.SetDelegate(delegate);
        currentPackagedTask.SetCommitment(currentCommitment);
        
        
        PDP::SharedPointer<ThreadPoolTaskResource> newlyMadeTask = PDP::SharedPointer<ThreadPoolTaskResource>(new ThreadPoolTaskResource(new  PDP::PackagedTask<void(PDP::Commitment<T> &,Args...),Functor>(currentPackagedTask)));
        
        Tasks->enqueue(newlyMadeTask);//add the task to the queue which will be executed at some point in the future
        return currentCommitment;
    }
    
    template<typename T, typename U, template<typename> class Functor, typename ... Args>
    inline PDP::DataStructures::Pair<PDP::Promise<T>,PDP::Commitment<U>> ThreadPool::EnqueueWithPromiseAndCommitment(const  Functor<T(PDP::Commitment<U>, Args...)> & delegate, const PDP::Decay_T<Args> & ... arguements)
    {
        PDP::Promise<T> promise;
        PDP::Commitment<U> currentCommitment;
        
        
        PDP::DataStructures::Pair<PDP::Promise<T>,PDP::Commitment<U>> returnablePair(promise,currentCommitment);
        
        PDP::PackagedTask<T(PDP::Commitment<U> ,Args...),Functor> currentPackagedTask;
        
        currentPackagedTask.SetArguements(arguements...);//perfect forwarding isn't required because I'm making the gaurantee that every single arguement will be in the form const T & item (this is due to the flexibility of PDP::Detay_T
        currentPackagedTask.SetDelegate(delegate);
        currentPackagedTask.SetCommitment(currentCommitment);
        currentPackagedTask.SetPromise(promise);
        
        PDP::SharedPointer<ThreadPoolTaskResource> newlyMadeTask = PDP::SharedPointer<ThreadPoolTaskResource>(new ThreadPoolTaskResource(new  PDP::PackagedTask<T(PDP::Commitment<T> &,Args...),Functor>(currentPackagedTask)));
        
        
        Tasks->enqueue(newlyMadeTask);//add the task to the queue which will be executed at some point in the future
        
        return returnablePair;
    }
    
    
    inline void ThreadPool::AddThread()
    {
        ++RequestedNumberOfThreads;
    }
    
    inline void ThreadPool::RemoveThread()
    {
        --RequestedNumberOfThreads;
        
    }
    
    inline void ThreadPool::SetAmountOfThreads(const PDP::UInteger &requestedNumberOfThreads)
    {
        RequestedNumberOfThreads.store(requestedNumberOfThreads,PDP::AcquireRelease);
    }
    
    inline void ThreadPool::RemoveThreadByIndex(const PDP::UInteger &index)
    {
        CurrentSpinLock.Lock();
        {
            this->Threads.Remove(index);
        }
        CurrentSpinLock.Unlock();
    }
    
    inline const bool LocalThreadPoolPolicy::OnThreadPoolEvent(const PDP::EventMetaData &eventMetaData, const PDP::ElementReference<ThreadPool> &instance, const PDP::UInteger &threadID)
    {
        ThreadPool * pool = instance.GetPointer();
        
        
        
        
        return false;
    }
}
#endif /* defined(__DataStructures__MultiThreadingSandbox__) */

