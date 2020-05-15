//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISARRAY_HPP
#define LANDESSDEVCORE_ISARRAY_HPP

#include "IntegralConstant.hpp"
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  IsArray
                : public LD::FalseType {};
        template <class _Tp> struct  IsArray<_Tp[]>
                : public LD::TrueType {};
        template <class _Tp, LD::UInteger _Np> struct  IsArray<_Tp[_Np]>
                : public LD::TrueType {};
    }
}
#endif //LANDESSDEVCORE_ISARRAY_HPP
