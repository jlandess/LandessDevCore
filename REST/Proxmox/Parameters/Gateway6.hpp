//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PXE_GATEWAY6_HPP
#define LANDESSDEVCORE_PXE_GATEWAY6_HPP
#include "Primitives/General/Immutable.hpp"
#include "Network/Gateway6.hpp"
#include "Algorithms/DecimalToBinary.h"
#include "Primitives/General/Hexademical.hpp"
namespace LD
{
    namespace PVE
    {
        class Gateway6
        {
        private:
            LD::Gateway6 mGateway;
        public:
            Gateway6(LD::Gateway6 gateway) noexcept:mGateway{gateway}
            {

            }
            Gateway6(LD::ImmutableString<45> gateway) noexcept:mGateway{gateway}
            {

            }


            Gateway6(LD::IPV6Address gateway) noexcept:mGateway{gateway}
            {

            }

            LD::IPV6Address Address() const noexcept
            {
                return this->mGateway.Address();
            }
        };
    }

    inline auto ToImmutableString(const LD::PVE::Gateway6 param) noexcept
    {
        LD::IPV6Address address = param.Address();
        LD::ImmutableString colon{":"};
        auto octets = address.Octets();

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
        /*
        unsigned char firstOctet = address.FirstOctet();
        unsigned char secondOctet = address.SecondOctet();
        unsigned char thirdOctet = address.ThirdOctet();
        unsigned char fourthOctet = address.FourthOctet();
        unsigned char fifthOctet = address.FifthOctet();
        unsigned char sixthOctet = address.SixthOctet();
        unsigned char seventhOctet = address.SeventhOctet();
        unsigned char eighthOctet = address.EightOctet();
        unsigned char ninthOctet = address.NinthOctet();
        unsigned char tenthOctet = address.TenthOctet();
        unsigned char eleventhOctet = address.EleventhOctet();
        unsigned char twelfthOctet = address.TwelfthOctet();
        unsigned char thirteenthOctet = address.ThirteenthOctet();
        unsigned char fourteenthOctet = address.FourteenthOctet();
        unsigned char fifteenthOctet = address.FifteenthOctet();
        unsigned char sixteenthOctet = address.SixteenthOctet();

        //std::cout << "First octet: " << firstOctet << std::endl;
        auto firstQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},firstOctet)}) +
                LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},secondOctet)});

        auto secondQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirdOctet)})+
        LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourthOctet)});

        auto thirdQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifthOctet)})+
        LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixthOctet)});

        auto fourthQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},seventhOctet)})+
                LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eighthOctet)});
        auto fifthQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},ninthOctet)})+
        LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},tenthOctet)});

        auto sixthQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},eleventhOctet)})+
        LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},twelfthOctet)});

        auto seventhQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},thirteenthOctet)})+
        LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fourteenthOctet)});

        auto eightQuad = LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifteenthOctet)})+
         */
        return LD::ImmutableString{"gw6="}+firstHextet+colon+secondHextet+colon+thirdHextet+colon+fourthHextet+colon+fifthHextet+colon+sixthHextet+colon+seventhHextet+colon+eigthHextet;
    }
}
#endif //LANDESSDEVCORE_GATEWAY6_HPP
