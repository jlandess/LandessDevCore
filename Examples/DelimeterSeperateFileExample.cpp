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
#include "MVC/TermboxTextLabel.hpp"
#include "MVC/TermBoxButton.hpp"
#include "IO/SystemLoad.hpp"
#include "MVC/Cusor.h"
#include "MVC/MultiTenantApplication.hpp"
#include "MVC/TermBoxCheckBox.hpp"
#include "Async/Thread.h"
//#include "PatternMatching/patterns.hpp"
namespace LD
{




    namespace Example
    {
        void DelimeterSeperatedFileExample()
        {
            /*
            using namespace mpark::patterns;

            for (int i = 1; i <= 100; ++i)
            {

                match(i % 3, i % 5)(
                        pattern(0, 0) = [] { std::printf("fizzbuzz\n"); },
                        pattern(0, _) = [] { std::printf("fizz\n"); },
                        pattern(_, 0) = [] { std::printf("buzz\n"); },
                        pattern(_, _) = [i] { std::printf("%d\n", i); });
            }
             */
            LD::Timer timer;
            LD::Float timeElapsed = 0;
            timer.Start();
            using SysLoad = SystemLoad<16>;
            bool shouldContinue = true;
            SysLoad system;
            auto TUIApplicationStarted = [](const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicaitonStartedEvent) noexcept ->bool
            {
                LD::Get<LD::TermBoxRenderContext>(applicaitonStartedEvent)->EnableMouse();
                return LD::ApplicationRunningPredicate{};
            };
            auto TUIApplicationFrameStarted = [&](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                return LD::ApplicationQuittingPredicate{ shouldContinue == false};
            };
            auto TUIApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {
                return PDP::Second<LD::Float>{1.0/67.0};
            };
            LD::TUI::RadioButton box{{0,-10}};
            LD::ImmutableString<11> defaultString;
            LD::TUI::BasicImmutableTextInput<12> input{{0,-7},defaultString};

            auto TUIApplicationExecution = [&](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicationExecutionEvent ) noexcept
            {
                auto GenerateColorBasedOnLoad = [](const LD::Float & load) noexcept
                {
                    unsigned short res = (load < 0.5)*TB_GREEN+  (load >= 0.5 && load < 0.85)*TB_YELLOW + (load >= 0.85)*TB_RED;
                    return res;
                };

                LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);
                renderingContext->Render(input);
                renderingContext->Render(box);
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

                    LD::TUI::ImmutableTextLabel elapsedTimeLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+27,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(timeElapsed)
                    };

                    LD::TUI::ImmutableTextLabel cursorLabel{
                            LD::BasicVec2D<LD::Integer>{-tb_width()/2+50,(tb_height()/2)-2*n-1},
                            LD::ToImmutableString(renderingContext->GetCursor().X()) + LD::ToImmutableString(',') + LD::ToImmutableString(renderingContext->GetCursor().Y())
                    };
                    auto doStuff = [&](){shouldContinue = false;};

                    LD::TUI::ImmutableButton quit{LD::BasicVec2D<LD::Integer>{-tb_width()/2+65,(tb_height()/2)-2*n-1},"Quit",doStuff};

                    renderingContext->Render(cpuLabel);
                    renderingContext->Render(coreNumberLabel);
                    renderingContext->Render(loadBar);
                    renderingContext->Render(elapsedTimeLabel);
                    renderingContext->Render(loadLabel);
                    renderingContext->Render(cursorLabel);
                    renderingContext->Render(quit);
                    renderingContext->Render(box);
                }
            };
            auto TUIApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {
            };
            auto TUIApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
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
            };
            auto onSysApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {
            };

            auto onSysApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {

            };
            auto systemRunnable = LD::MakeVisitor(onSysApplicationStarted,onSysApplicationFrameStarted,onSysApplicationPeriod,onSysApplicationExecution,onSysApplicationFrameEnded,onSysApplicationQuit);
            auto tooldes = MakeTermBoxApplication(LD::MakeVisitor(
                    TUIApplicationStarted,
                    TUIApplicationFrameStarted,
                    TUIApplicationPeriod,
                    TUIApplicationExecution,
                    TUIApplicationFrameEnded,
                    TUIApplicationQuit
            ),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});

            auto schExe = LD::MakeMultiTenantApplication(LD::MakeContext(systemRunnable,tooldes),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});
            LD::TermBoxRenderContext cntxt;

            LD::Timer timez;
            LD::MainLoop(schExe,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
        }
    }
}