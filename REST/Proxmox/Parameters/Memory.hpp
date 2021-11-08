//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PXE_MEMORY_HPP
#define LANDESSDEVCORE_PXE_MEMORY_HPP
#include "Core/Unit.hpp"
namespace LD
{
    namespace PVE
    {
        class Memory
        {
        private:
            LD::MegaByte<LD::UInteger> mByte;
        public:
            Memory() noexcept:mByte{LD::MegaByte<LD::UInteger>{512ul}}
            {

            }
            Memory(LD::GigaByte<LD::UInteger> size) noexcept:mByte{size}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"memory"};
            }
            auto Value() const noexcept
            {
                return LD::ToImmutableString(this->mByte.NativeRepresentation().Value());
            }
        };
    }
}
#endif //LANDESSDEVCORE_MEMORY_HPP
