//
// Created by phoenixflower on 8/1/21.
//
#include <iostream>
#include "REST/Proxmox/ProxyAPI.h"
#include "Network/SSH/SSH.hpp"
#include "Network/SSH/SSHHost.hpp"
#include "REST/Proxmox/Parameters/InterfaceIndex.hpp"
#include "REST/Proxmox/Parameters/SSHKey.h"
#include "REST/Proxmox/Parameters/BridgeName.hpp"
#include "REST/Proxmox/Parameters/BridgeIndex.hpp"
#include "REST/Proxmox/Parameters/NetworkBridge.hpp"

#include "Primitives/General/mapboxvariantvisitor.h"
#include "IO/json.hpp"
#include "IaaS/DockerManifest.hpp"
#include "Algorithms/ToJSON.hpp"
namespace LD
{
    template<typename ... ManifestOptiosn>
    void DeployDockerRunTime(LD::IPV6Address generatedAddress, const LD::Docker::Manifest<ManifestOptiosn...> & manifest) noexcept
    {
        //static constexpr auto StringValue = LD::ImmutableString{"abc"};
        LD::RequestResponse<bool()> connectionStatus = LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{});
        LD::SSH currentShell;
        auto address = LD::ToImmutableString(generatedAddress);
        while(!LD::IsTransactionalResponse(connectionStatus))
        {
            currentShell = LD::SSH{};
            currentShell.SetHost(LD::SSHHost{generatedAddress});
            connectionStatus = currentShell.ConnectAndAutoAccept(LD::SSHUserName{LD::ImmutableString{"root"}},LD::SSHPublicKeyAuthentication{});
        }
        nlohmann::json  document;
        //TODO - figure out exactly how much storage is required;
        LD::ToJson(document,manifest);
        auto dockerRuntime = LD::ImmutableString{"'{\n"}+
                             LD::ImmutableString{"    \"registry-mirrors\": [\"https://registry.landesshome.com\"],\n"}+
                             LD::ImmutableString{"    \"ipv6\": true,\n"} +
                             LD::ImmutableString{"    \"fixed-cidr-v6\": \"64:ff9b:99:0000:fc00::/80\"\n"}+
                             LD::ImmutableString{"}'"};



        //LD::ImmutableString<294> dockerRuntimeDaemonString{dockerRuntimeDaemon,strlen(dockerRuntimeDaemon)};
        auto echoCommandString = LD::ImmutableString{"echo "} + dockerRuntime + LD::ImmutableString{" > /etc/docker/daemon.json"};

