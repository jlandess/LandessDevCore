//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_ISCONST_HPP
#define LANDESSDEVCORE_ISCONST_HPP

#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  IsConst            : public LD::FalseType {};
        template <class _Tp> struct  IsConst<_Tp const> : public LD::TrueType {};
    }
}
#endif //LANDESSDEVCORE_ISCONST_HPP
