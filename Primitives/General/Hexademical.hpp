//
// Created by phoenixflower on 7/3/20.
//

#ifndef LANDESSDEVCORE_HEXADEMICAL_HPP
#define LANDESSDEVCORE_HEXADEMICAL_HPP
#include "BitSet.hpp"
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/StringView.hpp"
#include "Algorithms/CompileTimeOperations.hpp"
namespace LD
{
    class HexElement
    {
    private:
        unsigned char  mHex;
    public:
        constexpr HexElement() noexcept:mHex{'0'}
        {

        }
        constexpr HexElement(unsigned char hex) noexcept
        {
            (*this) = hex;
        }
        HexElement & operator = (unsigned char hex) noexcept
        {
            this->mHex = hex;
            this->mHex = (hex >= 'A' && hex <='F')*tolower(this->mHex) + !(hex >= 'A' && hex <='F')*this->mHex;
            //this->mHex = (hex >= '0' && hex <= '9')*hex + (hex >= 'a' && hex <= 'f')*hex + (hex >= 'A' && hex <= 'F')*hex;
            return (*this);
        }

        constexpr operator unsigned char() const noexcept { return this->mHex; }
    };
    template<LD::UInteger AmountOfBits>
    class Hexademical
    {
    private:
        LD::ImmutableArray<LD::HexElement,AmountOfBits> mBits;
    public:

        constexpr Hexademical() noexcept
        {
            for(LD::UInteger n = 0;n<AmountOfBits;++n)
            {
                this->mBits = LD::HexElement('0');
            }
        }
        constexpr Hexademical(LD::ImmutableArray<LD::HexElement,AmountOfBits> hex) noexcept: mBits{hex}
        {

        }
        constexpr Hexademical(const LD::ImmutableString<AmountOfBits> & hex) noexcept
        {
            for(LD::UInteger n = 0;n<AmountOfBits;++n)
            {
                this->mBits[n] = hex[n];
            }
        }
        constexpr Hexademical(const LD::BitSet<AmountOfBits*4> & bits) noexcept
        {

            LD::BitSet<AmountOfBits*4> usableSet;
            int i;
            LD::UInteger j = 0;
            LD::UInteger len = AmountOfBits*4;
            for(i = len-1; i>=0;i--)
            {
                //usableSet[j++] = bits[i];
            }

            LD::For<AmountOfBits>([](
                    auto I,
                    const LD::BitSet<AmountOfBits*4> & binaryBits,
                    LD::ImmutableArray<LD::HexElement,AmountOfBits> & hexBits)
            {
                LD::UInteger amount = 8*binaryBits[4*I+0] + 4*binaryBits[4*I+1] + 2*binaryBits[4*I+2] + binaryBits[4*I+3];
                hexBits[I] = (amount < 10)*('0'+amount) + (amount >=10)*('a'+(amount-10));
                return true;
            },bits,this->mBits);

        }

        //constexpr Hexademical(LD::StringView hexString) noexcept
        //{

        //}

        const LD::HexElement & operator [](const LD::UInteger & index) const noexcept
        {
            return this->mBits[index];
        }
        LD::HexElement & operator [](const LD::UInteger & index) noexcept
        {
            return this->mBits[index];
        }

    };
    template<LD::UInteger Amount> Hexademical(LD::BitSet<Amount> ) -> Hexademical<Amount/4>;


    namespace Detail
    {
        inline LD::BitSet<4> HexToBinaryConversion(unsigned char input) noexcept
        {
            switch (input)
            {
                case '0':
                {
                    return LD::BitSet<4>{0,0,0,0};
                }
                case '1':
                {
                    return LD::BitSet<4>{0,0,0,1};
                }
                case '2':
                {
                    return LD::BitSet<4>{0,0,1,0};
                }
                case '3':
                {
                    return LD::BitSet<4>{0,0,1,1};
                }
                case '4':
                {
                    return LD::BitSet<4>{0,1,0,0};
                }
                case '5':
                {
                    return LD::BitSet<4>{0,1,0,1};
                }
                case '6':
                {
                    return LD::BitSet<4>{0,1,1,0};
                }
                case '7':
                {
                    return LD::BitSet<4>{0,1,1,1};
                }
                case '8':
                {
                    return LD::BitSet<4>{1,0,0,0};
                }
                case '9':
                {
                    return LD::BitSet<4>{1,0,0,1};
                }
                case 'a':
                {
                    return LD::BitSet<4>{1,0,1,0};
                }
                case 'b':
                {
                    return LD::BitSet<4>{1,0,1,1};
                }
                case 'c':
                {
                    return LD::BitSet<4>{1,1,0,0};
                }
                case 'd':
                {
                    return LD::BitSet<4>{1,1,0,1};
                }
                case 'e':
                {
                    return LD::BitSet<4>{1,1,1,0};
                }
                case 'f':
                {
                    return LD::BitSet<4>{1,1,1,1};
                }
            }
            return LD::BitSet<4>{};
        }
    }
    template<LD::UInteger Amount>
    LD::BitSet<Amount*4> ToBitset(LD::Hexademical<Amount> hex) noexcept
    {
        LD::BitSet<Amount*4> usableSet;
        for(LD::UInteger n = 0;n<Amount*4;n+=4)
        {
            LD::BitSet<4> currentBitsetRepresentation = LD::Detail::HexToBinaryConversion(hex[n/4]);
            usableSet[n] = currentBitsetRepresentation[0];
            usableSet[n+1] = currentBitsetRepresentation[1];
            usableSet[n+2] = currentBitsetRepresentation[2];
            usableSet[n+3] = currentBitsetRepresentation[3];
        }

        return usableSet;
    }
    template<LD::UInteger Amount>
    LD::UInteger ToBase10(const LD::Hexademical<Amount> & hex) noexcept
    {
        LD::UInteger sum = 0;
        LD::For<Amount>([](auto I, const LD::Hexademical<Amount> & hex, LD::UInteger & sum)
        {
            unsigned char rawValue = hex[Amount-I-1];
            unsigned char decimalValue = (rawValue >= '0' && rawValue <'9')*(rawValue-'0') + (rawValue >='a' && rawValue<='f')*(10 + (rawValue-'a'));
            sum += LD::CT::Pow<16,I>*decimalValue;
            return true;
        },hex,sum);
        return sum;
    }
    template<LD::UInteger Amount>
    LD::ImmutableString<Amount> ToImmutableString(LD::Hexademical<Amount> hex) noexcept
    {
        char buffer[Amount+1];
        for(LD::UInteger n = 0;n<Amount;++n)
        {
            buffer[n] = hex[n];
        }
        buffer[Amount] = '\0';
        return LD::ImmutableString{buffer};
    }
}
#endif //LANDESSDEVCORE_HEXADEMICAL_HPP
