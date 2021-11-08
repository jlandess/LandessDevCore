//
// Created by phoenixflower on 11/5/21.
//

#ifndef LANDESSDEVCORE_FINITESTATEMACHINE_HPP
#define LANDESSDEVCORE_FINITESTATEMACHINE_HPP
#include "Algorithms/Move.hpp"
#include "Algorithms/Forward.hpp"
#include "Memory/Optional.h"
#include "TypeTraits/Detection.hpp"
#include "Algorithms/Visitation.hpp"
#include "TypeTraits/IsVariant.hpp"
#include "TypeTraits/CompileTimeControlFlow.hpp"
namespace LD
{
    template<typename Derived, typename StateVariant>
    class FiniteStateMachine
    {

        StateVariant mState;

    public:
        const StateVariant& State() const { return mState; }

        StateVariant& State() { return mState; }

        template<typename Event>
        void Dispatch(Event&& event) noexcept
        {
            Derived& child = static_cast<Derived&>(*this);


            auto newStateFunctor = [&](auto && s) noexcept ->LD::Optional<StateVariant>
            {
                return child.OnEvent(s, LD::Forward<Event>(event));
            };
            auto newState = LD::Visit(newStateFunctor,this->mState);

            if (newState)
            {
                this->mState = *LD::Move(newState);
            }
        }
    };


    template<typename Executor,typename T, class = void>
    class StateManager;

    namespace CT
    {
        template<typename T, typename Events>
        constexpr bool CanStateCallEvents(LD::Type<T>,LD::Type<Events> ) noexcept
        {
            return false;
        }
        template<typename T, typename ... Events>
        constexpr LD::Enable_If_T<LD::Require<LD::CT::IsVariant(LD::Type<T>{})>,bool> CanStateCallEvents(LD::Type<T>,LD::Type<LD::CT::TypeList<Events...>> ) noexcept
        {
            using TypesToCheck = LD::CT::RebindList<T,LD::CT::TypeList>;
            constexpr LD::UInteger amount = LD::CT::TypeListSize(LD::Type<TypesToCheck>{});
            return false;
        }
    }
    template<typename Executor,typename State, typename ... Events>
    class StateManager<Executor,State(Events...),LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::Type<State>{})
                    >>>: public LD::FiniteStateMachine<StateManager<Executor,State(Events...)>,State>
    {
    private:
        Executor mExecutor;
    public:
        using StateType = State;
        StateManager( Executor && executor) noexcept:mExecutor{LD::Forward<Executor>(executor)}
        {

        }

        template<typename PassedInState, typename Event>
        auto OnEvent(PassedInState & state,  Event &&  event) noexcept -> LD::Optional<State>
        {
            this->mExecutor(state);

            auto visitEvent = [&](const auto & passedInEvent) noexcept
            {
                return state(passedInEvent);
            };
            //auto newState = state(event);

            auto newState = LD::Visit(visitEvent,LD::Forward<Event>(event));
            if(newState)
            {
                auto visitUpdatedState = [&](auto  newState)
                {
                    this->mExecutor(state,newState);
                };

                //todo - consider allowing transition to new state based on transition
                LD::Visit(visitUpdatedState,*newState);



                return newState;
            }

            return LD::nullopt;
        }
    };
    namespace CT
    {
        template<typename Executor,typename State, typename ... Events>
        using FiniteStateMachineStateType = typename StateManager<Executor,State(Events...)>::StateType;

        template<typename T>
        constexpr LD::Type<void> FiniteStateMachineType(LD::Type<T>) noexcept
        {
            return LD::Type<void>{};
        }
        template<typename Executor,typename State, typename ... Events>
        constexpr LD::Type<State> FiniteStateMachineType(LD::Type<StateManager<Executor,State(Events...)>>) noexcept
        {
            return LD::Type<State>{};
        }
    }

    namespace CT
    {
        template<typename Executor, typename Event, typename State>
        constexpr LD::Enable_If_T<LD::Require<!LD::CT::IsVariant(LD::Type<Event>{}),!LD::CT::IsVariant(LD::Type<State>{})>,bool> CanExecutorDispatchEvent(LD::Type<Executor>,LD::Type<Event> ,LD::Type<State>) noexcept
        {
            return false;
        }

        template<typename Executor, typename Event, typename State>
        constexpr LD::Enable_If_T<LD::Require<LD::CT::IsVariant(LD::Type<Event>{}),LD::CT::IsVariant(LD::Type<State>{})>,bool> CanExecutorDispatchEvent(LD::Type<Executor>,LD::Type<Event> ,LD::Type<State>) noexcept
        {
            using ReboundEvent = LD::CT::RebindList<Event,LD::CT::TypeList>;
            using ReboundState = LD::CT::RebindList<State,LD::CT::TypeList>;
            //LD::CT::DebugTemplate<ReboundState>{};
            bool isDispatchable = true;
            LD::For<LD::CT::TypeListSize(LD::Type<ReboundState>{})>([](auto I, bool & dispatchable)
            {
                //LD::CT::DebugTemplate<LD::CT::TypeAtIndex<I,ReboundState>>{};
                dispatchable = dispatchable && LD::ConvertiblyCallable<Executor,LD::Optional<Event>(LD::CT::TypeAtIndex<I,ReboundState>)>::Value();
                return true;
            },isDispatchable);
            return isDispatchable;
        }
    }
    //FiniteStateMachineType
    //LD::CT::CanExecutorDispatchEvent(LD::Type<Executor>{},LD::Type<Event>{},LD::CT::FiniteStateMachineType(LD::Type<FiniteStateMachineType>{}))
    template<typename FiniteStateMachineType, typename Executor, class = void >
    class EventDispatcher;
    //todo - use sfinae to determine if Event is a variant
    //todo - use sfinae to ensure the Executor can call every type specified in the state variant provided by the FSM, and return an optional event
    template<typename FiniteStateMachineType, typename Executor, typename Event>
    class EventDispatcher<FiniteStateMachineType,Executor(Event),
            LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsVariant(LD::Type<Event>{}),
                            LD::CT::CanExecutorDispatchEvent(LD::CT::RemoveQualifiers(LD::Type<Executor>{}),LD::CT::RemoveQualifiers(LD::Type<Event>{}),LD::CT::FiniteStateMachineType(LD::CT::RemoveQualifiers(LD::Type<FiniteStateMachineType>{})))
                            >>>
    {
    private:
        FiniteStateMachineType mFSM;
        Executor mExecutor;
    public:
        EventDispatcher(FiniteStateMachineType && fsm, Executor && executor) noexcept:mFSM{LD::Forward<FiniteStateMachineType>(fsm)},mExecutor{LD::Forward<Executor>(executor)}
        {

        }
        void operator()() noexcept
        {
            auto eventGenerationFunctor = [&](auto && s) noexcept ->LD::Optional<Event>
            {
                return this->mExecutor(LD::Forward<decltype(s)>(s));
            };
            auto generatedEvent = LD::Visit(eventGenerationFunctor,this->mFSM.State());
            if (generatedEvent)
            {
                this->mFSM.Dispatch(*generatedEvent);
            }
        }
    };
}
#endif //LANDESSDEVCORE_FINITESTATEMACHINE_HPP
