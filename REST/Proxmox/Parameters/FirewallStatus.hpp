//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_FIREWALLSTATUS_HPP
#define LANDESSDEVCORE_FIREWALLSTATUS_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class FirewallStatus
        {
        private:
            bool mStatus;
        public:
            FirewallStatus() noexcept:mStatus{false}{}
            FirewallStatus(bool status) noexcept:mStatus{status}{}
            bool Status() const noexcept{return this->mStatus;}
        };
    }

    inline auto ToImmutableString(LD::PVE::FirewallStatus firewallStatus) noexcept
    {
        return LD::ImmutableString{"firewall="}+LD::ToImmutableString(LD::UInteger(firewallStatus.Status()));
    }
}
#endif //LANDESSDEVCORE_FIREWALLSTATUS_HPP
