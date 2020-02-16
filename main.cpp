#include "LandessDevCore.h"
#include "MVC/TermBoxApplication.h"
#include "MVC/Scheduling.h"
#include "Chrono/Timer.h"
#include "MVC/TermBoxMenu.h"
#include "MVC/TermBoxCursorNavigator.h"
#include "Primitives/General/Observer.h"
#include "Algorithms/CompileTimeControlFlow.h"
#include "TypeTraits/TypeList.h"
#include "MVC/Renderable.h"
namespace LD
{
    namespace TB
    {
        class DummyDelegate
        {
        private:
        public:
            template<typename ... Context>
            const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent){return true;}
            template<typename ... Context>
            const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent) noexcept{return true;}
            template<typename ... Context>
            PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept{ return PDP::Second<LD::Float>(1.0);}
            template<typename ... Context>
            void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent) noexcept{}
            template<typename ... Context>
            void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent) noexcept{}
            template<typename ... Context>
            void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept{}
        };
    }
}
template<typename ... Context>
class DummyDelegate
{
private:
    LD::Integer N;
public:
    DummyDelegate():N(0)
    {

    }
    void operator()(const LD::ApplicationExecutionEvent<Context...> & event)
    {
        //LD::Get<LD::TermBoxRenderContext>(event)->Render("selection",{9,8+N});
    }
    void operator()(const LD::UInteger & selectedIndex,const LD::ApplicationExecutionEvent<Context...> & event)
    {
        N = (selectedIndex == 0)*(N+1) + (selectedIndex == 1)*(N-1);

    }

    const LD::Integer GetN() const
    {
        return N;
    }
};

#include <iostream>
#include <MVC/FormattedFloatingPoint.h>

static constexpr auto pattern = ctll::fixed_string{ "h.*" };
static constexpr auto numericPattern = ctll::fixed_string{ "[0-9]" };
static constexpr auto patternz = ctll::fixed_string{"%[a-z]"};
constexpr auto match(std::string_view sv) noexcept {
    return ctre::match<pattern>(sv);
}



template<typename ... Arguements>
void DoStuff(Arguements && ... arguements)
{
    //static_assert(LD::Detail::IsSame<typename LD::Detail::UniqueInsert<LD::Pack<Arguements...>,char>::type,bool>::value,"") ;
}

template<template<typename> class Predicate, class TypeList>
struct ApplyPredicate;


//: LD::Detail::IntegralConstant<bool,(sizeof...(T0s) == sizeof...(T1s))
                                    //&& (PDP::IsSame<T0s, T1s>::value && ...)>
template<template<typename> class Predicate,template<typename...> class TypeListType, typename ... Arguements>
struct ApplyPredicate<Predicate,TypeListType<Arguements...>>: public LD::Detail::IntegralConstant<bool,(Predicate<Arguements>::value && ...)>
{

};




