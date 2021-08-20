//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_INTERFACEINDEX_HPP
#define LANDESSDEVCORE_INTERFACEINDEX_HPP
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class InterfaceIndex
        {
        private:
            LD::UInteger mIndex;
        public:
            InterfaceIndex() noexcept:mIndex{0}
            {

            }
            InterfaceIndex(LD::UInteger index) noexcept:mIndex{index}
            {

            }
            LD::UInteger Index() const noexcept
            {
                return this->mIndex;
            }
        };
    }
}
#endif //LANDESSDEVCORE_INTERFACEINDEX_HPP
