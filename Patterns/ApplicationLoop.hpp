//
// Created by phoenixflower on 12/13/21.
//

#ifndef LANDESSDEVCORE_APPLICATIONLOOP_HPP
#define LANDESSDEVCORE_APPLICATIONLOOP_HPP
#include "Primitives/General/mapboxvariant.hpp"
#include "Memory/Optional.h"
#include "Definitions/TimeExtension.hpp"
namespace LD
{
    namespace Events
    {
        class ApplicationStarted{};
        class ApplicationFrameStarted{};
        class ApplicationExecution{};
        class ApplicationFrameEnded{};
        class ApplicationSleeping{};
        class ApplicationQuitting{};
    }
    namespace States
    {
        class ApplicationInitialized;
        class ApplicationStarted;
        class ApplicationFrameStarted;
        class ApplicationExecution;
        class ApplicationSleeping;
        class ApplicationFrameEnded;
        class ApplicationQuitting;
    }
    using ApplicationState = LD::Variant<
            LD::States::ApplicationInitialized,
            LD::States::ApplicationStarted,
            LD::States::ApplicationFrameStarted,
            LD::States::ApplicationExecution,
            LD::States::ApplicationSleeping,
            LD::States::ApplicationFrameEnded,
            LD::States::ApplicationQuitting
    >;

    namespace States
    {
        class ApplicationInitialized
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
        class ApplicationStarted
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
        class ApplicationFrameStarted
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
        class ApplicationExecution
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
        class ApplicationSleeping
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
        class ApplicationFrameEnded
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event  && event)  noexcept;

