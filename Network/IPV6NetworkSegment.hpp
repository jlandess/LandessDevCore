//
// Created by phoenixflower on 2/13/21.
//

#ifndef LANDESSDEVCORE_IPV6NETWORKSEGMENT_HPP
#define LANDESSDEVCORE_IPV6NETWORKSEGMENT_HPP
#include "ipv6.h"
#include "Primitives/General/Immutable.hpp"
#include "Prefix.hpp"
#include "Primitives/General/Hexademical.hpp"
namespace LD
{
    class IPV6NetworkSegment
    {
    private:
        LD::ImmutableArray<LD::HexElement,8> mSegments;
        LD::Hexademical<16> mOctets;
        LD::Prefix mMask;
    public:
        IPV6NetworkSegment() noexcept
        {

        }

        constexpr IPV6NetworkSegment(LD::ImmutableString<25> lit)
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(lit.Data(),lit.GetSize(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {
                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint16_t thirdSegment = outputAddress.address.components[2];
                uint16_t fourthSegment = outputAddress.address.components[3];


                uint8_t firstPart = firstSegment >> 8;
                uint8_t secondPart = firstSegment;
                uint8_t thirdPart = secondSegment >> 8;
                uint8_t fourthPart = secondSegment;
                uint8_t fifthPart = thirdSegment >> 8;
                uint8_t sixthPart = thirdSegment;
                uint8_t seventhPart = fourthSegment >> 8;
                uint8_t eightPart = fourthSegment;

                LD::Hexademical firstPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},firstPart)};
                LD::Hexademical secondPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},secondPart)};

                LD::Hexademical thirdPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirdPart)};
                LD::Hexademical fourthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourthPart)};

                LD::Hexademical fifthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifthPart)};
                LD::Hexademical sixthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixthPart)};

                LD::Hexademical seventPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},seventhPart)};
                LD::Hexademical eigthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eightPart)};

                this->mOctets[0] = firstPair[0];
                this->mOctets[1] = firstPair[1];

                this->mOctets[2] = secondPair[0];
                this->mOctets[3] = secondPair[1];

                this->mOctets[4] = thirdPair[0];
                this->mOctets[5] = thirdPair[1];

                this->mOctets[6] = fourthPair[0];
                this->mOctets[7] = fourthPair[1];

                this->mOctets[8] = fifthPair[0];
                this->mOctets[9] = fifthPair[1];

                this->mOctets[10] = sixthPair[0];
                this->mOctets[11] = sixthPair[1];

                this->mOctets[12] = seventPair[0];
                this->mOctets[13] = seventPair[1];

                this->mOctets[14] = eigthPair[0];
                this->mOctets[15] = eigthPair[1];


                this->mSegments[0] = firstPart;
                this->mSegments[1] = secondPart;
                this->mSegments[2] = thirdPart;
                this->mSegments[3] = fourthPart;
                this->mSegments[4] = fifthPart;
                this->mSegments[5] = sixthPart;
                this->mSegments[6] = seventhPart;
                this->mSegments[7] = eightPart;
                this->mMask = outputAddress.mask;
            }
        }

        template<LD::UInteger N>
        constexpr IPV6NetworkSegment(const char (&lit)[N])
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(lit,N,&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {
                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint16_t thirdSegment = outputAddress.address.components[2];
                uint16_t fourthSegment = outputAddress.address.components[3];


                uint8_t firstPart = firstSegment >> 8;
                uint8_t secondPart = firstSegment;
                uint8_t thirdPart = secondSegment >> 8;
                uint8_t fourthPart = secondSegment;
                uint8_t fifthPart = thirdSegment >> 8;
                uint8_t sixthPart = thirdSegment;
                uint8_t seventhPart = fourthSegment >> 8;
                uint8_t eightPart = fourthSegment;

                LD::Hexademical firstPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},firstPart)};
                LD::Hexademical secondPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},secondPart)};

                LD::Hexademical thirdPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirdPart)};
                LD::Hexademical fourthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourthPart)};

                LD::Hexademical fifthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifthPart)};
                LD::Hexademical sixthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixthPart)};

                LD::Hexademical seventPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},seventhPart)};
                LD::Hexademical eigthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eightPart)};

                this->mOctets[0] = firstPair[0];
                this->mOctets[1] = firstPair[1];

                this->mOctets[2] = secondPair[0];
                this->mOctets[3] = secondPair[1];

                this->mOctets[4] = thirdPair[0];
                this->mOctets[5] = thirdPair[1];

                this->mOctets[6] = fourthPair[0];
                this->mOctets[7] = fourthPair[1];

                this->mOctets[8] = fifthPair[0];
                this->mOctets[9] = fifthPair[1];

                this->mOctets[10] = sixthPair[0];
                this->mOctets[11] = sixthPair[1];

                this->mOctets[12] = seventPair[0];
                this->mOctets[13] = seventPair[1];

                this->mOctets[14] = eigthPair[0];
                this->mOctets[15] = eigthPair[1];


                this->mSegments[0] = firstPart;
                this->mSegments[1] = secondPart;
                this->mSegments[2] = thirdPart;
                this->mSegments[3] = fourthPart;
                this->mSegments[4] = fifthPart;
                this->mSegments[5] = sixthPart;
                this->mSegments[6] = seventhPart;
                this->mSegments[7] = eightPart;
                this->mMask = outputAddress.mask;
            }
        }
        auto Segments() const noexcept
        {
            return this->mSegments;
        }
        auto Octets() const noexcept
        {
            return this->mOctets;
        }
        auto Mask() const noexcept
        {
            return this->mMask;
        }
    };

    inline auto ToImmutableString(const LD::IPV6NetworkSegment address) noexcept
    {
        auto octets = address.Octets();
        LD::ImmutableString colon{":"};
        LD::ImmutableString forwardSlash{"/"};
        auto firstHextet =
                LD::ToImmutableString(char(octets[0])) +
                LD::ToImmutableString(char(octets[1])) +
                LD::ToImmutableString(char(octets[2])) +
                LD::ToImmutableString(char(octets[3]));
        auto secondHextet =
                LD::ToImmutableString(char(octets[4])) +
                LD::ToImmutableString(char(octets[5])) +
                LD::ToImmutableString(char(octets[6])) +
                LD::ToImmutableString(char(octets[7])) ;
        auto thirdHextet =
                LD::ToImmutableString(char(octets[8])) +
                LD::ToImmutableString(char(octets[9])) +
                LD::ToImmutableString(char(octets[10])) +
                LD::ToImmutableString(char(octets[11])) ;
        auto fourthHextet =
                LD::ToImmutableString(char(octets[12])) +
                LD::ToImmutableString(char(octets[13])) +
                LD::ToImmutableString(char(octets[14])) +
                LD::ToImmutableString(char(octets[15])) ;
        return firstHextet+colon+secondHextet+colon+thirdHextet+colon+fourthHextet+colon+colon+forwardSlash+LD::ToImmutableString(address.Mask().Value());
    }
}
#endif //LANDESSDEVCORE_IPV6NETWORKSEGMENT_HPP
