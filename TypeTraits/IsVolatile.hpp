//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISVOLATILE_HPP
#define LANDESSDEVCORE_ISVOLATILE_HPP

#include "IntegralConstant.hpp"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  IsVolatile               : public LD::FalseType {};
        template <class _Tp> struct  IsVolatile<_Tp volatile> : public LD::TrueType {};
    }

}
#endif //LANDESSDEVCORE_ISVOLATILE_HPP
