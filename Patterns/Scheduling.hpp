//
//  Schduler.h
//  DataStructures5
//
//  Created by James Landess on 2/3/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef Schduler_h
#define Schduler_h

#include "Definitions/Integer.hpp"
#include "Definitions/PDPUnits.hpp"
#include "Memory/ElementReference.h"
#include "MVC/SchedulingEvent.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Core/HeteregenousTuple.hpp"
#include "MVC/TermBoxRenderingContext.hpp"

#include "Algorithms/Sampler.hpp"
#include <Algorithms/fsm.hpp>

#include "MVC/TermBoxConsoleApplication.hpp"
#include "Definitions/TimeExtension.hpp"
#include "MVC/SchedulingQuittingPredicate.hpp"
#include "Core/Unit.hpp"
namespace LD
{
    
    //beginning of application states
    class LowMemoryState
    {
        
    };
    
    
    class ExecutionState
    {
        
    };
    
    class ApplicationStartState
    {
        
    };
    
    class ApplicationExitState
    {
        
    };
    
    
    class SuspensionState
    {
        
    };
    
    class AwakeningState
    {
        
    };
    
    class ApplicaitonInputState
    {
        
    };
    
    
    
    class ApplicationHasStartedState
    {
    private:
        bool ShouldRun;
    public:
        ApplicationHasStartedState(const bool & shouldRun):ShouldRun(shouldRun)
        {
            
        }
        
        
        operator bool() const
        {
            return this->ShouldRun;
        }
    };
    
    class OverHeatingState
    {
        
    };
    
    class QuittingState
    {
        
    };
    
    class InitialApplicationState
    {
        
    };
    
    class ApplicationReadingHeatState
    {
        
    };
    
    class ApplicationExecutionState
    {
    private:
        LD::Second<LD::Float> Dt;
    public:
        
    };
    
    
    class ApplicationFrameEndedState
    {
        
    };
    
    class ApplicationPeriodState
    {
    private:
        LD::Second<LD::Float> Period;
    public:
        
        ApplicationPeriodState(const LD::Second<LD::Float> & period):Period(period)
        {
            
        }
        
        
        operator LD::Second<LD::Float>() const
        {
            return this->Period;
        }
    };
    class ApplicationFrameStartedState
    {
    private:
        bool ShouldRun;
    public:
        ApplicationFrameStartedState(const bool & shouldRun):ShouldRun(shouldRun)
        {
            
        }
        
        operator bool() const
        {
            return this->ShouldRun;
        }
    };
    //end of application states
    
    //beginning of application events
    //end of application events
    
    using ApplicationState = LD::Variant <
    LD::InitialApplicationState,
    LD::ApplicationHasStartedState,
    LD::ExecutionState,
    LD::QuittingState,
    LD::ApplicationExecutionState,
    LD::ApplicationFrameEndedState,
    LD::ApplicationPeriodState,
    LD::ApplicationFrameStartedState  
    >;
    
    class CoreContext
    {
        
    };
    

    template<typename T, class = void>
    class BasicApplication;
    
    
    //ArrayList<Object> events = new ArrayList<Object>();
    //std::vector<std::function<void()>> events;
    //event[0]();
    
    template<typename ... Executors, typename ... Context>
    class BasicApplication<LD::CT::TypeList<Executors...>(Context...),LD::Enable_If_T<
    LD::Require<
    true
>>>: public mp::fsm<BasicApplication<LD::CT::TypeList<Executors...>(Context...)>,LD::ApplicationState>
    {
    private:
        
        struct Bucket
        {

            mapbox::util::variant<Executors...> Obj;
            LD::Second<LD::Float> AccumulatedPeriod;
            LD::Second<LD::Float> CachedPeriod;
        };

        LD::StaticArray<Bucket,sizeof...(Executors)> Events;
        
        template<LD::UInteger Index ,typename T, typename E>
        static void EventDispatcherBase(T && bucketArray, const E & event)
        {
            //LD::Get<Index>(bucket).Obj(event);
        }
        template<LD::UInteger ... Indices,typename T, typename E>
        static void EventDispatcherFold(LD::IndexSequence<Indices...>,T && bucketArray, const E & event)
        {
            //(...,EventDispatcherBase<Indices>(bucket,event));
        }
        template<typename T, typename E>
        static void EventDispatcher(T && bucketArray, const E & event)
        {
            //EventDispatcherFold(LD::MakeIndexSequence_T<sizeof...(Executors)>{},bucketArray,event);
        }
        
        //LD::Get<0>(Events)();
        //LD::Get<1>(Events)();
        //.
        //.
        //.
        //LD::Get<N>(Events)();
        template<LD::UInteger Index ,typename T, typename E>
        static const bool BooleanEventDispatcherBase(T && bucketArray, const E && event)
        {
            return bucketArray[Index].Obj(event);
        }
        template<LD::UInteger ... Indices,typename T, typename E>
        static const bool BooleanEventDispatcherFold(LD::IndexSequence<Indices...>,T && bucketArray, const E && event)
        {
            return (BooleanEventDispatcherBase<Indices>(LD::Forward<T>(bucketArray),LD::Forward<E>(event)) && ...);
        }
        template<typename T, typename E>
        static const bool BooleanEventDispatcher(T && bucketArray, const E & event)
        {
            return BooleanEventDispatcherFold(LD::MakeIndexSequence_T<sizeof...(Executors)>{},LD::Forward<T>(bucketArray),LD::Forward<E>(event));
        }
        
        template<LD::UInteger Index ,typename T, typename E>
        static void PeriodEventDispatcherBase(T && bucketArray, const E && event, LD::Float & gcd)
        {
            auto & currentBucket = bucketArray[Index];
            LD::Second<LD::Float> calculatedPeriod = currentBucket.Obj(event);
            currentBucket.CachedPeriod = calculatedPeriod;
            
            gcd  =  LD::GCD(calculatedPeriod.NativeRepresentation().Value(),gcd);
        }
        template<LD::UInteger ... Indices,typename T, typename E>
        static const LD::Second<LD::Float> PeriodEventDispatcherFold(LD::IndexSequence<Indices...>,T && bucketArray, const E & event)
        {
            LD::Float gcd = 0;
            (...,PeriodEventDispatcherBase<Indices>(LD::Forward<T>(bucketArray),LD::Forward<E>(event),gcd));
            return LD::Second<LD::Float>(gcd);
        }
        
        
        template<typename T, typename E>
        static const LD::Second<LD::Float> PeriodEventDispatcher(T && object, const E && event)
        {
            return PeriodEventDispatcherFold(LD::MakeIndexSequence_T<sizeof...(Executors)>{},LD::Forward<T>(object),LD::Forward<E>(event));
        }
        
        template<LD::UInteger Index, typename Tuple>
        void AssignExecutorsBase(Tuple && tuple)
        {
            this->Events[Index] = LD::Get<Index>(tuple);
        }
        template<LD::UInteger ... Indices, typename Tuple>
        void AssignExecutorsFold(LD::IndexSequence<Indices...>, Tuple && tuple)
        {
            (...,AssignExecutorsBase<Indices>(LD::Forward<Tuple>(tuple)));
        }
        
        template<typename Tuple>
        void AssignExecutors(Tuple && tuple)
        {
            return AssignExecutorsFold(LD::MakeIndexSequence_T<sizeof...(Executors)>{},LD::Forward<Tuple>(tuple));
        }
    public:
        
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
                true
        //LD::is_permutation_v<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>
        >>>
        inline BasicApplication(A && ... objects)
        {
            auto tiedTuple = LD::MakeTuple(LD::Forward<A>(objects)...);
            
            AssignExecutors(tiedTuple);
        }
        
        template<typename State, typename Event>
        auto OnEvent(State&, const Event&)
        {
            
            return LD::nullopt;
            
        }
        
        
        LD::ApplicationState OnEvent(const LD::InitialApplicationState & initialApplicationState, const ApplicationStartedEvent<Context...> & applicationStartedEvent)
        {

            const bool shouldRun = BooleanEventDispatcher(this->Events, applicationStartedEvent);
            return LD::ApplicationHasStartedState{shouldRun};
            
        }
        
        
        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState & applicationHasStartedState, const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            //resolve any initial conditions that have to met for the application to run, if not then just quit
            const bool shouldRun = BooleanEventDispatcher(this->Events, applicationFrameStartedEvent);
            
            return LD::ApplicationFrameStartedState{shouldRun};
        }
        
        
        LD::ApplicationState OnEvent(const LD::ApplicationFrameEndedState & applicationFrameEndedState, const LD::ApplicationFrameStartedEvent<Context...> & newFrameEvent)
        {
            //decide betweent the end of the last frame, and the beginning of the new frame if the application should continue to run
            const bool shouldRun = BooleanEventDispatcher(this->Events, newFrameEvent);
            return LD::ApplicationFrameStartedState{shouldRun};
        }
        
        
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState & frameStartedState, const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            
            LD::Second<LD::Float> period =  PeriodEventDispatcher(this->Events,sleepingEvent);
            
