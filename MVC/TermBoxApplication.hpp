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
#include "Functor/fixed_size_function.hpp"
namespace LD
{

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
}
#endif //LANDESSDEVDATASTRUCTURES_TERMBOXAPPLICATION_H
