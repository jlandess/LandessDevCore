//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_VLAN_HPP
#define LANDESSDEVCORE_VLAN_HPP
#include "Primitives/General/Immutable.hpp"

namespace LD
{
    namespace PVE
    {
        class VLAN
        {
        private:
            LD::UInteger mValue;
        public:
            VLAN() noexcept:mValue{0}
            {

            }
            VLAN(LD::UInteger value) noexcept:mValue{value}
            {

            }
            LD::UInteger Value() const noexcept
            {
                return this->mValue;
            }
        };
    }
    inline auto ToImmutableString(const LD::PVE::VLAN vLAN) noexcept
    {
        return LD::ImmutableString{"tag="}+LD::ToImmutableString(vLAN.Value());
    }
}
#endif //LANDESSDEVCORE_VLAN_HPP