            return LD::ApplicationPeriodState{period};
        }
        
        
        LD::ApplicationState OnEvent(const LD::ApplicationPeriodState & periodState, const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            EventDispatcher(this->Events,executionEvent);
            return LD::ApplicationExecutionState{};
        }
        
        LD::ApplicationState OnEvent(const LD::ApplicationExecutionState &, const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            EventDispatcher(this->Events,frameEndedEvent);
            
            return LD::ApplicationFrameEndedState{};
        }
        
        
        /**
         @brief - The application reached a new frame, and has requested to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            EventDispatcher(this->Events,quittingEvent);
            return LD::QuittingState{};
        }
        
        /**
         @brief - The application failed to meet initialization conditions and needed to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            EventDispatcher(this->Events,quittingEvent);
            return LD::QuittingState{};
        }
    };
    //LD::ConvertiblyCallable<Executor, void(const LD::ApplicationExecutionEvent<Context...> &)>::Value()
    //LD::CovertiblyCallable<Executor, const bool(const ApplicaitonStartedEvent<Context...> &)>::Value()
    //const LD::ApplicationFrameStartedEvent<Context...> &
    //const LD::ApplicationPeriodEvent<Context...> &
    //const LD::ApplicationFrameEndedEvent<Context...> &
    //const LD::ApplicationQuittingEvent<Context...> &
    
    
    
    /**
     
     LD::ConvertiblyCallable<Executor, const bool(const LD::ApplicaitonStartedEvent<Context...> &)>::Value(),
     LD::ConvertiblyCallable<Executor, const bool(const LD::ApplicationFrameStartedEvent<Context...> &)>::Value(),
     LD::ConvertiblyCallable<Executor, LD::Second<LD::Float>(const LD::ApplicationPeriodEvent<Context...> &)>::Value(),
     LD::ConvertiblyCallable<Executor, void(const LD::ApplicationExecutionEvent<Context...> &)>::Value(),
     LD::ConvertiblyCallable<Executor, void(const LD::ApplicationFrameEndedEvent<Context...> &)>::Value(),
     LD::ConvertiblyCallable<Executor, void(const LD::ApplicationQuittingEvent<Context...> &)>::Value()
     
     
     */
    template<typename ... Executor, typename ... Context>
    class BasicApplication<LD::CT::TypeList<Executor...>(Context...),LD::Enable_If_T<
            LD::Require<
                    (sizeof...(Executor) > 1)
                    >>>: public mp::fsm<BasicApplication<LD::CT::TypeList<Executor...>(Context...)>,LD::ApplicationState>
    {

    private:
        LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> mExecutors;
    public:
        BasicApplication( Executor && ... currentExecutor) noexcept
        {
            //this->get_state() = LD::InitialApplicationState{};
            auto context = LD::MakeContext(LD::Forward<Executor>(currentExecutor)...);
            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    auto && context) noexcept
            {
                executors[I] = LD::Get(LD::Get<I>(LD::Forward<decltype(context)>(context)));
                return true;
            }, this->mExecutors,LD::Forward<decltype(context)>(context));
        }

        template<typename State, typename Event>
        auto OnEvent(State&, const Event&) noexcept
        {

            return LD::nullopt;

        }

        /**
         @param initialApplicationState - The application's initial state, this state only happens once per application life cycle
         @param applicationStartedEvent - This event is called when the application has been initialized, this even is called once per application
         @brief - This event transition occurs from the point in which the application is initiated to the application beginning the process of executing events
         */
        LD::ApplicationState OnEvent(const LD::InitialApplicationState & initialApplicationState, const ApplicationStartedEvent<Context...> & applicationStartedEvent) noexcept
        {
            //const bool shouldRun = this->CurrentExecutor(applicationStartedEvent);
            bool shouldRun = true;
            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const ApplicationStartedEvent<Context...> & applicaitonStartedEvent,
                    bool & shouldRun) noexcept
            {
                shouldRun = shouldRun && executors[I](applicaitonStartedEvent);
                return true;
            },this->mExecutors,applicationStartedEvent,shouldRun);

            return LD::ApplicationHasStartedState{shouldRun};
        }



        //the application has started, and the first frame is about to start.
        /**
         @param applicationHasStartedState - The application started event, this event also only occurs once per application lifecycle.
         @param applicationFrameStartedEvent - The new frame event representing each new frame of the application
         @brief - After the application has started, the next important aspect of the application is for the first frame of the application to be invoked.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState & applicationHasStartedState, const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent) noexcept
        {
            //resolve any initial conditions that have to met for the application to run, if not then just quit
            //const bool shouldRun = this->CurrentExecutor(applicationFrameStartedEvent);

            bool shouldRun = true;
            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent,
                    bool & shouldRun) noexcept
            {
                shouldRun = shouldRun && executors[I](applicationFrameStartedEvent);
                return true;
            },this->mExecutors,applicationFrameStartedEvent,shouldRun);
            return LD::ApplicationFrameStartedState{shouldRun};
        }


        /**
         @param applicationFrameEndedState - The frame of the current application has ended
         @param newFrameEvent - A new frame is starting to be processed
         @brief - An application frame can either be transitioned from the LD::ApplicationHasStartedState or from LD::ApplicationFrameEndedState
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameEndedState & applicationFrameEndedState, const LD::ApplicationFrameStartedEvent<Context...> & newFrameEvent) noexcept
        {
            //decide betweent the end of the last frame, and the beginning of the new frame if the application should continue to run
            //const bool shouldRun = this->CurrentExecutor(newFrameEvent);
            bool shouldRun = true;
            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationFrameStartedEvent<Context...> & newFrameEvent,
                    bool & shouldRun) noexcept
            {

                shouldRun = shouldRun && executors[I](newFrameEvent);
                return true;
            },this->mExecutors,newFrameEvent,shouldRun);
            return LD::ApplicationFrameStartedState{true};
        }


        /**
         @brief The amount of time that should be sleep before the execution event is meant to be called.  Since it is possible for that time to change over the lifetime of the
         application's execution, it is necessary to ask for the period length once per frame
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState & frameStartedState, const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept
        {
            //PDP::Second<LD::Float> period =  this->CurrentExecutor(sleepingEvent);

            LD::Second<LD::Float> period = LD::Second<LD::Float>{LD::SecondTag<LD::Float>{0}};
            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationPeriodEvent<Context...> & sleepingEvent,
                    LD::Second<LD::Float> & period) noexcept
            {
                period = LD::Second<LD::Float>{LD::GCD(executors[I](sleepingEvent).GetValue(),period.NativeRepresentation().Value())};
                return true;
            },this->mExecutors,sleepingEvent,period);
            return LD::ApplicationPeriodState{period};
        }


        /**
         @brief After the period has been calculated, we're ready to execute.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationPeriodState & periodState, const LD::ApplicationExecutionEvent<Context...> & executionEvent) noexcept
        {
            //this->CurrentExecutor(executionEvent);

            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationExecutionEvent<Context...> & executionEvent) noexcept
            {

                executors[I](executionEvent);
                return true;

            },this->mExecutors,executionEvent);
            return LD::ApplicationExecutionState{};
        }

        /**
         @brief After the event has been scheduled, and executed.  We're ready to go through the process of ending the frame.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationExecutionState &, const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent) noexcept
        {
            //this->CurrentExecutor(frameEndedEvent);

            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent) noexcept
            {
                executors[I](frameEndedEvent);
                return true;
            },this->mExecutors,frameEndedEvent);
            return LD::ApplicationFrameEndedState{};
        }

        /**
         @brief - The application reached a new frame, and has requested to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
        {
            //this->CurrentExecutor(quittingEvent);

            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
            {
                executors[I](quittingEvent);
                return true;
            },this->mExecutors,quittingEvent);
            return LD::QuittingState{};
        }

        /**
         @brief - The application failed to meet initialization conditions and needed to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
        {
            //this->CurrentExecutor(quittingEvent);

            LD::For<sizeof...(Executor)>([](
                    auto I,
                    LD::StaticArray<LD::Variant<Executor...>,sizeof...(Executor)> & executors,
                    const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
            {

                executors[I](quittingEvent);
                return true;

            },this->mExecutors,quittingEvent);
            return LD::QuittingState{};
        }


    };
    template<typename Executor, typename ... Context>
    class BasicApplication<Executor(Context...),LD::Enable_If_T<
    LD::Require<
    true
    >>>: public mp::fsm<BasicApplication<Executor(Context...)>,LD::ApplicationState>
    {
    private:
        Executor CurrentExecutor;
    public:
        
        BasicApplication(Executor && currentExecutor):CurrentExecutor{LD::Forward<Executor>(currentExecutor)}
        {
            //this->get_state() = LD::InitialApplicationState{};
            
        }
        BasicApplication(Executor & currentExecutor):CurrentExecutor{currentExecutor}
        {
            //this->get_state() = LD::InitialApplicationState{};

        }
        
        //always return no opt and clear the state
        
        
        template<typename State, typename Event>
        auto OnEvent(State&, const Event&) noexcept
        {
            
            return LD::nullopt;
            
        }
        
        
        
        /**
         @param initialApplicationState - The application's initial state, this state only happens once per application life cycle
         @param applicationStartedEvent - This event is called when the application has been initialized, this even is called once per application
         @brief - This event transition occurs from the point in which the application is initiated to the application beginning the process of executing events
         */
        LD::ApplicationState OnEvent(const LD::InitialApplicationState & initialApplicationState, const ApplicationStartedEvent<Context...> & applicationStartedEvent) noexcept
        {
            const bool shouldRun = LD::Get(this->CurrentExecutor)(applicationStartedEvent);
            
            return LD::ApplicationHasStartedState{shouldRun};
            
            ;
            
        }
        
        
        
        //the application has started, and the first frame is about to start.
        /**
         @param applicationHasStartedState - The application started event, this event also only occurs once per application lifecycle.
         @param applicationFrameStartedEvent - The new frame event representing each new frame of the application
         @brief - After the application has started, the next important aspect of the application is for the first frame of the application to be invoked.
         */
        ///LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState & applicationHasStartedState, const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent) noexcept
        //{
            //resolve any initial conditions that have to met for the application to run, if not then just quit
            //const bool shouldRun = LD::Get(this->CurrentExecutor)(applicationFrameStartedEvent);
            
            //return LD::ApplicationFrameStartedState{shouldRun};
        //}

        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState & applicationHasStartedState, const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept
        {
        //resolve any initial conditions that have to met for the application to run, if not then just quit
            LD::Second<LD::Float> period =  LD::Get(this->CurrentExecutor)(sleepingEvent);

            return LD::ApplicationPeriodState{period};
        }
        
        /**
         @param applicationFrameEndedState - The frame of the current application has ended
         @param newFrameEvent - A new frame is starting to be processed
         @brief - An application frame can either be transitioned from the LD::ApplicationHasStartedState or from LD::ApplicationFrameEndedState
         */
        //LD::ApplicationState OnEvent(const LD::ApplicationFrameEndedState & applicationFrameEndedState, const LD::ApplicationFrameStartedEvent<Context...> & newFrameEvent) noexcept
        //{
            //decide betweent the end of the last frame, and the beginning of the new frame if the application should continue to run
            //const bool shouldRun = LD::Get(this->CurrentExecutor)(newFrameEvent);
            //return LD::ApplicationFrameStartedState{shouldRun};
        //}

        LD::ApplicationState OnEvent(const LD::ApplicationFrameEndedState & applicationFrameEndedState, const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept
        {
            //decide betweent the end of the last frame, and the beginning of the new frame if the application should continue to run
            LD::Second<LD::Float> period =  LD::Get(this->CurrentExecutor)(sleepingEvent);
            return LD::ApplicationPeriodState{period};
        }
        
        
        /**
         @brief The amount of time that should be sleep before the execution event is meant to be called.  Since it is possible for that time to change over the lifetime of the
         application's execution, it is necessary to ask for the period length once per frame
         */
        //LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState & frameStartedState, const LD::ApplicationPeriodEvent<Context...> & sleepingEvent) noexcept
        //{
            //PDP::Second<LD::Float> period =  LD::Get(this->CurrentExecutor)(sleepingEvent);
            
            //return LD::ApplicationPeriodState{period};
        //}


        LD::ApplicationState OnEvent(const LD::ApplicationPeriodState & frameStartedState, const LD::ApplicationFrameStartedEvent<Context...> & newFrameEvent) noexcept
        {
            const bool shouldRun = LD::Get(this->CurrentExecutor)(newFrameEvent);

            return LD::ApplicationFrameStartedState{shouldRun};
        }
        
        
        /**
         @brief After the period has been calculated, we're ready to execute.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState & periodState, const LD::ApplicationExecutionEvent<Context...> & executionEvent) noexcept
        {
            LD::Get(this->CurrentExecutor)(executionEvent);
            
            return LD::ApplicationExecutionState{};
        }
        
        /**
         @brief After the event has been scheduled, and executed.  We're ready to go through the process of ending the frame.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationExecutionState &, const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent) noexcept
        {
            LD::Get(this->CurrentExecutor)(frameEndedEvent);
            
            return LD::ApplicationFrameEndedState{};
        }
        
        /**
         @brief - The application reached a new frame, and has requested to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
        {
            LD::Get(this->CurrentExecutor)(quittingEvent);
            
            return LD::QuittingState{};
        }
        
        /**
         @brief - The application failed to meet initialization conditions and needed to quit.
         */
        LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState &, const LD::ApplicationQuittingEvent<Context...> & quittingEvent) noexcept
        {
            LD::Get(this->CurrentExecutor)(quittingEvent);
            
            return LD::QuittingState{};
        }
    };
    
    
    
}


