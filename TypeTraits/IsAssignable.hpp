//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISASSIGNABLE_HPP
#define LANDESSDEVCORE_ISASSIGNABLE_HPP

#include "Declval.hpp"
#include "IntegralConstant.hpp"
#include "IsVoid.hpp"
#include "IsFunction.hpp""
#include "AddRValueReference.hpp"
#include "AddLValueReference.hpp"

namespace LD
{
    template<typename, typename _Tp> struct __select_2nd { typedef _Tp type; };

    template <class _Tp, class _Arg>
    typename __select_2nd<decltype((LD::Declval<_Tp>() = LD::Declval<_Arg>())), LD::TrueType>::type
    __is_assignable_test(int);

    template <class, class>
    LD::FalseType __is_assignable_test(...);


    template <class _Tp, class _Arg, bool = LD::Detail::IsVoid<_Tp>::value || LD::Detail::IsVoid<_Arg>::value>
    struct __is_assignable_imp
            : public decltype((LD::__is_assignable_test<_Tp, _Arg>(0))) {};

    template <class _Tp, class _Arg>
    struct __is_assignable_imp<_Tp, _Arg, true>
            : public LD::FalseType
    {
    };

    template <class _Tp, class _Arg>
    struct IsAssignable
            : public __is_assignable_imp<_Tp, _Arg> {};

//#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
    template <class _Tp, class _Arg>  bool IsAssignable_V
            = IsAssignable<_Tp, _Arg>::value;
//#endif






    // is_copy_assignable

    template <class _Tp> struct  IsCopyAssignable
            : public LD::IsAssignable<typename LD::Detail::AddLValueReference<_Tp>::type,
                    typename LD::Detail::AddLValueReference<typename LD::Detail::AddConst<_Tp>::type>::type> {};

//#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
    template <class _Tp>  bool IsCopyAssignable_V
            = IsCopyAssignable<_Tp>::value;
//#endif

    // is_move_assignable

    template <class _Tp> struct  IsMoveAssignable
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
            : public LD::IsAssignable<typename LD::Detail::AddLValueReference<_Tp>::type,
                    typename LD::Detail::AddRvalueReference<_Tp>::type> {};
#else
    : public LD::IsCopyAssignable<_Tp> {};
#endif

//#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
    template <class _Tp>  bool IsMoveAssignable_V
            = IsMoveAssignable<_Tp>::value;
//#endif
}
#endif //LANDESSDEVCORE_ISASSIGNABLE_HPP
