//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_FIBONNACI_HPP
#define LANDESSDEVCORE_FIBONNACI_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace Detail
    {
        namespace Fibonnaci
        {
            inline void multiply(LD::UInteger F[2][2], LD::UInteger M[2][2])
            {
                LD::UInteger x =  F[0][0] * M[0][0] + F[0][1] * M[1][0];
                LD::UInteger y =  F[0][0] * M[0][1] + F[0][1] * M[1][1];
                LD::UInteger z =  F[1][0] * M[0][0] + F[1][1] * M[1][0];
                LD::UInteger w =  F[1][0] * M[0][1] + F[1][1] * M[1][1];
                F[0][0] = x;
                F[0][1] = y;
                F[1][0] = z;
                F[1][1] = w;
            }

            /*
                 * function to calculate power of a matrix
                 */
            inline void power(LD::UInteger F[2][2], LD::UInteger n)
            {
                if (n == 0 || n == 1)
                    return;
                LD::UInteger M[2][2] = {{1,1},{1,0}};
                LD::Detail::Fibonnaci::power(F, n / 2);
                LD::Detail::Fibonnaci::multiply(F, F);
                if (n % 2 != 0)
                    multiply(F, M);
            }

            /*
                 * function that returns nth Fibonacci number
                 */
        }
    }

    inline LD::UInteger Fibonnaci(LD::UInteger n)
    {
        LD::UInteger F[2][2] = {{1,1},{1,0}};
        if (n == 0)
            return 0;
        Detail::Fibonnaci::power(F, n - 1);
        return F[0][0];
    }
}
#endif //LANDESSDEVCORE_FIBONNACI_HPP