        LD::SSHCommand nointeraction{LD::SSHRawCommand{LD::ImmutableString{"export DEBIAN_FRONTEND=noninteractive"}}};
        LD::SSHCommand update{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   update"}}};
        LD::SSHCommand upgrade{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   full-upgrade"}}};
        LD::SSHCommand installapttransporthttps{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install apt-transport-https"}}};
        LD::SSHCommand installCURL{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install curl"}}};
        LD::SSHCommand installGNUPg{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install gnupg"}}};
        LD::SSHCommand installSoftwarePropertiesCommon{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install software-properties-common"}}};
        LD::SSHCommand addDockerKey{LD::SSHRawCommand{LD::ImmutableString{"curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -"}}};
        LD::SSHCommand aptkey{LD::SSHRawCommand{LD::ImmutableString{"apt-key fingerprint 0EBFCD88"}}};
        LD::SSHCommand addDockerRepo{LD::SSHRawCommand{LD::ImmutableString{"add-apt-repository  \"deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable\""}}};
        LD::SSHCommand installDocker{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install docker-ce docker-ce-cli containerd.io docker-compose --fix-missing"}}};
        LD::SSHCommand installPeristentIPTables{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install iptables-persistent"}}};
        LD::SSHCommand applyIPV6NATFirewallRule{LD::SSHRawCommand{LD::ImmutableString{"/usr/sbin/ip6tables -w10 -t nat -I POSTROUTING -s 64:ff9b:99:0000:fc00::/80 ! -o docker0 -j MASQUERADE"}}};
        LD::SSHCommand setDockerRuntime{LD::SSHRawCommand{echoCommandString}};
        LD::SSHCommand restartDockerRuntime{LD::SSHRawCommand{LD::ImmutableString{"systemctl restart docker"}}};
        LD::SSHCommand dockerInstallNginx{LD::SSHRawCommand{LD::ImmutableString{"docker run -d -p 80:80 nginx:latest"}}};


        currentShell.Execute(LD::Type<int>{},nointeraction);
        currentShell.Execute(LD::Type<int>{},update);
        currentShell.Execute(LD::Type<int>{},upgrade);
        currentShell.Execute(LD::Type<int>{},installapttransporthttps);
        currentShell.Execute(LD::Type<int>{},installCURL);
        currentShell.Execute(LD::Type<int>{},installGNUPg);
        currentShell.Execute(LD::Type<int>{},installSoftwarePropertiesCommon);
        currentShell.Execute(LD::Type<int>{},addDockerKey);
        currentShell.Execute(LD::Type<int>{},aptkey);
        currentShell.Execute(LD::Type<int>{},addDockerRepo);
        currentShell.Execute(LD::Type<int>{},update);
        currentShell.Execute(LD::Type<int>{},upgrade);
        currentShell.Execute(LD::Type<int>{},installDocker);
        currentShell.Execute(LD::Type<int>{},setDockerRuntime);
        currentShell.Execute(LD::Type<int>{},installPeristentIPTables);
        currentShell.Execute(LD::Type<int>{},applyIPV6NATFirewallRule);
        currentShell.Execute(LD::Type<int>{},restartDockerRuntime);
        currentShell.Execute(LD::Type<int>{},dockerInstallNginx);
    }

    template<
            typename URLType,
            typename UserNameType,
            typename PasswordType,
            typename T, typename G,
            typename H,
            typename ... A,
            typename NodeType,
            typename FQDNType,
            typename TemplateType>
    LD::RequestResponse<LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>(A...)> DeployInstance(
            LD::PVE::ProxyAPI<URLType,UserNameType,PasswordType> & pveProxy,
            T && instanceType,
            NodeType && node,
            FQDNType && fqdn,
            TemplateType && usableTemplate,
            LD::UInteger numberOfCores,
            G && primaryBridge,
            H && instanceOptions,
            A && ... args) noexcept
    {
        using ProxyAPIType = LD::PVE::ProxyAPI<URLType,UserNameType,PasswordType>;


        LD::RequestResponse<LD::PVE::CreateInstanceResponse(ProxyAPIType&,NodeType&&, A&&...)> response =  pveProxy.CreateInstance(
                LD::Forward<T>(instanceType),
                LD::Forward<NodeType>(node),
                LD::Forward<FQDNType>(fqdn),
                4,
                LD::Forward<G>(primaryBridge),
                LD::Forward<TemplateType>(usableTemplate),
                LD::Forward<H>(instanceOptions),
                pveProxy,
                LD::Forward<NodeType>(node),
                LD::Forward<A>(args)...
        );


        auto onResponse = [](LD::PVE::CreateInstanceResponse response, ProxyAPIType & api, NodeType & node,A & ... args) noexcept
        {
            auto status = api.FetchInstanceConfiguration(
                    typename T::Type{},
                    node,
                    response.ID(),
                    args...);

            while (LD::IsErrorResponse(status))
            {
                sleep(1);
                status = api.FetchInstanceConfiguration(
                        typename T::Type{},
                        node,
                        response.ID(),
                        args...);
            }

            auto onError =  [](LD::TransactionError error, A & ... args) noexcept
            {
                return LD::CreateResponse(LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},LD::TransactionError{},LD::Forward<A>(args)...);
            };

            auto onLXCInstance = [](LD::PVE::LXC instance, A & ... args) noexcept
            {
                return LD::CreateResponse(LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},instance,LD::Forward<A>(args)...);
            };

            auto onVMInstance = [](LD::PVE::VirtualMachine instance, A & ... args) noexcept
            {
                return LD::CreateResponse(LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},instance,LD::Forward<A>(args)...);
            };

            return LD::InvokeVisitation(LD::Overload{onError,onLXCInstance,onVMInstance},status);
        };
        auto onError = [](LD::TransactionError response, ProxyAPIType & api, NodeType,A &  ... args) noexcept
        {
            return LD::CreateResponse(LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},LD::TransactionError{},LD::Forward<A>(args)...);
        };

        return LD::InvokeVisitation(LD::Overload{onResponse,onError},response);
    }
    namespace Example
    {
        extern void DeploymentExample()
        {
            LD::PVE::ProxyAPI proxmox{LD::ImmutableString{"null"},LD::ImmutableString{"null"},LD::ImmutableString{"null"}};


            LD::PVE::NetworkBridge bridge{
                    LD::PVE::InterfaceIndex{0},
                    LD::PVE::BridgeName{LD::ImmutableString{"eth0"}},
                    LD::PVE::BridgeIndex{8},
                    LD::PVE::MTU{1500},
                    LD::PVE::FirewallStatus{false},
                    LD::PVE::IPV6Auto{}
            };

            auto options = LD::MakeTuple(
                    LD::PVE::RootFS{LD::DiskSize{8_GB},LD::ImmutableString{"local-zfs"}},
                    LD::PVE::Memory{8_GB},
                    LD::PVE::Swap{8_GB},
                    LD::PVE::FeatureSet {LD::PVE::Nesting{true}},
                    LD::PVE::SSHKey{LD::SSH::GetPublicKey()},
                    LD::PVE::StartOnCreation{true});
            LD::RequestResponse<LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>()> deployedInstance = DeployInstance(
                    proxmox,
                    LD::PVE::LXCRequest{},
                    LD::ImmutableString{"virtualhome"},
                    LD::ImmutableString{"cloud.landesshome.com"},
                    LD::ImmutableString{"debian-10-standard_10.7-1_amd64.tar.gz"},
                    4,bridge,
                    options);


            auto onLXCInstance = [](LD::PVE::LXC instance) noexcept
            {
                LD::PVE::Interface interface = instance.PrimaryNetworkInterface();
                LD::IPV6Address hostAddress{LD::IPV6NetworkSegment{"2600:1700:6070:65df::/64"},interface.Layer2Address()};
                std::cout << "Layer 2 Address: " << LD::ToImmutableString(interface.Layer2Address()).Data() << std::endl;
                std::cout << "Layer 3 Address: " << LD::ToImmutableString(hostAddress).Data() << std::endl;


                LD::SSH currentShell;
                LD::RequestResponse<bool(LD::SSH&)> connectionStatus = LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},currentShell);

                while(!LD::IsTransactionalResponse(connectionStatus))
                {
                    std::cout << "Waiting on SSH connection " << std::endl;
                    currentShell = LD::SSH{};
                    currentShell.SetHost(LD::SSHHost{hostAddress});
                    connectionStatus = currentShell.ConnectAndAutoAccept(LD::SSHUserName{LD::ImmutableString{"root"}},LD::SSHPublicKeyAuthentication{},currentShell);
                }
                auto onConnectionSuccess = [](bool didComplete, LD::SSH & currentShell) noexcept
                {
                    LD::SSHCommand update{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   update"}}};
                    LD::SSHCommand upgrade{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   full-upgrade"}}};
                    LD::SSHCommand installCURL{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install curl"}}};
                    LD::SSHCommand installGNUPg{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install gnupg"}}};
                    LD::SSHCommand installKeepAlived{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install keepalived"}}};
                    currentShell.Execute(LD::Type<int>{},update);
                    currentShell.Execute(LD::Type<int>{},upgrade);
                    currentShell.Execute(LD::Type<int>{},installCURL);
                    currentShell.Execute(LD::Type<int>{},installGNUPg);
                    currentShell.Execute(LD::Type<int>{},installKeepAlived);
                };
                auto onConnectionFailure = [](LD::TransactionError error, LD::SSH & currentShell) noexcept
                {

                };



                return LD::InvokeVisitation(LD::Overload{onConnectionSuccess,onConnectionFailure},connectionStatus);

            };

            auto onVMInstance = [](LD::PVE::VirtualMachine instance) noexcept
            {

            };

            auto onError = [](LD::TransactionError error) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onLXCInstance,onVMInstance,onError},deployedInstance);
            /*
            LD::PVE::ProxyAPI proxmox{LD::ImmutableString{"https://virtualhome.landesshome.com:8006"},LD::ImmutableString{"root"},LD::ImmutableString{"Virtual_Home_Dcs1224"}};
            using ProxyAPIType = decltype(proxmox);

            LD::PVE::NetworkBridge bridge{
                    LD::PVE::InterfaceIndex{0},
                    LD::PVE::BridgeName{LD::ImmutableString{"eth0"}},
                    LD::PVE::BridgeIndex{8},
                    LD::PVE::MTU{1500},
                    LD::PVE::FirewallStatus{false},
                    LD::PVE::IPV6Auto{}
            };

            auto options = LD::MakeTuple(
                    LD::PVE::RootFS{LD::DiskSize{8_GB},LD::ImmutableString{"local-zfs"}},
                    LD::PVE::Memory{8_GB},
                    LD::PVE::Swap{8_GB},
                    LD::PVE::FeatureSet {LD::PVE::Nesting{true}},
                    LD::PVE::SSHKey{LD::SSH::GetPublicKey()},
                    LD::PVE::StartOnCreation{true});



            LD::RequestResponse<LD::PVE::CreateInstanceResponse(ProxyAPIType&)> response =  proxmox.CreateInstance(
                    LD::PVE::LXCRequest{},
                    LD::ImmutableString{"virtualhome"},
                    LD::ImmutableString{"cloud.landesshome.com"},
                    4,
                    bridge,
                    LD::ImmutableString {"debian-10-standard_10.7-1_amd64.tar.gz"},
                    options,
                    proxmox
            );

            auto onResponse = [](LD::Context<LD::TransactionResult, LD::PVE::CreateInstanceResponse,ProxyAPIType&> context) noexcept
            {
                auto response = LD::Get(LD::Get<1>(context));
                std::cout << "Response ID: " << response.ID() << std::endl;
                auto & proxmoxAPI = LD::Get(LD::Get<2>(context));

                auto status = proxmoxAPI.FetchInstanceConfiguration(
                        LD::PVE::LXC{},
                        LD::ImmutableString{"virtualhome"},
                        response.ID());
                while (!LD::IsTransactionalResponse(status))
                {
                    sleep(1);
                    status = proxmoxAPI.FetchInstanceConfiguration(
                            LD::PVE::LXC{},
                            LD::ImmutableString{"virtualhome"},
                            response.ID());
                }
                auto onError = [](LD::Context<LD::TransactionError> ) noexcept
                {

                };
                auto onInstance = [](const LD::Context<LD::TransactionResult,LD::PVE::LXC> context) noexcept
                {
                    const LD::PVE::LXC & instance = LD::Get(LD::Get<1>(context));
                    LD::PVE::Interface interface = instance.PrimaryNetworkInterface();
                    //LD::SSHCommand command{LD::AptCommand{LD::AptInstall{LD::RawPackageName{LD::ImmutableString{"nginx"}}}}};

                    //LD::SSHCommand command{LD::AptCommand{LD::AptInstall{LD::RawPackageName{LD::ImmutableString{"docker run -p 80 nginx:latest"}}}}};

                    std::cout << "Layer 2 Address: " << LD::ToImmutableString(interface.Layer2Address()).Data() << std::endl;
                    LD::IPV6Address generatedAddress{LD::IPV6NetworkSegment{"2600:1700:6070:65df::/64"},LD::IPV6HostSegment{interface.Layer2Address()}};
                    auto address = LD::ToImmutableString(generatedAddress);
                    std::cout << "Address test: " << address.Data() << std::endl;

                    LD::RequestResponse<bool()> connectionStatus = LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{});
                    LD::SSH currentShell;
                    while(!LD::IsTransactionalResponse(connectionStatus))
                    {
                        currentShell = LD::SSH{};
                        currentShell.SetHost(LD::SSHHost{LD::IPV6Address{address}});
                        //currentShell.SetHost(LD::SSHHost{LD::StringView {address.Data()}});
                        connectionStatus = currentShell.ConnectAndAutoAccept(LD::SSHUserName{LD::ImmutableString{"root"}},LD::SSHPublicKeyAuthentication{});
                    }


                    //export DEBIAN_FRONTEND=noninteractive

                    "{\n"
                    "    \"registry-mirrors\": [\"https://registry.landesshome.com\"],\n"
                    "    \"ipv6\": true,\n"
                    "    \"fixed-cidr-v6\": \"fd00::/80\"\n"
                    "}";
                    auto dockerRuntime = LD::ImmutableString{"'{\n"}+
                                         LD::ImmutableString{"    \"registry-mirrors\": [\"https://registry.landesshome.com\"],\n"}+
                                         LD::ImmutableString{"    \"ipv6\": true,\n"} +
                                         LD::ImmutableString{"    \"fixed-cidr-v6\": \"fd00::/80\"\n"}+
                                         LD::ImmutableString{"}'"};
                    //LD::ImmutableString<294> dockerRuntimeDaemonString{dockerRuntimeDaemon,strlen(dockerRuntimeDaemon)};
                    auto echoCommandString = LD::ImmutableString{"echo "} + dockerRuntime + LD::ImmutableString{" > /etc/docker/daemon.json"};
                    //LD::ImmutableString<294> dockerRuntimeDaemonString{dockerRuntimeDaemon,strlen(dockerRuntimeDaemon)};
                    LD::SSHCommand nointeraction{LD::SSHRawCommand{LD::ImmutableString{"export DEBIAN_FRONTEND=noninteractive"}}};
                    LD::SSHCommand update{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   update"}}};
                    LD::SSHCommand upgrade{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   full-upgrade"}}};
                    LD::SSHCommand installapttransporthttps{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install apt-transport-https"}}};
                    LD::SSHCommand installCURL{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install curl"}}};
                    LD::SSHCommand installGNUPg{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install gnupg"}}};
                    LD::SSHCommand installSoftwarePropertiesCommon{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install software-properties-common"}}};
                    LD::SSHCommand addDockerKey{LD::SSHRawCommand{LD::ImmutableString{"curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -"}}};
                    LD::SSHCommand aptkey{LD::SSHRawCommand{LD::ImmutableString{"apt-key fingerprint 0EBFCD88"}}};
                    LD::SSHCommand addDockerRepo{LD::SSHRawCommand{LD::ImmutableString{"add-apt-repository  \"deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable\""}}};
                    LD::SSHCommand installDocker{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install docker-ce docker-ce-cli containerd.io docker-compose --fix-missing"}}};
                    LD::SSHCommand setDockerRuntime{LD::SSHRawCommand{echoCommandString}};
                    LD::SSHCommand restartDockerRuntime{LD::SSHRawCommand{LD::ImmutableString{"systemctl restart docker"}}};
                    LD::SSHCommand dockerInstallNginx{LD::SSHRawCommand{LD::ImmutableString{"docker run -d -p 80:80 nginx:latest"}}};

                    LD::SSHCommandSet DockerInstallCommandSet{
                            LD::SSHCommand{LD::SSHRawCommand{LD::ImmutableString{"export DEBIAN_FRONTEND=noninteractive"}}},
                            LD::SSHCommand{LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   update"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y   full-upgrade"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install apt-transport-https"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install curl"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y  install gnupg"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install software-properties-common"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-key fingerprint 0EBFCD88"}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"add-apt-repository  \"deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable\""}}},
                            LD::SSHCommand {LD::SSHRawCommand{LD::ImmutableString{"apt-get -y install docker-ce docker-ce-cli containerd.io docker-compose --fix-missing"}}}
                    };



                    currentShell.Execute(LD::Type<int>{},nointeraction);
                    currentShell.Execute(LD::Type<int>{},update);
                    currentShell.Execute(LD::Type<int>{},upgrade);
                    currentShell.Execute(LD::Type<int>{},installapttransporthttps);
                    currentShell.Execute(LD::Type<int>{},installCURL);
                    currentShell.Execute(LD::Type<int>{},installGNUPg);
                    currentShell.Execute(LD::Type<int>{},installSoftwarePropertiesCommon);
                    currentShell.Execute(LD::Type<int>{},addDockerKey);
                    currentShell.Execute(LD::Type<int>{},aptkey);
                    currentShell.Execute(LD::Type<int>{},addDockerRepo);
                    currentShell.Execute(LD::Type<int>{},update);
                    currentShell.Execute(LD::Type<int>{},upgrade);
                    currentShell.Execute(LD::Type<int>{},installDocker);
                    currentShell.Execute(LD::Type<int>{},setDockerRuntime);
                    currentShell.Execute(LD::Type<int>{},restartDockerRuntime);

                    //currentShell.Execute(LD::Type<int>{},DockerInstallCommandSet);
                    currentShell.Execute(LD::Type<int>{},dockerInstallNginx);
                    std::cout << LD::ToImmutableString(interface.Layer2Address()).Data() << std::endl;
                    auto systemCommand = LD::ImmutableString{"chromium "} + LD::ImmutableString{"http://["}+address+LD::ImmutableString{"] &"};
                    system(systemCommand.Data());
                };
                auto onVM  = [](const LD::Context<LD::TransactionResult,LD::PVE::VirtualMachine> context) noexcept
                {

                };

                //LD::MultiMatch(LD::Overload{onError,onInstance,onVM},status);
                std::cout << "System has been created" << std::endl;

            };

            //{"data":{"net0":"name=eth0,bridge=vmbr0,firewall=0,gw=192.168.105.1,gw6=2001:0db8:0a0b:12f0:0000:0000:0000:0001,hwaddr=A2:98:0B:4E:09:6C,ip=192.168.105.28/24,ip6=2600:0000:0000:0000:0000:0000:0000:0001/64,mtu=1500,tag=77,type=veth","digest":"7a2f7da0f9cf6477ee41801d0b3b6a41ea954f64","memory":8192,"features":"force_rw_sys=0,fuse=0,keyctl=0,nesting=1,mknod=0","swap":8192,"rootfs":"local-zfs:subvol-110-disk-0,size=8G","hostname":"cloud.landesshome.com","cores":4,"arch":"amd64","ostype":"debian"}}
            auto onError = [](LD::Context<LD::TransactionError,ProxyAPIType&> error) noexcept
            {

            };


            //LD::MultiMatch(LD::Overload{onResponse,onError},response);


            /*
             proxmox.FetchInstanceConfiguration(
                     LD::PVE::LXC{},
                     LD::ImmutableString{"virtualhome"},
                     109);
                     */
        }
    }
}