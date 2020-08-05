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
#include "MVC/MultiTenantApplication.hpp"
#include "MVC/TermBoxCheckBox.h"

#include <iostream>
namespace LD
{
    namespace Example
    {
        void DelimeterSeperatedFileExample()
        {



            //LD::TUI::BasicImmutableTextInput

            LD::Timer timer;
            LD::Float timeElapsed = 0;
            timer.Start();
            using SysLoad = SystemLoad<16>;

            bool shouldContinue = true;

            SysLoad system;
            auto toodlesApplicationStarted = [](const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept ->bool
            {
                return LD::ApplicationRunningPredicate{};
            };

            auto toodlesApplicationFrameStarted = [&](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                return LD::ApplicationQuittingPredicate{timer.GetElapsedTimeInSec() > 25 || shouldContinue == false};
            };

            auto toodlesApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {

                return PDP::Second<LD::Float>{1.0/67.0};
            };



            LD::TUI::CheckBox box{{0,-10}};
            //LD::TUI::BasicImmutableTextInput

            LD::ImmutableString<11> defaultString;
            LD::TUI::BasicImmutableTextInput<12> input{{0,-5},defaultString};
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

                    auto doStuff = [&](){shouldContinue = false;};

                    LD::TUI::ImmutableButton quit{LD::BasicVec2D<LD::Integer>{-tb_width()/2+65,(tb_height()/2)-2*n-1},"Quit",doStuff};


                    //auto button = LD::TUI::MakeButton(LD::ToImmutableString("Quit"),doStuff,LD::BasicVec2D<LD::Integer>{-tb_width()/2+65,(tb_height()/2)-2*n-1});
                    renderingContext->Render(cpuLabel);
                    renderingContext->Render(coreNumberLabel);
                    renderingContext->Render(loadBar);
                    renderingContext->Render(elapsedTimeLabel);
                    renderingContext->Render(loadLabel);
                    renderingContext->Render(cursorLabel);
                    renderingContext->Render(quit);
                    renderingContext->Render(input);



                    //renderingContext->Render(getch(),{5,-5});
                    renderingContext->Render(box);
                    //renderingContext->Render(wchar_t{9673},{0,-10});
                    //renderingContext->Render(wchar_t {9635},{0,-11});
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
                std::cout << "application started event " << std::endl;
                return LD::ApplicationRunningPredicate{};
            };

            auto onSysApplicationFrameStarted = [](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & ) noexcept -> bool
            {
                std::cout << "application frame started event " << std::endl;
                return LD::ApplicationRunningPredicate{};
            };

            auto onSysApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept -> PDP::Second<LD::Float>
            {

                std::cout << "application period event " << std::endl;
                return PDP::Second<LD::Float>{1.0};
            };

            auto onSysApplicationExecution = [&](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> & applicationExecutionEvent ) noexcept
            {
                std::cout << "application execution event " << std::endl;
                system();

                timeElapsed = applicationExecutionEvent.GetElapsedTime().GetValue();

                //std::cout << 1.0/timeElapsed << std::endl;

            };

            auto onSysApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float> &) noexcept
            {
                std::cout << "application frame ended event " << std::endl;
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


            auto schExe = LD::MakeMultiTenantApplication(LD::MakeContext(systemRunnable,tooldes),LD::CT::TypeList<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>,float>{});
            LD::TermBoxRenderContext cntxt;

            LD::Timer timez;
            //LD::MainLoop(schExe,timez,cntxt,LD::Cursor<LD::Integer>{},float{});

            LD::MainLoop(systemRunnable,timez,cntxt,LD::Cursor<LD::Integer>{},float{});
        }

    }
}