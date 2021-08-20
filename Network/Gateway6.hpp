//
// Created by phoenixflower on 1/26/21.
//

#ifndef LANDESSDEVCORE_GATEWAY6_HPP
#define LANDESSDEVCORE_GATEWAY6_HPP
#include "IPV6Address.hpp"
namespace LD
{
    class Gateway6
    {
    private:
        LD::IPV6Address mAddress;
    public:
        Gateway6() noexcept
        {

        }
        Gateway6(LD::IPV6Address address) noexcept:mAddress{address}
        {
        }
        Gateway6(const LD::ImmutableString<49> & address) noexcept:mAddress{address}
        {

        }

        LD::IPV6Address Address() const noexcept
        {
            return this->mAddress;
        }
    };
}
#endif //LANDESSDEVCORE_GATEWAY6_HPP
