//
// Created by phoenixflower on 1/26/21.
//

#ifndef LANDESSDEVCORE_MTU_HPP
#define LANDESSDEVCORE_MTU_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    class MTU
    {
    private:
        LD::UInteger mAmount;
    public:
        MTU() noexcept:mAmount{0}
        {

        }
        MTU(LD::UInteger amount) noexcept:mAmount{amount}
        {

        }
    };
}
#endif //LANDESSDEVCORE_MTU_HPP
