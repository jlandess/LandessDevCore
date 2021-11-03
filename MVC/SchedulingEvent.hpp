//
//  SchedulingEvent.h
//  DataStructures5
//
//  Created by James Landess on 2/3/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef SchedulingEvent_h
#define SchedulingEvent_h

#include "Definitions/PDPUnits.hpp"
#include "Functor/LightWeightDelegate.h"
#include "Definitions/Integer.hpp"
#include "Functor/FastDelegate.h"
#include "Memory/ElementReference.h"
#include "Core/HeteregenousTuple.hpp"
#include "Functor/fixed_size_function.hpp"
#include "Algorithms/Sampler.hpp"
#include "Functor/Delegate.h"
#include "Chrono/Timer.h"
#include "MVC/TermBoxRenderingContext.hpp"
#include "Core/HeteregenousTuple.hpp"

namespace LD
{
    template<typename ... A>
    class HeteregenousTuple;
    template<typename ... Context>
    class ApplicationStartedEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
        ElementReference<LD::Timer> TimerReference;
    public:

        template<typename ... A, typename = typename LD::Enable_If_T<LD::IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationStartedEvent(const LD::HeteregenousTuple<ElementReference<A>...> & context,
                                LD::Timer & timer) noexcept :CurrentContext(context),TimerReference(timer)
        {
            
        }
        //Assignable<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>
        >>>
        ApplicationStartedEvent(const LD::ApplicationStartedEvent<A...> & event) noexcept :CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}

        template<typename ... A, typename = typename LD::Enable_If_T<
                LD::Require<
                        LD::is_permutation_v<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>
                >>>
        ApplicationStartedEvent(const LD::ApplicationStartedEvent<A...> & event, const PDP::Second<LD::Float> & dt) noexcept :CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{dt}
        {}
        
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            //return PDP::Second<LD::Float>(this->TimerReference->GetElapsedTimeInSec());
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        
        
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        template<typename ... A>
        friend class ApplicationStartedEvent;
    };
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationStartedEvent<Context...> & applicationStartEvent) noexcept
    {
        return LD::Get<ElementReference<T>>(applicationStartEvent.GetContext());
    }
    
    
    template<typename ... Context>
    class ApplicationFrameStartedEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
        ElementReference<LD::Timer>  TimerReference;
    public:
        
        
        ApplicationFrameStartedEvent() noexcept{}
        template<typename ... A, typename = typename LD::Enable_If_T<IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationFrameStartedEvent(const LD::HeteregenousTuple<ElementReference<A>...> & context,LD::Timer & timer) noexcept :CurrentContext(context),TimerReference(timer),mElapsedTime{timer}
        {}
        
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
        >>>
        ApplicationFrameStartedEvent(const LD::ApplicationFrameStartedEvent<A...> & event) noexcept :CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}

        /*
        template<typename ... A, typename = typename LD::Enable_If_T<
                LD::Require<
                        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
                >>>
        ApplicationFrameStartedEvent(const LD::ApplicationFrameStartedEvent<A...> & event, const PDP::Second<LD::Float> & dt) noexcept :CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{dt}
        {}
         */

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            //return PDP::Second<LD::Float>(this->TimerReference->GetElapsedTimeInSec());
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }
        
        
        
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        
        
         template<typename ... A>
         friend class ApplicationFrameStartedEvent;
        
    };
    
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent) noexcept
    {
        return LD::Get<ElementReference<T>>(applicationFrameStartedEvent.GetContext());
    }
    
    template<typename ... Context>
    class ApplicationExecutionEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        ElementReference<LD::Timer> TimerReference;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
    public:
        
        template<typename ... A, typename = typename LD::Enable_If_T<IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationExecutionEvent(const LD::HeteregenousTuple<ElementReference<A>...> & context,LD::Timer & timer) noexcept:CurrentContext(context),TimerReference(timer),mElapsedTime{timer}
        {}
        
        
        
        //Assignable<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
        >>>
        ApplicationExecutionEvent(const LD::ApplicationExecutionEvent<A...> & event) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}

        //Assignable<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value
        template<typename ... A, typename = typename LD::Enable_If_T<
                LD::Require<
                        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
                >>>
        ApplicationExecutionEvent(const LD::ApplicationExecutionEvent<A...> & event, const PDP::Second<LD::Float> & dt) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{dt}
        {}
        
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            //return PDP::Second<LD::Float>(this->TimerReference->GetElapsedTimeInSec());
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        
        
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        
        
        template<typename ... A>
        friend class ApplicationExecutionEvent;
        
    };
    
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationExecutionEvent<Context...> & executionEvent) noexcept
    {
        return LD::Get<ElementReference<T>>(executionEvent.GetContext());
    }
    
    
    template<typename ... Context>
    class ApplicationFrameEndedEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
        ElementReference<LD::Timer> TimerReference;
    public:
        
        template<typename ... A, typename = typename LD::Enable_If_T<IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationFrameEndedEvent(const LD::HeteregenousTuple<ElementReference<A>...> & context,LD::Timer & timer) noexcept:CurrentContext(context),TimerReference(timer),mElapsedTime{timer}
        {}
        
        //Assignable<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
        >>>
        ApplicationFrameEndedEvent(const LD::ApplicationFrameEndedEvent<A...> & event) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}
        
        
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        
        
        ElementReference<LD::Timer> GetTimer() const noexcept
        {
            return this->TimerReference;
        }
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        template<typename ... A>
        friend class ApplicationFrameEndedEvent;
    };
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationFrameEndedEvent<Context...> & applicationFrameEndedEvent) noexcept
    {
        return LD::Get<ElementReference<T>>(applicationFrameEndedEvent.GetContext());
    }
    
    
    template<typename ... Context>
    class ApplicationQuittingEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
        ElementReference<LD::Timer> TimerReference;
    public:
        
        template<typename ... A, typename = typename LD::Enable_If_T<IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationQuittingEvent(const LD::HeteregenousTuple<ElementReference<A>...> & context,LD::Timer & timer) noexcept:CurrentContext(context),TimerReference(timer),mElapsedTime{timer}
        {}
        
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
        >>>
        ApplicationQuittingEvent(const LD::ApplicationQuittingEvent<A...> & event) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}

        template<typename ... A, typename = typename LD::Enable_If_T<
                LD::Require<
                        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
                >>>
        ApplicationQuittingEvent(const LD::ApplicationQuittingEvent<A...> & event, const PDP::Second<LD::Float> & dt) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{dt}
        {}
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        
        
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        template<typename ... A>
        friend class ApplicationQuittingEvent;
    };
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationQuittingEvent<Context...> & applicationQuittingEvent) noexcept
    {
        return LD::Get<ElementReference<T>>(applicationQuittingEvent.GetContext());
    }
    
    template<typename ... Context>
    class ApplicationPeriodEvent
    {
    private:
        LD::HeteregenousTuple<ElementReference<Context>...> CurrentContext;
        LD::Variant<LD::ElementReference<LD::Timer>,PDP::Second<LD::Float>> mElapsedTime;
        ElementReference<LD::Timer> TimerReference;
    public:
        
        template<typename ... A, typename = typename LD::Enable_If_T<IsUniquelyContained<LD::VariadicPack<Context...>,LD::VariadicPack<A...>>::value>>
        ApplicationPeriodEvent(const LD::HeteregenousTuple<LD::ElementReference<A>...> & context,LD::Timer & timer) noexcept:CurrentContext(context),TimerReference(timer),mElapsedTime{timer}
        {}
        
        template<typename ... A, typename = typename LD::Enable_If_T<
        LD::Require<
        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
        >>>
        ApplicationPeriodEvent(const LD::ApplicationPeriodEvent<A...> & event) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{event.mElapsedTime}
        {}

        template<typename ... A, typename = typename LD::Enable_If_T<
                LD::Require<
                        LD::is_permutation_v<LD::VariadicPack<Context...>, LD::VariadicPack<A...>>
                >>>
        ApplicationPeriodEvent(const LD::ApplicationPeriodEvent<A...> & event, const PDP::Second<LD::Float> & dt) noexcept:CurrentContext(event.CurrentContext),TimerReference(event.TimerReference),mElapsedTime{dt}
        {}
        
        
        PDP::Second<LD::Float> GetElapsedTime() const noexcept
        {
            auto onTimerOnly = [](const LD::ElementReference<LD::Timer> & timer) noexcept
            {
                return PDP::Second<LD::Float>(timer->GetElapsedTimeInSec());
            };

            auto onDT = [](const PDP::Second<LD::Float> & elapsedtime) noexcept
            {
                return elapsedtime;
            };
            return LD::Match(this->mElapsedTime,onTimerOnly,onDT);
        }

        LD::MicroSecond<LD::Float> TimeElapsed() const noexcept
        {
            return this->TimerReference->Time();
        }
        
        
        const LD::HeteregenousTuple<ElementReference<Context>...> & GetContext() const noexcept
        {
            return this->CurrentContext;
        }
        
        
        template<typename ... A>
        friend class ApplicationPeriodEvent;
    };
    
    template<typename T, typename ... Context>
    auto Get(const ApplicationPeriodEvent<Context...> & applicationPeriodEvent) noexcept
    {
        return LD::Get<LD::ElementReference<T>>(applicationPeriodEvent.GetContext());
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
            //&F::operator();
            //PDP::Delegate<const bool(const LD::ApplicaitonStartedEvent<A...> &)>(LD::Forward<F>(functor));
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
    
    */
    
    template<typename T, template<typename> class = PDP::Delegate,class = void>
    class ApplicationImpl;
    
    
    template<typename ...A, template<typename> class Functor>
    class ApplicationImpl<LD::VariadicPack<A...>,Functor,LD::Enable_If_T<
    LD::Require<
    LD::IsUnique<A...>
    >>>
    {
    private:
        Functor<const bool(const LD::ApplicationStartedEvent<A...> &)> ApplicationStarted;
        Functor<const bool(const LD::ApplicationFrameStartedEvent<A...> &)> FrameStarted;
        Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)> SleepingEvent;
        Functor<void(const LD::ApplicationFrameEndedEvent<A...> &)> FrameEnded;
        Functor<void(const LD::ApplicationExecutionEvent<A...> &)> ExecutionEvent;
        Functor<void(const LD::ApplicationQuittingEvent<A...> &)> QuittingEvent;
        
    public:
        
        ApplicationImpl()
        {
            this->ApplicationStarted = Functor<const bool(const LD::ApplicationStartedEvent<A...> &)>([](const LD::ApplicationStartedEvent<A...> &)
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
        LD::ConvertiblyCallable<F, bool(const LD::ApplicationStartedEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnApplicationStart(F && functor)
        {
            //&F::operator();
            //PDP::Delegate<const bool(const LD::ApplicaitonStartedEvent<A...> &)>(LD::Forward<F>(functor));
            this->ApplicationStarted = Functor<const bool(const LD::ApplicationStartedEvent<A...> &)>(LD::Forward<F>(functor));
            
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, bool(const LD::ApplicationStartedEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            //&F::operator();
            //PDP::Delegate<const bool(const LD::ApplicaitonStartedEvent<A...> &)>(LD::Forward<F>(functor));
            this->ApplicationStarted = Functor<const bool(const LD::ApplicationStartedEvent<A...> &)>(LD::Forward<F>(functor));

            return (*this);
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<F, bool(const LD::ApplicationFrameStartedEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnFrameStarted(F && functor)
        {
            this->FrameStarted = Functor<const bool(const LD::ApplicationFrameStartedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, bool(const LD::ApplicationFrameStartedEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            this->FrameStarted = Functor<const bool(const LD::ApplicationFrameStartedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<F, PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnPeriodRequest(F && functor)
        {
            this->SleepingEvent = Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }


        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            this->SleepingEvent = Functor<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<F, void(const LD::ApplicationFrameEndedEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnFrameEnded(F && functor)
        {
            this->FrameEnded = Functor<void(const LD::ApplicationFrameEndedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationFrameEndedEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            this->FrameEnded = Functor<void(const LD::ApplicationFrameEndedEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<F, void(const LD::ApplicationExecutionEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnExecution(F && functor)
        {
            this->ExecutionEvent = Functor<void(const LD::ApplicationExecutionEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationExecutionEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            this->ExecutionEvent = Functor<void(const LD::ApplicationExecutionEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }
        
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::ConvertiblyCallable<F, void(const LD::ApplicationQuittingEvent<A...> &)>::Value()
        >,ApplicationImpl &> OnQuit(F && functor)
        {
            this->QuittingEvent = Functor<void(const LD::ApplicationQuittingEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }

        template<typename F>
        LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<F, void(const LD::ApplicationQuittingEvent<A...> &)>::Value()
                >,ApplicationImpl &> operator()(F && functor)
        {
            this->QuittingEvent = Functor<void(const LD::ApplicationQuittingEvent<A...> &)>(LD::Forward<F>(functor));
            return (*this);
        }
        
        
        const bool operator()(const LD::ApplicationStartedEvent<A...> & applicationStartedEvent)
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
    
    
    //template<typename ...A>
    //using TermBoxApplication = LD::TermBoxApplicationImpl<LD::VariadicPack<A...>>;
    
    template<typename ...A>
    using Application = LD::ApplicationImpl<LD::VariadicPack<A...>>;
    
    template<typename T, typename U ,template<typename> class F = PDP::LightWeightDelegate> class ApplicationEvent;
    
    
    
    template<typename ThermalEvaluator>
    class ThermalMonitor
    {
    private:
        
        PDP::Second<LD::Float> CurrentPeriod;
        LD::ElementReference<ThermalEvaluator> Thermal;
    public:
        inline ThermalMonitor()
        {
            
        }
        ThermalMonitor(const LD::ElementReference<ThermalEvaluator> & thermal ,const PDP::Second<LD::Float> & defaultPeriod = PDP::Second<LD::Float>(1.0)):CurrentPeriod(defaultPeriod),Thermal(thermal)
        {
            
        }
        
        template<typename ... Context>
        const bool operator()(const LD::ApplicationStartedEvent<Context...> & applicationStartedEvent)
        {
            return true;
        }
        template<typename ... Context>
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            return true;
        }
        template<typename ... Context>
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            return this->CurrentPeriod;
        }
        template<typename ... Context>
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            
        }
        template<typename ... Context>
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            
        }
        
        LD::Float GetTemp(const LD::UInteger & core = 0) const
        {
            return Thermal->GetPhysicalCPUTemperature(core);
        }
        
        LD::Float GetCPULoad() const
        {
            return Thermal->GetCPULoad();
        }
        
        template<typename ... Context>
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            
        }
    };
    
    /*
    class MouseAndKeyboard
    {
    private:
        PDP::Keyboard keyboard;
        PDP::Mouse CurrentMouse;
    public:
        
        PDP::Keyboard & GetKeyboard() {return this->keyboard;}
        PDP::Mouse & GetMouse()  {return this->CurrentMouse;}
    };
    
    template<typename Input>
    class IOInput
    {
    private:
        
        PDP::Second<LD::Float> CurrentPeriod;
        LD::ElementReference<Input> InputIO;
    public:
        inline IOInput()
        {
            
        }
        inline IOInput(const LD::ElementReference<Input> & input,const PDP::Second<LD::Float> & defaultPeriod = PDP::Second<LD::Float>(1.0)):CurrentPeriod(defaultPeriod),InputIO(input)
        {
            
        }
        template<typename ... Context>
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            return true;
        }
        template<typename ... Context>
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            return true;
        }
        template<typename ... Context>
        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            return this->CurrentPeriod;
        }
        template<typename ... Context>
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            
        }
        template<typename ... Context>
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            
        }
        template<typename ... Context>
        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            
        }
    };
     */
    
    /*
    template<typename ... Context>
    class TermBoxApplicationExample
    {
    private:
        LD::UInteger Index;
        LD::Sampler<LD::Float,64> FPS;
        LD::Sampler<LD::Float,64> CPULoad;
    public:
        TermBoxApplicationExample()
        {
            uint32_t cpuInfo[4];
            
            this->Index = 1;
        }
        
        ~TermBoxApplicationExample()
        {
            
        }
        const bool operator()(const LD::ApplicaitonStartedEvent<LD::TermBoxRenderContext,Context...> & applicationStartedEvent)
        {
            return true;
        }
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,Context...> & applicationFrameStartedEvent)
        {
            //run as long as you the user has not pressed the letter s
            return !LD::Get<LD::MouseAndKeyboard>(applicationFrameStartedEvent)->GetKeyboard().GetKeyState(PDP::kVK_ANSI_S);
        }
        
        LD::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,Context...> & sleepingEvent)
        {
            return LD::Second<LD::Float>(1.0/144.0);
        }
        
        void operator()(const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,Context...> & executionEvent)
        {
            
            this->FPS.Sample(executionEvent.GetElapsedTime().GetValue());
            
            
            //PDP::Promise<int> futureResult =  currentPool.EnqueueWithPromise(function,x);
            char output[80];
            memset(output, 0, 80);
            LD::ElementReference<LD::ThermalMonitor<LD::System>> thermal =  LD::Get<LD::ThermalMonitor<LD::System>>(executionEvent);
            
            
            this->CPULoad.Sample(thermal->GetCPULoad());
            
            //{:03.3f}
            //fmt::format_to(output, "Application State Size {} {} \n", sizeof(LD::ApplicationState),1.0/this->FPS());
            
            LD::Get<LD::TermBoxRenderContext>(executionEvent)->Write(output);
            memset(output, 0, 80);
            //LD::Get<LD::System>(executionEvent);
            
            fmt::format_to(output, "{},{},{},{} {}", thermal->GetTemp(0),thermal->GetTemp(1),thermal->GetTemp(2),thermal->GetTemp(3));
            
            
            LD::Get<LD::TermBoxRenderContext>(executionEvent)->Write(output);
            
            
            
        }
        
        void operator()(const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,Context...> & frameEndedEvent)
        {
            
        }
        
        void operator()(const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,Context...> & quittingEvent)
        {
            std::cout << "Application has quit with index : " << this->Index << std::endl;
        }
    };
    */
    
    
    
    
    
    template<typename T, typename Huerestic ,typename ... Args, template<typename> class F>
    class ApplicationEvent<T(Args...),Huerestic,F>
    {
    private:
        F<const bool(const ApplicationStartedEvent<Args...> &)> ApplicationStart;
        
        F<const bool(const LD::ApplicationFrameStartedEvent<Args...> &)> FrameStart;
        
        F<T(const LD::ApplicationExecutionEvent<Args...> &)> Executor;
        
        F<Huerestic(const LD::ApplicationExecutionEvent<Args...> &)> SortablePredicate;
        
        
        F<PDP::Second<LD::Float>(const LD::ApplicationPeriodEvent<Args...> &)> Period;
        
        F<void(const LD::ApplicationFrameEndedEvent<Args...> &)> FrameEnded;
        
        F<const bool(const LD::ApplicationQuittingEvent<Args...> &)> QuitPredicate;
        
        
        
        
        
        Huerestic CachedHuerestic;
        T CachedValue;
        LD::UInteger Identification;
    public:
        
        
        inline ApplicationEvent()
        {
            
        }
        
        /*
        inline ApplicationEvent(const F<T(Args...)> & executor,
                     const F<PDP::Second<LD::Float>()> & period,
                     const F<Huerestic()> & sortablePredicate,
                     F<const bool()> & quitPredicate,
                     const LD::UInteger & id):Executor(executor),Period(period),SortablePredicate(sortablePredicate),QuitPredicate(quitPredicate),ID(id)
        {
            
        }
         */
        
        const bool operator()(const ApplicationStartedEvent<Args...> & applicationStartedEvent)
        {
            return this->ApplicationStart(applicationStartedEvent);
        }
        
        T operator()(const LD::ApplicationExecutionEvent<Args...> & applicationExecutionEvent)
        {
            Huerestic firstHeuristic = SortablePredicate(applicationExecutionEvent);
            this->CachedValue =  Executor(applicationExecutionEvent);
            Huerestic secondHeuristic = SortablePredicate(applicationExecutionEvent);
            
            this->CachedHuerestic = (secondHeuristic-firstHeuristic);
            
            return this->CachedValue;
        }
        
        /*
        void operator()(const LD::ApplicationInputEvent<Args...> & applicationInputEvent)
        {
            return Input(applicationInputEvent);
        }
         */
        
        
        void operator()(const LD::ApplicationPeriodEvent<Args...> & applicationPeriodEvent)
        {
            return this->Period(applicationPeriodEvent);
        }
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<Args...> & applicationFrameStartedEvent)
        {
            return this->FrameStart(applicationFrameStartedEvent);
        }
        
        void operator()(const LD::ApplicationFrameEndedEvent<Args...> & applicationFrameEndedEvent)
        {
            return this->FrameEnded(applicationFrameEndedEvent);
        }
        
        const bool operator()(const LD::ApplicationQuittingEvent<Args...> & applicationQuitEvent)
        {
            return this->QuitPredicate(applicationQuitEvent);
        }
        
        inline const bool operator < (const ApplicationEvent<T(Args...),Huerestic,F> & b) const
        {
            const bool A = this->Period().GetValue() < b.Period().GetValue();
            const bool B = (this->CachedHuerestic < b.CachedHuerestic);
            const bool C = (this->CachedValue < b.CachedValue);
            
            
            return (this->Period().GetValue() < b.Period().GetValue()) && (this->CachedHuerestic < b.CachedHuerestic) && (this->CachedValue < b.CachedValue);
        }
        
        inline const bool operator == (const ApplicationEvent<T(Args...),Huerestic,F> & b) const
        {
            const bool A = this->Period().GetValue() == b.Period().GetValue();
            const bool B = (this->CachedHuerestic == b.CachedHuerestic);
            const bool C = (this->CachedValue == b.CachedValue);
            
            
            return (this->Period().GetValue() == b.Period().GetValue()) && (this->CachedHuerestic == b.CachedHuerestic) && (this->CachedValue == b.CachedValue);
        }
        
    };
    
    template<typename Huerestic ,typename ... Args, template<typename> class F>
    class ApplicationEvent<void(Args...),Huerestic,F>
    {
    private:
        F<void(Args...)> Executor;
        F<PDP::Second<LD::Float>()> Period;
        F<Huerestic()> SortablePredicate;
        F<const bool()> QuitPredicate;
        Huerestic CachedHuerestic;
        LD::UInteger ID;
    public:
        inline ApplicationEvent(const F<void(Args...)> & executor,
                     const F<PDP::Second<LD::Float>()> & period,
                     const F<Huerestic()> & sortablePredicate,
                     F<const bool()> & quitPredicate,
                     const LD::UInteger & id):Executor(executor),Period(period),SortablePredicate(sortablePredicate),QuitPredicate(quitPredicate),ID(id)
        {
            
        }
        
        void operator()(Args && ... arguements) const
        {
            Huerestic firstHeuristic = SortablePredicate();
            
            
            Executor(LD::Forward<Args>(arguements)...);
            
            
            Huerestic secondHeuristic = SortablePredicate();
            
            Huerestic differential = (secondHeuristic-firstHeuristic);
            
            this->CachedHuerestic = differential;
        }
        
        inline const bool ShouldQuit() const
        {
            return this->QuitPredicate();
        }
        
        inline const bool operator < (const ApplicationEvent<void(Args...),Huerestic,F> & b) const
        {
            return (this->Period().GetValue() < b.Period().GetValue()) && (this->CachedHuerestic < b.CachedHuerestic);
        }
        
        inline const bool operator == (const ApplicationEvent<void(Args...),Huerestic,F> & b) const
        {
            return (this->Period().GetValue() == b.Period().GetValue()) && (this->CachedHuerestic == b.CachedHuerestic);
        }
        
    };
    template<typename T ,typename ... Args, template<typename> class F>
    class ApplicationEvent<T(Args...),LD::NullClass,F>
    {
    private:
        F<T(Args...)> Executor;
        F<PDP::Second<LD::Float>()> Period;
        F<const bool()> QuitPredicate;
        LD::UInteger ID;
    public:
        inline ApplicationEvent(const F<T(Args...)> & executor,
                     const F<PDP::Second<LD::Float>()> & period,
                     F<const bool()> & quitPredicate,
                     const LD::UInteger & id):Executor(executor),Period(period),QuitPredicate(quitPredicate),ID(id)
        {
            
        }
        
        T operator()(Args && ... arguements) const
        {
            return Executor(LD::Forward<Args>(arguements)...);
        }
        inline const bool ShouldQuit() const
        {
            return this->QuitPredicate();
        }
        
        inline const bool operator < (const ApplicationEvent<T(Args...),LD::NullClass,F> & b) const
        {
            return this->Period.GetValue() < b.Period.GetValue();
        }
        
        inline const bool operator == (const ApplicationEvent<T(Args...),LD::NullClass,F> & b) const
        {
            return this->Period.GetValue() == b.Period.GetValue();
        }
    };
    
    template<typename ... Args, template<typename> class F>
    class ApplicationEvent<void(Args...),LD::NullClass,F>
    {
    private:
        F<void(Args...)> Executor;
        F<PDP::Second<LD::Float>()> Period;
        F<const bool()> QuitPredicate;
        LD::UInteger ID;
    public:
        inline ApplicationEvent(const F<void(Args...)> & executor,
                     const F<PDP::Second<LD::Float>()> & period,
                     F<const bool()> & quitPredicate,
                     const LD::UInteger & id):Executor(executor),Period(period),QuitPredicate(quitPredicate),ID(id)
        {
            
        }
        
        void operator()(Args && ... arguements) const
        {
            return Executor(LD::Forward<Args>(arguements)...);
        }
        
        inline const bool ShouldQuit() const
        {
            return this->QuitPredicate();
        }
        
        inline const bool operator < (const ApplicationEvent<void(Args...),LD::NullClass,F> & b) const
        {
            return this->Period.GetValue() < b.Period.GetValue();
        }
        
        inline const bool operator == (const ApplicationEvent<void(Args...),LD::NullClass,F> & b) const
        {
            return this->Period.GetValue() == b.Period.GetValue();
        }
    };
}
#endif /* SchedulingEvent_h */
