//
// Created by phoenixflower on 2/8/21.
//

#ifndef LANDESSDEVCORE_INTERFACE_HPP
#define LANDESSDEVCORE_INTERFACE_HPP
#include "Network/Gateway.hpp"
#include "Network/Gateway6.hpp"
#include "Network/IPV4Address.hpp"
#include "Network/IPV6Address.hpp"
#include "Primitives/General/Hexademical.hpp"
#include "InterfaceType.h"
#include "Network/MacAddress.hpp"
#include "SumTypes/MPark/variant.hpp"
#include "REST/Proxmox/Parameters/IPV4DHCP.hpp"
#include "Core/RequestResponse.hpp"
#include "REST/Proxmox/Parameters/FirewallStatus.hpp"
namespace LD
{
    namespace PVE
    {
        class Interface
        {
        private:
            mpark::variant<LD::Gateway,LD::PVE::IPV4DHCP> mIPV4Gateway;
            mpark::variant<LD::Gateway6,LD::PVE::IPV6Auto> mIPV6Gateway;
            mpark::variant<LD::IPV4Address,LD::PVE::IPV4DHCP> mIPV4Address;
            mpark::variant<LD::IPV6Address,LD::PVE::IPV6Auto> mIPV6Address;
            LD::MacAddress mAddress;
            LD::MTU mMTU;
            LD::UInteger mBridge;
            LD::UInteger mName;
            LD::PVE::InterfaceType mType;
            LD::PVE::FirewallStatus mFirewall;
        public:
            Interface() noexcept:mType{LD::PVE::Veth}
            {

            }
            Interface(mpark::variant<LD::Gateway,LD::PVE::IPV4DHCP> ipv4Gateway,
                      mpark::variant<LD::Gateway6,LD::PVE::IPV6Auto> ipv6Gateway,
                      mpark::variant<LD::IPV4Address,LD::PVE::IPV4DHCP> ipv4Address,
                      mpark::variant<LD::IPV6Address,LD::PVE::IPV6Auto> ipv6Address,
                      LD::MacAddress address,
                      LD::UInteger name,
                      LD::MTU mtu = 1500,
                      LD::PVE::FirewallStatus firewall = LD::PVE::FirewallStatus{false}) noexcept:
                      mType{LD::PVE::Veth},
                      mIPV4Gateway{ipv4Gateway},
                      mIPV6Gateway{ipv6Gateway},
                      mIPV4Address{ipv4Address},
                      mIPV6Address{ipv6Address},
                      mAddress{address},
                      mName{name},
                      mMTU{mtu},
                      mFirewall{firewall}
            {

            }

            LD::MTU MTU() const noexcept
            {
                return this->mMTU;
            }
            LD::MacAddress Layer2Address() const noexcept
            {
                return this->mAddress;
            }
            mpark::variant<LD::Gateway,LD::PVE::IPV4DHCP> IPV4Gateway() const noexcept
            {
                return this->mIPV4Gateway;
            }

            mpark::variant<LD::Gateway6,LD::PVE::IPV6Auto> IPV6Gateway() const noexcept
            {
                return this->mIPV6Gateway;
            }

        };
    }
}
#endif //LANDESSDEVCORE_INTERFACE_HPP
