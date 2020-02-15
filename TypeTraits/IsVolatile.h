//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISVOLATILE_H
#define LANDESSDEVCORE_ISVOLATILE_H

#include "IntegralConstant.h"
namespace LD
{
    namespace Detail
    {
        template <class _Tp> struct  IsVolatile               : public LD::FalseType {};
        template <class _Tp> struct  IsVolatile<_Tp volatile> : public LD::TrueType {};
    }

}
#endif //LANDESSDEVCORE_ISVOLATILE_H
