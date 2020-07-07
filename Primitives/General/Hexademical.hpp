//
// Created by phoenixflower on 7/3/20.
//

#ifndef LANDESSDEVCORE_HEXADEMICAL_HPP
#define LANDESSDEVCORE_HEXADEMICAL_HPP
#include "BitSet.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
namespace LD
{
    template<LD::UInteger AmountOfBits>
    class Hexademical
    {
    private:
        LD::ImmutableArray<unsigned char,AmountOfBits> mBits;
    public:


        constexpr Hexademical(const LD::BitSet<AmountOfBits*4> & bits) noexcept
        {

            LD::For<AmountOfBits>([](
                    auto I,
                    const LD::BitSet<AmountOfBits*4> & binaryBits,
                    LD::ImmutableArray<unsigned char,AmountOfBits> & hexBits)
            {
                LD::UInteger amount = 8*binaryBits[4*I+0] + 4*binaryBits[4*I+1] + 2*binaryBits[4*I+2] + binaryBits[4*I+3];
                hexBits[I] = (amount < 10)*('0'+amount) + (amount >=10)*('a'+(amount-10));
                return true;
            },bits,this->mBits);
        }

        const unsigned char & operator [](const LD::UInteger & index) noexcept
        {
            return this->mBits[index];
        }

    };
}
#endif //LANDESSDEVCORE_HEXADEMICAL_HPP
