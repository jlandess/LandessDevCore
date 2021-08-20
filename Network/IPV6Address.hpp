//
// Created by phoenixflower on 1/25/21.
//

#ifndef LANDESSDEVCORE_IPV6ADDRESS_HPP
#define LANDESSDEVCORE_IPV6ADDRESS_HPP
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/Hexademical.hpp"
#include "ipv6.h"
#include "Prefix.hpp"
#include "Port.hpp"
#include "Algorithms/DecimalToBinary.h"
#include "IPV6HostSegment.hpp"
#include "IPV6NetworkSegment.hpp"
//2001:0db8:85a3:08d3:1319:8a2e:0370:7348
namespace LD
{
    class IPV6Address
    {
    private:
        LD::Hexademical<32> mOctets;
        LD::Prefix mMask;
        LD::Port mPort;
    public:
        constexpr IPV6Address() noexcept
        {

        }




        IPV6Address(LD::IPV6NetworkSegment networkSegment, LD::IPV6HostSegment hostSegment) noexcept
        {
            auto networkOctets = networkSegment.Octets();
            this->mOctets[0] = networkOctets[0];
            this->mOctets[1] = networkOctets[1];
            this->mOctets[2] = networkOctets[2];
            this->mOctets[3] = networkOctets[3];
            this->mOctets[4] = networkOctets[4];
            this->mOctets[5] = networkOctets[5];
            this->mOctets[6] = networkOctets[6];
            this->mOctets[7] = networkOctets[7];
            this->mOctets[8] = networkOctets[8];
            this->mOctets[9] = networkOctets[9];
            this->mOctets[10] = networkOctets[10];
            this->mOctets[11] = networkOctets[11];
            this->mOctets[12] = networkOctets[12];
            this->mOctets[13] = networkOctets[13];
            this->mOctets[14] = networkOctets[14];
            this->mOctets[15] = networkOctets[15];


            auto hostOctets = hostSegment.Octets();
            this->mOctets[16] = hostOctets[0];
            this->mOctets[17] = hostOctets[1];
            this->mOctets[18] = hostOctets[2];
            this->mOctets[19] = hostOctets[3];
            this->mOctets[20] = hostOctets[4];
            this->mOctets[21] = hostOctets[5];
            this->mOctets[22] = hostOctets[6];
            this->mOctets[23] = hostOctets[7];
            this->mOctets[24] = hostOctets[8];
            this->mOctets[25] = hostOctets[9];
            this->mOctets[26] = hostOctets[10];
            this->mOctets[27] = hostOctets[11];
            this->mOctets[28] = hostOctets[12];
            this->mOctets[29] = hostOctets[13];
            this->mOctets[30] = hostOctets[14];
            this->mOctets[31] = hostOctets[15];


            this->mMask = networkSegment.Mask();
            this->mPort = hostSegment.Port();

        }
        IPV6Address(LD::IPV6HostSegment hostSegment, LD::IPV6NetworkSegment networkSegment) noexcept:
                mPort{hostSegment.Port()}
        {

        }
        IPV6Address(LD::Hexademical<16> network, LD::Hexademical<16> host) noexcept
        {
            LD::Hexademical<2> firstNetworkOctetPair{{network[0],network[1]}};
            LD::Hexademical<2> secondNetworkOctetPair{{network[2],network[3]}};
            LD::Hexademical<2> thirdNetworkOctetPair{{network[4],network[5]}};
            LD::Hexademical<2> fourthNetworkOctetPair{{network[6],network[7]}};
            LD::Hexademical<2> fifthNetworkOctetPair{{network[8],network[9]}};
            LD::Hexademical<2> sixthNetworkOctetPair{{network[10],network[11]}};
            LD::Hexademical<2> seventhNetworkOctetPair{{network[12],network[13]}};
            LD::Hexademical<2> eightNetworkOctetPair{{network[14],network[15]}};

            LD::Hexademical<2> firstHostOctetPair{{host[0],host[1]}};
            LD::Hexademical<2> secondHostOctetPair{{host[2],host[3]}};
            LD::Hexademical<2> thirdHostOctetPair{{host[4],host[5]}};
            LD::Hexademical<2> fourthHostOctetPair{{host[6],host[7]}};
            LD::Hexademical<2> fifthHostOctetPair{{host[8],host[9]}};
            LD::Hexademical<2> sixthHostOctetPair{{host[10],host[11]}};
            LD::Hexademical<2> seventhHostOctetPair{{host[12],host[13]}};
            LD::Hexademical<2> eightHostOctetPair{{host[14],host[15]}};





        }
        IPV6Address(LD::ImmutableString<25> network, LD::MacAddress hostMacAddress) noexcept
        {
            LD::Hexademical<16> hex1;
            hex1[0] = hostMacAddress[0];
            hex1[1] = hostMacAddress[1];

            hex1[2] = hostMacAddress[2];
            hex1[3] = hostMacAddress[3];

            hex1[4] = hostMacAddress[4];
            hex1[5] = hostMacAddress[5];

            hex1[6] = 'f';
            hex1[7] = 'f';

            hex1[8] = 'f';
            hex1[9] = 'e';

            hex1[10] = hostMacAddress[6];
            hex1[11] = hostMacAddress[7];


            hex1[12] = hostMacAddress[8];
            hex1[13] = hostMacAddress[9];

            hex1[14] = hostMacAddress[10];
            hex1[15] = hostMacAddress[11];

            LD::BitSet<64> hostBitSet = LD::ToBitset(hex1);
            hostBitSet[6] = !hostBitSet[6];
            LD::Hexademical host{hostBitSet};

            ipv6_address_full_t outputAddress;
            ipv6_from_str(network.Data(),network.GetSize(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {
                LD::Hexademical<2> firstHostOctetPair{{host[0],host[1]}};
                LD::Hexademical<2> secondHostOctetPair{{host[2],host[3]}};
                LD::Hexademical<2> thirdHostOctetPair{{host[4],host[5]}};
                LD::Hexademical<2> fourthHostOctetPair{{host[6],host[7]}};
                LD::Hexademical<2> fifthHostOctetPair{{host[8],host[9]}};
                LD::Hexademical<2> sixthHostOctetPair{{host[10],host[11]}};
                LD::Hexademical<2> seventhHostOctetPair{{host[12],host[13]}};
                LD::Hexademical<2> eightHostOctetPair{{host[14],host[15]}};

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

                this->mPort = LD::Port{outputAddress.port};
                this->mMask = LD::Prefix(outputAddress.mask);
            }


        }

        IPV6Address(LD::StringView address) noexcept
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(address.data(),address.size(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {


                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint16_t thirdSegment = outputAddress.address.components[2];
                uint16_t fourthSegment = outputAddress.address.components[3];
                uint16_t fifthSegment = outputAddress.address.components[4];
                uint16_t sixthSegment = outputAddress.address.components[5];
                uint16_t seventhSegment = outputAddress.address.components[6];
                uint16_t eightSegment = outputAddress.address.components[7];

                uint8_t firstPart = firstSegment >> 8;
                uint8_t secondPart = firstSegment;
                uint8_t thirdPart = secondSegment >> 8;
                uint8_t fourthPart = secondSegment;
                uint8_t fifthPart = thirdSegment >> 8;
                uint8_t sixthPart = thirdSegment;
                uint8_t seventhPart = fourthSegment >> 8;
                uint8_t eightPart = fourthSegment;
                uint8_t ninthPart = fifthSegment >> 8;
                uint8_t tenthPart = fifthSegment;
                uint8_t eleventhPart = sixthSegment >> 8;
                uint8_t twelfthPart = sixthSegment;
                uint8_t thirteenthPart = seventhSegment >> 8;
                uint8_t fourteenthPart = seventhSegment;
                uint8_t fifteenthPart = eightSegment >> 8;
                uint8_t sixteenthPart = eightSegment;


                LD::Hexademical firstPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},firstPart)};
                LD::Hexademical secondPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},secondPart)};

