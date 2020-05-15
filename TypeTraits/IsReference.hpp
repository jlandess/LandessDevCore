//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISREFERENCE_HPP
#define LANDESSDEVCORE_ISREFERENCE_HPP

#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  IsLValueReference       : public LD::FalseType {};
        template <class _Tp> struct  IsLValueReference<_Tp&> : public LD::TrueType {};

        template <class _Tp> struct  IsRValueReference        : public LD::FalseType {};
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Tp> struct  IsRValueReference<_Tp&&> : public LD::TrueType {};
#endif


        template <class _Tp> struct  IsReference        : public LD::FalseType {};
        template <class _Tp> struct  IsReference<_Tp&>  : public LD::TrueType {};
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Tp> struct  IsReference<_Tp&&> : public LD::TrueType {};
#endif
    }
}
#endif //LANDESSDEVCORE_ISREFERENCE_HPP
