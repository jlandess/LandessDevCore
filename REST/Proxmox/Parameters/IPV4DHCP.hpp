//
// Created by phoenixflower on 2/15/21.
//

#ifndef LANDESSDEVCORE_IPV4DHCP_HPP
#define LANDESSDEVCORE_IPV4DHCP_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class IPV4DHCP
        {

        };
    }

    inline auto ToImmutableString(const LD::PVE::IPV4DHCP object) noexcept
    {
        return LD::ImmutableString{"ip=dhcp"};
    }
}
#endif //LANDESSDEVCORE_IPV4DHCP_HPP
