//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_FASTABS_HPP
#define LANDESSDEVCORE_FASTABS_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    inline LD::UInteger FastAbs(LD::Integer integer)
    {
        LD::Integer value = integer;
        LD::Integer temp = value >> 31;     // make a mask of the sign bit
        value ^= temp;                   // toggle the bits if value is negative
        value += temp & 1;
        return value;
    }
}
#endif //LANDESSDEVCORE_FASTABS_HPP
