//
// Created by phoenixflower on 9/10/21.
//

#ifndef LANDESSDEVCORE_THEN_HPP
#define LANDESSDEVCORE_THEN_HPP
#include "TypeTraits/FunctionalReflection.hpp"
#include "TypeTraits/Declval.hpp"
namespace LD
{
    namespace Async
    {
        namespace Detail
        {
            template<typename T, typename Type>
            class Runnable
            {
            public:
                T mRunnable;
                typedef  T RunnableType ;
            };
        }

        namespace CT
        {
            template<typename T, typename CurrentType>
            constexpr CurrentType RunnableType(LD::Type<LD::Async::Detail::Runnable<T,CurrentType>> ) noexcept
            {
                return CurrentType{};
            }

            template<typename T>
            constexpr bool IsAsyncTask(LD::Type<T> ) noexcept
            {
                return false;
            }

            template<typename T, typename U>
            constexpr bool IsAsyncTask(LD::Type<LD::Async::Detail::Runnable<T,U>> ) noexcept
            {
                return true;
            }
        }
        namespace Detail
        {
            template<typename P, typename Fun>
            struct _promise1
            {
                static constexpr LD::UInteger Number = P::Number;
                P p_;
                Fun fun_;

                template<typename ... A>
                void set_value(A ... vs)
                {
                    if constexpr(P::Number == 0)
                    {
                        p_.set_value(fun_(vs...));
                    }else if constexpr(P::Number == 1)
                    {
                        fun_(vs...);
                        p_.set_value();
                    }
                }

                /*
                void set_value()
                {
                    p_.set_value();
                }
                 */

                template<typename E>
                void set_exception(E e) { p_.set_exception(e); }
            };
        }


        template<typename T, typename U>
        auto Then2(T task, U fun)
        {
            return [=](auto p)
            {

                task(LD::Async::Detail::_promise1<decltype(p),U>{p,fun});
            };
        }


        /*
        template<typename T, typename U, class =
                LD::Enable_If_T<LD::Either<
                LD::Require<
                        LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}),
                        (LD::CT::TypeListSize(LD::Type<decltype(GetSignatureArguments(LD::CT::FunctionSignature<decltype(&U::operator())>{}))>{}) == 0)
                >,
                LD::Require<
                        LD::Negate<LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{})>,
                        (LD::CT::TypeListSize(LD::Type<decltype(GetSignatureArguments(LD::CT::FunctionSignature<decltype(&U::operator())>{}))>{}) == 1)
                >
                        >>>
        auto Then(T task, U fun) noexcept
        {

            if constexpr(LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}) && not LD::Async::CT::IsAsyncTask(LD::Type<U>{}))
            {
                auto function =  [=](auto p)
                {

                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),U>{p,fun});
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<U>()())>>{function};
            }else if constexpr(not LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}) && not LD::Async::CT::IsAsyncTask(LD::Type<U>{}))
            {
                auto function =  [=](auto p)
                {

                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),U>{p,fun});
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<U>()(LD::Declval<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))>>()))>>{function};
            }
        }
         */

        namespace Detail
        {
            template<typename T, class = void>
            struct ArgumentCount;


            template<typename T>
            struct ArgumentCount<T,LD::Enable_If_T<
                    LD::Require<
                    LD::Async::CT::IsAsyncTask(LD::Type<T>{})
                    >>>
            {
                using RunnableType = typename T::RunnableType;
                static constexpr LD::UInteger Count = LD::CT::TypeListSize(LD::Type<decltype(LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<RunnableType>{}))>{});
            };

            template<typename T>
            struct ArgumentCount<T,LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsFunctionalSignature(LD::CT::FunctionSignature<T>{})
                    >>>
            {
                static constexpr LD::UInteger Count = LD::CT::TypeListSize(LD::Type<decltype(LD::CT::GetSignatureArguments(LD::CT::FunctionSignature<T>{}))>{});
            };
        }

        namespace CT
        {
            template<typename T>
            constexpr LD::UInteger ArgumentCount = LD::Async::Detail::ArgumentCount<T>::Count;
        }

        template<typename T, typename U, class =
        LD::Enable_If_T<LD::Either<
                LD::Require<LD::Async::CT::IsAsyncTask(LD::Type<U>{}),LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{})>,
                LD::Require<LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{})>,
                LD::Require<LD::Negate<LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{})>>,
                LD::Require<LD::Async::CT::IsAsyncTask(LD::Type<U>{}),LD::Negate<LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{})>>,
                true
        >>>
        auto Then(T task, U fun) noexcept
        {

            if constexpr(LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}) && not LD::Async::CT::IsAsyncTask(LD::Type<U>{}))
            {
                auto function =  [=](auto p) noexcept
                {

                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),U>{p,fun});
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<U>()())>>{function};
            }else if constexpr(not LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}) && not LD::Async::CT::IsAsyncTask(LD::Type<U>{}))
            {
                auto function =  [=](auto p) noexcept
                {

                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),U>{p,fun});
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<U>()(LD::Declval<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))>>()))>>{function};
            }else if constexpr(LD::CT::IsSame(LD::Async::CT::RunnableType(LD::Type<T>{}),LD::Type<void>{}) &&  LD::Async::CT::IsAsyncTask(LD::Type<U>{}))
            {
                auto function = [=](auto p)
                {
                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),typename U::RunnableType>{p,fun.mRunnable});
                };
                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<typename U::RunnableType>()())>>{function};
            }else
            {
                auto function =  [=](auto p) noexcept
                {

                    task.mRunnable(LD::Async::Detail::_promise1<decltype(p),U>{p,fun.mRunnable});
                };

                return LD::Async::Detail::Runnable<decltype(function),LD::Type<decltype(LD::Declval<typename U::RunnableType>()(LD::Declval<LD::GetType<decltype(LD::Async::CT::RunnableType(LD::Type<T>{}))>>()))>>{function};
            }
        }

    }
}
#endif //LANDESSDEVCORE_THEN_HPP
