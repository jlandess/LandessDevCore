//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_ENABLEFIREWALL_HPP
#define LANDESSDEVCORE_ENABLEFIREWALL_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class EnableFirewall
        {
        private:
            LD::UInteger mValue;
        public:
            EnableFirewall() noexcept:mValue{false}
            {

            }

            EnableFirewall(bool value) noexcept:mValue{value}
            {

            }
            LD::UInteger Status() const noexcept
            {
                return this->mValue;
            }
        };
    }
    inline auto ToImmutableString(const LD::PVE::EnableFirewall object) noexcept
    {
        return LD::ImmutableString{"firewall="}+LD::ToImmutableString(object.Status());
    }
}
#endif //LANDESSDEVCORE_ENABLEFIREWALL_HPP
