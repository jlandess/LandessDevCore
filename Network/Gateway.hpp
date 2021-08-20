//
// Created by phoenixflower on 1/26/21.
//

#ifndef LANDESSDEVCORE_GATEWAY_HPP
#define LANDESSDEVCORE_GATEWAY_HPP
#include "IPV4Address.hpp"
namespace LD
{
    class Gateway
    {
    private:
        LD::IPV4Address mAddress;
    public:
        Gateway() = default;
        Gateway(LD::IPV4Address address) noexcept:mAddress{address}
        {

        }

        LD::IPV4Address Address() const noexcept
        {
            return this->mAddress;
        }
    };
}
#endif //LANDESSDEVCORE_GATEWAY_HPP
