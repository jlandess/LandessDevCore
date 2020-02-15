//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISVOID_H
#define LANDESSDEVCORE_ISVOID_H

#include "IntegralConstant.h"
#include "RemoveCV.h"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct __libcpp_is_void       : public LD::FalseType {};
        template <>          struct __libcpp_is_void<void> : public LD::TrueType {};

        template <class _Tp> struct  IsVoid
                : public __libcpp_is_void<typename LD::Detail::RemoveCV<_Tp>::type> {};

#if _LIBCPP_STD_VER > 14 && !defined(_LIBCPP_HAS_NO_VARIABLE_TEMPLATES)
        template <class _Tp>  bool IsVoid_V
        = IsVoid<_Tp>::value;
#endif
    }
}
#endif //LANDESSDEVCORE_ISVOID_H
