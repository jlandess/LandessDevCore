//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISSAME_H
#define LANDESSDEVCORE_ISSAME_H

#include "IntegralConstant.h"
#include "__two.h"
namespace LD
{
    namespace Detail
    {
        struct __is_referenceable_impl {
            template <class _Tp> static _Tp& __test(int);
            template <class _Tp> static __two __test(...);
        };

        template <class _Tp, class _Up> struct  IsSame           : public LD::FalseType {};
        template <class _Tp>            struct  IsSame<_Tp, _Tp> : public LD::TrueType {};
        template <class _Tp, class _Up> struct  IsA           : public LD::FalseType {};
        template <class _Tp>            struct  IsA<_Tp, _Tp> : public LD::TrueType {};


        template <class _Tp>
        struct __is_referenceable : IntegralConstant<bool,
                !LD::Detail::IsSame<decltype(__is_referenceable_impl::__test<_Tp>(0)), __two>::value> {};
    }
}
#endif //LANDESSDEVCORE_ISSAME_H
