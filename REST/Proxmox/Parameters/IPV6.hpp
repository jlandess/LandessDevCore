//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_IPV6_HPP
#define LANDESSDEVCORE_IPV6_HPP
#include "Primitives/General/Immutable.hpp"
#include "Network/IPV6Address.hpp"
#include "Primitives/General/Hexademical.hpp"
#include "Algorithms/DecimalToBinary.h"
namespace LD
{
    namespace PVE
    {
        class IPV6
        {
        private:
            LD::IPV6Address mAddress;
        public:
            IPV6() noexcept{}

            IPV6(LD::IPV6Address address) noexcept:mAddress{address}
            {

            }
            IPV6(const LD::ImmutableString<49> & address) noexcept:mAddress{address}
            {

            }

            LD::IPV6Address Address() const noexcept
            {
                return this->mAddress;
            }
        };
    }

    inline auto ToImmutableString(const LD::PVE::IPV6 param) noexcept
    {
        LD::IPV6Address address = param.Address();
        LD::ImmutableString colon{":"};
        LD::ImmutableString slash{"/"};
        auto octets = address.Octets();
        LD::Prefix mask = address.Mask();
        /*
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
                         LD::ToImmutableString(LD::Hexademical{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},sixteenthOctet)});
        LD::Hexademical a{LD::DecimalToBinary(LD::CompileTimeIndex<8>{},fifteenthOctet)};
         */

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
        return LD::ImmutableString{"ip6="}+firstHextet+colon+secondHextet+colon+thirdHextet+colon+fourthHextet+colon+fifthHextet+colon+sixthHextet+colon+secondHextet+colon+eigthHextet+slash+LD::ToImmutableString(mask.Value());
    }
}
#endif //LANDESSDEVCORE_IPV6_HPP
