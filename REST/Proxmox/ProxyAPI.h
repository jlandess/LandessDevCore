//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PROXYAPI_H
#define LANDESSDEVCORE_PROXYAPI_H
#include <simdjson/simdjson.h>
#include "REST/Proxmox/Parameters/DHCP.hpp"
#include "REST/Proxmox/Parameters/EnableFirewall.hpp"
#include "REST/Proxmox/Parameters/IPV6Auto.hpp"
#include "REST/Proxmox/Parameters/MTU.hpp"
#include "REST/Proxmox/Parameters/VLAN.hpp"
#include "REST/Proxmox/Parameters/IPV4.hpp"
#include "REST/Proxmox/Parameters/IPV6.hpp"
#include "REST/Proxmox/Parameters/Gateway.hpp"
#include "REST/Proxmox/Parameters/Fuse.hpp"
#include "REST/Proxmox/Parameters/KeyCTL.hpp"
#include "REST/Proxmox/Parameters/MkNode.hpp"
#include "REST/Proxmox/Parameters/ForceSWSys.hpp"
#include "REST/Proxmox/Parameters/Nesting.hpp"
#include "REST/Proxmox/Parameters/RootFS.hpp"
#include "REST/Proxmox/Parameters/Swap.hpp"
#include "REST/Proxmox/Parameters/Memory.hpp"
#include "REST/Proxmox/Parameters/Gateway.hpp"
#include "REST/Proxmox/Parameters/Gateway6.hpp"
#include "REST/Proxmox/Permissions/Storage.h"
#include "REST/Proxmox/Permissions/VirtualMachine.h"
#include "REST/Proxmox/Permissions/Datacenter.h"
#include "REST/Proxmox/Permissions/Access.h"
#include "REST/Proxmox/Permissions/Node.h"
#include "REST/Proxmox/Permissions/SDN.h"
#include "REST/Proxmox/Parameters/IsNetworkBridge.h"
#include "LinuxContainer.hpp"
#include "VirtualMachine.hpp"
#include "IO/FetchRequest.h"
#include "Primitives/General/ctre.hpp"
#include "CreateInstanceResponse.hpp"
#include "REST/Proxmox/Parameters/IPV4DHCP.hpp"
#include "REST/Proxmox/Parameters/StartOnCreation.hpp"
#include "Primitives/General/ctre.hpp"
#include "Core/RequestResponse.hpp"
#include "REST/CPR/cpr.h"
namespace LD
{
    namespace PVE
    {
        namespace Detail
        {
            static constexpr auto  CreationRegex = ctll::basic_fixed_string{"UPID:([a-zA-Z]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([0-9a-fA-F]+):([a-zA-Z0-9]+):(\\d+):(\\w+)@([a-zA-Z0-9:]+)"};
            static constexpr auto  SegmentationRegex = ctll::basic_fixed_string{"[^,]+"};
            static constexpr auto  ParameterSegmentationRegex = ctll::basic_fixed_string{"[^=]+"};
            static constexpr auto  InterfaceNameRegularExpression = ctll::basic_fixed_string{"(\\w+)(\\d+)"};
            static constexpr auto  BridgeNameRegularExpression = ctll::basic_fixed_string{"(\\w+)(\\d+)"};
            //local-zfs:subvol-110-disk-0
            static constexpr auto  StorageRegularExpression = ctll::basic_fixed_string{"(\\w+):(\\w+)-(\\d+)-(\\w+)-(\\d+)"};
        }
    }

}
namespace LD
{
    namespace PVE
    {
        template<typename URLType,typename UserNameType,typename PasswordType>
        class ProxyAPI
        {
        private:
            URLType mURL;
            UserNameType mUserName;
            PasswordType mPassword;
            LD::ImmutableString<512> mCSRFPreventionToken;
            LD::ImmutableString<512> mTicket;
            LD::PVE::StoragePermission mStoragePermissions;
            LD::PVE::VMPermission mVirtualMachinePermissions;
            LD::PVE::SDNPermission mSDNPermissions;
            LD::PVE::DCPermission mDataCenterPermissions;
            LD::PVE::AccessPermission mAccessPermissions;
            LD::PVE::NodePermission mNodePermission;

