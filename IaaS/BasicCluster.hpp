//
// Created by phoenixflower on 8/1/21.
//

#ifndef LANDESSDEVCORE_BASICCLUSTER_HPP
#define LANDESSDEVCORE_BASICCLUSTER_HPP
#include "IO/Database1.h"
#include "SumTypes/MPark/variant.hpp"
namespace LD
{
    namespace IaaS
    {
        class Service
        {

        };

        class PVENode
        {

        };

        class ESXiNode
        {

        };

        class OpenStackNode
        {

        };

        class CloudStackNode
        {

        };

        class DockerNode
        {

        };

        class PodmanNode
        {

        };

        class KubernetesNode
        {

        };

        class Node
        {
        private:
            mpark::variant<LD::IaaS::PVENode,LD::IaaS::ESXiNode,LD::IaaS::OpenStackNode,LD::IaaS::CloudStackNode,LD::IaaS::DockerNode,LD::IaaS::PodmanNode,LD::IaaS::KubernetesNode> mCore;
            LD::MegaByte<LD::UInteger> mMemory;
            LD::GigaByte<LD::UInteger> mStorage;
        public:
        };

        class GlusterCluster
        {

        };

        class MooseCluster
        {

        };

        class LizardCluster
        {

        };

        class CephCluster
        {

        };

        class SoftwareDefinedStorageCluster
        {

        };

        class HAPolicy
        {

        };


    }
    namespace PVE
    {
        class NodeReplication
        {

        };

        class ClusterReplication
        {

        };



        class BasicClusterAgent
        {

        };

        template<typename BackingStore>
        class BasicCluster
        {
        private:
            BackingStore & mBackingStore;
            LD::UInteger mNumberOfLXCInstances;
            LD::UInteger mNumberOfVirtualMachines;
            LD::UInteger mNumberOfDistributedFileSystemNodes;
            LD::PVE::ClusterMetaData mMetaData;
        public:
            BasicCluster(BackingStore & backingStore) noexcept:mBackingStore{backingStore},mNumberOfLXCInstances{0},mNumberOfVirtualMachines{0},mNumberOfDistributedFileSystemNodes{0}
            {
                auto clusterMetaDataQuery = LD::Fetch(this->mBackingStore,LD::ImmutableString{"ClusterMetaData"},LD::Type<LD::PVE::ClusterMetaData>{});


                auto onSuccessfulMetaDataQuery = [](LD::PVE::ClusterMetaData metaData) noexcept
                {
                    return LD::Optional<LD::PVE::ClusterMetaData>{metaData};
                };

                auto onUnSuccessfulMetaDataQuery = [](LD::TransactionError error) noexcept
                {
                    return LD::Optional<LD::PVE::ClusterMetaData>{};
                };

                auto possibleClusterMetaData = LD::InvokeVisitation(LD::Overload{onSuccessfulMetaDataQuery,onUnSuccessfulMetaDataQuery},clusterMetaDataQuery);
                if (possibleClusterMetaData)
                {
                    this->mMetaData = (*possibleClusterMetaData);
                }
                else
                {
                    LD::Insert(this->mBackingStore,LD::ImmutableString{"ClusterMetaData"},this->mMetaData,[](){return true;});
                }

            }

            void AddHost(LD::PVE::ProxyAPI<LD::ImmutableString<64>,LD::ImmutableString<64>,LD::ImmutableString<64>> proxy) noexcept
            {

                auto hostNamePrescence = LD::Fetch(this->mBackingStore,proxy.Hostname(),LD::Type<LD::PVE::ClusterMetaData>{});
                LD::Insert(this->mBackingStore,LD::ImmutableString{"ClusterMetaData"},this->mMetaData,[](){return true;});
            }

            void RemoveHost() noexcept
            {

            }

            LazyEvaluator Hosts() const noexcept
            {
                return {};
            }


            LD::IaaS::SoftwareDefinedStorageCluster AddSoftwareDefinedStorage() noexcept
            {
                return {};
            }


            template<typename PolicyFunctor, typename ... A>
            void ScheduleService(LD::IaaS::Service service,NodeReplication nodeReplication,ClusterReplication clusterReplication, PolicyFunctor && policyFunctor, LD::IaaS::HAPolicy policy,A && ... args) noexcept
            {

            }
        };
    }
}
#endif //LANDESSDEVCORE_BASICCLUSTER_HPP
