//
// Created by phoenixflower on 6/27/20.
//
#include "DelimeterSeperatedFileExample.h"
#include "TypeTraits/IntegerSequence.hpp"
#include "ReflectionDemoTypes.h"
#include "IO/RowBackingStore.h"
#include "IO/BasicDelimeterSeperatedFile.h"
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
class ScheduledExecutors;

template<typename ... Executors, typename ... A>
class ScheduledExecutors<LD::CT::TypeList<Executors...>(A...)>
{
private:
    using Storage = LD::Variant<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Executors>::value),LD::ElementReference <LD::Detail::Decay_T<Executors>>,LD::Detail::Decay_T<Executors>>...>;
    static constexpr LD::UInteger NumberOfExecutors = sizeof...(Executors);
    using ExecutorList = LD::StaticArray<LD::Tuple<Storage,PDP::Second<LD::Float>,PDP::Second<LD::Float>>,NumberOfExecutors>;
    static_assert(LD::IsUnique<A...>,"LD::ScheduledExecutors should have a unique context");
    ExecutorList mExecutors;
    PDP::Second<LD::Float> mPeriod;
public:

    inline ScheduledExecutors(Executors && ... executors) noexcept:mPeriod{0.0}
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

    bool operator()(const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent) noexcept
    {
        bool shouldrun = true;
        LD::For<NumberOfExecutors>([](
                auto I,
                ExecutorList & executors,
                const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent,
                bool & runnable) noexcept
        {

            auto & currentExecutor = LD::Get<0>(executors[I]);
            bool shouldRun = ScheduledExecutors<LD::CT::TypeList<Executors...>(A...)>::Apply(currentExecutor,applicationStartedEvent);
            runnable = runnable && shouldRun;
            return true;
        },this->mExecutors,applicationStartedEvent,shouldrun);
        return shouldrun;
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

    bool operator()(const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent) noexcept
    {
        LD::For<NumberOfExecutors>([](
                auto I,
                ExecutorList & executorList,
                const PDP::Second<LD::Float> & period,
                const LD::ApplicationFrameStartedEvent<A...> & frameStartedEvent) noexcept
        {
            PDP::Second<LD::Float> & currentPeriodTally = LD::Get(LD::Get<1>(LD::Get<I>(executorList)));
            PDP::Second<LD::Float> & talliedPeriod = LD::Get(LD::Get<2>(LD::Get<I>(executorList)));
            if(currentPeriodTally >= talliedPeriod)
            {

                auto onFrameStarted = [](auto && object) noexcept
                {

                };

                auto & executor = LD::Get(LD::Get<0>(LD::Get<I>(executorList)));
                //LD::QueryResult<Storage(const LD::ApplicationFrameStartedEvent<A...> &)> query;
                LD::Match(executor,onFrameStarted);
                talliedPeriod = PDP::Second<LD::Float>{0};
            }

            currentPeriodTally += period;
            return true;
        },this->mExecutors,this->mPeriod,applicationFrameStartedEvent);
        //return TermBoxApplication::Apply(this->mFunctor,applicationFrameStartedEvent);
    }
};
namespace LD
{
    namespace Example
    {
        void DelimeterSeperatedFileExample()
        {

            LD::Context<int,float>{};
            LD::Timer timer;
            timer.Start();
            auto toodlesApplicationStarted = [](const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept ->bool
            {
                return LD::ApplicationRunningPredicate{};
            };

            auto toodlesApplicationFrameStarted = [&](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                return LD::ApplicationQuittingPredicate{timer.GetElapsedTimeInSec() > 5};
            };

            auto toodlesApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {

                return PDP::Second<LD::Float>{1.0/60.0};
            };


            auto toodlesApplicationExecution = [](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicationExecutionEvent ) noexcept
            {

                LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);
                for(LD::UInteger n = 0;n<Thread::GetHardwareConcurrency();++n)
                {
                    LD::TUI::ImmutableTextLabel cpuLabel(LD::BasicVec2D<LD::Integer>{-tb_width()/2,(tb_height()/2)-2*n-1},"cpu");



                    LD::Float load = LD::UInteger (rand())%100;
                    LD::TUI::ProgressBar loadBar{{-tb_width()/2+15,(tb_height()/2)-2*n-1},load/100.0};

                    LD::TUI::ImmutableTextLabel coreNumberLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+4,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(n)
                    };

                    renderingContext->Render(cpuLabel);
                    renderingContext->Render(coreNumberLabel);
                    renderingContext->Render(loadBar);

                }

            };

            auto toodlesApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto toodlesApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };

            auto tooldes = MakeTermBoxApplication(LD::MakeVisitor(
                    toodlesApplicationStarted,
                    toodlesApplicationFrameStarted,
                    toodlesApplicationPeriod,
                    toodlesApplicationExecution,
                    toodlesApplicationFrameEnded,
                    toodlesApplicationQuit
            ),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});

            LD::TermBoxRenderContext cntxt;

            LD::Timer timez;
            LD::MainLoop(tooldes,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
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