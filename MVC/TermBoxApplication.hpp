//
// Created by phoenixflower on 12/24/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H
#define LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H

//#include <Definitions/Common.hpp>
#include "TermBoxRenderingContext.hpp"
#include "Core/HeteregenousTuple.hpp"
#include "Definitions/PDPUnits.hpp"
#include "MVC/SchedulingEvent.hpp"
#include "Mouse.hpp"
#include "Cusor.h"
#include "SchedulingQuittingPredicate.hpp"
#include "TermBoxCursorNavigator.hpp"
namespace LD
{

    template<typename T>
    class TermBoxApplication ;
    template<typename T,typename ... A>
    class TermBoxApplication<T(A...)>
    {
    private:
        static_assert(LD::IsUnique<A...>,"LD::TermBoxApplication should have a unique context");
        static_assert(LD::IsInTuple<LD::TermBoxRenderContext, LD::VariadicPack<A...>>::value,"LD::TermBoxApplication requires LD::TermBoxRenderContext to be made available in the context");
        T mFunctor;
        LD::UInteger StartStatus;
        template<typename B>
        using CanBeAMouse = LD::Detail::IntegralConstant<bool,LD::IsBasicMouse<B>>;

        template<typename B>
        using CanBeACursor = LD::Detail::IntegralConstant<bool,LD::IsCursor<B>>;

        using Mice = LD::CT::Filter<LD::CT::TypeList<A...>,CanBeAMouse>;

        using Cursor = LD::CT::Filter<LD::CT::TypeList<A...>,CanBeACursor>;

        static constexpr LD::UInteger NumberOfMice = Mice::size();
        static constexpr LD::UInteger NumberOfCursors = Cursor::size();
        //constexpr static auto cursorNavigator = LD::TB::MakeCursorNavigator<A...>(cursorLeftPredicate,cursorRightPredicate,cursorDownPredicate,cursorUpPredicate);
    public:
        constexpr TermBoxApplication(T && functor)noexcept: mFunctor{LD::Forward<T>(functor)},StartStatus{0}
        {
        }



        ~TermBoxApplication() noexcept
        {
            if(this->StartStatus == 1)
            {
                tb_shutdown();
            }

        }

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
        /**
         *
         * @param applicationStartedEvent
         * @return If the Application can keep running
         * @brief Returns the value from the functor if the functor supports the LD::ApplicationStartedEvent<A..>, otherwise the application
         * simply stops running if the functor does not support LD::ApplicationStartedEvent<A...>
         */
        bool operator()(const LD::ApplicationStartedEvent<A...> & applicationStartedEvent) noexcept
        {
            this->StartStatus = (tb_init() == 0);

            //if we want mouse input then it will be dicated by the context
            if constexpr(NumberOfMice > 0)
            {
                //tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);
            }
            //return (this->StartStatus) && this->CurrentApplication(applicationStartedEvent);
            return (this->StartStatus) && TermBoxApplication::Apply(this->mFunctor,applicationStartedEvent);
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

        /**
         *
         * @param applicationFrameStartedEvent
         * @return Checks if the application should run once per frame (the frequency at which this frame occurs is determined by the period
         * of the application).  If the functor provides support for the LD::ApplicationFrameStartedEvent<A...> then it returns
         * what the functor does, if not then it will simply return false causing the application to end
         */
        bool operator()(const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent) noexcept
        {
            tb_event tbEvent;

            tb_peek_event(&tbEvent, 0);//get the even from this frame
            LD::Get<LD::TermBoxRenderContext>(applicationFrameStartedEvent)->CurrentEvent = tbEvent;
            //tb_clear_screen();
            tb_clear();//clear the buffer at the beginning of the frame
            LD::Get<LD::TermBoxRenderContext>(applicationFrameStartedEvent)->Clear();
            return TermBoxApplication::Apply(this->mFunctor,applicationFrameStartedEvent);
        }


        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationPeriodEvent<Context...> & event) noexcept -> LD::TypedRequires<PDP::Second<LD::Float>,LD::ConvertiblyCallable<F,PDP::Second<LD::Float>(LD::ApplicationPeriodEvent<Context...>)>::Value()>
        {
            return object(event);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationPeriodEvent<Context...> & event) noexcept -> LD::TypedRequires<bool,LD::CT::Negate(LD::ConvertiblyCallable<F,PDP::Second<LD::Float>(LD::ApplicationPeriodEvent<Context...>)>::Value())>
        {
            return PDP::Second<LD::Float>{1.0};
        }

