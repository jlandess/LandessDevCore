//
//  GrandCentralDispatch.h
//  DataStructures5
//
//  Created by James Landess on 2/17/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef GrandCentralDispatch_h
#define GrandCentralDispatch_h

#include <Definitions/Common.h>
#include <Primitives/Threads/Atomic/Atomic.h>
#include <Definitions/Integer.h>
#include <Primitives/Functor/fixed_size_function.hpp>
#include <Primitives/Threads/Locks/SpinLock.h>
#include "concurrentqueue.h"
#include <Primitives/Threads/Thread.h>
#include <Definitions/TimeExtension.h>
#include <Algorithms/CompileTimeControlFlow.h>
#include <Primitives/MVC/SchedulingEvent.h>
#include <Primitives/MVC/Scheduling.h>
namespace LD
{

    template<typename ... A>
    class TaskDispatcherMetaData
    {
    private:
        PDP::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> mData;

    public:

        TaskDispatcherMetaData()
        {
            this->mData = PDP::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>>(nullptr);
        }


        TaskDispatcherMetaData(const LD::Tuple<A...> & initialState, void * threadpool)
        {
            LD::Tuple<LD::Tuple<A...>,void*> init;
            LD::Get<LD::Tuple<A...>>(init) = initialState;
            LD::Get<void*>(init) = threadpool;

           this->mData = PDP::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>>(new LD::Tuple<LD::Tuple<A...>,void*>{init});

        }
        void operator()()
        {
            LD::For<sizeof...(A)>([&](auto I)
            {
                LD::Tuple<LD::Tuple<A...>,void*> * data = mData.Get();
               // LD::Tuple<A...> & tuple =  LD::Get<LD::Tuple<A...>>(*data);
                //LD::Get<I>(tuple)();
                return true;
            });
        }


        PDP::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> Data()
        {
            return this->mData;
        }
        PDP::Second<LD::Float > AccumulatedPeriod() const
        {
            return PDP::Second<LD::Float >(0.0);
        }
    };





    template<typename Context,class = void>
    class BasicTaskDispatcher;



    /**
     *
     * @tparam A
     */
    template<typename ... A>
    class BasicTaskDispatcher<LD::VariadicPack<A...>,LD::Enable_If_T <
            LD::Require<
                    LD::Either<
                            LD::Require<
                                    (LD::ConvertiblyCallable<A, bool(const LD::ApplicaitonStartedEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    (LD::ConvertiblyCallable<A, bool(const LD::ApplicationFrameStartedEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    (LD::ConvertiblyCallable<A, PDP::Second<LD::Float >(const LD::ApplicationPeriodEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    (LD::ConvertiblyCallable<A, void(const LD::ApplicationExecutionEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    (LD::ConvertiblyCallable<A, void(const LD::ApplicationFrameEndedEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    (LD::ConvertiblyCallable<A, void(const LD::ApplicationQuittingEvent<LD::ThreadAffinity> &)>::Value() && ...),
                                    LD::IsUnique<A...>
                                    >
                            ,
                            (sizeof...(A) == 0)>
                    >>>
    {
    private:
        //todo Replace LD::fized_size_function<void()> with LD::Variant<LD::fized_size_function,LD::LightWeightDelegate<void()>,LD::Function<void()>> to take advantage of optimizations
        //and dealing with very large functor calls
        moodycamel::ConcurrentQueue<LD::fixed_size_function<void()>>  Tasks;
        PDP::DataStructures::Array<LD::Tuple<PDP::CUTThread,TaskDispatcherMetaData<A...>>> Threads;
        PDP::Atomic<LD::UInteger> WorkingStatus;
        PDP::Atomic<LD::UInteger> RequestedNumberOfThreads;
        PDP::SpinLock CurrentSpinLock;

        static void * InternalThreadFunction(void * data)
        {
            LD::Tuple<LD::Tuple<A...>,void*> * passedInContext = (LD::Tuple<LD::Tuple<A...>,void*>*)data;


            //LD::UInteger threadAffinity = PDP::Thread::GetThreadAffinity();
            //BasicTaskDispatcher<LD::VariadicPack<A...>> * currentPool = (BasicTaskDispatcher<LD::VariadicPack<A...>>*)data;



            BasicTaskDispatcher<LD::VariadicPack<A...>> * currentPool = (BasicTaskDispatcher<LD::VariadicPack<A...>> *)LD::Get<void*>(*passedInContext);
            LD::Application<LD::ThreadAffinity> application;
            LD::Tuple<A...> & schedulingHeuerestics = LD::Get<LD::Tuple<A...>>(*passedInContext);

            application([&](const LD::ApplicaitonStartedEvent<LD::ThreadAffinity> & event)
            {

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        LD::Get<I>(schedulingHeuerestics)(event);
                        return true;
                    });
                }

                return true;
            });



            application([&](const LD::ApplicationFrameStartedEvent<LD::ThreadAffinity> & event)
            {
                bool functorRunnable = true;
                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        functorRunnable = (functorRunnable && LD::Get<I>(schedulingHeuerestics)(event));
                        return true;
                    });
                }
                return !currentPool->GetWorkingStatus() && functorRunnable;
            });



            application([&](const LD::ApplicationPeriodEvent<LD::ThreadAffinity> & event)
            {
                PDP::Second<LD::Float> period = PDP::Second<LD::Float >(0);

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        PDP::Second<LD::Float> currentPeriod = LD::Get<I>(schedulingHeuerestics)(event);
                        period = PDP::Second<LD::Float>(period.GetValue()+currentPeriod.GetValue());

                        return true;
                    });
                }
                period = PDP::Second<LD::Float >(period.GetValue()+0.01666666667);
                //todo - change to dynamic tick based on load
                return period;
            });


            application([&](const LD::ApplicationExecutionEvent<LD::ThreadAffinity> & event)
            {

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        LD::Get<I>(schedulingHeuerestics)(event);
                        return true;
                    });
                }
                LD::fixed_size_function<void()> possibleTask;
                if (currentPool->Tasks.try_dequeue(possibleTask))
                {
                    possibleTask();
                }

            });



