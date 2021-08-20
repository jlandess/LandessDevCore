//
// Created by phoenixflower on 1/25/21.
//

#ifndef LANDESSDEVCORE_MACADDRESS_HPP
#define LANDESSDEVCORE_MACADDRESS_HPP
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/BitSet.hpp"
#include "Primitives/General/Hexademical.hpp"
namespace LD
{
    class MacAddressElement
    {
    private:
        unsigned char  mHex;
    public:
        MacAddressElement() noexcept:mHex{'0'}
        {

        }
        MacAddressElement(unsigned char hex) noexcept
        {
            (*this) = hex;
        }
        MacAddressElement & operator = (unsigned char hex) noexcept
        {
            this->mHex = (hex >= '0' && hex <= '9')*hex + (hex >= 'a' && hex <= 'f')*hex + (hex >= 'A' && hex <= 'F')*tolower(hex);
            return (*this);
        }

        constexpr operator unsigned char() const noexcept { return this->mHex; }
    };
    class MacAddress
    {
    private:
        LD::HexElement mOctets[12];
    public:
        MacAddress() noexcept

        {

        }
        MacAddress(LD::StringView view) noexcept:
        mOctets{
            view.data()[0],view.data()[1],
            view.data()[3],view.data()[4],
            view.data()[6],view.data()[7],
            view.data()[9],view.data()[10],
            view.data()[12],view.data()[13],
            view.data()[15],view.data()[16]}
        {

        }

        MacAddress(const LD::ImmutableString<17> & address) noexcept:
                mOctets{
                        LD::HexElement(address[0]),LD::HexElement(address[1]),
                        LD::HexElement(address[3]),LD::HexElement(address[4]),
                        LD::HexElement(address[6]),LD::HexElement(address[7]),
                        LD::HexElement(address[9]),LD::HexElement(address[10]),
                        LD::HexElement(address[12]),LD::HexElement(address[13]),
                        LD::HexElement(address[15]),LD::HexElement(address[16])}
        {
        }

        MacAddress(LD::Hexademical<12> address) noexcept:
                mOctets{
                        LD::HexElement(address[0]),LD::HexElement(address[1]),
                        LD::HexElement(address[3]),LD::HexElement(address[4]),
                        LD::HexElement(address[6]),LD::HexElement(address[7]),
                        LD::HexElement(address[9]),LD::HexElement(address[10]),
                        LD::HexElement(address[12]),LD::HexElement(address[13]),
                        LD::HexElement(address[15]),LD::HexElement(address[16])}
        {

        }
        auto FirstOctet() const noexcept
        {
            return this->mOctets[0];
        }

        auto SecondOctet() const noexcept
        {
            return this->mOctets[1];
        }

        auto ThirdOctet() const noexcept
        {
            return this->mOctets[2];
        }

        auto FourthOctet() const noexcept
        {
            return this->mOctets[3];
        }

        auto FifthOctet() const noexcept
        {
            return this->mOctets[4];
        }

        auto SixthOctet() const noexcept
        {
            return this->mOctets[5];
        }
        auto SeventhOctet() const noexcept
        {
            return this->mOctets[6];
        }

        auto EightOctet() const noexcept
        {
            return this->mOctets[7];
        }

        auto NinthOctet() const noexcept
        {
            return this->mOctets[8];
        }

        auto TenthOctet() const noexcept
        {
            return this->mOctets[9];
        }

        auto EleventhOctet() const noexcept
        {
            return this->mOctets[10];
        }

        auto TwelfthOctet() const noexcept
        {
            return  this->mOctets[11];
        }
        const LD::HexElement & operator[](const LD::UInteger & index) const noexcept
        {
            return this->mOctets[index];
        }
        LD::HexElement & operator[](const LD::UInteger & index) noexcept
        {
            return this->mOctets[index];
        }

        LD::Hexademical<12> HexRepresentation() const noexcept
        {
            LD::Hexademical<12> hex;
            hex[0] = this->mOctets[0];
            hex[1] = this->mOctets[1];
            hex[2] = this->mOctets[2];
            hex[3] = this->mOctets[3];
            hex[4] = this->mOctets[4];
            hex[5] = this->mOctets[5];
            hex[6] = this->mOctets[6];
            hex[7] = this->mOctets[7];
            hex[8] = this->mOctets[8];
            hex[9] = this->mOctets[9];
            hex[10] = this->mOctets[10];
            hex[11] = this->mOctets[11];
            return hex;
        }

    };

    inline LD::MacAddress ParseMacAddress(LD::StringView view) noexcept
    {
        return LD::MacAddress{view};
    }
    inline auto ToImmutableString(const LD::MacAddress & address) noexcept
    {
        auto colon = LD::ImmutableString{":"};
        unsigned char firstOctet = address.FirstOctet();
        unsigned char secondOctet = address.SecondOctet();
        unsigned char thirdOctet = address.ThirdOctet();
        unsigned char fourthOctet = address.FourthOctet();
        unsigned char fifthOctet = address.FifthOctet();
        unsigned char sixthOctet = address.SixthOctet();
        unsigned char seventhOctet = address.SeventhOctet();
        unsigned char eightOctet = address.EightOctet();
        unsigned char ninthOctet = address.NinthOctet();
        unsigned char tenthOctet = address.TenthOctet();
        unsigned char eleventhOctet = address.EleventhOctet();
        unsigned char twelftOctet = address.TwelfthOctet();
        auto firstPair = LD::ToImmutableString(firstOctet)+LD::ToImmutableString(secondOctet);
        auto secondPair = LD::ToImmutableString(thirdOctet)+LD::ToImmutableString(fourthOctet);
        auto thirdPair = LD::ToImmutableString(fifthOctet)+LD::ToImmutableString(sixthOctet);
        auto fourthPair = LD::ToImmutableString(seventhOctet)+LD::ToImmutableString(eightOctet);
        auto fifthPair = LD::ToImmutableString(ninthOctet)+LD::ToImmutableString(tenthOctet);
        auto sixthPair = LD::ToImmutableString(eleventhOctet)+LD::ToImmutableString(twelftOctet);

        return firstPair+colon+secondPair+colon+thirdPair+colon+fourthPair+colon+fifthPair+colon+sixthPair;
    }


}
#endif //LANDESSDEVCORE_MACADDRESS_HPP
