//
// Created by phoenixflower on 8/1/21.
//
#include <iostream>
#include <map>
#include <memory>
#include <webdav/client.hpp>
#include "IaaS/HashiVault/HashiVault.hpp"
#include "Reflection/Reflection.hpp"

namespace LD
{
    namespace Example
    {
        class WebDavKey
        {
        private:
            LD::ImmutableString<128> mHostName;
            LD::ImmutableString<30> mUserName;
            LD::ImmutableString<30> mPassword;
        public:

            const LD::ImmutableString<128> & HostName() const noexcept {return this->mHostName;}
            LD::ImmutableString<128> & HostName()  noexcept {return this->mHostName;}

            const LD::ImmutableString<30> & UserName() const noexcept {return this->mUserName;}
            LD::ImmutableString<30> & UserName()  noexcept {return this->mUserName;}

            const LD::ImmutableString<30> & Password() const noexcept {return this->mPassword;}
            LD::ImmutableString<30> & Password()  noexcept {return this->mPassword;}
        };
    }
}

template<>
struct LD::CT::TypeDescriptor<LD::Example::WebDavKey>
{
private:
    static constexpr auto HostNameLabel = LD::ImmutableString{"HostName"};
    static constexpr auto UserNameLabel = LD::ImmutableString("UserName");
    static constexpr auto PassWordLabel = LD::ImmutableString("Password");
public:
    static constexpr auto ClassName = LD::ImmutableString{"PBXKey"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<HostNameLabel,LD::CT::SelectOverload<LD::ImmutableString<128> & (LD::Example::WebDavKey::*)(),&LD::Example::WebDavKey::HostName>(),LD::CT::SelectOverload<const LD::ImmutableString<128> & (LD::Example::WebDavKey::*)() const,&LD::Example::WebDavKey::HostName>()>,
            LD::CT::EncapsulatedMemberDescriptor<UserNameLabel,LD::CT::SelectOverload<LD::ImmutableString<30> & (LD::Example::WebDavKey::*)(),&LD::Example::WebDavKey::UserName>(),LD::CT::SelectOverload<const LD::ImmutableString<30> & (LD::Example::WebDavKey::*)() const,&LD::Example::WebDavKey::UserName>()>,
            LD::CT::EncapsulatedMemberDescriptor<PassWordLabel,LD::CT::SelectOverload<LD::ImmutableString<30> & (LD::Example::WebDavKey::*)(),&LD::Example::WebDavKey::Password>(),LD::CT::SelectOverload<const LD::ImmutableString<30> & (LD::Example::WebDavKey::*)() const,&LD::Example::WebDavKey::Password>()>
    >;


    static constexpr MemberList Members{  };

};
namespace LD
{
    namespace Example
    {

        void WebDavExample()
        {

            LD::HashiVault vault{
                    LD::ImmutableString{"http://192.168.30.50:8200"},
                    LD::ImmutableString{"s.w8hIFCNwl59CMABCvaZ1qlGV"}};

            auto vaultResponse = vault.Query(
                    LD::Type<LD::Example::WebDavKey>{},
                    LD::ImmutableString{"demoapplications"},
                    LD::ImmutableString{"landessdevwebdavexample"});


            auto onResponse = [](const LD::Example::WebDavKey & key) noexcept
            {
                std::cout << "being called on resposne" << std::endl;
                std::map<std::string, std::string> options =
                        {
                                {"webdav_hostname", key.HostName().Data()},
                                {"webdav_username", key.UserName().Data()},
                                {"webdav_password", key.Password().Data()}
                        };
                std::unique_ptr<WebDAV::Client> client = std::make_unique<WebDAV::Client>(options);


                auto listing = client->list();

                for(const auto & currentListing: listing)
                {
                    std::cout << currentListing << std::endl;
                }
            };

            auto onError = [](const LD::TransactionError & error) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onResponse,onError},vaultResponse);

        }
    }
}