namespace LD
{
    
    
    class Disconnector
    {
    public:
        virtual void operator()( LD::UInteger index ) = 0;
    };
    
    
    /// Deleter that doesn't delete
    
    inline void no_delete(Disconnector*)
    
    {
    };
    
    
    
    
    
    
    class Connection
    {
    private:
        LD::UInteger Index;
        LD::ElementReference<LD::Disconnector> Disconnector;
    public:
        Connection()
        {
            
        }
        Connection(const ElementReference<LD::Disconnector> & disconnector, const LD::UInteger & index):Disconnector(disconnector),Index(index)
        {
            
        }
        
        
        bool Connected() const
        {
            
            //return InternalResource != nullptr &&  !InternalResource->_weak_disconnector.expired();
            
            return (this->Disconnector.GetPointer() == nullptr);
            
        }
        
        
        void Disconnect()
        {
            (*Disconnector.GetPointer())(this->Index);
            this->Disconnector = ElementReference<LD::Disconnector>(nullptr);
        }
    };
    
    
    class ScopedConnection
    {
    private:
        Connection CurrentConnection;
    public:
        ScopedConnection(const Connection & connection)
        {
            this->CurrentConnection = connection;
        }
        ScopedConnection & operator = (const Connection & connection)
        {
            this->CurrentConnection = connection;
            return (*this);
        }
        ~ScopedConnection()
        {
            CurrentConnection.Disconnect();
        }
        
    };
    
    
    template<typename T, typename U = LD::NullClass, template<typename> class F  = LD::LightWeightDelegate> class Scheduler;
    
