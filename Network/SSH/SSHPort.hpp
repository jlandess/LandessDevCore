//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHPORT_HPP
#define LANDESSDEVCORE_SSHPORT_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    template<typename T>
    class SSHPort
    {
    private:
        LD::UInteger mPort;
    public:
        SSHPort() noexcept:mPort{22}
        {

        }
        SSHPort(LD::UInteger port) noexcept: mPort{port}
        {

        }
        LD::UInteger Port() const noexcept
        {
            return this->mPort;
        }
    };
}
#endif //LANDESSDEVCORE_SSHPORT_HPP
