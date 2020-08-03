//
// Created by phoenixflower on 6/27/20.
//
#include "DelimeterSeperatedFileExample.h"
#include "TypeTraits/IntegerSequence.hpp"
#include "ReflectionDemoTypes.h"
#include "IO/RowBackingStore.h"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Primitives/General/Span.hpp"
#include "MVC/TermBoxApplication.hpp"
#include "MVC/Scheduling.hpp"
#include "MVC/FormattedFloatingPoint.hpp"
#include "MVC/TermBoxProgressBar.hpp"
#include "MVC/TUI/TUITextInput.h"
#include "MVC/TermboxTextLabel.h"
#include "MVC/TermBoxButton.hpp"
#include "IO/SystemLoad.hpp"
#include "MVC/Cusor.h"



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
        PDP::Second<LD::Float> mPeriod;
        PDP::Second<LD::Float> mElapsed;
        PDP::Second<LD::Float> test = PDP::Second<LD::Float>{0};
        //double mAccumulated;

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicaitonStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::ConvertiblyCallable<F,bool(LD::ApplicaitonStartedEvent<Context...>)>::Value()>
        {

            return object(event);
        }


        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicaitonStartedEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::CT::Negate(LD::ConvertiblyCallable<F,bool(LD::ApplicaitonStartedEvent<Context...>)>::Value())>
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
        inline constexpr Bucket() noexcept : mElapsed{PDP::Second<LD::Float>{0}},mPeriod{PDP::Second<LD::Float>{0}}
        {

        }

        inline constexpr Bucket(T && executor) noexcept: mElapsed{PDP::Second<LD::Float>{0}},mPeriod{PDP::Second<LD::Float>{0}},mCurrentExecutor{LD::Forward<T>(executor)}
        {

        }

        Bucket & operator = (const BucketExecutor & executor) noexcept
        {
            this->mCurrentExecutor = executor;
            return (*this);
        }

        bool operator()(const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent) noexcept
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


    bool operator()(const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent) noexcept
    {
        bool shouldrun = true;
        LD::For<NumberOfExecutors>([](
                auto I,
                ExecutorList & executors,
                const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent,
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

    PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & applicationPeriodEvent) noexcept
    {
        bool runnable = true;
        PDP::Second<LD::Float> period = PDP::Second<LD::Float>{0};
        LD::For<NumberOfExecutors>([](
                auto I,
                ExecutorList & executors,
                PDP::Second<LD::Float> & period,
                const LD::ApplicationPeriodEvent<A...> & applicationPeriodEvent) noexcept
        {

            auto & currentExecutor = LD::Get<I>(executors);

            PDP::Second<LD::Float> currentPeriod = currentExecutor(applicationPeriodEvent);

            period =  PDP::Second<LD::Float>{LD::GCD(currentPeriod.GetValue(),period.GetValue())};

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

namespace LD
{
    namespace Example
    {
        void DelimeterSeperatedFileExample()
        {

            LD::Context<int,float>{};
            LD::Timer timer;
            LD::Float timeElapsed = 0;
            timer.Start();
            using SysLoad = SystemLoad<16>;

            SysLoad system;
            auto toodlesApplicationStarted = [](const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept ->bool
            {
                return LD::ApplicationRunningPredicate{};
            };

            auto toodlesApplicationFrameStarted = [&](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                return LD::ApplicationQuittingPredicate{timer.GetElapsedTimeInSec() > 25};
            };

            auto toodlesApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {

                return PDP::Second<LD::Float>{1.0/67.0};
            };


            auto toodlesApplicationExecution = [&](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicationExecutionEvent ) noexcept
            {

                auto GenerateColorBasedOnLoad = [](const LD::Float & load) noexcept
                {
                    unsigned short res = (load < 0.5)*TB_GREEN+  (load >= 0.5 && load < 0.85)*TB_YELLOW + (load >= 0.85)*TB_RED;
                    return res;
                };

                LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);
                for(LD::UInteger n = 0;n<Thread::GetHardwareConcurrency();++n)
                {
                    LD::TUI::ImmutableTextLabel cpuLabel(LD::BasicVec2D<LD::Integer>{-tb_width()/2,(tb_height()/2)-2*n-1},"cpu");



                    LD::Float load = system.Load(n);//LD::UInteger (rand())%100;
                    LD::TUI::ProgressBar loadBar{{-tb_width()/2+15,(tb_height()/2)-2*n-1},load/100.0};

                    LD::TUI::ImmutableTextLabel coreNumberLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+4,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(n)
                    };

                    LD::TUI::ImmutableTextLabel loadLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+6,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(load,2),
                            GenerateColorBasedOnLoad(load/100.0)
                    };

                    ;

                    LD::TUI::ImmutableTextLabel elapsedTimeLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+27,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(timeElapsed)
                    };

                    //LD::ElementReference<LD::Cursor<LD::Integer>> cursor = LD::Get<LD::Cursor<LD::Integer>>(applicationExecutionEvent);
                    LD::TUI::ImmutableTextLabel cursorLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+50,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(renderingContext->GetCursor().X()) + LD::ToImmutableString(',') + LD::ToImmutableString(renderingContext->GetCursor().Y())
                    };
                    renderingContext->Render(cpuLabel);
                    renderingContext->Render(coreNumberLabel);
                    renderingContext->Render(loadBar);
                    renderingContext->Render(elapsedTimeLabel);
                    renderingContext->Render(loadLabel);
                    renderingContext->Render(cursorLabel);

                }

            };

            auto toodlesApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto toodlesApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto onSysApplicationStarted =  [](const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept ->bool
            {
                return LD::ApplicationRunningPredicate{};
            };

            auto onSysApplicationFrameStarted = [](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                return LD::ApplicationRunningPredicate{};
            };

            auto onSysApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {

                return PDP::Second<LD::Float>{1.0};
            };

            auto onSysApplicationExecution = [&](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicationExecutionEvent ) noexcept
            {
                system();

                timeElapsed = applicationExecutionEvent.GetElapsedTime().GetValue();

                //std::cout << 1.0/timeElapsed << std::endl;

            };

            auto onSysApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto onSysApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto systemRunnable = LD::MakeVisitor(onSysApplicationStarted,onSysApplicationFrameStarted,onSysApplicationPeriod,onSysApplicationExecution,onSysApplicationFrameEnded,onSysApplicationQuit);
            auto tooldes = MakeTermBoxApplication(LD::MakeVisitor(
                    toodlesApplicationStarted,
                    toodlesApplicationFrameStarted,
                    toodlesApplicationPeriod,
                    toodlesApplicationExecution,
                    toodlesApplicationFrameEnded,
                    toodlesApplicationQuit
            ),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});


            auto schExe = MakeMultiTenantApplication(LD::MakeContext(systemRunnable,tooldes),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});
            LD::TermBoxRenderContext cntxt;

            LD::Timer timez;
            LD::MainLoop(schExe,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
            //LD::MainLoop(systemRunnable,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
            //std::cout << LD::GCD(1.0/60.0,1.0/128.0) << std::endl;
            //LD::MainLoop(tooldes,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
            //tooldes(LD::ApplicaitonStartedEvent<int,float>{});


            /*
            using SysLoad = SystemLoad<16>;

            SysLoad system;

            LD::TermBoxApplication<SysLoad ,LD::TermBoxRenderContext,LD::Timer> termBoxApplication;


            termBoxApplication.OnApplicationStart([](const LD::ApplicaitonStartedEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicaitonStartedEvent)
            {


                //LD::Get<LD::TermBoxRenderContext>(applicaitonStartedEvent)->EnableMouse();
                return LD::ApplicationRunningPredicate{};

            });

            termBoxApplication.OnPeriodRequest([](const LD::ApplicationPeriodEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicationPeriodRequest)

            {
                return PDP::Second<LD::Float >(1.0);
            });

            termBoxApplication.OnFrameStarted([](const LD::ApplicationFrameStartedEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicationFrameStartedEvent)
            {
                return LD::ApplicationQuittingPredicate{LD::Get<LD::Timer>(applicationFrameStartedEvent)->GetElapsedTimeInSec() > 20};
            });



            LD::TUI::BasicImmutableTextInput<12> input(LD::BasicVec2D<LD::UInteger >{0,3});
            termBoxApplication.OnExecution([&](const LD::ApplicationExecutionEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicationExecutionEvent)
            {

                auto GenerateColorBasedOnLoad = [](const LD::Float & load) noexcept
                {
                    unsigned short res = (load < 0.5)*TB_GREEN+  (load >= 0.5 && load < 0.85)*TB_YELLOW + (load >= 0.85)*TB_RED;
                    return res;
                };
                //const SysLoad & system = LD::Get(LD::Get<SysLoad>(applicationExecutionEvent));
                LD::Ref<SysLoad> system = LD::Get<SysLoad>(applicationExecutionEvent);
                LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);

                (*system)();
                for(LD::UInteger n = 0;n<8;++n)
                {

                    LD::FormattedFloatingPoint<LD::Float > currentNumber{system->Load(n),4};
                    LD::Float load = system->Load(n);



                    LD::TUI::ImmutableTextLabel cpuLabel(LD::BasicVec2D<LD::Integer>{-tb_width()/2,(tb_height()/2)-2*n-1},"cpu");

                    LD::TUI::ImmutableTextLabel coreNumberLabel{
                        LD::BasicVec2D<LD::Integer>{-tb_width()/2+4,(tb_height()/2)-2*n-1},
                        LD::ToImmutableString(n)
                    };
                    //(*renderingContext).Render(LD::ToImmutableString("abc"),{0,0});

                    LD::TUI::ImmutableTextLabel loadLabel{
                        LD::BasicVec2D<LD::Integer>{-tb_width()/2+6,(tb_height()/2)-2*n-1},
                        LD::ToImmutableString(load,2),
                        GenerateColorBasedOnLoad(load/100.0)
                    };

                    LD::TUI::ProgressBar loadBar{{-tb_width()/2+15,(tb_height()/2)-2*n-1},load/100.0};


                    cpuLabel(*renderingContext);
                    coreNumberLabel(*renderingContext);
                    loadLabel(*renderingContext);

                    loadBar(*renderingContext);

                    LD::ImmutableString<11> defaultString;
                    //LD::TUI::ProgressBar bar{{0,0},(double)rand()/(RAND_MAX)};
                    //bar(*renderingContext);

                    input(*renderingContext);
                    //std::cout << system.Load(n) << " ";
                }


            });

            termBoxApplication.OnFrameEnded([](const LD::ApplicationFrameEndedEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicationFrameEndedEvent)
            {


            });

            termBoxApplication.OnQuit([](const LD::ApplicationQuittingEvent<SysLoad ,LD::TermBoxRenderContext,LD::Timer> & applicationQuittingEvent)
            {


            });

            LD::Timer applicationTimer;
            LD::Timer secondaryTimer;
            secondaryTimer.Start();
            //int a;
            //double b;
            //LD::TermBoxRenderContext currentRenderableContext;

            //LD::MainLoop(termBoxApplication,applicationTimer,system,currentRenderableContext,secondaryTimer);
             */
        }

    }
}