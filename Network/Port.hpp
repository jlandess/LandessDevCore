//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PORT_HPP
#define LANDESSDEVCORE_PORT_HPP
namespace LD
{
    class Port
    {
    private:
        LD::UInteger mPort;
    public:
        constexpr Port() noexcept:mPort{0}
        {

        }
        constexpr Port(LD::UInteger port) noexcept:mPort{port}
        {

        }

        LD::UInteger Value() const noexcept
        {
            return this->mPort;
        }
    };
}
#endif //LANDESSDEVCORE_PORT_HPP
