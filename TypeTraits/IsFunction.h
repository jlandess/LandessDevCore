//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISFUNCTION_H
#define LANDESSDEVCORE_ISFUNCTION_H
#include "IsClassType.h"
#include "IsUnion.h"
#include "IsVoid.h"
#include "IsReference.h"
#include "IsNullPtr.h"
#include "IsConst.h"
namespace __libcpp_is_function_imp
{
    struct __dummy_type {};
    template <class _Tp> char  __test(_Tp*);
    template <class _Tp> char __test(__dummy_type);
    template <class _Tp> LD::Detail::__two __test(...);
    template <class _Tp> _Tp&  __source(int);
    template <class _Tp> __dummy_type __source(...);
}
namespace LD
{
    namespace Detail
    {
        template <class _Tp, bool =
        LD::Detail::IsClassType<_Tp>::value ||
        LD::Detail::IsUnion<_Tp>::value ||
        LD::Detail::IsVoid<_Tp>::value  ||
        LD::Detail::IsReference<_Tp>::value ||
        LD::Detail::IsNullPtr<_Tp>::value >
        struct __libcpp_is_function
                : public LD::Detail::IntegralConstant<bool, sizeof(__libcpp_is_function_imp::__test<_Tp>(__libcpp_is_function_imp::__source<_Tp>(0))) == 1>
        {};
        template <class _Tp> struct __libcpp_is_function<_Tp, true> : public LD::FalseType {};

        template <class _Tp> struct IsFunction
                : public __libcpp_is_function<_Tp> {};


        template <class _Tp, bool = LD::Detail::IsReference<_Tp>::value ||LD::Detail::IsFunction<_Tp>::value  ||LD::Detail::IsConst<_Tp>::value     >
        struct __add_const             {typedef _Tp type;};

        template <class _Tp>
        struct __add_const<_Tp, false> {typedef const _Tp type;};

        template <class _Tp> struct AddConst
        {typedef typename __add_const<_Tp>::type type;};


    }
}
#endif //LANDESSDEVCORE_ISFUNCTION_H
