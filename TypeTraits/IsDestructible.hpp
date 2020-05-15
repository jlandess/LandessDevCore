//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISDESTRUCTIBLE_HPP
#define LANDESSDEVCORE_ISDESTRUCTIBLE_HPP

#include "IsReference.hpp"
#include "RemoveAllExtents.hpp"
#include "IntegralConstant.hpp"
#include "IsFunction.hpp"
namespace LD
{
    //  if it's a reference, return true
    //  if it's a function, return false
    //  if it's   void,     return false
    //  if it's an array of unknown bound, return false
    //  Otherwise, return "std::declval<_Up&>().~_Up()" is well-formed
    //    where _Up is remove_all_extents<_Tp>::type

    template <class>
    struct __is_destructible_apply { typedef int type; };

    template <typename _Tp>
    struct __is_destructor_wellformed {
        template <typename _Tp1>
        static char  __test (
                typename __is_destructible_apply<decltype(LD::Declval<_Tp1&>().~_Tp1())>::type
        );

        template <typename _Tp1>
        static LD::Detail::__two __test (...);

        static const bool value = sizeof(__test<_Tp>(12)) == sizeof(char);
    };

    template <class _Tp, bool>
    struct __destructible_imp;

    template <class _Tp>
    struct __destructible_imp<_Tp, false>
            : public LD::Detail::IntegralConstant<bool,
                    __is_destructor_wellformed<typename LD::RemoveAllExtents<_Tp>::type>::value> {};

    template <class _Tp>
    struct __destructible_imp<_Tp, true>
            : public LD::TrueType {};

    template <class _Tp, bool>
    struct __destructible_false;

    template <class _Tp>
    struct __destructible_false<_Tp, false> : public __destructible_imp<_Tp, LD::Detail::IsReference<_Tp>::value> {};

    template <class _Tp>
    struct __destructible_false<_Tp, true> : public LD::FalseType {};

    template <class _Tp>
    struct IsDestructible
            : public __destructible_false<_Tp, LD::Detail::IsFunction<_Tp>::value> {};

    template <class _Tp>
    struct IsDestructible<_Tp[]>
            : public LD::FalseType {};

    template <>
    struct IsDestructible<void>
            : public LD::FalseType {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
    template <class _Tp>  bool IsDestructible_V
            = IsDestructible<_Tp>::value;
#endif

}
#endif //LANDESSDEVCORE_ISDESTRUCTIBLE_HPP
