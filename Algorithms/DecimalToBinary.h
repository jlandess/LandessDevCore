//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_DECIMALTOBINARY_H
#define LANDESSDEVCORE_DECIMALTOBINARY_H
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "Primitives/General/BitSet.hpp"
namespace LD
{
    template<LD::UInteger Bit, typename T>
    LD::BitSet<Bit> DecimalToBinary(LD::CompileTimeIndex<Bit>, T  number) noexcept
    {
        LD::BitSet<Bit> ret;
        for(LD::UInteger n = 0;n<Bit;++n)
        {
            ret[Bit-n-1] = number&1;
            number >>=1;
        }

        return ret;
    }
}
#endif //LANDESSDEVCORE_DECIMALTOBINARY_H
