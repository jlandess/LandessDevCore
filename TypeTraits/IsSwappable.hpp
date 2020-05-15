//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_ISSWAPPABLE_HPP
#define LANDESSDEVCORE_ISSWAPPABLE_HPP
#include "IntegralConstant.hpp"
#include "IsAssignable.hpp"
#include "Algorithms/Swap.hpp"
#include "IsConstructible.hpp"
#include "IsSame.hpp"
namespace LD
{
    namespace Detail
    {
        struct tag {};

        template<class T>
        tag swap(T&, T&);

        template<typename T>
        struct would_call_std_swap_impl {

            template<typename U>
            static auto check(int)
            -> LD::Detail::IntegralConstant<bool, LD::Detail::IsSame<decltype( LD::Swap(Declval<U&>(), Declval<U&>())), tag>::value>;

            template<typename>
            static LD::FalseType check(...);

            using type = decltype(check<T>(0));
        };

        template<typename T>
        struct would_call_std_swap : would_call_std_swap_impl<T>::type { };
        using LD::Swap;

        template<typename T>
        struct can_call_swap_impl {

            template<typename U>
            static auto check(int)
            -> decltype( Swap(Declval<T&>(), Declval<T&>()),
                    LD::TrueType());

            template<typename>
            static LD::FalseType check(...);

            using type = decltype(check<T>(0));
        };

        template<typename T>
        struct can_call_swap : can_call_swap_impl<T>::type { };
        template<typename T>
        struct IsSwappable :
                LD::Detail::IntegralConstant<bool,
                        Detail::can_call_swap<T>::value &&
                        (!Detail::would_call_std_swap<T>::value ||
                         (LD::IsMoveAssignable<T>::value &&
                          IsMoveConstructible<T>::value))
                > { };
    }

}
#endif //LANDESSDEVCORE_ISSWAPPABLE_HPP