            template<typename T>
            static LD::ImmutableString<4> InstanceTypeToString(LD::Type<T>) noexcept
            {
                if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<T>{},LD::Type<LD::PVE::LXC>{}))
                {
                    return LD::ImmutableString{"lxc"};
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<T>{},LD::Type<LD::PVE::VirtualMachine>{}))
                {
                    return LD::ImmutableString{"qemu"};
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<T>{},LD::Type<LD::PVE::QEMURequest>{}))
                {
                    return LD::ImmutableString{"qemu"};
                }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<T>{},LD::Type<LD::PVE::LXCRequest>{}))
                {
                    return LD::ImmutableString{"lxc"};
                }
                return LD::ImmutableString{""};
            }
        public:
            ProxyAPI(URLType && url, UserNameType && userName, PasswordType && password) noexcept:mURL{LD::Forward<URLType>(url)},mUserName{LD::Forward<UserNameType>(userName)},mPassword{LD::Forward<PasswordType>(password)}
            {
                auto ticketURL = url+LD::ImmutableString{"/api2/json/access/ticket"};
                auto body = LD::ImmutableString{"username="}+userName+LD::ImmutableString{"@pam"}+LD::ImmutableString{"&"}+LD::ImmutableString{"password"}+LD::ImmutableString{"="}+password;
                cpr::Response r = cpr::Post(cpr::Url{ticketURL.Data()},cpr::Body{body.Data()});
                //cpr::Response r = cpr::Post(cpr::Url{url.Data()},cpr::Body{body.Data()});
                std::cout << r.text << std::endl;
                simdjson::dom::parser parser;
                //parser["abc"];
                simdjson::simdjson_result<simdjson::dom::element> element =  parser.parse(LD::StringView{r.text.c_str()});
                auto dataNode = element["data"];
                auto CSRFPreventionToken = dataNode["CSRFPreventionToken"];
                auto ticket = dataNode["ticket"];
                auto cap = dataNode["cap"];
                auto storage = cap["storage"];
                auto vms = cap["vms"];
                auto dc = cap["dc"];
                auto access = cap["access"];
                auto sdn = cap["sdn"];
                auto nodes = cap["nodes"];
                LD::StringView CSRFPreventionTokenBuffer;
                LD::StringView ticketBuffer;


                LD::UInteger dataStoreAudit = false;
                LD::UInteger dataStoreAllocationTemplate = false;
                LD::UInteger dataStoreAllocateSpace = false;
                LD::UInteger dataStorePermissionsModify = false;
                LD::UInteger dataStoreAllocate = false;



                LD::UInteger vmSnapShot = false;
                LD::UInteger vmConfigOptions = false;
                LD::UInteger vmConfigCDRom = false;
                LD::UInteger vmConfigCPU = false;
                LD::UInteger vmConfigNetwork = false;
                LD::UInteger vmConfigHardwareType = false;
                LD::UInteger vmMigrate = false;
                LD::UInteger vmConfigMemory = false;
                LD::UInteger vmConfigCloudInit = false;
                LD::UInteger vmAudit = false;
                LD::UInteger vmPowerManagement = false;
                LD::UInteger vmPermissionsModify = false;
                LD::UInteger vmAllocate = false;
                LD::UInteger vmConfigDisk = false;
                LD::UInteger vmSnapshotRollback = false;
                LD::UInteger vmBackup = false;
                LD::UInteger vmConsole = false;
                LD::UInteger vmClone = false;
                LD::UInteger vmMonitor = false;



                LD::UInteger sdnAudit = false;
                LD::UInteger sdnAllocate = false;
                LD::UInteger sdnPermissionsModify = false;




                LD::UInteger dcSystemAudit = false;
                LD::UInteger dcSDNAudit = false;
                LD::UInteger dcSDNAllocate = false;

                LD::UInteger accessGroupAllocate = false;
                LD::UInteger accessUserModify = false;
                LD::UInteger accessPermissionsModify = false;


                LD::UInteger nodesSysLog = false;
                LD::UInteger nodesPermissionsModify = false;
                LD::UInteger nodesSysPowerManagment = false;
                LD::UInteger nodesSystemModify = false;
                LD::UInteger nodesSystemAudit = false;
                LD::UInteger nodesSystemConsole = false;
                CSRFPreventionToken.get(CSRFPreventionTokenBuffer);

                for(LD::UInteger n = 0;n<CSRFPreventionTokenBuffer.size();++n)
                {
                    this->mCSRFPreventionToken[n] = CSRFPreventionTokenBuffer[n];
                }

                ticket.get(ticketBuffer);


                for(LD::UInteger n = 0;n<ticketBuffer.size();++n)
                {
                    this->mTicket[n] = ticketBuffer[n];
                }

                storage["Datastore.Audit"].get(dataStoreAudit);
                storage["Datastore.AllocateTemplate"].get(dataStoreAllocationTemplate);
                storage["Datastore.AllocateSpace"].get(dataStoreAllocateSpace);
                storage["Permissions.Modify"].get(dataStorePermissionsModify);
                storage["Datastore.Allocate"].get(dataStoreAllocate);



                this->mStoragePermissions = StoragePermission{dataStoreAudit,dataStoreAllocationTemplate,dataStoreAllocateSpace,dataStorePermissionsModify,dataStoreAllocate};

                vms["VM.Snapshot"].get(vmSnapShot);
                vms["VM.Config.Options"].get(vmConfigOptions);
                vms["VM.Config.CDROM"].get(vmConfigCDRom);
                vms["VM.Config.CPU"].get(vmConfigCPU);
                vms["VM.Config.Network"].get(vmConfigNetwork);
                vms["VM.Config.HWType"].get(vmConfigHardwareType);
                vms["VM.Migrate"].get(vmMigrate);
                vms["VM.Config.Memory"].get(vmConfigMemory);
                vms["VM.Config.Cloudinit"].get(vmConfigCloudInit);
                vms["VM.Audit"].get(vmAudit);
                vms["VM.PowerMgmt"].get(vmPowerManagement);
                vms["Permissions.Modify"].get(vmPermissionsModify);
                vms["VM.Allocate"].get(vmAllocate);
                vms["VM.Config.Disk"].get(vmConfigDisk);
                vms["VM.Snapshot.Rollback"].get(vmSnapshotRollback);
                vms["VM.Backup"].get(vmBackup);
                vms["VM.Console"].get(vmConsole);
                vms["VM.Clone"].get(vmClone);
                vms["VM.Monitor"].get(vmMonitor);


                this->mVirtualMachinePermissions = VMPermission{
                        vmSnapShot,
                        vmConfigOptions,
                        vmConfigCDRom,
                        vmConfigCPU,
                        vmConfigNetwork,
                        vmConfigHardwareType,
                        vmMigrate,
                        vmConfigMemory,
                        vmConfigCloudInit,
                        vmAudit,
                        vmPowerManagement,
                        vmPermissionsModify,
                        vmAllocate,
                        vmConfigDisk,
                        vmSnapshotRollback,
                        vmBackup,
                        vmConsole,
                        vmClone,
                        vmMonitor};

                sdn["SDN.Audit"].get(sdnAudit);
                sdn["SDN.Allocate"].get(sdnAllocate);
                sdn["Permissions.Modify"].get(sdnPermissionsModify);

                this->mSDNPermissions = SDNPermission{sdnAudit,sdnAllocate,sdnPermissionsModify};


                dc["Sys.Audit"].get(dcSystemAudit);
                dc["SDN.Audit"].get(dcSDNAudit);
                dc["SDN.Allocate"].get(dcSDNAllocate);

                this->mDataCenterPermissions = DCPermission{dcSystemAudit,dcSDNAudit,dcSDNAllocate};




                access["Group.Allocate"].get(accessGroupAllocate);
                access["User.Modify"].get(accessUserModify);
                access["Permissions.Modify"].get(accessPermissionsModify);

                this->mAccessPermissions = AccessPermission{accessGroupAllocate,accessUserModify,accessPermissionsModify};


                nodes["Sys.Syslog"].get(nodesSysLog);
                nodes["Permissions.Modify"].get(nodesPermissionsModify);
                nodes["Sys.PowerMgmt"].get(nodesSysPowerManagment);
                nodes["Sys.Modify"].get(nodesSystemModify);
                nodes["Sys.Audit"].get(nodesSystemAudit);
                nodes["Sys.Console"].get(nodesSystemConsole);


                this->mNodePermission =  NodePermission{nodesSysLog,nodesPermissionsModify,nodesSysPowerManagment,nodesSystemModify,nodesSystemAudit,nodesSystemConsole};
            }


            const URLType & Hostname() const noexcept
            {
                return this->mURL;
            }

            LD::PVE::DCPermission DCPermissions() const noexcept
            {
                return this->mDataCenterPermissions;
            }
            LD::PVE::StoragePermission StoragePermissions() const noexcept
            {
                return this->mStoragePermissions;
            }

            LD::PVE::VMPermission VirtualMachinePermissions() const noexcept
            {
                return this->mVirtualMachinePermissions;
            }

            LD::PVE::AccessPermission AccessPermissions() const noexcept
            {
                return this->mAccessPermissions;
            }

            LD::PVE::NodePermission NodePermissions() const noexcept
            {
                return this->mNodePermission;
            }
            LD::PVE::SDNPermission SDNPermissions() const noexcept
            {
                return this->mSDNPermissions;
            }


            //LD::MegaByte<LD::UInteger>{512ul}
            // = LD::GigaByte<LD::UInteger>{4ul}
            //LD::GigaByte<LD::UInteger>{4ul}
            //Options && ... parametOptions) const noexcept
            template<typename InstanceType,typename NodeName,typename HostType,typename TemplateNameType,typename PrimaryBridge,typename ... Options, typename ... A>
            LD::RequestResponse<LD::PVE::CreateInstanceResponse(A...)> CreateInstance(
                    InstanceType && instance,
                    NodeName && node,
                    HostType && host,
                    LD::UInteger cores,
                    PrimaryBridge && bridge,
                    TemplateNameType && templateName,
                    LD::Tuple<Options...> options, A && ... args) const noexcept
            {
                //LD::RequestResponse<LD::PVE::CreateInstanceResponse(A...)>{};

                //we need to get the next id from the cluster
                auto resultantTemplateName = LD::ImmutableString{"local:vztmpl/"}+templateName;
                auto nextvmidURL = this->mURL+LD::ImmutableString{"/api2/json/cluster/nextid"};

                //authorizing the login
                cpr::Response nextVMIDResponse =  cpr::Get(cpr::Url{nextvmidURL.Data()},cpr::Header{{"CSRFPreventionToken",this->mCSRFPreventionToken.Data()}},cpr::Cookies{{"PVEAuthCookie",this->mTicket.Data()}});
                simdjson::dom::parser parser;

                simdjson::simdjson_result<simdjson::dom::element> element =  parser.parse(LD::StringView{nextVMIDResponse.text.c_str()});
                LD::StringView nextVMIDBuffer;
                element["data"].get(nextVMIDBuffer);

                LD::UInteger currentVMID = atoi(nextVMIDBuffer.data());

                LD::ImmutableArray<cpr::Pair,5+sizeof...(Options)> payload;

                //payload[0] =(cpr::Pair{"net0","name=eth0,bridge=vmbr0"});
                payload[0] =(cpr::Pair{"net0",bridge.Value().Data()});
                payload[1] = (cpr::Pair{"vmid",LD::ToImmutableString(currentVMID).Data()});
                payload[2] = (cpr::Pair{"ostemplate",resultantTemplateName.Data()});
                payload[3] = (cpr::Pair{"hostname",host.Data()});
                payload[4] = (cpr::Pair{"cores",LD::ToImmutableString(cores).Data()});

                LD::For<sizeof...(Options)>([](
                        auto I,
                        auto & options,
                        LD::ImmutableArray<cpr::Pair,5+sizeof...(Options)> & payload) noexcept
                                            {

                                                auto object = LD::Get(LD::Get<I>(options));
                                                auto key = object.Key();
                                                auto value = object.Value();
                                                payload[5+I] = (cpr::Pair{key.Data(),value.Data()});
                                                return true;
                                            },options,payload);

                //auto creationURL = this->mURL+LD::ImmutableString{"/api2/json/nodes/"}+node+LD::ImmutableString{"/lxc"};
                auto creationURL = this->mURL+LD::ImmutableString{"/api2/json/nodes/"}+node+LD::ImmutableString{"/"}+ProxyAPI::InstanceTypeToString(LD::Type<InstanceType>{});
                std::cout << "Creation URL: " << creationURL.Data() << std::endl;
                cpr::Response r = cpr::Post(cpr::Url{creationURL.Data()},cpr::Cookies{{"PVEAuthCookie",this->mTicket.Data()}},
                                            cpr::Header{{"CSRFPreventionToken",this->mCSRFPreventionToken.Data()}},
                                            cpr::Payload{payload.Begin(),payload.End()});


                if (r.status_code == 200)
                {
                    simdjson::dom::parser responseParser;
                    simdjson::simdjson_result<simdjson::dom::element> responseBlob =  responseParser.parse(LD::StringView{r.text.c_str()});
                    LD::StringView responseDataBlob;
                    responseBlob["data"].get(responseDataBlob);
                    if (auto [whole, nodeNameInput, hex1Input, hex2Input,hex3Input,creationMethodInput,indexInput,usernameInput,baseInput] = ctre::match<LD::PVE::Detail::CreationRegex>(responseDataBlob); whole)
                    {
                        LD::ImmutableString<25> nodeName;
                        for(LD::UInteger n = 0;n<25 && n < nodeNameInput.view().size();++n)
                        {
                            nodeName[n] = nodeNameInput.view()[n];
                        }
                        LD::Hexademical<12> hex1;
                        LD::Hexademical<12> hex2;
                        LD::Hexademical<12> hex3;
                        LD::UInteger vmID = atol(indexInput.view().data());
                        LD::ImmutableString<25> userName;
                        for(LD::UInteger n = 0;n<25 && n<usernameInput.view().size();++n)
                        {
                            userName[n] = usernameInput.view()[n];
                        }
                        LD::ImmutableString<25> base;
                        for(LD::UInteger n = 0;n<25 && n<baseInput.view().size();++n)
                        {
                            base[n] = baseInput.view()[n];
                        }

                        return LD::CreateResponse(
                            LD::Type<LD::PVE::CreateInstanceResponse>{},
                            LD::PVE::CreateInstanceResponse{nodeName,hex1,hex2,hex3,LD::PVE::VZCreate,vmID,userName,base},
                            LD::Forward<A>(args)...);
                         //return LD::MakeContext(LD::TransactionResult{},LD::PVE::CreateInstanceResponse{nodeName,hex1,hex2,hex3,LD::PVE::VZCreate,vmID,userName,base},LD::Forward<A>(args)...);
                    }
                }
                std::cout << r.status_code << std::endl;
                std::cout << r.reason << std::endl;
                std::cout << r.text << std::endl;
                //return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
                return LD::CreateResponse(LD::Type<LD::PVE::CreateInstanceResponse>{},LD::TransactionError{},LD::Forward<A>(args)...);
            }

            template<typename InstanceType, typename NodeType, typename ... A>
            void FetchInstanceStatus(
                    InstanceType &&,
                    NodeType && node,
                    LD::UInteger identification,
                    A && ... args)
            {
                auto instanceType = LD::ImmutableString{"/"}+ProxyAPI::InstanceTypeToString(LD::Type<InstanceType>{});
                auto statusURL = this->mURL+LD::ImmutableString{"/api2/json/nodes/"}+node+LD::ImmutableString{"/"}+instanceType+LD::ImmutableString{"/"}+LD::ToImmutableString(identification)+LD::ImmutableString{"/"}+LD::ImmutableString{"status/current"};
            }
            template<typename InstanceType,typename NodeType, typename ... A>
            //LD::QueryResult<LD::Variant<LD::PVE::LXC,LD::PVE::VirtualMachine>(A...)> FetchInstanceConfiguration(
            LD::RequestResponse<LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>(A...)> FetchInstanceConfiguration(
                    InstanceType &&,
                    NodeType && node,
                    LD::UInteger identification,
                    A && ... args) const noexcept
            {
                auto instanceType = LD::ImmutableString{"/"}+ProxyAPI::InstanceTypeToString(LD::Type<InstanceType>{});
                auto configURL = this->mURL+LD::ImmutableString{"/api2/json/nodes/"}+node+LD::ImmutableString{"/"}+instanceType+LD::ImmutableString{"/"}+LD::ToImmutableString(identification)+LD::ImmutableString{"/"}+LD::ImmutableString{"config/"};
                cpr::Response getConfigResponse =  cpr::Get(cpr::Url{configURL.Data()},cpr::Header{{"CSRFPreventionToken",this->mCSRFPreventionToken.Data()}},cpr::Cookies{{"PVEAuthCookie",this->mTicket.Data()}});
                simdjson::dom::parser parser;
                simdjson::simdjson_result<simdjson::dom::element> element =  parser.parse(LD::StringView{getConfigResponse.text.c_str()});
                std::cout << getConfigResponse.text << std::endl;
                //{"data":{"lock":"create","digest":"a7e9f8c42935261b36fcd1d7d6cd8cd41648da49"}}
                LD::StringView possiblyLocked;
                auto dataLock = element["data"]["lock"].get(possiblyLocked);

                if (dataLock == simdjson::SUCCESS)
                {
                    return LD::CreateResponse(LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},LD::TransactionError{},LD::Forward<A>(args)...);
                    //return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
                }
                //{"data":{"net0":"name=eth0,bridge=vmbr0,firewall=0,gw=192.168.105.1,gw6=2001:0db8:0a0b:12f0:0000:0000:0000:0001,hwaddr=A2:98:0B:4E:09:6C,ip=192.168.105.28/24,ip6=2600:0000:0000:0000:0000:0000:0000:0001/64,mtu=1500,tag=77,type=veth","digest":"7a2f7da0f9cf6477ee41801d0b3b6a41ea954f64","memory":8192,"features":"force_rw_sys=0,fuse=0,keyctl=0,nesting=1,mknod=0","swap":8192,"rootfs":"local-zfs:subvol-110-disk-0,size=8G","hostname":"cloud.landesshome.com","cores":4,"arch":"amd64","ostype":"debian"}}

                LD::StringView rootFS;
                auto rootfsResult = element["data"]["net0"].get(rootFS);
                //"local-zfs:subvol-110-disk-0,size=8G

                auto rootFSMatchSet = ctre::range<LD::PVE::Detail::SegmentationRegex>(LD::StringView {rootFS.data(),rootFS.size()});



                LD::StringView net0;

                auto net0Result = element["data"]["net0"].get(net0);


                auto net0MatchSet = ctre::range<LD::PVE::Detail::SegmentationRegex>(LD::StringView {net0.data(),net0.size()});

                mpark::variant<LD::Gateway6,LD::PVE::IPV6Auto> gateway6;
                mpark::variant<LD::Gateway,LD::PVE::IPV4DHCP> gateway;
                mpark::variant<LD::IPV6Address,LD::PVE::IPV6Auto> ip6;
                mpark::variant<LD::IPV4Address,LD::PVE::IPV4DHCP> ip;
                LD::MacAddress linkLayer2Address;
                LD::UInteger vmbrOffset = 0;
                for(auto it = net0MatchSet.begin();it!=net0MatchSet.end();++it)
                {
                    //std::cout << (*it).view() << std::endl;
                    auto paramMatchSet = ctre::range<LD::PVE::Detail::ParameterSegmentationRegex>(LD::StringView {(*it).view().data(),(*it).view().size()});
                    auto paramMatchSetBegin = paramMatchSet.begin();

                    auto paramName = (*paramMatchSetBegin++).view();
                    auto paramValue = (*paramMatchSetBegin).view();
                    std::cout << "Param: " << paramName << "    " << "Value: " << paramValue << std::endl;
                    if (paramName == LD::StringView{"name"})
                    {
                        //std::cout << "found name" << std::endl;
                        if (auto [whole, prefix, index] = ctre::match<LD::PVE::Detail::InterfaceNameRegularExpression>(paramValue); whole)
                        {
                            vmbrOffset = atoi(index.view().data());
                            std::cout << "Index: " << index.view() << std::endl;
                        }

                    }else if(paramName == LD::StringView{"bridge"})
                    {
                        if (auto [whole, prefix, index] = ctre::match<LD::PVE::Detail::BridgeNameRegularExpression>(paramValue); whole)
                        {
                            std::cout << "Index: " << index.view() << std::endl;
                        }

                    }else if(paramName == LD::StringView{"hwaddr"})
                    {
                        linkLayer2Address = LD::MacAddress{paramValue};

                    }
                    else if(paramName == LD::StringView{"ip"})
                    {
                        if (paramValue == LD::StringView{"dhcp"})
                        {
                            ip = LD::PVE::IPV4DHCP{};
                        }else
                        {
                            LD::IPV4Address address{paramValue};
                            ip = address;
                        }

                    }else if(paramName == LD::StringView{"ip6"})
                    {
                        if (paramValue == LD::StringView{"auto"})
                        {
                            ip6 = LD::PVE::IPV6Auto{};
                        }else
                        {
                            LD::IPV6Address address{paramValue};
                            ip6 = address;
                        }


                    }else if(paramName == LD::StringView{"mtu"})
                    {
                        LD::UInteger mtu = atol(paramValue.data());

                    }else if(paramName == LD::StringView{"gw"})
                    {
                        gateway = LD::IPV4Address{paramValue};

                    }else if(paramName == LD::StringView{"gw6"})
                    {
                        gateway6 = LD::IPV6Address{paramValue};

                    }else if(paramName == LD::StringView{"firewall"})
                    {
                        LD::UInteger firewallStatus = atol(paramValue.data());
                    }
                }

                std::cout << net0 << std::endl;

                LD::StringView arch;
                auto archResult = element["data"]["arch"].get(arch);
                LD::PVE::Architecture architecture = LD::PVE::amd64;
                if (arch == LD::StringView{"amd64"})
                {
                    architecture = LD::PVE::amd64;

                }else if(arch == LD::StringView{"i386"})
                {
                    architecture = LD::PVE::i386;

                }else if(arch == LD::StringView{"arm64"})
                {
                    architecture = LD::PVE::arm64;

                }else if(arch == LD::StringView{"armhf"})
                {
                    architecture = LD::PVE::armhf;
                }
                LD::PVE::OSType operatingSystemType = LD::PVE::UnManaged;
                LD::StringView  os;
                auto osResult = element["data"]["ostype"].get(os);
                if (os == LD::StringView{"debian"})
                {
                    operatingSystemType = LD::PVE::Debian;

                }else if(os == LD::StringView{"ubuntu"})
                {
                    operatingSystemType = LD::PVE::Ubuntu;

                }else if(os == LD::StringView{"centos"})
                {
                    operatingSystemType = LD::PVE::Centos;

                }else if(os == LD::StringView{"fedora"})
                {
                    operatingSystemType = LD::PVE::Fedora;

                }else if(os == LD::StringView{"opensuse"})
                {
                    operatingSystemType = LD::PVE::OpenSuse;

                }else if(os == LD::StringView{"archlinux"})
                {
                    operatingSystemType = LD::PVE::ArchLinux;

                }else if(os == LD::StringView{"alpine"})
                {
                    operatingSystemType = LD::PVE::Alpine;

                }else if(os == LD::StringView{"gentoo"})
                {
                    operatingSystemType = LD::PVE::Alpine;
                }
                LD::UInteger memoryAmount;

                auto memoryResult = element["data"]["memory"].get(memoryAmount);
                LD::PVE::Memory memory{LD::MegaByte<LD::UInteger>{memoryAmount}};
                LD::UInteger swapAmount;

                auto swapResult = element["data"]["swap"].get(swapAmount);

                LD::PVE::Swap swap{LD::MegaByte<LD::UInteger>{memoryAmount}};
                LD::UInteger numberOfCores;

                auto coreCountResult = element["data"]["cores"].get(numberOfCores);

                LD::StringView hostName;
                auto hostnameResult = element["data"]["hostname"].get(hostName);

                LD::StringView features;
                //"features":"force_rw_sys=0,fuse=0,keyctl=0,nesting=1,mknod=0
                auto featuresResult = element["data"]["features"].get(features);

                LD::UInteger featuresForceRWSysValue = 0;
                element["data"]["features"]["force_rw_sys"].get(featuresForceRWSysValue);

                LD::UInteger featuresFuseSysValue = 0;
                element["data"]["features"]["fuse"].get(featuresFuseSysValue);

                LD::UInteger featuresKeyCTLValue = 0;
                element["data"]["features"]["keyctl"].get(featuresKeyCTLValue);

                LD::UInteger featuresNestingValue = 0;
                element["data"]["features"]["nesting"].get(featuresNestingValue);

                LD::UInteger featuresMkNodeValue = 0;
                element["data"]["features"]["mknod"].get(featuresMkNodeValue);

                LD::PVE::FeatureSet featureSet{
                    LD::PVE::Nesting{bool(featuresNestingValue)},
                    LD::PVE::MkNode{bool(featuresMkNodeValue)},
                    LD::PVE::KeyCTL{bool(featuresKeyCTLValue)},
                    LD::PVE::Fuse{bool(featuresFuseSysValue)},
                    LD::PVE::ForceSWSys{bool(featuresForceRWSysValue)}};


                LD::PVE::Interface primaryInterface{mpark::variant<LD::Gateway,LD::PVE::IPV4DHCP>{gateway},
                                   mpark::variant<LD::Gateway6,LD::PVE::IPV6Auto>{gateway6},
                                   mpark::variant<LD::IPV4Address,LD::PVE::IPV4DHCP>{ip},
                                   mpark::variant<LD::IPV6Address,LD::PVE::IPV6Auto>{ip6},
                                   LD::MacAddress{linkLayer2Address},
                                   vmbrOffset,
                                   LD::MTU{1500},
                                   LD::PVE::FirewallStatus{false}
                };

                /*
                 * LXC(LD::PVE::Interface interface,
                LD::PVE::Architecture arch,
                LD::PVE::OSType osType,
                LD::ImmutableString<25> hostName,
                LD::PVE::RootFS<LD::ImmutableString<25>> rootFS,
                LD::PVE::Swap sap,
                LD::PVE::Memory memory,
                LD::PVE::FeatureSet featureSet,
                LD::UInteger cpuCoreCount)
                 */

                /*
                return LD::MakeContext(
                        LD::TransactionResult{},
                        LD::PVE::LXC
                        {
                            primaryInterface,
                            architecture,
                            operatingSystemType,
                            LD::ImmutableString<25>{},
                            LD::PVE::RootFS<LD::ImmutableString<25>>{},
                            swap,
                            memory,
                            featureSet,
                            numberOfCores

                    },LD::Forward<A>(args)...);
                    */
                   return LD::CreateResponse(
                       LD::CT::TypeList<LD::PVE::LXC,LD::PVE::VirtualMachine>{},
                       LD::PVE::LXC
                        {
                            primaryInterface,
                            architecture,
                            operatingSystemType,
                            LD::ImmutableString<25>{},
                            LD::PVE::RootFS<LD::ImmutableString<25>>{},
                            swap,
                            memory,
                            featureSet,
                            numberOfCores

                        },
                       LD::Forward<A>(args)...);
            }
        };
    }
}
#endif //LANDESSDEVCORE_PROXYAPI_H
