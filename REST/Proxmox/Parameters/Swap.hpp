//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PXE_SWAP_HPP
#define LANDESSDEVCORE_PXE_SWAP_HPP
#include "SystemHeurestics/DiskSize.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        class Swap
        {
        private:
            LD::DiskSize<LD::MegaByte<LD::UInteger>> mSize;
        public:
            Swap() noexcept: mSize{LD::MegaByte<LD::UInteger>{512ul}}
            {

            }
            Swap(LD::MegaByte<LD::UInteger> size) noexcept:mSize{size}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"swap"};
            }
            auto Value() const noexcept
            {
                return LD::ToImmutableString(LD::UInteger(this->mSize.Size().NativeRepresentation().Value()));
            }
        };
    }
}
#endif //LANDESSDEVCORE_SWAP_HPP
