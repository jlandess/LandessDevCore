//
// Created by phoenixflower on 2/8/21.
//

#ifndef LANDESSDEVCORE_LINUXCONTAINER_HPP
#define LANDESSDEVCORE_LINUXCONTAINER_HPP
#include "Network/Gateway.hpp"
#include "Network/Gateway6.hpp"
#include "Interface.hpp"
#include "REST/Proxmox/Parameters/RootFS.hpp"
#include "REST/Proxmox/Parameters/Memory.hpp"
#include "REST/Proxmox/Parameters/Swap.hpp"
#include "REST/Proxmox/Parameters/FeatureSet.hpp"
#include "REST/Proxmox/ArchitectureType.h"
#include "REST/Proxmox/OSType.hpp"

namespace LD
{

    namespace PVE
    {
        class LXC
        {
        private:
            LD::PVE::Interface mPrimaryInterface;
            LD::PVE::Architecture mArchitecture;
            LD::PVE::OSType mOSType;
            LD::ImmutableString<25> mHostName;
            LD::PVE::RootFS<LD::ImmutableString<25>> mRootFS;
            LD::PVE::Swap mSwapSize;
            LD::PVE::Memory mMemory;
            LD::PVE::FeatureSet mFeatures;
            LD::UInteger mCPUCoreCount;
        public:
            LXC() noexcept
            {

            }
            LXC(LD::PVE::Interface interface,
                LD::PVE::Architecture arch,
                LD::PVE::OSType osType,
                LD::ImmutableString<25> hostName,
                LD::PVE::RootFS<LD::ImmutableString<25>> rootFS,
                LD::PVE::Swap sap,
                LD::PVE::Memory memory,
                LD::PVE::FeatureSet featureSet,
                LD::UInteger cpuCoreCount) noexcept:
                mPrimaryInterface{interface},
                mArchitecture{arch},
                mOSType{osType},
                mHostName{hostName},
                mRootFS{rootFS},
                mSwapSize{sap},
                mMemory{memory},
                mFeatures{featureSet},
                mCPUCoreCount{cpuCoreCount}
            {

            }

            LD::PVE::Interface PrimaryNetworkInterface() const noexcept
            {
                return this->mPrimaryInterface;
            }
        };

        class LXCRequest
        {
        public:
            using Type = LD::PVE::LXC;
        };
    }
}
#endif //LANDESSDEVCORE_LINUXCONTAINER_HPP
