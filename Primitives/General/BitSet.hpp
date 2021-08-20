//
// Created by phoenixflower on 7/2/20.
//

#ifndef LANDESSDEVCORE_BITSET_HPP
#define LANDESSDEVCORE_BITSET_HPP
#include "Definitions/Integer.hpp"
#include "Immutable.hpp"
#include "TypeTraits/IsConvertible.hpp"
//#include "Definitions/Common.hpp"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{
    class BitElement
    {
    private:
        unsigned char mBit;
    public:
        constexpr BitElement(const bool & bit = 0):mBit('0'+bit)
        {

        }
        constexpr BitElement & operator = (const bool & value) noexcept
        {
            this->mBit = '0'+value;
            return (*this);
        }

        constexpr operator bool() const noexcept { return (this->mBit == '1'); }
    };
    template<LD::UInteger Bits>
    class BitSet
    {
    private:
        LD::ImmutableArray<LD::BitElement,Bits> mBits;
    public:
        constexpr BitSet() noexcept :mBits{}
        {

        }

        constexpr LD::UInteger GetSize() const noexcept
        {
            return Bits;
        }
        void Clear(const bool & shouldClear) noexcept
        {
            LD::For<Bits>([](
                    auto I,
                    LD::ImmutableArray<LD::BitElement,Bits> & bits,
                    const bool & shouldClear)
            {
                bits[I] = (bits[I] && (not shouldClear));
                return true;
            },this->mBits,shouldClear);
        }
        template<typename ... Args, typename = LD::Enable_If_T<
                LD::Require<
                        (sizeof...(Args) == Bits),
                        (LD::Detail::IsConvertible<Args,bool>::value && ...)
                        >>>
        constexpr BitSet(Args && ... bits) noexcept
        {
            LD::StaticArray<bool,Bits> array{{bool(LD::Forward<Args>(bits))...}};
            LD::For<Bits>([](
                    auto I,
                    LD::ImmutableArray<LD::BitElement,Bits> & bits,
                    auto && tiedValues)
            {
                bits[I] = tiedValues[I];
                return true;
            },this->mBits,array);

        }

        constexpr BitSet(const LD::ImmutableArray<bool,Bits> & bits) noexcept
        {
            LD::For<Bits>([](
                    auto I,
                    const LD::ImmutableArray<bool,Bits> & bits,
                    LD::ImmutableArray<LD::BitElement,Bits> & instanceBits)
            {
                instanceBits[I] = bits[I];
                return true;
            },bits,this->mBits);
        }
        constexpr BitSet(const bool & value) noexcept
        {

            LD::For<Bits>([](
                    auto I,
                    LD::ImmutableArray<LD::BitElement,Bits> & bits,
                    const bool & value)
            {
                bits[I] = value;
                return true;

            },this->mBits,value);

        }
        constexpr const LD::BitElement & operator[](const LD::UInteger & index) const noexcept
        {
            return this->mBits[index];
        }

        constexpr LD::BitElement & operator[](const LD::UInteger & index) noexcept
        {
            return this->mBits[index];
        }
    };
}
#endif //LANDESSDEVCORE_BITSET_HPP
