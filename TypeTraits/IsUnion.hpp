//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISUNION_HPP
#define LANDESSDEVCORE_ISUNION_HPP

#include "TypeTraits/IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
#if __has_feature(is_union) || (_GNUC_VER >= 403)

        template <class _Tp> struct  IsUnion
                : public LD::Detail::IntegralConstant<bool, __is_union(_Tp)> {};

#else

        template <class _Tp> struct __libcpp_union : public LD::FalseType {};
        template <class _Tp> struct _LIBCPP_TEMPLATE_VIS IsUnion
        : public __libcpp_union<typename LD::Detail::RemoveCV<_Tp>::type> {};

#endif
    }
}
#endif //LANDESSDEVCORE_ISUNION_HPP