int main()
{
    auto input = LD::ToImmutableString(("abc"));
    static constexpr auto pattern = ctll::fixed_string{"%[a-z]"};
    LD::StaticArray<LD::UInteger ,20> delimeterIndices = {20};
    LD::UInteger amountOfDelimeters;
    for (const auto & match: ctre::range<pattern>(std::string_view {input.Data()}))
    {
        //std::cout << std::string_view{match.get<0>()} << "\n";
        delimeterIndices[amountOfDelimeters] = (match.get_end_position()-input.Data())-1;
        ++amountOfDelimeters;
    }
    auto cursorLeftPredicate = [](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & context)->LD::TB::CursorLeftResult
    {
        return LD::TB::CursorLeftResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_LEFT)};
    };

    auto cursorRightPredicate = [](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & context)->LD::TB::CursorRightResult
    {
        return LD::TB::CursorRightResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_RIGHT)};
    };

    auto cursorUpPredicate = [](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & context)->LD::TB::CursorUpResult
    {
        return LD::TB::CursorUpResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_UP)};
    };
    auto cursorDownPredicate = [](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & context)->LD::TB::CursorDownResult
    {
        return LD::TB::CursorDownResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_DOWN)};
    };

    auto cursorNavigator = LD::TB::MakeCursorNavigator<int,double,LD::TermBoxRenderContext,LD::Timer>(cursorLeftPredicate,cursorRightPredicate,cursorDownPredicate,cursorUpPredicate);
    DummyDelegate<int,double,LD::TermBoxRenderContext,LD::Timer> del;
    LD::ImmutableString<5> firstMenuItem = "first";
    LD::ImmutableString<6> secondMenuItem = "second";

    PDP::Detail::tVec2<LD::UInteger> menuOrigin;
    auto menuPredicate = [](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationExecutionEvent)->bool
     {
        return (LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->GetEvent().key == TB_KEY_ENTER);
     };


    auto firstMenuOptionAction = [](const LD::UInteger & selectedIndex,const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & event)->void
    {
        LD::ContextObserver<int> observer = LD::Get<int>(event);
        observer+=1;

    };
    auto secondMenuOptionAction = [](const LD::UInteger & selectedIndex,const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & event)->void
    {
        LD::ContextObserver<int> observer = LD::Get<int>(event);
        observer-=1;
    };



    LD::TermBoxApplication<int,double,LD::TermBoxRenderContext,LD::Timer> termBoxApplication;

    //auto menu = LD::TB::MakeImmutableMenu<int,double,LD::TermBoxRenderContext,LD::Timer>(menuPredicate,LD::Tie(firstMenuOptionAction,secondMenuOptionAction),{0,7},firstMenuItem,secondMenuItem);

    auto menu = LD::TB::MakeImmutableMenu(termBoxApplication,menuPredicate,LD::Tie(firstMenuOptionAction,secondMenuOptionAction),{0,7},firstMenuItem,secondMenuItem);

    termBoxApplication.OnApplicationStart([&](const LD::ApplicaitonStartedEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicaitonStartedEvent)
    {
        LD::Get<LD::TermBoxRenderContext>(applicaitonStartedEvent)->EnableMouse();
        return LD::ApplicationRunningPredicate{};
    });

    termBoxApplication.OnPeriodRequest([](const LD::ApplicationPeriodEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationPeriodRequest)
    {
        return PDP::Second<LD::Float >(1.0/60.0);
    });

    termBoxApplication.OnFrameStarted([](const LD::ApplicationFrameStartedEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationFrameStartedEvent)
    {
        return LD::ApplicationQuittingPredicate{LD::Get<LD::Timer>(applicationFrameStartedEvent)->GetElapsedTimeInSec() > 10};
    });

    termBoxApplication.OnExecution([&](const LD::ApplicationExecutionEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationExecutionEvent)
    {
        menu(applicationExecutionEvent);
        LD::ElementReference<LD::TermBoxRenderContext> context = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);
        const LD::UInteger isMouseEnabled = (LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->IsMouseEnabled());

        LD::ContextObserver<int> observer = LD::Get<int>(applicationExecutionEvent);

        LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->Render(LD::Get<LD::Timer>(applicationExecutionEvent)->GetElapsedTimeInSec(),{9,8+observer.Value()},3);
        del(applicationExecutionEvent);
        cursorNavigator(applicationExecutionEvent);

        LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->Render("origin",{0,0});
        LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->Render(LD::Get<LD::Timer>(applicationExecutionEvent)->GetElapsedTimeInSec(),{0,5},3);
        LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->RenderWithFormat(LD::ToImmutableString("{} {} {} {}abcd"),{0,-1},71,'c',22.7,99.73);
        LD::FormattedFloatingPoint<LD::Float > currentNumber{7.7263,2};
        LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent)->Render(currentNumber,{0,-2});
    });

    termBoxApplication.OnFrameEnded([](const LD::ApplicationFrameEndedEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationFrameEndedEvent)
    {

    });

    termBoxApplication.OnQuit([](const LD::ApplicationQuittingEvent<int,double,LD::TermBoxRenderContext,LD::Timer> & applicationQuittingEvent)
    {

    });


    //_mm_lfence();
    LD::Timer applicationTimer;
    LD::Timer secondaryTimer;
    secondaryTimer.Start();
    //int a;
    //double b;
    LD::TermBoxRenderContext currentRenderableContext;
    return LD::MainLoop(termBoxApplication,applicationTimer,int(),double(),currentRenderableContext,secondaryTimer);
}
