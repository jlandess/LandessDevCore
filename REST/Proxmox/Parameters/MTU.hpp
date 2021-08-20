//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_MTU_H
#define LANDESSDEVCORE_MTU_H
#include "Primitives/General/Immutable.hpp"
#include "Network/MTU.hpp"

namespace LD
{
    namespace PVE
    {
        class MTU
        {
        private:
            LD::UInteger mValue;
        public:
            MTU() noexcept:mValue{1500}
            {

            }
            MTU(LD::UInteger mtu) noexcept:mValue{mtu}
            {

            }

            LD::UInteger Value() const noexcept
            {
                return this->mValue;
            }
        };
    }

    inline auto ToImmutableString(const LD::PVE::MTU mtu) noexcept
    {
        return LD::ImmutableString{"mtu="}+LD::ToImmutableString(mtu.Value());
    }
}
#endif //LANDESSDEVCORE_MTU_HPP