    /*
    template<typename T ,typename U, typename ... Args ,template<typename> class F>
    class Scheduler<T(Args...), U,F>: public Disconnector
    {
    private:
        PDP::DataStructures::Array<LD::ApplicationEvent<T(Args...), U,F>> Events;
        //LD::Tuple<Args...>  Context;
        LD::UInteger CurrentIdentification;
    public:
        
        //template<typename X, typename Y>
        //Connection Connect(const X & object,R (Y::* function_to_bind)(A...) const)
        
        template<typename E, typename P, typename S, typename Q>
        void Connect(E && executor, P && period, S && sortablePredicate, Q && quitPredicate);
        
        //F<U(LD::Tuple<Args...> &)>
        template<typename X, typename Y, typename Z, typename W, typename M>
        Connection Connect(const X & object,
                           
                           T (Y::* executor)(Args && ...) const,
                           
                           PDP::Second<LD::Float> (Z::* period)() const,
                           
                           U (M::* sortablePredicate)() const,
                           
                           
                           const bool (W::* quit)() const)
        {
            LD::UInteger currentTag = this->CurrentIdentification++;
            Connection currentConnection(this,currentTag);
            
            //Event<T(Args...), U,F> currentEvent(executor,period,sortablePredicate,quit,currentTag,this->Context);
            
            //Events.PushBack(currentEvent);
            return currentConnection;
        }
        
        template<typename X, typename Y, typename Z, typename W, typename M>
        Connection Connect(X & object,
                           
                           T (Y::* executor)(Args && ...) ,
                           
                           PDP::Second<LD::Float> (Z::* period)() ,
                           
                           U (M::* sortablePredicate)(),
                           const bool (W::* quit)() )
        {
            LD::UInteger currentTag = this->CurrentIdentification++;
            Connection currentConnection(this,currentTag);
            
            //Event<T(Args...), U,F> currentEvent(executor,period,sortablePredicate,quit,currentTag,this->Context);
            
            //Events.PushBack(currentEvent);
            return currentConnection;
        }
        
        
        
        
        virtual void operator()( LD::UInteger index ) const
        {
            
            
            this->Events.Remove(index);
            
            
        }
        
        
        
        //the application has been initialized, but hasn't started yet
        auto OnEvent(const ApplicaitonStartedEvent<Args...> & applicationStartedEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
            
        }
        
        
        
        
        //the frame has started, and we've moved onto the execution
        auto OnEvent(const LD::ApplicationExecutionEvent<Args...> & applicationExecutionEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
        }
        
        
        
        
        
        auto OnEvent(const LD::ApplicationPeriodEvent<Args...> & applicationPeriodEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
        };
        
        
        
        
        //we just calculated the period for the next frame, and we're getting
        auto OnEvent(const LD::ApplicationFrameEndedEvent<Args...> & applicationFrameEndedEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
        }
        
        
        //we've reached the end of the last frame, and have reached the new frame
        auto OnEvent(const LD::ApplicationFrameStartedEvent<Args...> & applicationFrameStartedEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
        }
        
        
        
        
        //the frame started, but the application wants to quit
        auto OnEvent(LD::ApplicationQuittingEvent<Args...> & applicationQuitEvent)
        {
            const LD::UInteger maxSize = this->Events.GetSize();
            
            for (LD::UInteger n = 0; n<maxSize; ++n )
            {
                
            }
        }
        
        
    };
    
    template<typename T , typename ... Args ,template<typename> class F>
    class Scheduler<T(Args...), LD::NullClass,F>: public Disconnector
    {
    private:
        PDP::DataStructures::Array<LD::ApplicationEvent<T(Args...), LD::NullClass,F>> Events;
    public:
        
    };
     */
}
namespace LD
{


