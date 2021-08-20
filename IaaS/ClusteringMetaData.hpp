//
// Created by phoenixflower on 3/23/21.
//

#ifndef LANDESSDEVCORE_CLUSTERINGMETADATA_HPP
#define LANDESSDEVCORE_CLUSTERINGMETADATA_HPP
#include "Definitions/Integer.hpp"
#include "Reflection/Reflection.hpp"
namespace LD
{
    namespace PVE
    {
        class ClusterMetaData
        {
        private:
            LD::UInteger mNumberOfLXCInstances;
            LD::UInteger mNumberOfVirtualMachineInstances;
            LD::UInteger mNumberOfDistributedFileSystemNodes;
            LD::UInteger mNumberOfHosts;
        public:
            ClusterMetaData() noexcept:mNumberOfLXCInstances{0},mNumberOfVirtualMachineInstances{0},mNumberOfDistributedFileSystemNodes{0},mNumberOfHosts{0}
            {

            }

            LD::UInteger & NumberOfLXCInstances() noexcept{return this->mNumberOfLXCInstances;}
            const LD::UInteger & NumberOfLXCInstances() const noexcept{return this->mNumberOfLXCInstances;}
            LD::UInteger & NumberOfVirtualMachineInstances() noexcept{return this->mNumberOfVirtualMachineInstances;}
            const LD::UInteger & NumberOfVirtualMachineInstances() const noexcept{return this->mNumberOfVirtualMachineInstances;}
            LD::UInteger & NumberOfDistributedFileSystemNodes() noexcept{return this->mNumberOfDistributedFileSystemNodes;}
            const LD::UInteger & NumberOfDistributedFileSystemNodes() const noexcept{return this->mNumberOfDistributedFileSystemNodes;}
            LD::UInteger & NumberOfHosts() noexcept{return this->mNumberOfHosts;}
            const LD::UInteger & NumberOfHosts() const noexcept{return this->mNumberOfHosts;}
        };
    }
}
template<>
struct LD::CT::TypeDescriptor<LD::PVE::ClusterMetaData>
{
private:
    static constexpr auto LXCInstance = ctll::basic_fixed_string("LXCInstanceCount");
    static constexpr auto NumberOfVirtualMachines = ctll::basic_fixed_string("VirtualMachineCount");
    static constexpr auto NumberOfDistributedFSNodes = ctll::basic_fixed_string("DistributedFileSystemNodeCount");
    static constexpr auto NumberOfHosts = ctll::basic_fixed_string("HostCount");
public:
    static constexpr auto ClassName = ctll::fixed_string{"ClusterMetaData"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<LXCInstance,LD::CT::SelectOverload<LD::UInteger & (LD::PVE::ClusterMetaData::*)(),&LD::PVE::ClusterMetaData::NumberOfLXCInstances>(),LD::CT::SelectOverload<const LD::UInteger & (LD::PVE::ClusterMetaData::*)() const,&LD::PVE::ClusterMetaData::NumberOfLXCInstances>()>,
            LD::CT::EncapsulatedMemberDescriptor<NumberOfVirtualMachines,LD::CT::SelectOverload<LD::UInteger & (LD::PVE::ClusterMetaData::*)(),&LD::PVE::ClusterMetaData::NumberOfVirtualMachineInstances>(),LD::CT::SelectOverload<const LD::UInteger & (LD::PVE::ClusterMetaData::*)() const,&LD::PVE::ClusterMetaData::NumberOfVirtualMachineInstances>()>,
            LD::CT::EncapsulatedMemberDescriptor<NumberOfDistributedFSNodes,LD::CT::SelectOverload<LD::UInteger & (LD::PVE::ClusterMetaData::*)(),&LD::PVE::ClusterMetaData::NumberOfDistributedFileSystemNodes>(),LD::CT::SelectOverload<const LD::UInteger & (LD::PVE::ClusterMetaData::*)() const,&LD::PVE::ClusterMetaData::NumberOfDistributedFileSystemNodes>()>,
            LD::CT::EncapsulatedMemberDescriptor<NumberOfHosts,LD::CT::SelectOverload<LD::UInteger & (LD::PVE::ClusterMetaData::*)(),&LD::PVE::ClusterMetaData::NumberOfHosts>(),LD::CT::SelectOverload<const LD::UInteger & (LD::PVE::ClusterMetaData::*)() const,&LD::PVE::ClusterMetaData::NumberOfHosts>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_CLUSTERINGMETADATA_HPP
