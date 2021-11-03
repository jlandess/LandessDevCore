//
// Created by phoenixflower on 8/3/20.
//

#ifndef LANDESSDEVCORE_MULTITENANTAPPLICATION_HPP
#define LANDESSDEVCORE_MULTITENANTAPPLICATION_HPP
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/IntegralConstant.hpp"
#include "Definitions/PDPUnits.hpp"
#include "Memory/ElementReference.h"
#include "TypeTraits/Detection.hpp"
#include "MVC/SchedulingEvent.hpp"
#include "MVC/SchedulingQuittingPredicate.hpp"
namespace LD
{
    template<typename T>
    class MultiTenantApplication;

    template<typename ... Executors, typename ... A>
    class MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>
    {
    private:
        template<typename T>
        class Bucket
        {
        private:
            using BucketExecutor = LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<T>::value),LD::ElementReference <LD::Detail::Decay_T<T>>,LD::Detail::Decay_T<T>>;
            T mCurrentExecutor;
            LD::Second<LD::Float> mPeriod;
            LD::Second<LD::Float> mElapsed;
            LD::Second<LD::Float> test = LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}};
            //double mAccumulated;

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::ConvertiblyCallable<F,bool(LD::ApplicationStartedEvent<Context...>)>::Value()>
            {

                return object(event);
            }


            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::CT::Negate(LD::ConvertiblyCallable<F,bool(LD::ApplicationStartedEvent<Context...>)>::Value())>
            {
                return LD::ApplicationQuittingPredicate{};
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationFrameStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::ConvertiblyCallable<F,bool(LD::ApplicationFrameStartedEvent<Context...>)>::Value()>
            {
                return object(event);
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationFrameStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::CT::Negate(LD::ConvertiblyCallable<F,bool(LD::ApplicationFrameStartedEvent<Context...>)>::Value())>
            {
                return LD::ApplicationQuittingPredicate{};
            }


            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationPeriodEvent<Context...> & event) noexcept -> LD::TypedRequires<PDP::Second<LD::Float>,LD::ConvertiblyCallable<F,PDP::Second<LD::Float>(LD::ApplicationPeriodEvent<Context...>)>::Value()>
            {
                return object(event);
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationPeriodEvent<Context...> & event) noexcept -> LD::TypedRequires<PDP::Second<LD::Float>,LD::CT::Negate(LD::ConvertiblyCallable<F,PDP::Second<LD::Float>(LD::ApplicationPeriodEvent<Context...>)>::Value())>
            {
                return PDP::Second<LD::Float>{0};
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationExecutionEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationExecutionEvent<Context...>)>::Value()>
            {
                return object(event);
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationExecutionEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationExecutionEvent<Context...>)>::Value())>
            {
                return ;
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationFrameEndedEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationFrameEndedEvent<Context...>)>::Value()>
            {
                return object(event);
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationFrameEndedEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationFrameEndedEvent<Context...>)>::Value())>
            {
                return ;
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationQuittingEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationQuittingEvent<Context...>)>::Value()>
            {
                return object(event);
            }

            template<typename F, typename ... Context>
            static auto Apply(F && object, const LD::ApplicationQuittingEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationQuittingEvent<Context...>)>::Value())>
            {
                return ;
            }
        public:
            inline constexpr Bucket() noexcept : mElapsed{LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}}},mPeriod{LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}}}
            {

            }

            inline constexpr Bucket(T && executor) noexcept: mElapsed{LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}}},mPeriod{LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}}},mCurrentExecutor{LD::Forward<T>(executor)}
            {

            }

            Bucket & operator = (const BucketExecutor & executor) noexcept
            {
                this->mCurrentExecutor = executor;
                return (*this);
            }

            bool operator()(const LD::ApplicationStartedEvent<A...> & applicationStartedEvent) noexcept
            {
                return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationStartedEvent);
            }

            bool operator()(const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent) noexcept
            {
                this->mElapsed += applicationFrameStartedEvent.GetElapsedTime();
                //this is the comparable of executing frames on the rising edge of the desired period
                if(this->mElapsed >= this->mPeriod)
                {
                    return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationFrameStartedEvent);
                }
                return true;
            }

            PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & applicationPeriodEvent) noexcept
            {
                this->mPeriod = MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationPeriodEvent);
                return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationPeriodEvent);
            }

            void operator()(const LD::ApplicationExecutionEvent<A...> & applicationExecutionEvent) noexcept
            {
                if(this->mElapsed >= this->mPeriod)
                {

                    return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),LD::ApplicationExecutionEvent<A...>{applicationExecutionEvent,this->mElapsed});
                }
                return;
            }

            void operator()(const LD::ApplicationFrameEndedEvent<A...> & applicationFrameEndedEvent) noexcept
            {
                if(this->mElapsed >= this->mPeriod)
                {
                    //reset it to 0 to alow it to re-accumulate
                    this->mElapsed = PDP::Second<LD::Float>{0};
                    return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationFrameEndedEvent);
                }
                return;
            }

            void operator()(const LD::ApplicationQuittingEvent<A...> & applicationQuittingEvent) noexcept
            {
                return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>::Bucket<T>::Apply(LD::Get(this->mCurrentExecutor),applicationQuittingEvent);
            }

        };
        static constexpr LD::UInteger NumberOfExecutors = sizeof...(Executors);
        using ExecutorList = LD::Tuple<Bucket<Executors>...>;;
        static_assert(LD::IsUnique<A...>,"LD::ScheduledExecutors should have a unique context");
        ExecutorList mExecutors;
    public:

        inline MultiTenantApplication(Executors && ... executors) noexcept
        {
            auto context = LD::MakeContext(LD::Forward<Executors>(executors)...);
            LD::For<NumberOfExecutors>([](
                    auto I,
                    auto && context,
                    ExecutorList & executors)
            {
                LD::Get<0>(LD::Get<I>(executors)) = LD::Get<I>(LD::Forward<decltype(context)>(context));
                return true;
            },LD::Forward<decltype(context)>(context),this->mExecutors);

        }

        inline MultiTenantApplication(const LD::Tuple<Executors...> & executors)noexcept
        {

            LD::For<NumberOfExecutors>([](
                    auto I,
                    auto && context,
                    ExecutorList & executors)
            {
                LD::Get<I>(executors) = LD::Get<I>(context);
                return true;
            },executors,this->mExecutors);
        }


        bool operator()(const LD::ApplicationStartedEvent<A...> & applicationStartedEvent) noexcept
        {
            bool shouldrun = true;
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    const LD::ApplicationStartedEvent<A...> & applicationStartedEvent,
                    bool & runnable) noexcept
            {


                auto & currentExecutor = LD::Get<I>(executors);

                bool shouldRun = currentExecutor(applicationStartedEvent);

                runnable = runnable && shouldRun;

                return true;

            },this->mExecutors,applicationStartedEvent,shouldrun);
            return shouldrun;
        }



        bool operator()(const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent) noexcept
        {
            bool runnable = true;
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    bool & runnable,
                    const LD::ApplicationFrameStartedEvent<A...> & frameStartedEvent) noexcept
                                       {
                                           auto & currentExecutor = LD::Get<I>(executors);
                                           bool shouldRun = currentExecutor(frameStartedEvent);
                                           runnable = runnable && shouldRun;
                                           return true;
                                       },this->mExecutors,runnable,applicationFrameStartedEvent);
            return runnable;
        }

        LD::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & applicationPeriodEvent) noexcept
        {
            bool runnable = true;
            LD::Second<LD::Float> period = LD::Second<LD::Float >{LD::SecondTag<LD::Float >{0}};
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    LD::Second<LD::Float> & period,
                    const LD::ApplicationPeriodEvent<A...> & applicationPeriodEvent) noexcept
            {


                auto & currentExecutor = LD::Get<I>(executors);


                LD::Second<LD::Float> currentPeriod = currentExecutor(applicationPeriodEvent);


                period =  LD::Second<LD::Float>{LD::GCD(currentPeriod.NativeRepresentation().Value(),period.NativeRepresentation().Value())};


                return true;

            },this->mExecutors,period,applicationPeriodEvent);
            return period;
        }

        void operator()(const LD::ApplicationExecutionEvent<A...> & applicationExecutionEvent) noexcept
        {
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    const LD::ApplicationExecutionEvent<A...> & applicationExecutionEvent) noexcept
                                       {
                                           auto & currentExecutor = LD::Get<I>(executors);
                                           currentExecutor(applicationExecutionEvent);
                                           return true;

                                       },this->mExecutors,applicationExecutionEvent);
        }

        void operator()(const LD::ApplicationFrameEndedEvent<A...> & applicationFrameEndedEvent) noexcept
        {
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    const LD::ApplicationFrameEndedEvent<A...> & applicationFrameEndedEvent) noexcept
                                       {
                                           auto & currentExecutor = LD::Get<I>(executors);
                                           currentExecutor(applicationFrameEndedEvent);
                                           return true;
                                       },this->mExecutors,applicationFrameEndedEvent);
        }

        void operator()(const LD::ApplicationQuittingEvent<A...> & applicationQuttingEvent) noexcept
        {
            LD::For<NumberOfExecutors>([](
                    auto I,
                    ExecutorList & executors,
                    const LD::ApplicationQuittingEvent<A...> & applicationQuttingEvent) noexcept
                                       {
                                           auto & currentExecutor = LD::Get<I>(executors);
                                           currentExecutor(applicationQuttingEvent);
                                           return true;
                                       },this->mExecutors,applicationQuttingEvent);
        }
    };


    template<typename ... Executors, typename ... A>
    MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)> MakeMultiTenantApplication(const LD::Tuple<Executors...> & executors, LD::CT::TypeList<A...>) noexcept
    {
        return MultiTenantApplication<LD::CT::TypeList<Executors...>(A...)>{executors};
    }
}
#endif //LANDESSDEVCORE_MULTITENANTAPPLICATION_HPP
