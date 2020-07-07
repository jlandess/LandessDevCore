//
// Created by phoenixflower on 7/3/20.
//

#ifndef LANDESSDEVCORE_BYTEBUFFER_HPP
#define LANDESSDEVCORE_BYTEBUFFER_HPP
#include "Immutable.hpp"
#include "BitSet.hpp"
#include "Algorithms/CompileTimeOperations.hpp"
namespace LD
{
    template<LD::UInteger Bytes>
    class ImmutableByteBuffer
    {
    private:
        LD::ImmutableArray<unsigned char,Bytes> mBytes;
    public:
        constexpr ImmutableByteBuffer() noexcept
        {
            LD::For<Bytes>([](
                    auto I,
                    LD::ImmutableArray<unsigned char,Bytes> & bytes)
            {
                bytes[I] = 0;
                return true;
            },this->mBytes);
        }

        constexpr ImmutableByteBuffer(const LD::BitSet<Bytes*8> & bits) noexcept
        {
            LD::For<Bytes>([](
                    auto I,
                    const LD::BitSet<Bytes*8> & binaryBits,
                    LD::ImmutableArray<unsigned char,Bytes> & bytes)
            {

                LD::UInteger amount = LD::CT::Pow<2,7>*binaryBits[8*I+0] +
                LD::CT::Pow<2,6>*binaryBits[8*I+1] +
                LD::CT::Pow<2,5>*binaryBits[8*I+2] +
                LD::CT::Pow<2,4>*binaryBits[8*I+3] +
                LD::CT::Pow<2,3>*binaryBits[8*I+4] +
                LD::CT::Pow<2,2>*binaryBits[8*I+5] +
                LD::CT::Pow<2,1>*binaryBits[8*I+6] +
                LD::CT::Pow<2,0>*binaryBits[8*I+7];
                bytes[I] = amount;
                return true;
            },bits,this->mBytes);

        }

        const unsigned char & operator[](const LD::UInteger & index) noexcept
        {
            return this->mBytes[index];
        }
    };
}
#endif //LANDESSDEVCORE_BYTEBUFFER_HPP
