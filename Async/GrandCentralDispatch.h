//
//  GrandCentralDispatch.h
//  DataStructures5
//
//  Created by James Landess on 2/17/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef GrandCentralDispatch_h
#define GrandCentralDispatch_hf

//#include "Definitions/Common.hpp"
#include "Atomic/Atomic.h"
#include "Definitions/Integer.hpp"
#include "Functor/fixed_size_function.hpp"
#include "Async/Locks/SpinLock.h"
#include "concurrentqueue.h"
#include "Async/Thread.h"
#include "Definitions/TimeExtension.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "MVC/SchedulingEvent.hpp"
#include "MVC/Scheduling.hpp"
#include "Primitives/General/Unit.hpp"
namespace LD
{

    template<typename ... A>
    class TaskDispatcherMetaData
    {
    private:
        LD::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> mData;
    public:

        TaskDispatcherMetaData()
        {
            this->mData = LD::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>>(nullptr);
        }


        TaskDispatcherMetaData(const LD::Tuple<A...> & initialState, void * threadpool)
        {
            LD::Tuple<LD::Tuple<A...>,void*> init;
            LD::Get<LD::Tuple<A...>>(init) = initialState;
            LD::Get<void*>(init) = threadpool;

           this->mData = LD::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>>(new LD::Tuple<LD::Tuple<A...>,void*>{init});

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


        LD::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> Data()
        {
            return this->mData;
        }
        LD::Second<LD::Float > AccumulatedPeriod() const
        {
            return LD::Second<LD::Float >(0.0);
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
                                    (LD::ConvertiblyCallable<A, LD::Second<LD::Float >(const LD::ApplicationPeriodEvent<LD::ThreadAffinity> &)>::Value() && ...),
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

        //PDP::DataStructures::Array<LD::Tuple<LD::CUTThread,TaskDispatcherMetaData<A...>>> Threads;
        std::vector<LD::Tuple<LD::CUTThread,TaskDispatcherMetaData<A...>>> Threads;
        LD::Atomic<LD::UInteger> WorkingStatus;
        LD::Atomic<LD::UInteger> RequestedNumberOfThreads;
        LD::SpinLock CurrentSpinLock;

        static void * InternalThreadFunction(void * data)
        {
            LD::Tuple<LD::Tuple<A...>,void*> * passedInContext = (LD::Tuple<LD::Tuple<A...>,void*>*)data;


            //LD::UInteger threadAffinity = PDP::Thread::GetThreadAffinity();
            //BasicTaskDispatcher<LD::VariadicPack<A...>> * currentPool = (BasicTaskDispatcher<LD::VariadicPack<A...>>*)data;

            BasicTaskDispatcher<LD::VariadicPack<A...>> * currentPool = (BasicTaskDispatcher<LD::VariadicPack<A...>> *)LD::Get<void*>(*passedInContext);
            /*
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
                LD::Second<LD::Float> period = LD::Second<LD::Float >(LD::Float(0));

                if constexpr (sizeof...(A) > 0)
                {
                    LD::For<sizeof...(A)>([&](auto I)
                    {
                        LD::Second<LD::Float> currentPeriod = LD::Get<I>(schedulingHeuerestics)(event);

                        period = LD::Second<LD::Float>(period.NativeRepresentation().Value()+currentPeriod.NativeRepresentation().Value());

                        return true;
                    });
                }

                period = LD::Second<LD::Float >(period.NativeRepresentation().Value()+0.01666666667);
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
             */

            while (currentPool->GetWorkingStatus() == true)
            {

                LD::fixed_size_function<void()> possibleTask;
                if (currentPool->Tasks.try_dequeue(possibleTask))
                {
                    possibleTask();
                }

                LD::Usleep(LD::Second<LD::Float>(0.01666666667));

            }



            //LD::Timer currentTimer;
            //LD::ThreadAffinity affinity = LD::Thread::GetThreadAffinity();
            //LD::MainLoop(application,currentTimer,affinity);
            return nullptr;
        }


        inline void InitThreads(const LD::Tuple<A...> & init)
        {

            LD::UInteger numberOfInitialThreads = RequestedNumberOfThreads.load(LD::AcquireRelease);


            for (LD::UInteger n = 0; n<numberOfInitialThreads; ++n )
            {

                TaskDispatcherMetaData<A...> threadMetaData(init,this);

                //PDP::CUTThread thread = PDP::CutStartThread(&BasicTaskDispatcher::InternalThreadFunction, this);


                LD::SharedPointer<LD::Tuple<LD::Tuple<A...>,void*>> managedPtr = threadMetaData.Data();

                //this is only safe because the pointer is being managed by a shared pointer and will only go out of scope when the instance of this class does.
                LD::CUTThread thread = LD::CutStartThread(&BasicTaskDispatcher::InternalThreadFunction, managedPtr.Get());
                LD::Tuple<LD::CUTThread ,LD::TaskDispatcherMetaData<A...>> blob;
                LD::Get<LD::CUTThread>(blob) = thread;
                LD::Get< TaskDispatcherMetaData<A...>>(blob) = threadMetaData;
                //this->Threads.PushBack(blob);
                this->Threads.push_back(blob);

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
            this->RequestedNumberOfThreads.store(numberOfThreads,LD::AcquireRelease);

            this->WorkingStatus.store(true,LD::AcquireRelease);

            InitThreads(init);
        }


        inline ~BasicTaskDispatcher()
        {

            this->RequestedNumberOfThreads.store(0,LD::AcquireRelease);

            this->WorkingStatus.store(false,LD::AcquireRelease);

            for(LD::UInteger n = 0 ;n<this->Threads.size();++n)
            {

                LD::CutEndThread(LD::Get<LD::CUTThread>(this->Threads[n]));
            }
            //PDP::CutWaitForThreads(this->Threads.GetBuffer(), this->Threads.GetSize());

        }

        template<typename F, typename ... Args>
        inline LD::Promise<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> EnqueueWithPromise(
                F && function,
                Args && ... arguements) noexcept
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;

            LD::Promise<Ret> promise;

            PackagedTask<F, Ret(LD::Detail::Decay_T<Args>...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),promise);

            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Detail::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });

            this->Tasks.enqueue(workerFunction);


            return promise;
        }


        template<typename F, typename ... Args>
        inline LD::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> EnqueueWithCommitment(
                F && function,
                Args && ... arguements) noexcept
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;
            LD::Commitment<Ret> commitment;

            PackagedTask<F, Ret(Args...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),commitment);


            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Detail::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });

            this->Tasks.enqueue(workerFunction);

            return currentPackage;

        }

        template<typename F, typename ... Args>
        inline LD::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> & EnqueueWithCommitment(
                const LD::Commitment<decltype(LD::Declval<F>()(LD::Declval<Args>()...)  )> & commitment,
                F && function,
                Args && ... arguements) noexcept
        {
            typedef decltype(LD::Declval<F>()(LD::Declval<Args>()...)  ) Ret;

            PackagedTask<F, Ret(Args...)> currentPackage(function,LD::MakeTuple(LD::Forward<Args>(arguements)...),commitment);


            LD::fixed_size_function<void()> workerFunction = LD::fixed_size_function<void()>([currentPackage]()
                                                                                             {
                                                                                                 PackagedTask<F, Ret(LD::Detail::Decay_T<Args>...)> localizedPackage = currentPackage;
                                                                                                 localizedPackage();
                                                                                             });



            this->Tasks.enqueue(workerFunction);

            return currentPackage;

        }

        inline const LD::UInteger  GetAmountOfActiveThreads() noexcept
        {
            LD::UInteger amount;
            CurrentSpinLock.Lock();
            {
                amount = this->Threads.size();
            }
            CurrentSpinLock.Unlock();

            return amount;
        }

        inline const LD::UInteger GetAmountOfRequestedThreads() const noexcept
        {


            return RequestedNumberOfThreads.load(LD::AcquireRelease);;
        }


        const bool GetWorkingStatus() const noexcept
        {

            return WorkingStatus.load(LD::AcquireRelease);
        }
    };

    using TaskDispatcher = LD::BasicTaskDispatcher<LD::VariadicPack<>>;

    template<typename ...A>
    using ParametricTaskDispatcher = LD::BasicTaskDispatcher<LD::VariadicPack<A...>>;

}
#endif /* GrandCentralDispatch_h */
