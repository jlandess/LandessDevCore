//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_CELLOG2_HPP
#define LANDESSDEVCORE_CELLOG2_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    inline LD::UInteger ceil_log2(LD::UInteger x)
    {
        static const unsigned long long t[6] = {
                0xFFFFFFFF00000000ull,
                0x00000000FFFF0000ull,
                0x000000000000FF00ull,
                0x00000000000000F0ull,
                0x000000000000000Cull,
                0x0000000000000002ull
        };

        LD::Integer y = (((x & (x - 1)) == 0) ? 0 : 1);
        LD::Integer j = 32;
        LD::Integer i;

        for (i = 0; i < 6; i++)
        {
            LD::Integer k = (((x & t[i]) == 0) ? 0 : j);
            y += k;
            x >>= k;
            j >>= 1;
        }

        return y;
    }
}
#endif //LANDESSDEVCORE_CELLOG2_HPP
