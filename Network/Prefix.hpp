//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PREFIX_HPP
#define LANDESSDEVCORE_PREFIX_HPP
#include "Definitions/Integer.hpp"

namespace LD
{
    class Prefix
    {
    private:
        unsigned short mPort;
    public:
        constexpr Prefix() noexcept:mPort{0}{}
        constexpr Prefix(unsigned short port) noexcept:mPort{port}
        {

        }
        unsigned short Value() const noexcept
        {
            return this->mPort;
        }

    };
}
#endif //LANDESSDEVCORE_PREFIX_HPP