    /**
     *
     * @tparam Executor
     * @tparam Context
     * @tparam A
     * @tparam TimerType
     * @param fsm
     * @param applicationTimer
     * @param contextArguements
     * @return
     */


    //sfinae for only lvalues, no rvalues are allowed
    template<typename Executor,typename ... Context , typename ... A,typename TimerType>
    LD::Enable_If_T<
    LD::Require<
    LD::ConvertiblyCallable<Executor, bool(const LD::ApplicationStartedEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, bool(const LD::ApplicationFrameStartedEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, LD::Second<LD::Float>(const LD::ApplicationPeriodEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationExecutionEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationFrameEndedEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationQuittingEvent<typename LD::Decay<Context>::type...> &)>::Value(),
    LD::IsUnique<Context...>,
    LD::IsUnique<A...>,
    LD::is_permutation_v<LD::VariadicPack<typename LD::Decay<A>::type...>,LD::VariadicPack<typename LD::Decay<Context>::type...>>
    >,int> ApplicationLoop(LD::BasicApplication<Executor(Context...)> & fsm, TimerType & applicationTimer,A && ... contextArguements)
    {
        using ContextTypeList = LD::CT::TypeList<LD::Detail::Decay_T<A>...>;
        typedef LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<A>::value),LD::ElementReference <LD::Detail::Decay_T<A>>,LD::Detail::Decay_T<A>>...> PackedArguementType;
        PackedArguementType packedArguements = LD::MakeReferenceableTuple(LD::Forward<A>(contextArguements)...);
        LD::HeteregenousTuple< LD::ElementReference<typename LD::Decay<A>::type>...> context;
        if constexpr(sizeof...(A) > 0)
        {
            LD::For<sizeof...(A)>([](auto Index, PackedArguementType & refereneableTuple, LD::HeteregenousTuple< LD::ElementReference<typename LD::Decay<A>::type>...> & context)
            {

                 LD::Get<Index>(context) = LD::ElementReference<typename LD::TypeAtIndex<Index,ContextTypeList>::type>{LD::Get(LD::Get<Index>(refereneableTuple))};
                 return true;
            },packedArguements,context);
        }

        applicationTimer.Start();//start the timer to clock the amount of time it takes the applicaiton to startup
        
        //dispatch the beginning of the application and determine if the application should keep running.

        fsm.Dispatch(LD::ApplicationStartedEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});

        bool applicationShouldRun = LD::Match(fsm.get_state(),[](auto && abc)
        {
            //maybe add an error handler to the parameters instead of just exiting?
            //exit(0);
            return false;
        },[](const LD::ApplicationHasStartedState & state)
        {
            //applicationShouldRun = state;
            return bool(state);
        });
        //LD::ApplicationHasStartedState & applicationStartedState = nonstd::get<LD::ApplicationHasStartedState>(fsm.get_state());
        //const bool applicationShouldRun = applicationStartedState;
        //the application has failed to initialize, and it will now exit
        if(!applicationShouldRun)
        {
            //_mm_lfence();
            fsm.Dispatch(LD::ApplicationQuittingEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});
            return 0;
        }
        applicationTimer.Stop();
        //LD::Second<LD::Float> startupTime = LD::Second<LD::Float>(applicationTimer.GetElapsedTimeInSec());
        auto frameFunctor = [](bool & applicationShouldrun,LD::BasicApplication<Executor(Context...)> & fsm,TimerType & applicationTimer,LD::HeteregenousTuple< LD::ElementReference<typename LD::Decay<A>::type>...> & context, LD::Second<LD::Float> & dynamicTick)->const bool
        {
            fsm.Dispatch(LD::ApplicationPeriodEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});


            LD::Second<LD::Float> sleepingPeriod = LD::Match(fsm.get_state(),
            [](auto &&)
            {

                return LD::Second<LD::Float >(LD::SecondTag<LD::Float>{0});

            },

            [](const LD::ApplicationPeriodState & sleepingState)

            {
                //sleepingPeriod = sleepingState;
                return LD::Second<LD::Float>(sleepingState);
            });


            applicationTimer.Start();//start the timer once per frame
            LD::Usleep(sleepingPeriod);//sleep for the designated amount of time

            LD::Second<LD::Float> granuilarity(LD::SecondTag<LD::Float>{sleepingPeriod.NativeRepresentation().Value()*0.0625});//keep multiplying 0.016 until you reach the number, it's just a frame folding constant
            //PDP::Second<LD::Float> zero(0);
            //soft-real time operating systems don't return an exact amount for the requested frame time, constantly adjust through the lifetime of the application
            //const bool increaseTick = (sleepingPeriod.NativeRepresentation().Value() < applicationTimer.NativeRepresentation().Value() );
            //const bool decreaseTick =(sleepingPeriod.NativeRepresentation().Value()  > applicationTimer.NativeRepresentation().Value() );



            //dynamicTick += LD::Second<LD::Float>{LD::SecondTag<LD::Float>{LD::Float(increaseTick* granuilarity.NativeRepresentation().Value())}};
            //dynamicTick += LD::Second<LD::Float>{LD::SecondTag<LD::Float>{-1*LD::Float(decreaseTick* granuilarity.NativeRepresentation().Value())}};




