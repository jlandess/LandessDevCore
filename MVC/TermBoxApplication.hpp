//
// Created by phoenixflower on 12/24/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H
#define LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H

#include <Definitions/Common.hpp>
#include "TermBoxRenderingContext.hpp"
#include "Core/HeteregenousTuple.hpp"
#include "Definitions/PDPUnits.hpp"
#include "MVC/SchedulingEvent.hpp"
#include "Mouse.h"
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
        constexpr TermBoxApplication(T && functor)noexcept: mFunctor{LD::Forward<T>(functor)}
        {
        }



        ~TermBoxApplication() noexcept
        {
            tb_shutdown();
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
        /**
         *
         * @param applicationStartedEvent
         * @return If the Application can keep running
         * @brief Returns the value from the functor if the functor supports the LD::ApplicationStartedEvent<A..>, otherwise the application
         * simply stops running if the functor does not support LD::ApplicationStartedEvent<A...>
         */
        bool operator()(const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent) noexcept
        {
            this->StartStatus = (tb_init() == 0);

            //if we want mouse input then it will be dicated by the context
            if constexpr(NumberOfMice > 0)
            {
                tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);
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
            tb_peek_event(&tbEvent, 0);//get the event from this frame
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


            //we there's a cursor in the context, then update it
            if constexpr(NumberOfCursors > 0)
            {
                auto cursorLeftPredicate = [](const LD::ApplicationExecutionEvent<A...> & context)->LD::TB::CursorLeftResult
                {
                    return LD::TB::CursorLeftResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_LEFT)};
                };


                auto cursorRightPredicate = [](const LD::ApplicationExecutionEvent<A...> & context)->LD::TB::CursorRightResult
                {
                    return LD::TB::CursorRightResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_RIGHT)};
                };

                auto cursorUpPredicate = [](const LD::ApplicationExecutionEvent<A...> & context)->LD::TB::CursorUpResult
                {
                    return LD::TB::CursorUpResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_UP)};
                };
                auto cursorDownPredicate = [](const LD::ApplicationExecutionEvent<A...> & context)->LD::TB::CursorDownResult
                {
                    return LD::TB::CursorDownResult{(LD::Get<LD::TermBoxRenderContext>(context)->GetEvent().key == TB_KEY_ARROW_DOWN)};
                };
                auto cursorNavigator = LD::TB::MakeCursorNavigator<A...>(cursorLeftPredicate,cursorRightPredicate,cursorDownPredicate,cursorUpPredicate);
                cursorNavigator(executionEvent);

                auto cursor = LD::Get<LD::CT::TypeAtIndex<0,Cursor>>(executionEvent);

                auto key = LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetEvent().key;
                bool rightPredicate = (key == TB_KEY_ARROW_RIGHT);
                bool leftPredicate = (key == TB_KEY_ARROW_LEFT);
                bool upPredicate = (key == TB_KEY_ARROW_UP);
                bool downPredicate = (key == TB_KEY_ARROW_DOWN);
                auto & position = cursor->Position();

                using PositionType = LD::Detail::Decay_T<decltype(position)>;
                LD::Integer horizontalDirection = (1*rightPredicate) + -1*(leftPredicate);
                LD::Integer verticalDirection = (1*upPredicate + -1*(downPredicate));

                position += PositionType {horizontalDirection,verticalDirection};
                tb_set_cursor((tb_width()/2+position.X()),(tb_height()/2-position.Y()));
            }
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

        /**
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
    /*
    template<typename T, template<typename,typename...> class = fixed_size_function,class = void>
    class TermBoxApplicationImpl;

    template<typename ...A, template<typename,typename...> class Functor>
    class TermBoxApplicationImpl<LD::VariadicPack<A...>,Functor,LD::Enable_If_T<
            LD::Either<
                    !LD::IsInTuple<LD::TermBoxRenderContext, LD::VariadicPack<A...>>::value,
                    !LD::IsUnique<A...>
            >>>
    {
        static_assert(LD::Require<LD::IsInTuple<LD::TermBoxRenderContext, LD::VariadicPack<A...>>::value,LD::IsUnique<A...>>,"Your Context must Contain LD::TermBoxRenderContext and must be unique");
    };

    template<typename ...A, template<typename,typename...> class Functor>
    class TermBoxApplicationImpl<LD::VariadicPack<A...>,Functor,LD::Enable_If_T<
            LD::Require<
                    LD::IsInTuple<LD::TermBoxRenderContext, LD::VariadicPack<A...>>::value,
                    LD::IsUnique<A...>
            >>>
    {
    private:
        Functor<const bool(const LD::ApplicaitonStartedEvent<A...> &)> ApplicationStarted;
        Functor<const bool(const LD::ApplicationFrameStartedEvent<A...> &)> FrameStarted;
        Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)> SleepingEvent;
        Functor<void(const LD::ApplicationFrameEndedEvent<A...> &)> FrameEnded;
        Functor<void(const LD::ApplicationExecutionEvent<A...> &)> ExecutionEvent;
        Functor<void(const LD::ApplicationQuittingEvent<A...> &)> QuittingEvent;
    public:
        TermBoxApplicationImpl()
        {
            this->ApplicationStarted = Functor<const bool(const LD::ApplicaitonStartedEvent<A...> &)>([](const LD::ApplicaitonStartedEvent<A...> &)
                                                                                                      {
                                                                                                          return true;
                                                                                                      });
            this->SleepingEvent = Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>([](const LD::ApplicationPeriodEvent<A...> &)
                                                                                                            {
                                                                                                                return PDP::Second<LD::Float>(1.0);
                                                                                                            });

            this->QuittingEvent = Functor<void(const LD::ApplicationQuittingEvent<A...> &)>([](const LD::ApplicationQuittingEvent<A...> &)
                                                                                            {
                                                                                            });


        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, bool(const LD::ApplicaitonStartedEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnApplicationStart(F && functor)
        {
            this->ApplicationStarted = Functor<const bool(const LD::ApplicaitonStartedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, bool(const LD::ApplicationFrameStartedEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnFrameStarted(F && functor)
        {
            this->FrameStarted = Functor<const bool(const LD::ApplicationFrameStartedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnPeriodRequest(F && functor)
        {
            this->SleepingEvent = Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationFrameEndedEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnFrameEnded(F && functor)
        {
            this->FrameEnded = Functor<void(const LD::ApplicationFrameEndedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationExecutionEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnExecution(F && functor)
        {
            this->ExecutionEvent = Functor<void(const LD::ApplicationExecutionEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }


        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationQuittingEvent<A...> &)>::Value()
                >,TermBoxApplicationImpl &> OnQuit(F && functor)
        {
            this->QuittingEvent = Functor<void(const LD::ApplicationQuittingEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }


        const bool operator()(const LD::ApplicaitonStartedEvent<A...> & applicationStartedEvent)
        {
            return this->ApplicationStarted(applicationStartedEvent);
        }

        const bool operator()(const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent)
        {
            return this->FrameStarted(applicationFrameStartedEvent);
        }
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & sleepingEvent)
        {
            return this->SleepingEvent(sleepingEvent);
        }
        void operator()(const LD::ApplicationExecutionEvent<A...> & executionEvent)
        {
            return this->ExecutionEvent(executionEvent);
        }
        void operator()(const LD::ApplicationFrameEndedEvent<A...> & frameEndedEvent)
        {
            return this->FrameEnded(frameEndedEvent);
        }

        void operator()(const LD::ApplicationQuittingEvent<A...> & quittingEvent)
        {
            return this->QuittingEvent(quittingEvent);
        }
    };

    template<typename ...A>
    using TermBoxApplication = LD::TermBoxApplicationImpl<LD::VariadicPack<A...>>;
     */
}
#endif //LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H
