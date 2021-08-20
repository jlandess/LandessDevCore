//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PXE_GATEWAY_HPP
#define LANDESSDEVCORE_PXE_GATEWAY_HPP
#include "Network/Gateway.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class Gateway
        {
        private:
            LD::Gateway mGateway;
        public:
            Gateway(){}
            Gateway(LD::Gateway gateway) noexcept:mGateway{gateway}
            {

            }
            Gateway(LD::IPV4Address address) noexcept:mGateway{address}
            {

            }

            LD::IPV4Address Address() const noexcept
            {
                return this->mGateway.Address();
            }
        };
    }

}
namespace LD
{
    inline auto ToImmutableString(const  LD::PVE::Gateway & param) noexcept
    {
        LD::IPV4Address address = param.Address();
        LD::ImmutableString period{"."};
        //LD::ImmutableString slash{"/"};
        auto firstOctetAsString = LD::ToImmutableString(LD::UInteger(address.FirstOctet()));
        auto secondOctetAsString = LD::ToImmutableString(LD::UInteger(address.SecondOctet()));
        auto thirdOctetAsString = LD::ToImmutableString(LD::UInteger(address.ThirdOctet()));
        auto fourthOctetAsString = LD::ToImmutableString(LD::UInteger(address.FourthOctet()));
        //auto maskAsString = LD::ToImmutableString(LD::UInteger(address.Mask()));
        return LD::ImmutableString{"gw="}+firstOctetAsString+period+secondOctetAsString+period+thirdOctetAsString+period+fourthOctetAsString;
    }
}
#endif //LANDESSDEVCORE_GATEWAY_HPP