            fsm.Dispatch(LD::ApplicationFrameStartedEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});//dispatch the frame started event
            //bool shouldRun = false;
            applicationShouldrun = LD::Match(fsm.get_state(),
                    [](auto &&)
                    {
                        return false;
                       // exit(0);
                    },
                    [](const LD::ApplicationFrameStartedState & frameStartedState)
                    {
                        return bool(frameStartedState);
                        //shouldRun = frameStartedState;
                    });

            return applicationShouldrun;
        };
        LD::Second<LD::Float> dynamicTick = LD::Second<LD::Float>(LD::SecondTag<LD::Float>{0});
        //check to see when the condition for running is no longer true
        while (frameFunctor(applicationShouldRun,fsm,applicationTimer,context,dynamicTick))
        {

            fsm.Dispatch(LD::ApplicationPeriodEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});

            //PDP::Second<LD::Float> sleepingPeriod;
            LD::Second<LD::Float> sleepingPeriod = LD::Match(fsm.get_state(),
                    [](auto &&)
                    {
                        return LD::Second<LD::Float >(LD::SecondTag<LD::Float>{0});
                    },
                    [](const LD::ApplicationPeriodState & sleepingState)
                    {
                        //sleepingPeriod = sleepingState;
                        return LD::Second<LD::Float>(sleepingState);
                    });
            
            //usleep(sleepingPeriod + dynamicTick);//sleep for the designated amount of time

            //PDP::Second<LD::Float> granuilarity(sleepingPeriod*0.0625);//keep multiplying 0.016 until you reach the number, it's just a frame folding constant
            //PDP::Second<LD::Float> zero(0);
            //soft-real time operating systems don't return an exact amount for the requested frame time, constantly adjust through the lifetime of the application
            //const bool increaseTick = (sleepingPeriod.GetValue() < applicationTimer.GetElapsedTime());
            //const bool decreaseTick = (sleepingPeriod.GetValue() > applicationTimer.GetElapsedTime());

            //dynamicTick += (decreaseTick*granuilarity);
            //dynamicTick -= (increaseTick*granuilarity);
            //do event prcessing and dispatch the events of the application
            fsm.Dispatch(LD::ApplicationExecutionEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});
            //declare the frame has ended
            fsm.Dispatch(LD::ApplicationFrameEndedEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});
            
            applicationTimer.Stop();//end of the timer for the application frame

        }
        applicationTimer.Start();
        //allow the application to make the final set of actions before quitting
        fsm.Dispatch(LD::ApplicationQuittingEvent<typename LD::Decay<Context>::type...>{context,applicationTimer});
        applicationTimer.Stop();
        return 0;
    }
    

    
    /**
     *
     * @tparam Executor
     * @tparam A
     * @tparam TimerType
     * @param executor
     * @param applicationTimer
     * @param contextArguements
     * @return
     */
    template<typename Executor , typename ... A,typename TimerType>
    LD::Enable_If_T<
    LD::Require<
    LD::ConvertiblyCallable<Executor, bool(const LD::ApplicationStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, bool(const LD::ApplicationFrameStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, LD::Second<LD::Float>(const LD::ApplicationPeriodEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationExecutionEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationFrameEndedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationQuittingEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::IsUnique<A...>
    >,int> MainLoop(Executor && executor, TimerType & applicationTimer,A && ... contextArguements)
    {
        
        LD::BasicApplication<Executor(typename LD::Decay<A>::type...)>  applicationModel(LD::Forward<Executor>(executor));
        return LD::ApplicationLoop(applicationModel,applicationTimer,LD::Forward<A>(contextArguements)...);
    }
    
    /**
     *
     * @tparam Executor
     * @tparam A
     * @tparam TimerType
     * @param executor
     * @param applicationTimer
     * @param contextArguements
     * @return
     */
     /*
    template<typename Executor , typename ... A,typename TimerType>
    LD::Enable_If_T<
    LD::Require<
    LD::ConvertiblyCallable<Executor, const bool(const LD::ApplicaitonStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, const bool(const LD::ApplicationFrameStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationExecutionEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationFrameEndedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<Executor, void(const LD::ApplicationQuittingEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::IsUnique<A...>,
    LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<A>::type...>>::value
    >,int> MainLoop(Executor && executor, TimerType & applicationTimer,A && ... contextArguements)
    {
        typedef LD::BasicTermBoxApplication<typename LD::Decay<Executor>::type(typename LD::Decay<A>::type...)> TermBoxApplication;
        typedef LD::BasicApplication<TermBoxApplication(typename LD::Decay<A>::type...)> Runnable;
        TermBoxApplication currentTermBoxApplication(LD::Forward<Executor>(executor));
        
        
        Runnable currentRunnable(currentTermBoxApplication);

        return LD::ApplicationLoop(currentRunnable,applicationTimer,LD::Forward<A>(contextArguements)...);
    }
      */

    /*
    template<typename ... Executors , typename ... A,typename TimerType>
    LD::Enable_If_T<
    LD::Require<
    (sizeof...(Executors) > 1),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, const bool(const LD::ApplicaitonStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, const bool(const LD::ApplicationFrameStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationExecutionEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationFrameEndedEvent<typename LD::Decay<A>::type...> &)>::Value(),
    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationQuittingEvent<typename LD::Decay<A>::type...> &)>::Value()
    >,int> MainLoop(Executors &&                                                                                                                                                                                      ... executors, TimerType & applicationTimer, A && ... contextArguements)
    {
        LD::BasicApplication<LD::VariadicPack<typename LD::Decay<Executors>::type...>(typename LD::Decay<A>::type...)> applicationModel(LD::Forward<A>(executors)...);
        
        return LD::ApplicationLoop(applicationModel,applicationTimer,LD::Forward<A>(contextArguements)...);
    }
     */


    template<typename ... Executors , typename ... A,typename TimerType>
    LD::Enable_If_T<
            LD::Require<
                    (sizeof...(Executors) > 1),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, const bool(const LD::ApplicationStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, const bool(const LD::ApplicationFrameStartedEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, LD::Second<LD::Float>(const LD::ApplicationPeriodEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationExecutionEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationFrameEndedEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::ConvertiblyCallable<LD::VariadicPack<Executors...>, void(const LD::ApplicationQuittingEvent<typename LD::Decay<A>::type...> &)>::Value(),
                    LD::IsUnique<A...>,
                    LD::IsUnique<Executors...>
            >,int> Main(Executors  && ...executors, TimerType && applicationTimer,A && ... contextArguements)
    {
        LD::BasicApplication<LD::VariadicPack<typename LD::Decay<Executors>::type...>(typename LD::Decay<A>::type...)> applicationModel(LD::Forward<A>(executors)...);

        return LD::ApplicationLoop(applicationModel,applicationTimer,LD::Forward<A>(contextArguements)...);
    }



    
    /*
    int TermBoxApplicationTestExampleMooo()
    {
        
        typedef TermBoxApplicationExample<LD::ThermalMonitor<LD::System>,LD::MouseAndKeyboard> AppType;
        //typedef LD::BasicTermBoxApplication<AppType(LD::TermBoxRenderContext,LD::ThermalMonitor<LD::System>,LD::MouseAndKeyboard)> TermBoxApplication;
        //typedef LD::Application<TermBoxApplication(LD::TermBoxRenderContext,LD::ThermalMonitor<LD::System>,LD::MouseAndKeyboard)> Runnable;
        
        
        AppType currentApplication;
        
        
        
        LD::System currentSystem;
        LD::ThermalMonitor<LD::System> thermalMonitor(currentSystem);
        LD::MouseAndKeyboard ioInput;
        LD::TermBoxRenderContext currentRenderableContext;
        
        PDP::DataStructures::Timer applicationTimer;
        
        
        //_mm_lfence();
        return LD::MainLoop(currentApplication,applicationTimer,currentRenderableContext,ioInput,thermalMonitor);
    }
    
    */

    /*
    template<typename ... Context>
    class ApplicationExample
    {
    private:
        LD::UInteger Index;
    public:
        ApplicationExample()
        {
            this->Index = 0;
        }
        
        ~ApplicationExample()
        {
            
        }
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            return true;
        }
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            return (this->Index < 10);
        }
        
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            return PDP::Second<LD::Float>(1.5);
        }
        
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            ++this->Index;
            std::cout << "Current Index Count: " <<  Index << std::endl;
            std::cout << "Current Execution Time: " << executionEvent.GetElapsedTime().GetValue() << std::endl;
            //GetPhysicalCPUTemperature
            std::cout << "Temperature : " << LD::Get<LD::ThermalMonitor<LD::System>>(executionEvent)->GetTemp() << std::endl;
            
        }
        
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            
        }
        
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            std::cout << "Application has quit with index : " << this->Index << std::endl;
        }
    };
     */
    
    /*
    int ApplicationTestExample()
    {
        typedef ApplicationExample<LD::ThermalMonitor<LD::System>,LD::IOInput<LD::MouseAndKeyboard>> AppType;
        
        AppType currentApplicationExample;
        
        LD::System currentSystem;
        LD::ThermalMonitor<LD::System> thermalMonitor(currentSystem);
        LD::IOInput<LD::MouseAndKeyboard> ioInput;
        
        int firstPlace = 9;
        int secondPlace = 10;
        PDP::DataStructures::Timer applicationTimer;
        
        
        
        
        return LD::MainLoop(currentApplicationExample,applicationTimer,thermalMonitor,ioInput);
    }
    */
    
}

namespace LD
{
    template<typename T> class ApplicationEventQueue;
    
    /*
    template<typename T, typename ... Context>
    class ApplicationEventQueue<T(Context...)>: public LD::Disconnector
    {
    private:
        struct EventQueueBucket
        {
            T Object;
            LD::UInteger Identification;
        };
        
        PDP::DataStructures::Array<EventQueueBucket> HeteregenousContainer;
        PDP::DataStructures::Stack<LD::UInteger> IdentificationStack;
        LD::UInteger Identification;
    public:
        ApplicationEventQueue()
        {
            this->Identification = 0;
        }
        
        LD::Connection AddEvent(const T & variant)
        {
            LD::Connection currentConnection;
            //we have identification ids that we can recycle
            if (this->IdentificationStack.GetSize() > 0)
            {
                LD::UInteger currentID = *IdentificationStack.Front();
                currentConnection =  LD::Connection(this,currentID);
                
                this->HeteregenousContainer.PushBack(EventQueueBucket{variant,currentID});
                
                
                return currentConnection;
            }
            
            LD::UInteger currentID = this->Identification++;
            currentConnection = LD::Connection(this,currentID);
            this->HeteregenousContainer.PushBack({variant,currentID});
            
            
            return currentConnection;
        }
        
        virtual void operator()( LD::UInteger objectID )
        {
            bool canRemove = false;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                if (this->HeteregenousContainer[n].Identification == objectID)
                {
                    canRemove = true;
                    break;
                }
            }
            if (canRemove == true)
            {
                this->HeteregenousContainer.Remove(objectID);
                this->IdentificationStack.Push(objectID);
            }
            
        }
        
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            bool shouldRun = true;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                
                const bool result = this->HeteregenousContainer[n].Object(applicationStartedEvent);
                
                shouldRun = (shouldRun&&result);
            }
            
            return shouldRun;
        }
        
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            
            bool shouldRun = true;
            
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                const bool result = this->HeteregenousContainer[n].Object(applicationFrameStartedEvent);
                
                shouldRun = (shouldRun&&result);
                
            }
            
            return shouldRun;
        }
        
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            PDP::Second<LD::Float> resultantPeriod;
            
            if (this->HeteregenousContainer.GetSize() > 1)
            {
                resultantPeriod = this->HeteregenousContainer[0].Object(sleepingEvent);
            }
            
            for (LD::UInteger n = 1; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                PDP::Second<LD::Float> result =  this->HeteregenousContainer[n].Object(sleepingEvent);
                
                LD::Float newPeriod = PDP::GCD(resultantPeriod.GetValue(), result.GetValue());
                resultantPeriod = PDP::Second<LD::Float>(newPeriod);
                
            }
            return resultantPeriod;
        }
        
        
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                this->HeteregenousContainer[n].Object(executionEvent);
                
            }
        }
        
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
               this->HeteregenousContainer[n].Object(frameEndedEvent);
                
            }
        }
        
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                this->HeteregenousContainer[n].Object(quittingEvent);
                
            }
        }
        
    };
    */

    /*
    template<typename ... Args, typename ... Context>
    class ApplicationEventQueue<mapbox::util::variant<Args...>(Context...)>: public LD::Disconnector
    {
    private:
        struct EventQueueBucket
        {
            mapbox::util::variant<Args...> Object;
            LD::UInteger Identification;
        };
        
        PDP::DataStructures::Array<EventQueueBucket> HeteregenousContainer;
        PDP::DataStructures::Stack<LD::UInteger> IdentificationStack;
        LD::UInteger Identification;
    public:
        ApplicationEventQueue()
        {
            this->Identification = 0;
        }
        
        LD::Connection AddEvent(const mapbox::util::variant<Args...> & variant)
        {
            LD::Connection currentConnection;
            //we have identification ids that we can recycle the numeric identification
            if (this->IdentificationStack.GetSize() > 0)
            {
                LD::UInteger currentID = *IdentificationStack.Front();
                currentConnection =  LD::Connection(this,currentID);
                
                this->HeteregenousContainer.PushBack(EventQueueBucket{variant,currentID});
                
                
                return currentConnection;
            }
            
            LD::UInteger currentID = this->Identification++;
            currentConnection = LD::Connection(this,currentID);
            this->HeteregenousContainer.PushBack({variant,currentID});
            
            
            return currentConnection;
        }
        
        virtual void operator()( LD::UInteger objectID )
        {
            bool canRemove = false;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                if (this->HeteregenousContainer[n].Identification == objectID)
                {
                    canRemove = true;
                    break;
                }
            }
            if (canRemove == true)
            {
                this->HeteregenousContainer.Remove(objectID);
                this->IdentificationStack.Push(objectID);
            }
            
        }
        
        
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            bool shouldRun = true;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                const bool result = LD::Match(this->HeteregenousContainer[n].Object,
                                              [&](auto && indexedObject)
                                              
                                              {
                                                  return indexedObject(applicationStartedEvent);
                                              });
                shouldRun = (shouldRun&&result);
            }
            
            return shouldRun;
        }
        
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            bool shouldRun = true;
            
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                const bool result = LD::Match(this->HeteregenousContainer[n].Object,
                                              [&](auto && indexedObject)
                                              
                                              {
                                                  return indexedObject(applicationFrameStartedEvent);
                                              });
                
                shouldRun = (shouldRun&&result);
                
            }
            return shouldRun;
        }
        
        
        
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            PDP::Second<LD::Float> resultantPeriod;
            
            if (this->HeteregenousContainer.GetSize() > 1)
            {
                resultantPeriod = LD::Match(this->HeteregenousContainer[0].Object,
                                            [&](auto && indexedObject)
                                            {
                                                return indexedObject(sleepingEvent);
                                            });
            }
            
            for (LD::UInteger n = 1; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                PDP::Second<LD::Float> result =  LD::Match(this->HeteregenousContainer[n].Object,
                                                           [&](auto && indexedObject)
                                                           {
                                                               return indexedObject(sleepingEvent);
                                                           });
                
                LD::Float newPeriod = PDP::GCD(resultantPeriod.GetValue(), result.GetValue());
                resultantPeriod = PDP::Second<LD::Float>(newPeriod);
                
            }
            return resultantPeriod;
        }
        
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(executionEvent);
                          });
                
            }
        }
        
        
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(frameEndedEvent);
                          });
                
            }
        }
        
        
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(quittingEvent);
                          });
                
            }
        }
        
    };
     */
    /*
    template<typename ... Args>
    class ApplicationEventQueue: public LD::Disconnector
    {
    private:
        
        struct EventQueueBucket
        {
            mapbox::util::variant<Args...> Object;
            LD::UInteger Identification;
        };
        
        PDP::DataStructures::Array<EventQueueBucket> HeteregenousContainer;
        PDP::DataStructures::Stack<LD::UInteger> IdentificationStack;
        LD::UInteger Identification;
    public:
        ApplicationEventQueue()
        {
            this->Identification = 0;
        }
        template<typename ... Context>
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            bool shouldRun = true;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                const bool result = LD::Match(this->HeteregenousContainer[n].Object,
                                              [&](auto && indexedObject)
                                              
                                              {
                                                  return indexedObject(applicationStartedEvent);
                                              });
                shouldRun = (shouldRun&&result);
            }
        }
        
        LD::Connection AddEvent(const mapbox::util::variant<Args...> & variant)
        {
            LD::Connection currentConnection;
            //we have identification ids that we can recycle
            if (this->IdentificationStack.GetSize() > 0)
            {
                LD::UInteger currentID = *IdentificationStack.Front();
                currentConnection =  LD::Connection(this,currentID);
                
                this->HeteregenousContainer.PushBack(EventQueueBucket{variant,currentID});
                
                
                return currentConnection;
            }
            
            LD::UInteger currentID = this->Identification++;
            currentConnection = LD::Connection(this,currentID);
            this->HeteregenousContainer.PushBack({variant,currentID});
            
            
            return currentConnection;
        }
        
        template<typename ... Context>
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            bool shouldRun = true;
            
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                const bool result = LD::Match(this->HeteregenousContainer[n].Object,
                                              [&](auto && indexedObject)
                                              
                                              {
                                                  return indexedObject(applicationFrameStartedEvent);
                                              });
                
                shouldRun = (shouldRun&&result);
                
            }
            return true;
        }
        
        template<typename ... Context>
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            PDP::Second<LD::Float> resultantPeriod;
            
            if (this->HeteregenousContainer.GetSize() > 1)
            {
                resultantPeriod = LD::Match(this->HeteregenousContainer[0].Object,
                                            [&](auto && indexedObject)
                                            {
                                                return indexedObject(sleepingEvent);
                                            });
            }
            
            for (LD::UInteger n = 1; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                PDP::Second<LD::Float> result =  LD::Match(this->HeteregenousContainer[n].Object,
                                                           [&](auto && indexedObject)
                                                           {
                                                               return indexedObject(sleepingEvent);
                                                           });
                
                LD::Float newPeriod = LD::GCD(resultantPeriod.GetValue(), result.GetValue());
                resultantPeriod = PDP::Second<LD::Float>(newPeriod);
                
            }
            return resultantPeriod;
        }
        
        template<typename ... Context>
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(executionEvent);
                          });
                
            }
        }
        
        
        template<typename ... Context>
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(frameEndedEvent);
                          });
                
            }
        }
        
        template<typename ... Context>
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            for (LD::UInteger n = 0; n<HeteregenousContainer.GetSize(); ++n )
            {
                LD::Match(this->HeteregenousContainer[n].Object,
                          [&](auto && indexedObject)
                          {
                              return indexedObject(quittingEvent);
                          });
                
            }
        }
        
        virtual void operator()( LD::UInteger objectID )
        {
            bool canRemove = false;
            for (LD::UInteger n = 0; n<this->HeteregenousContainer.GetSize(); ++n )
            {
                if (this->HeteregenousContainer[n].Identification == objectID)
                {
                    canRemove = true;
                    break;
                }
            }
            if (canRemove == true)
            {
                this->HeteregenousContainer.Remove(objectID);
                this->IdentificationStack.Push(objectID);
            }
            
        }
    };
     */
}

/*

 */

#endif /* Schduler_h */
