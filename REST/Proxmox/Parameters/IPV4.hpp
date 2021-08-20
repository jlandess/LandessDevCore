//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_IPV4_HPP
#define LANDESSDEVCORE_IPV4_HPP
#include "Network/IPV4Address.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class IPV4
        {
        private:
            LD::IPV4Address mAddress;
        public:
            IPV4() noexcept{}
            IPV4(LD::IPV4Address address) noexcept:mAddress{address}
            {

            }
            IPV4(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet) noexcept:mAddress{firstOctet,secondOctet,thirdOctet,fourthOctet}
            {

            }
            IPV4(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Port port) noexcept:mAddress{firstOctet,secondOctet,thirdOctet,fourthOctet,port}
            {

            }
            IPV4(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Subnet mask) noexcept:mAddress{firstOctet,secondOctet,thirdOctet,fourthOctet,mask}
            {

            }
            IPV4(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Subnet mask, LD::Port port) noexcept:mAddress{firstOctet,secondOctet,thirdOctet,fourthOctet,mask,port}
            {

            }

            IPV4(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Port mask, LD::Subnet port) noexcept:mAddress{firstOctet,secondOctet,thirdOctet,fourthOctet,mask,port}
            {

            }

            LD::IPV4Address Address() const
            {
                return this->mAddress;
            }
        };
    }
    inline auto ToImmutableString(const LD::PVE::IPV4 param) noexcept
    {
        LD::IPV4Address address = param.Address();
        LD::ImmutableString period{"."};
        LD::ImmutableString slash{"/"};
        auto firstOctetAsString = LD::ToImmutableString(LD::UInteger(address.FirstOctet()));
        auto secondOctetAsString = LD::ToImmutableString(LD::UInteger(address.SecondOctet()));
        auto thirdOctetAsString = LD::ToImmutableString(LD::UInteger(address.ThirdOctet()));
        auto fourthOctetAsString = LD::ToImmutableString(LD::UInteger(address.FourthOctet()));
        auto maskAsString = LD::ToImmutableString(LD::UInteger(address.Mask().Value()));
        return LD::ImmutableString{"ip="}+firstOctetAsString+period+secondOctetAsString+period+thirdOctetAsString+period+fourthOctetAsString+slash+maskAsString;
    }
}
#endif //LANDESSDEVCORE_IPV4_HPP