            LD::Optional<LD::ApplicationState> operator()( LD::Events::ApplicationQuitting)  noexcept;
        };
        class ApplicationQuitting
        {
        public:
            template<typename Event>
            LD::Optional<LD::ApplicationState> operator()(Event && event)  noexcept;
        };
    }

    namespace States
    {
        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationInitialized::operator()(Event &&event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationStarted{}};
        }
        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationStarted::operator()(Event &&event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationFrameStarted{}};
        }

        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationFrameStarted::operator()(Event &&event)  noexcept
        {

            if constexpr(LD::CT::IsSame(LD::Type<Event>{},LD::Type<LD::Events::ApplicationQuitting>{}))
            {
                return LD::ApplicationState{LD::States::ApplicationQuitting{}};
            }
            //("Application Frame Has Started State\n");
            return LD::ApplicationState{LD::States::ApplicationExecution{}};
        }
        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationExecution::operator()(Event &&event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationSleeping{}};
        }

        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationSleeping::operator()(Event &&event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationFrameEnded{}};
        }
        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationFrameEnded::operator()(Event &&event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationFrameStarted{}};
        }

        LD::Optional<LD::ApplicationState> ApplicationFrameEnded::operator()(LD::Events::ApplicationQuitting event)  noexcept
        {
            return LD::ApplicationState{LD::States::ApplicationQuitting{}};
        }

        template<typename Event>
        LD::Optional<LD::ApplicationState> ApplicationQuitting::operator()(Event && event)  noexcept
        {
            return LD::nullopt;
        }
    }
    namespace PTN
    {
        class ApplicationInitializedState;
        class ApplicationHasStartedState;
        class ApplicationFrameHasStartedState;
        class ApplicationExecutionState;
        class ApplicationSleepingState;
        class ApplicationFrameHasEndedState;
        class ApplicationQuittingState;

        using ApplicationState = LD::Variant<
                ApplicationInitializedState,
                ApplicationHasStartedState,
                ApplicationFrameHasStartedState,
                ApplicationExecutionState,
                ApplicationSleepingState,
                ApplicationFrameHasEndedState,
                ApplicationQuittingState>;

        class ApplicationStartedEvent{};
        class ApplicationFrameStartedEvent{};
        class ApplicationExecutionEvent{};
        class ApplicationFrameEndedEvent{};
        class ApplicationSleepingEvent{};
        class ApplicationQuittingEvent{};

        using ApplicationEvent = LD::Variant<
                ApplicationStartedEvent,
                ApplicationFrameStartedEvent,
                ApplicationExecutionEvent,
                ApplicationSleepingEvent,
                ApplicationFrameEndedEvent,
                ApplicationQuittingEvent>;





        class ApplicationInitializedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };


        class ApplicationHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasStartedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationExecutionState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationSleepingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        class ApplicationFrameHasEndedState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event  event)  noexcept;

            LD::Optional<ApplicationState> operator()( ApplicationQuittingEvent)  noexcept;
        };


        class ApplicationQuittingState
        {
        public:
            template<typename Event>
            LD::Optional<ApplicationState> operator()(Event && event)  noexcept;
        };

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationInitializedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Initialized State\n");
            return ApplicationState{ApplicationHasStartedState{}};
        }
        template<typename Event>
        LD::Optional<ApplicationState> ApplicationHasStartedState::operator()(Event &&event)  noexcept
        {
            //printf("Application Has Started State\n");
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasStartedState::operator()(Event &&event)  noexcept
        {
            if constexpr(LD::CT::IsSame(LD::Type<Event>{},LD::Type<ApplicationQuittingEvent>{}))
            {
                return ApplicationState{ApplicationQuittingState{}};
            }
            //("Application Frame Has Started State\n");
            return ApplicationState{ApplicationExecutionState{}};
        }



        template<typename Event>
        LD::Optional<ApplicationState> ApplicationExecutionState::operator()(Event &&event)  noexcept
        {
            //printf("Application Execution State\n");
            return ApplicationState{ApplicationSleepingState{}};;
        }


        template<typename Event>
        LD::Optional<ApplicationState> ApplicationSleepingState::operator()(Event &&event)  noexcept
        {
            //printf("Application Sleeping State\n");
            return ApplicationState{ApplicationFrameHasEndedState{}};
        }





        template<typename Event>
        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(Event  event)  noexcept
        {
            return ApplicationState{ApplicationFrameHasStartedState{}};
        }

        LD::Optional<ApplicationState> ApplicationFrameHasEndedState::operator()(ApplicationQuittingEvent)  noexcept
        {
            //printf("Application is Quitting\n");
            return ApplicationState{ApplicationQuittingState{}};
        }

        template<typename Event>
        LD::Optional<ApplicationState> ApplicationQuittingState::operator()(Event &&event)  noexcept
        {
            return LD::nullopt;
        }

        class StateExecutorBase
        {
        private:

        public:
            virtual void operator()(ApplicationInitializedState) noexcept = 0;
            virtual void operator()(ApplicationHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationFrameHasStartedState) noexcept= 0;
            virtual void operator()(ApplicationExecutionState)noexcept = 0;
            virtual void operator()(ApplicationFrameHasEndedState) noexcept= 0;
            virtual LD::Second<LD::Float> operator()(ApplicationSleepingState) noexcept= 0;
            virtual void operator()(ApplicationQuittingState) noexcept= 0;
        };

        template<typename T>
        class BasicStateExecutor;

        template<typename T>
        class BasicStateExecutor
        {
        private:
            T mExecutor;
        public:

            BasicStateExecutor() = default;
            BasicStateExecutor(T && executor) noexcept:mExecutor{LD::Forward<T>(executor)}
            {

            }
            void operator()(LD::PTN::ApplicationInitializedState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationInitializedState)>::Value())
                {
                    this->mExecutor(state);
                }
            }

            void operator()(LD::PTN::ApplicationHasStartedState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationHasStartedState)>::Value())
                {
                    this->mExecutor(state);
                }
            }

            void operator()(LD::PTN::ApplicationFrameHasStartedState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationFrameHasStartedState)>::Value())
                {
                    this->mExecutor(state);
                }
            }

            void operator()(LD::PTN::ApplicationExecutionState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationExecutionState)>::Value())
                {
                    this->mExecutor(state);
                }
            }

            void operator()(LD::PTN::ApplicationFrameHasEndedState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationFrameHasEndedState)>::Value())
                {
                    this->mExecutor(state);
                }
            }

            void operator()(ApplicationSleepingState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,LD::Second<LD::Float>(LD::PTN::ApplicationSleepingState)>::Value())
                {
                    LD::SleepFor(this->mExecutor(state));
                }
                LD::SleepFor(1.0_s);
            }

            void operator()(LD::PTN::ApplicationQuittingState state) noexcept
            {
                if constexpr(LD::ConvertiblyCallable<T,void(LD::PTN::ApplicationQuittingState)>::Value())
                {
                    this->mExecutor(state);
                }
            }
        };
    }

}
#endif //LANDESSDEVCORE_APPLICATIONLOOP_HPP