        /**
         *
         * @param sleepingEvent
         * @return The period of the application, the period of the application is updated once per frame if it should need to change
         * throughout the lifetime of the application.  If the functor provides support for the LD::ApplicationPeriodEvent<A...>
         * then it shall return the functor's value otherwise it shall simply return 1s as the period.
         */
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & sleepingEvent) noexcept
        {
            return TermBoxApplication::Apply(this->mFunctor,sleepingEvent);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationExecutionEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationExecutionEvent<Context...>)>::Value()>
        {
            return object(event);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationExecutionEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationExecutionEvent<Context...>)>::Value())>
        {
            return;
        }

        /**
         *
         * @param executionEvent
         * @brief If the functor supports the LD::ApplicationExecutionEvent<A...> then it will execute the functor's workload.
         * Otherwise nothing shall be executed during the Execution phase of the application; however, it will continue to run
         */
        void operator ()(const LD::ApplicationExecutionEvent<A...> & executionEvent) noexcept
        {
            //set a default state for the output mode
            tb_select_output_mode(LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetSpectrum());
            //allow the application to render out
            TermBoxApplication::Apply(this->mFunctor,executionEvent);
            LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(executionEvent);
            auto left = (LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetEvent().key == TB_KEY_ARROW_LEFT);
            auto right = (LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetEvent().key == TB_KEY_ARROW_RIGHT);
            auto up = (LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetEvent().key == TB_KEY_ARROW_UP);
            auto down = (LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetEvent().key == TB_KEY_ARROW_DOWN);
            const LD::UInteger isMouseEnabled = (LD::Get<LD::TermBoxRenderContext>(executionEvent)->IsMouseEnabled());
            LD::Integer horizontalDirection = (isMouseEnabled*(1*(right) + -1*(left)));
            LD::Integer verticalDirection = (isMouseEnabled*(1*(up) + -1*(down)));
            LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetCursor().X() += horizontalDirection;
            LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetCursor().Y() += verticalDirection;
            auto position = renderingContext->GetCursor();
            tb_set_cursor((tb_width()/2+position.X()),(tb_height()/2-position.Y()));
            tb_present();
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationFrameEndedEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationFrameEndedEvent<Context...>)>::Value()>
        {
            return object(event);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationFrameEndedEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationFrameEndedEvent<Context...>)>::Value())>
        {
            return;
        }
        /**
         *
         * @param frameEndedEvent
         * @brief If the functor supports the LD::ApplicationFrameEndedEvent<A...> then it will execute the functor's workload.
         * Otherwise nothing shall be executed during the End of Frame phase of the application; however, it will continue to run
         */
        void operator()(const LD::ApplicationFrameEndedEvent<A...> & frameEndedEvent) noexcept
        {
            //LD::Get<LD::TermBoxRenderContext>(frameEndedEvent)->CurrentEvent = tb_event{};
            return TermBoxApplication::Apply(this->mFunctor,frameEndedEvent);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationQuittingEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::ConvertiblyCallable<F,void(LD::ApplicationQuittingEvent<Context...>)>::Value()>
        {
            return object(event);
        }

        template<typename F, typename ... Context>
        static auto Apply(F && object, const LD::ApplicationQuittingEvent<Context...> & event) noexcept -> LD::TypedRequires<void,LD::CT::Negate(LD::ConvertiblyCallable<F,void(LD::ApplicationQuittingEvent<Context...>)>::Value())>
        {
            return;
        }

        /**x
         *
         * @param quittingEvent
         */
        void operator()(const LD::ApplicationQuittingEvent<A...> & quittingEvent) noexcept
        {
            TermBoxApplication::Apply(this->mFunctor,quittingEvent);
        }

    };

    template<typename ... A,typename T>
    auto MakeTermBoxApplication(T && object, LD::CT::TypeList<A...>) noexcept -> LD::TypedRequires<TermBoxApplication<T(A...)>,(sizeof...(A) > 0)>
    {
        return TermBoxApplication<T(A...)>{LD::Forward<T>(object)};
    }
}
#endif //LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H
