//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHHOST_HPP
#define LANDESSDEVCORE_SSHHOST_HPP
#include "Primitives/General/Immutable.hpp"
#include "Definitions/Integer.hpp"
#include "Network/IPV6Address.hpp"
namespace LD
{
    class SSHHost
    {
    private:
        LD::IPV6Address mAddress;
    public:
        SSHHost() noexcept
        {

        }
        SSHHost(LD::IPV6Address address) noexcept:mAddress{address}
        {

        }
        LD::StringView Value() const noexcept
        {
            return LD::StringView {""};
        }
        LD::IPV6Address Address() const noexcept
        {
            return this->mAddress;
        }
    };

    //template<LD::UInteger Size> SSHHost(LD::ImmutableString<Size> ) -> SSHHost<LD::ImmutableString<Size>>;
    //template<> SSHHost(LD::IPV6Address ) -> SSHHost<LD::ImmutableString<128>>;
}
#endif //LANDESSDEVCORE_SSHHOST_HPP
