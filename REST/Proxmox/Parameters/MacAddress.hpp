//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PXE_MACADDRESS_HPP
#define LANDESSDEVCORE_PXE_MACADDRESS_HPP
#include "Network/MacAddress.hpp"
namespace LD
{
    namespace PVE
    {
        class MacAddress
        {
        private:
            LD::MacAddress mAddress;
        public:
            MacAddress() noexcept
            {

            }
            MacAddress(const LD::ImmutableString<17> & address) noexcept:mAddress{address}
            {

            }

            auto FirstOctet() const noexcept
            {
                return this->mAddress.FirstOctet();
            }

            auto SecondOctet() const noexcept
            {
                return this->mAddress.SecondOctet();
            }

            auto ThirdOctet() const noexcept
            {
                return this->mAddress.ThirdOctet();
            }

            auto FourthOctet() const noexcept
            {
                return this->mAddress.FourthOctet();
            }

            auto FifthOctet() const noexcept
            {
                return this->mAddress.FifthOctet();
            }

            auto SixthOctet() const noexcept
            {
                return this->mAddress.SixthOctet();
            }
            auto SeventhOctet() const noexcept
            {
                return this->mAddress.SeventhOctet();
            }

            auto EightOctet() const noexcept
            {
                return this->mAddress.EightOctet();
            }

            auto NinthOctet() const noexcept
            {
                return this->mAddress.NinthOctet();
            }

            auto TenthOctet() const noexcept
            {
                return this->mAddress.TenthOctet();
            }

            auto EleventhOctet() const noexcept
            {
                return this->mAddress.EleventhOctet();
            }

            auto TwelfthOctet() const noexcept
            {
                return this->mAddress.TwelfthOctet();
            }
        };
    }
    inline auto ToImmutableString(const LD::PVE::MacAddress & address) noexcept
    {
        auto colon = LD::ImmutableString{":"};
        auto firstPair = LD::ToImmutableString(address.FirstOctet())+LD::ToImmutableString(address.SecondOctet());
        auto secondPair = LD::ToImmutableString(address.ThirdOctet())+LD::ToImmutableString(address.FourthOctet());
        auto thirdPair = LD::ToImmutableString(address.FifthOctet())+LD::ToImmutableString(address.SixthOctet());
        auto fourthPair = LD::ToImmutableString(address.SeventhOctet())+LD::ToImmutableString(address.EightOctet());
        auto fifthPair = LD::ToImmutableString(address.NinthOctet())+LD::ToImmutableString(address.TenthOctet());
        auto sixthPair = LD::ToImmutableString(address.EleventhOctet())+LD::ToImmutableString(address.TwelfthOctet());

        return LD::ImmutableString{"hwaddr="}+fifthPair+colon+secondPair+colon+thirdPair+colon+fourthPair+colon+fifthPair+colon+sixthPair;
    }
}
#endif //LANDESSDEVCORE_MACADDRESS_HPP
