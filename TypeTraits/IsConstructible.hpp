//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISCONSTRUCTIBLE_HPP
#define LANDESSDEVCORE_ISCONSTRUCTIBLE_HPP

#include "IsFunction.hpp"
#include "AddRValueReference.hpp"
#include "IntegralConstant.hpp"
#include "RemoveAllExtents.hpp"
#include "RemoveReference.hpp"
#include "Conditional.hpp"
#include "Declval.hpp"
#include "RemoveConst.hpp"
#include "IsBaseOf.hpp"
#include "IsSwappable.hpp"

/*
 * #if __has_feature(is_constructible)

namespace LD
{
    template <class _Tp, class ..._Args>
    struct  IsConstructible
    : public LD::Detail::IntegralConstant<bool, __is_constructible(_Tp, _Args...)>
    {};
}



#endif // __has_feature(is_constructible)
 */
namespace LD
{
    namespace Detail
    {
        template<class T>
        struct TypeIdentity
        {
            // Used to work around Visual C++ 2008's spurious error: "a function-style conversion to a built-in type can only take one argument"
            typedef T type;
        };
        template<typename T, typename U>
        struct IsMoreConst : LD::Detail::IntegralConstant<bool, false> {};

        template<typename T, typename U>
        struct IsMoreConst<const T, U> : LD::Detail::IntegralConstant<bool, true> {};

        template<typename T, typename U>
        struct IsMoreConst<const T, const U> : LD::Detail::IntegralConstant<bool, false> {};

        template<typename T, typename U>
        struct IsMoreVolatile : LD::Detail::IntegralConstant<bool, false> {};

        template<typename T, typename U>
        struct IsMoreVolatile<volatile T, U> : LD::Detail::IntegralConstant<bool, true> {};

        template<typename T, typename U>
        struct IsMoreVolatile<volatile T, volatile U> : LD::Detail::IntegralConstant<bool, false> {};



        typedef char yes[1];
        typedef char no[2];

        template<typename T, typename U>
        struct IsMoreCV : LD::Detail::IntegralConstant<bool, IsMoreConst<T,U>::value && IsMoreVolatile<T,U>::value> {};
        template<typename T>
        struct is_default_constructible {
            template<typename U>
            static yes& test(int(*)[sizeof(new U)]);
            template<typename U>
            static no& test(...);
            enum {
                value = sizeof(test<T>(0)) == sizeof(yes)
            };
        };

        template<typename T, typename Arg>
        struct is_constructible_1 {
            template<typename U, typename Arg_>
            static yes& test(int(*)[sizeof(typename TypeIdentity<U>::type(static_cast<Arg_>(*((typename LD::RemoveReference<Arg_>::type*)0))))]);
            template<typename U, typename Arg_>
            static no& test(...);
            enum {
                value = sizeof(test<T, Arg>(0)) == sizeof(yes)
            };
        };


        // Base pointer construct from Derived Pointer
        template<typename T, typename U>
        struct is_constructible_1<T*, U*>
                : LD::Detail::Conditonal<
                        IsVoid<typename LD::RemoveConst<T>::type>::value,
                        LD::Detail::IntegralConstant<bool, true>,
                        typename Conditonal<
                                IsVoid<typename LD::RemoveConst<U>::type>::value,
                                LD::Detail::IntegralConstant<bool, false>,
                                typename LD::Detail::Conditonal<
                                        IsMoreCV<T, U>::value,
                                        LD::Detail::IntegralConstant<bool, false>,
                                        LD::Detail::IsBaseOf<T,U>
                                >::type
                        >::type
                >::type
        {};

        // Base pointer construct from Derived Pointer
        template<typename T, typename U>
        struct is_constructible_1<T&, U&>
                : Conditonal<
                        IsMoreCV<T, U>::value,
                        LD::Detail::IntegralConstant<bool, false>,
                        LD::Detail::IsBaseOf<T,U>
                >::type
        {};


        template<typename T, typename Arg1, typename Arg2>
        struct is_constructible_2 {
            template<typename U, typename Arg1_, typename Arg2_>
            static yes& test(int(*)[
            sizeof(typename TypeIdentity<U>::type(
                    static_cast<Arg1_>(*((typename LD::Detail::RemoveReference<Arg1_>::type*)0)),
                    static_cast<Arg2_>(*((typename LD::Detail::RemoveReference<Arg2_>::type*)0))
            ))
            ]);
            template<typename U, typename Arg1_, typename Arg2_>
            static no& test(...);
            enum {
                value = sizeof(test<T, Arg1, Arg2>(0)) == sizeof(yes)
            };
        };
    }
    template<typename T, typename Arg1 = void, typename Arg2 = void>
    struct IsConstructible : LD::Detail::IntegralConstant<bool, Detail::is_constructible_2<T, Arg1, Arg2>::value> {

    };

    template<typename T, typename Arg>
    struct IsConstructible<T, Arg> : LD::Detail::IntegralConstant<bool, Detail::is_constructible_1<T, Arg>::value> {

    };
    template<typename T>
    struct IsConstructible<T> : LD::Detail::IntegralConstant<bool, Detail::is_default_constructible<T>::value> {

    };




    template <class _Tp>
    struct  IsMoveConstructible
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
            : public IsConstructible<_Tp, typename LD::Detail::AddRvalueReference<_Tp>::type>
#else
        : public IsConstructible<_Tp>
#endif
    {};



}
#endif //LANDESSDEVCORE_ISCONSTRUCTIBLE_HPP
