//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_DHCP_HPP
#define LANDESSDEVCORE_DHCP_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class DHCP
        {

        };
    }

    inline auto ToImmutableString(const LD::PVE::DHCP object) noexcept
    {
        return LD::ImmutableString{"ip=dhcp"};
    }
}
#endif //LANDESSDEVCORE_DHCP_HPP
