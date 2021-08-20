//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_IPV6AUTO_HPP
#define LANDESSDEVCORE_IPV6AUTO_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class IPV6Auto
        {

        };
    }

    inline auto ToImmutableString(const LD::PVE::IPV6Auto object) noexcept
    {
        return LD::ImmutableString{"ip6=auto"};
    }
}
#endif //LANDESSDEVCORE_IPV6AUTO_HPP
