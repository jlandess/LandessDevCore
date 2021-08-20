//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_BRIDGEINDEX_HPP
#define LANDESSDEVCORE_BRIDGEINDEX_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class BridgeIndex
        {
        private:
            LD::UInteger mIndex;
        public:
            BridgeIndex() noexcept:mIndex{0}
            {

            }
            BridgeIndex(LD::UInteger index) noexcept:mIndex{index}
            {

            }
            LD::UInteger Index() const noexcept
            {
                return this->mIndex;
            }
        };
    }
    inline auto ToImmutableString(LD::PVE::BridgeIndex bridgeIndex) noexcept
    {
        return LD::ToImmutableString("bridge=vmbr")+LD::ToImmutableString(bridgeIndex.Index());
    }
}
#endif //LANDESSDEVCORE_BRIDGEINDEX_HPP
