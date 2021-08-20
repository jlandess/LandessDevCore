//
// Created by phoenixflower on 1/25/21.
//

#ifndef LANDESSDEVCORE_IPV4ADDRESS_HPP
#define LANDESSDEVCORE_IPV4ADDRESS_HPP
#include "Primitives/General/Immutable.hpp"
#include "Port.hpp"
#include "Subnet.hpp"
#include "ipv6.h"
#include "Primitives/General/StringView.hpp"
namespace LD
{
    class IPV4Address
    {
    private:
        unsigned char mFirstOctet;
        unsigned char mSecondOctet;
        unsigned char mThirdOctet;
        unsigned char mFourthOctet;
        LD::Subnet mMask;
        LD::Port mPort;
    public:
        IPV4Address() noexcept:mFirstOctet{0},mSecondOctet{0},mThirdOctet{0},mFourthOctet{0},mMask{},mPort{}
        {

        }

        IPV4Address(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet) noexcept:
        mFirstOctet{firstOctet},
        mSecondOctet{secondOctet},
        mThirdOctet{thirdOctet},
        mFourthOctet{fourthOctet},
        mMask{},
        mPort{}
        {

        }

        IPV4Address(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Port port) noexcept:
                mFirstOctet{firstOctet},
                mSecondOctet{secondOctet},
                mThirdOctet{thirdOctet},
                mFourthOctet{fourthOctet},
                mPort{port}
        {

        }

        IPV4Address(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Subnet mask) noexcept:
                mFirstOctet{firstOctet},
                mSecondOctet{secondOctet},
                mThirdOctet{thirdOctet},
                mFourthOctet{fourthOctet},
                mMask{mask}
        {

        }

        IPV4Address(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Subnet mask, LD::Port port) noexcept:
                mFirstOctet{firstOctet},
                mSecondOctet{secondOctet},
                mThirdOctet{thirdOctet},
                mFourthOctet{fourthOctet},
                mMask{mask},
                mPort{port}
        {

        }

        IPV4Address(unsigned char firstOctet, unsigned char secondOctet, unsigned char thirdOctet, unsigned char fourthOctet, LD::Port port,LD::Subnet mask) noexcept:
                mFirstOctet{firstOctet},
                mSecondOctet{secondOctet},
                mThirdOctet{thirdOctet},
                mFourthOctet{fourthOctet},
                mMask{mask},
                mPort{port}
        {

        }

        IPV4Address(LD::StringView address) noexcept
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(address.data(),address.size(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {
                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint8_t firstOctet = firstSegment >> 8;
                uint8_t secondOctet = firstSegment;
                uint8_t thirdOctet = secondSegment >> 8;
                uint8_t fourthOctet = secondSegment;
                this->mPort = LD::Port{outputAddress.port};
                this->mMask = outputAddress.mask;
                this->mFirstOctet = firstOctet;
                this->mSecondOctet = secondOctet;
                this->mThirdOctet = thirdOctet;
                this->mFourthOctet = fourthOctet;
            }
        }

        IPV4Address(const LD::ImmutableString<28> & address) noexcept
        {
            ipv6_address_full_t outputAddress;
            ipv6_from_str(address.Data(),address.GetSize(),&outputAddress);
            if (outputAddress.flags == IPV6_FLAG_IPV4_COMPAT|IPV6_FLAG_HAS_MASK|IPV6_FLAG_HAS_PORT)
            {
                uint16_t firstSegment = outputAddress.address.components[0];
                uint16_t secondSegment = outputAddress.address.components[1];
                uint8_t firstOctet = firstSegment >> 8;
                uint8_t secondOctet = firstSegment;
                uint8_t thirdOctet = secondSegment >> 8;
                uint8_t fourthOctet = secondSegment;
                this->mPort = LD::Port{outputAddress.port};
                this->mMask = outputAddress.mask;
                this->mFirstOctet = firstOctet;
                this->mSecondOctet = secondOctet;
                this->mThirdOctet = thirdOctet;
                this->mFourthOctet = fourthOctet;
            }
        }

        IPV4Address(const LD::ImmutableString<15> & ip) noexcept
        {

        }

        unsigned char FirstOctet() const noexcept
        {
            return this->mFirstOctet;
        }

        unsigned char SecondOctet() const noexcept
        {
            return this->mSecondOctet;
        }

        unsigned char ThirdOctet() const noexcept
        {
            return this->mThirdOctet;
        }

        unsigned char FourthOctet() const noexcept
        {
            return this->mFourthOctet;
        }
        LD::Port Port() const noexcept
        {
            return this->mPort;
        }

        LD::Subnet Mask() const noexcept
        {
            return this->mMask;
        }

        LD::StaticArray<unsigned char,4> Octets() const noexcept
        {
            return LD::StaticArray<unsigned char,4>{};
        }
    };

    inline auto ToImmutableString(const LD::IPV4Address address) noexcept
    {
        auto octets = address.Octets();
        LD::ImmutableString period{"."};

        LD::ImmutableString<1> first{octets[0]};
        LD::ImmutableString<2> second{octets[1]};
        LD::ImmutableString<3> third{octets[2]};
        LD::ImmutableString<4> fourth{octets[3]};
        return first+period+second+period+third+period+fourth;
    }
}
#endif //LANDESSDEVCORE_IPV4ADDRESS_HPP