            application([&](const LD::ApplicationFrameEndedEvent<LD::ThreadAffinity> & event)
            {

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        LD::Get<I>(schedulingHeuerestics)(event);
                        return true;
                    });
                }
            });


            application([&](const LD::ApplicationQuittingEvent<LD::ThreadAffinity> & event)
            {

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        LD::Get<I>(schedulingHeuerestics)(event);
                        return true;
                    });
                }
            });
            /*
            while (currentPool->GetWorkingStatus() == true)
            {

                LD::fixed_size_function<void()> possibleTask;
                if (currentPool->Tasks.try_dequeue(possibleTask))
                {
                    possibleTask();
                }

                usleep(PDP::Second<LD::Float>(0.01666666667));

            }
             */


            PDP::DataStructures::Timer currentTimer;
            LD::ThreadAffinity affinity = PDP::Thread::GetThreadAffinity();
            LD::MainLoop(application,currentTimer,affinity);
            return nullptr;
        }


        inline void InitThreads(const LD::Tuple<A...> & init)
        {

            LD::UInteger numberOfInitialThreads = RequestedNumberOfThreads.load(PDP::AcquireRelease);


            for (LD::UInteger n = 0; n<numberOfInitialThreads; ++n )
            {

                TaskDispatcherMetaData<A...> threadMetaData(init,this);

                //PDP::CUTThread thread = PDP::CutStartThread(&BasicTaskDispatcher::InternalThreadFunction, this);


                PDP::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> managedPtr = threadMetaData.Data();

                //this is only safe because the pointer is being managed by a shared pointer and will only go out of scope when the instance of this class does.
                PDP::CUTThread thread = PDP::CutStartThread(&BasicTaskDispatcher::InternalThreadFunction, managedPtr.Get());
                LD::Tuple<PDP::CUTThread ,LD::TaskDispatcherMetaData<A...>> blob;
                LD::Get<PDP::CUTThread>(blob) = thread;
                LD::Get< TaskDispatcherMetaData<A...>>(blob) = threadMetaData;
                this->Threads.PushBack(blob);

            }

        }
    public:

        /*
        inline ThreadPool(const LD::ElementReference<LD::System> & system)
        {

            this->RequestedNumberOfThreads.store(system->GetAmountOfLogicalCores(),PDP::AcquireRelease);

            this->WorkingStatus.store(true,PDP::AcquireRelease);

            InitThreads();
        }

         */
        inline BasicTaskDispatcher(const LD::UInteger & numberOfThreads, const LD::Tuple<A...>& init = LD::Tuple<A...>{})
        {
            this->RequestedNumberOfThreads.store(numberOfThreads,PDP::AcquireRelease);

            this->WorkingStatus.store(true,PDP::AcquireRelease);

            InitThreads(init);
        }


        inline ~BasicTaskDispatcher()
        {

            this->RequestedNumberOfThreads.store(0,PDP::AcquireRelease);

            this->WorkingStatus.store(false,PDP::AcquireRelease);

            for(LD::UInteger n = 0 ;n<this->Threads.GetSize();++n)
            {
                PDP::CutEndThread(LD::Get<PDP::CUTThread>(this->Threads[n]));
            }
            //PDP::CutWaitForThreads(this->Threads.GetBuffer(), this->Threads.GetSize());

        }

        template<typename F, typename ... Args>
        inline PDP::Promise<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> EnqueueWithPromise(
                F && function,
                Args && ... arguements)
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;

            PDP::Promise<Ret> promise;

            PackagedTask<F, Ret(LD::Decay_T<Args>...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),promise);

            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });

            this->Tasks.enqueue(workerFunction);


            return promise;
        }


        template<typename F, typename ... Args>
        inline PDP::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> EnqueueWithCommitment(
                F && function,
                Args && ... arguements)
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;
            PDP::Commitment<Ret> commitment;

            PackagedTask<F, Ret(Args...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),commitment);


            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });

            this->Tasks.enqueue(workerFunction);

            return currentPackage;

        }

        template<typename F, typename ... Args>
        inline PDP::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> & EnqueueWithCommitment(
                const PDP::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> & commitment,
                F && function,
                Args && ... arguements)
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;

            PackagedTask<F, Ret(Args...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),commitment);


            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });



            this->Tasks.enqueue(workerFunction);

            return currentPackage;

        }

        inline const LD::UInteger  GetAmountOfActiveThreads()
        {
            LD::UInteger amount;
            CurrentSpinLock.Lock();
            {
                amount = this->Threads.GetSize();
            }
            CurrentSpinLock.Unlock();

            return amount;
        }

        inline const LD::UInteger GetAmountOfRequestedThreads() const
        {


            return RequestedNumberOfThreads.load(PDP::AcquireRelease);;
        }


        const bool GetWorkingStatus() const
        {

            return WorkingStatus.load(PDP::AcquireRelease);
        }
    };

    using TaskDispatcher = LD::BasicTaskDispatcher<LD::VariadicPack<>>;

    template<typename ...A>
    using ParametricTaskDispatcher = LD::BasicTaskDispatcher<LD::VariadicPack<A...>>;

}
#endif /* GrandCentralDispatch_h */