                LD::Hexademical thirdPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirdPart)};
                LD::Hexademical fourthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourthPart)};

                LD::Hexademical fifthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifthPart)};
                LD::Hexademical sixthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixthPart)};

                LD::Hexademical seventPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},seventhPart)};
                LD::Hexademical eigthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eightPart)};


                LD::Hexademical ninthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},ninthPart)};
                LD::Hexademical tenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},tenthPart)};

                LD::Hexademical eleventhPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eleventhPart)};
                LD::Hexademical twelfPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},twelfthPart)};

                LD::Hexademical thirteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirteenthPart)};
                LD::Hexademical fourteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourteenthPart)};

                LD::Hexademical fifteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifteenthPart)};
                LD::Hexademical sixteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixteenthPart)};


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

                this->mOctets[16] = ninthPair[0];
                this->mOctets[17] = ninthPair[1];

                this->mOctets[18] = tenthPair[0];
                this->mOctets[19] = tenthPair[1];

                this->mOctets[20] = eleventhPair[0];
                this->mOctets[21] = eleventhPair[1];

                this->mOctets[22] = twelfPair[0];
                this->mOctets[23] = twelfPair[1];

                this->mOctets[24] = thirteenthPair[0];
                this->mOctets[25] = thirteenthPair[1];

                this->mOctets[26] = fourteenthPair[0];
                this->mOctets[27] = fourteenthPair[1];

                this->mOctets[28] = fifteenthPair[0];
                this->mOctets[29] = fifteenthPair[1];

                this->mOctets[30] = sixteenthPair[0];
                this->mOctets[31] = sixteenthPair[1];


                this->mPort = LD::Port{outputAddress.port};
                this->mMask = LD::Prefix(outputAddress.mask);
            }
        }
        //ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128:65000
        IPV6Address(const LD::ImmutableString<49> & address) noexcept
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(address.Data(),address.GetSize(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {


                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint16_t thirdSegment = outputAddress.address.components[2];
                uint16_t fourthSegment = outputAddress.address.components[3];
                uint16_t fifthSegment = outputAddress.address.components[4];
                uint16_t sixthSegment = outputAddress.address.components[5];
                uint16_t seventhSegment = outputAddress.address.components[6];
                uint16_t eightSegment = outputAddress.address.components[7];

                uint8_t firstPart = firstSegment >> 8;
                uint8_t secondPart = firstSegment;
                uint8_t thirdPart = secondSegment >> 8;
                uint8_t fourthPart = secondSegment;
                uint8_t fifthPart = thirdSegment >> 8;
                uint8_t sixthPart = thirdSegment;
                uint8_t seventhPart = fourthSegment >> 8;
                uint8_t eightPart = fourthSegment;
                uint8_t ninthPart = fifthSegment >> 8;
                uint8_t tenthPart = fifthSegment;
                uint8_t eleventhPart = sixthSegment >> 8;
                uint8_t twelfthPart = sixthSegment;
                uint8_t thirteenthPart = seventhSegment >> 8;
                uint8_t fourteenthPart = seventhSegment;
                uint8_t fifteenthPart = eightSegment >> 8;
                uint8_t sixteenthPart = eightSegment;


                LD::Hexademical firstPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},firstPart)};
                LD::Hexademical secondPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},secondPart)};

                LD::Hexademical thirdPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirdPart)};
                LD::Hexademical fourthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourthPart)};

                LD::Hexademical fifthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifthPart)};
                LD::Hexademical sixthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixthPart)};

                LD::Hexademical seventPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},seventhPart)};
                LD::Hexademical eigthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eightPart)};


                LD::Hexademical ninthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},ninthPart)};
                LD::Hexademical tenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},tenthPart)};

                LD::Hexademical eleventhPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eleventhPart)};
                LD::Hexademical twelfPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},twelfthPart)};

                LD::Hexademical thirteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirteenthPart)};
                LD::Hexademical fourteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourteenthPart)};

                LD::Hexademical fifteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifteenthPart)};
                LD::Hexademical sixteenthPair{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixteenthPart)};






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

                this->mOctets[16] = ninthPair[0];
                this->mOctets[17] = ninthPair[1];

                this->mOctets[18] = tenthPair[0];
                this->mOctets[19] = tenthPair[1];

                this->mOctets[20] = eleventhPair[0];
                this->mOctets[21] = eleventhPair[1];

                this->mOctets[22] = twelfPair[0];
                this->mOctets[23] = twelfPair[1];

                this->mOctets[24] = thirteenthPair[0];
                this->mOctets[25] = thirteenthPair[1];

                this->mOctets[26] = fourteenthPair[0];
                this->mOctets[27] = fourteenthPair[1];

                this->mOctets[28] = fifteenthPair[0];
                this->mOctets[29] = fifteenthPair[1];

                this->mOctets[30] = sixteenthPair[0];
                this->mOctets[31] = sixteenthPair[1];

                this->mPort = LD::Port{outputAddress.port};
                this->mMask = LD::Prefix(outputAddress.mask);
            }
        }



        auto Octets() const noexcept
        {
            return this->mOctets;
        }
        LD::Port Port() const noexcept
        {
            return this->mPort;
        }
        LD::Prefix Mask() const noexcept
        {
            return this->mMask;
        }
    };

    inline auto ToImmutableString(const LD::IPV6Address address) noexcept
    {
        auto octets = address.Octets();
        LD::ImmutableString colon{":"};

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
        auto fifthHextet =
                LD::ToImmutableString(char(octets[16])) +
                LD::ToImmutableString(char(octets[17])) +
                LD::ToImmutableString(char(octets[18])) +
                LD::ToImmutableString(char(octets[19])) ;
        auto sixthHextet =
                LD::ToImmutableString(char(octets[20])) +
                LD::ToImmutableString(char(octets[21])) +
                LD::ToImmutableString(char(octets[22])) +
                LD::ToImmutableString(char(octets[23])) ;
        auto seventhHextet =
                LD::ToImmutableString(char(octets[24])) +
                LD::ToImmutableString(char(octets[25])) +
                LD::ToImmutableString(char(octets[26])) +
                LD::ToImmutableString(char(octets[27])) ;
        auto eigthHextet =
                LD::ToImmutableString(char(octets[28])) +
                LD::ToImmutableString(char(octets[29])) +
                LD::ToImmutableString(char(octets[30])) +
                LD::ToImmutableString(char(octets[31])) ;
        return firstHextet+colon+secondHextet+colon+thirdHextet+colon+fourthHextet+colon+fifthHextet+colon+sixthHextet+colon+seventhHextet+colon+eigthHextet;
    }
}
#endif //LANDESSDEVCORE_IPV6ADDRESS_HPP
