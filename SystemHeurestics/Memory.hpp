//
// Created by phoenixflower on 1/23/21.
//

#ifndef LANDESSDEVCORE_MEMORY_HPP
#define LANDESSDEVCORE_MEMORY_HPP
#include "Primitives/General/Unit.hpp"

namespace LD
{
    class Memory
    {
    private:
        LD::MegaByte<LD::UInteger> mBytes;
    public:
        Memory(const LD::MegaByte<LD::UInteger> & bytes) noexcept:mBytes{bytes}
        {

        }

        const LD::MegaByte<LD::UInteger> Amount() const noexcept
        {
            return this->mBytes;
        }
    };
}
#endif //LANDESSDEVCORE_MEMORY_HPP
