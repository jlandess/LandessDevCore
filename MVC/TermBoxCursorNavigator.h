//
// Created by James Landess on 2/3/20.
//

#ifndef LANDESSDEVCORE_TERMBOXCURSORNAVIGATOR_H
#define LANDESSDEVCORE_TERMBOXCURSORNAVIGATOR_H

#include "Definitions/Common.h"
#include "MVC/Scheduling.h"
#include "Memory/ElementReference.h"
#include "Definitions/PDPUnits.h"
#include "Primitives/General/mapboxvariant.h"
namespace LD
{
    namespace TB
    {
        class CursorLeftResult
        {
        private:
            LD::UInteger mValue;
        public:
            inline constexpr CursorLeftResult() noexcept :mValue{false} {}
            inline constexpr CursorLeftResult(const bool & value) noexcept :mValue{value}{}
            constexpr const LD::UInteger & Value() noexcept {return this->mValue;}
        };

        class CursorRightResult
        {
        private:
            LD::UInteger mValue;
        public:
            inline constexpr CursorRightResult() noexcept :mValue{false} {}
            inline constexpr CursorRightResult(const bool & value) noexcept :mValue{value}{}
            constexpr const LD::UInteger & Value() noexcept {return this->mValue;}
        };

        class CursorDownResult
        {
        private:
            LD::UInteger mValue;
        public:
            inline constexpr CursorDownResult() noexcept :mValue{false} {}
            inline constexpr CursorDownResult(const bool & value) noexcept :mValue{value}{}
            constexpr const LD::UInteger & Value() noexcept {return this->mValue;}
        };

        class CursorUpResult
        {
        private:
            LD::UInteger mValue;
        public:
            inline constexpr CursorUpResult() noexcept :mValue{false} {}
            inline constexpr CursorUpResult(const bool & value) noexcept :mValue{value}{}
            constexpr const LD::UInteger & Value() noexcept {return this->mValue;}
        };
        //template<typename LeftPredicate, typename RightPredicate, typename DownPredicate, typename UpPredicate, typename Context ,class sfinae = void>
        //class BasicCursorNavigator;

        template<typename Preidcates, typename Context ,class sfinae = void>
        class BasicCursorNavigator;


