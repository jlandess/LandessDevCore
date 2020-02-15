//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_REMOVECV_H
#define LANDESSDEVCORE_REMOVECV_H

#include "IntegralConstant.h"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  remove_volatile               {typedef _Tp type;};

        template <class _Tp> struct  remove_volatile<volatile _Tp> {typedef _Tp type;};

        template <class _Tp> struct  remove_const            {typedef _Tp type;};

        template <class _Tp> struct  remove_const<const _Tp> {typedef _Tp type;};

        template <class _Tp> struct __libcpp_is_pointer       : public LD::FalseType {};

        template <class _Tp> struct __libcpp_is_pointer<_Tp*> : public LD::TrueType {};


        template <class _Tp> struct  RemoveCV

        {typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;};
    }
}
#endif //LANDESSDEVCORE_REMOVECV_H