        template<typename ... Predicates, typename ... Context>
        class BasicCursorNavigator<LD::VariadicPack<Predicates...>,LD::VariadicPack<Context...>,
                LD::Enable_If_T<
                LD::Require<
                        (LD::ConvertiblyCallable<Predicates,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                        (LD::ConvertiblyCallable<Predicates,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                        (LD::ConvertiblyCallable<Predicates,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                        (LD::ConvertiblyCallable<Predicates,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value
                >>>
        {
        private:
            static constexpr LD::UInteger LeftPredicateIndex = 0;
            static constexpr LD::UInteger RightPredicateIndex = 1;
            static constexpr LD::UInteger DownPredicateIndex = 2;
            static constexpr LD::UInteger UpPredicateIndex= 3;

            //LD::Variant<LD::NullClass,LD::ElementReference<LeftPredicate>,LD::ElementReference<RightPredicate>,LD::ElementReference<DownPredicate>,LD::ElementReference<UpPredicate>> mPredicates[4];
            LD::Variant<LD::ElementReference<Predicates>...> mPredicates[4];
        public:

            typedef LD::Variant<LD::ElementReference<Predicates>...> PredicateType;
            BasicCursorNavigator() = default;


            template<typename ... Args, typename = LD::Enable_If_T<
                    LD::Require<
                            (sizeof...(Args) == 4),
                            (LD::ConvertiblyCallable<Predicates,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                            (LD::ConvertiblyCallable<Predicates,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                            (LD::ConvertiblyCallable<Predicates,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                            (LD::ConvertiblyCallable<Predicates,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...)
                            >>>
            inline constexpr BasicCursorNavigator(Args & ... arguements) noexcept
            {

                auto tiedTuple = LD::Tie(arguements...);
                LD::For<4>([](auto Index,PredicateType predicateBuffer[4], auto & tiedTuple)
                {
                    using CurrentTupleType = LD::Detail::Decay_T<decltype(LD::Get<Index>(tiedTuple))>;
                    if constexpr(LD::ConvertiblyCallable<CurrentTupleType ,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value())
                    {
                        predicateBuffer[0] = LD::ElementReference<CurrentTupleType>(LD::Get<Index>(tiedTuple));
                    }else if constexpr(LD::ConvertiblyCallable<CurrentTupleType ,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value())
                    {
                        predicateBuffer[1] = LD::ElementReference<CurrentTupleType>(LD::Get<Index>(tiedTuple));
                    }else if constexpr (LD::ConvertiblyCallable<CurrentTupleType ,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value())
                    {
                        predicateBuffer[2] = LD::ElementReference<CurrentTupleType>(LD::Get<Index>(tiedTuple));
                    }else if constexpr(LD::ConvertiblyCallable<CurrentTupleType ,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value())
                    {
                        predicateBuffer[3] = LD::ElementReference<CurrentTupleType>(LD::Get<Index>(tiedTuple));
                    }
                    return true;
                },this->mPredicates,tiedTuple);

            }

            bool operator()(const LD::ApplicaitonStartedEvent<Context...> & context) const
            {
                return true;
            }

            PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> &) const
            {
                return PDP::Second<LD::Float >(1.0/60.0);
            }

            bool operator()(const LD::ApplicationFrameStartedEvent<Context...> &) const
            {
                return true;
            }

            void operator()(const LD::ApplicationExecutionEvent<Context...> & context) const
            {
                const LD::UInteger isMouseEnabled = (LD::Get<LD::TermBoxRenderContext>(context)->IsMouseEnabled());
                LD::UInteger results[4] = {0};
                LD::For<4>([](auto Index,LD::UInteger results[4], const LD::Variant<LD::ElementReference<Predicates>...>  predicateBuffer[4],const LD::ApplicationExecutionEvent<Context...> & context)
                {

                    results[Index] = LD::Match(predicateBuffer[Index],[&](const auto & predicate)
                    {
                        return (*predicate)(context).Value();

                    });

                    return true;
                },results,mPredicates,context);

                LD::Integer horizontalDirection = (isMouseEnabled*(1*(results[RightPredicateIndex]) + -1*(results[LeftPredicateIndex])));
                LD::Integer verticalDirection = (isMouseEnabled*(1*(results[UpPredicateIndex]) + -1*(results[DownPredicateIndex])));
                LD::Get<LD::TermBoxRenderContext>(context)->GetMouse().X() += horizontalDirection;
                LD::Get<LD::TermBoxRenderContext>(context)->GetMouse().Y() += verticalDirection;
                /*
                LD::Integer horizontalDirection = (isMouseEnabled*(1*(willGoRight) + -1*(willGoLeft)));
                LD::Integer verticalDirection = (isMouseEnabled*(1*(willGoUp) + -1*(willGoDown)));
                LD::Get<LD::TermBoxRenderContext>(context)->GetMouse().X() += horizontalDirection;
                LD::Get<LD::TermBoxRenderContext>(context)->GetMouse().Y() += verticalDirection;
                 */
            }

            void operator()(const LD::ApplicationFrameEndedEvent<Context...> & ) const
            {

            }

            void operator()(const LD::ApplicationQuittingEvent<Context...> &) const
            {

            }
        };

        /*
        template<typename ... Context, typename LeftPredicate, typename RightPredicate, typename DownPredicate, typename UpPredicate>
        auto MakeCursorNavigator(LeftPredicate & leftPredicate, RightPredicate & rightPRedicate, DownPredicate & downPredicate, UpPredicate & upPredicate)->BasicCursorNavigator<LeftPredicate,RightPredicate,DownPredicate,UpPredicate,LD::VariadicPack<Context...>>
        {
            return BasicCursorNavigator<LeftPredicate,RightPredicate,DownPredicate,UpPredicate,LD::VariadicPack<Context...>>{leftPredicate,rightPRedicate,downPredicate,upPredicate};
        }
         */

        template<typename ... Context, typename ... Predicates>
        LD::Enable_If_T<LD::Require<
                (LD::ConvertiblyCallable<Predicates,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value,
                sizeof...(Predicates) == 4
        >,BasicCursorNavigator<LD::VariadicPack<Predicates...>,LD::VariadicPack<Context...>>> MakeCursorNavigator(Predicates & ... predicates)
        {
            return BasicCursorNavigator<LD::VariadicPack<Predicates...>,LD::VariadicPack<Context...>>{predicates...};
        }


        template<typename ... Context, typename ... Predicates>
        LD::Enable_If_T<LD::FallBack<
                (LD::ConvertiblyCallable<Predicates,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                (LD::ConvertiblyCallable<Predicates,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value,
                sizeof...(Predicates) == 4
        >,BasicCursorNavigator<LD::VariadicPack<Predicates...>,LD::VariadicPack<Context...>>> MakeCursorNavigator(Predicates & ... predicates)
        {
            static_assert(LD::Require<(LD::ConvertiblyCallable<Predicates,CursorLeftResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                    (LD::ConvertiblyCallable<Predicates,CursorRightResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                    (LD::ConvertiblyCallable<Predicates,CursorDownResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                    (LD::ConvertiblyCallable<Predicates,CursorUpResult(const LD::ApplicationExecutionEvent<Context...> &)>::Value() || ...),
                    LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value,sizeof...(Predicates) == 4>,"Your predicates must support LD::TB::CursorRightResult,"
                                                                                                                           "LD::TB::CursorLeftResult,LD::TB::CursorUpResult, and LD::TB::CursorUpResult"
                                                                                                                           "and take a paramaeter of const LD::ApplicationExecutionEvent<Context...> &."
                                                                                                                           "There must only be 4 predicates");
            return BasicCursorNavigator<LD::VariadicPack<Predicates...>,LD::VariadicPack<Context...>>{predicates...};
        }
        /*
        template<typename ... Context, typename UniversalPredicate>
        auto MakeCursorNavigator(UniversalPredicate & universalPredicate)->BasicCursorNavigator<UniversalPredicate,UniversalPredicate,UniversalPredicate,UniversalPredicate,LD::VariadicPack<Context...>>
        {
            return BasicCursorNavigator<UniversalPredicate,UniversalPredicate,UniversalPredicate,UniversalPredicate,LD::VariadicPack<Context...>>{universalPredicate,universalPredicate,universalPredicate,universalPredicate};
        }
         */
    }
}
#endif //LANDESSDEVCORE_TERMBOXCURSORNAVIGATOR_H
