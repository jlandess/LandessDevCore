//
// Created by phoenixflower on 1/28/21.
//

#ifndef LANDESSDEVCORE_ISNETWORKBRIDGE_H
#define LANDESSDEVCORE_ISNETWORKBRIDGE_H

#include "TypeTraits/Type.h"
#include "NetworkBridgeForwardDeclaration.hpp"

namespace LD
{
    namespace PVE
    {
        template<typename T>
        inline constexpr bool IsNetworkBridge(LD::Type<T> ) noexcept
        {
            return false;
        }

        template<typename ... Options>
        inline constexpr bool IsNetworkBridge(LD::PVE::NetworkBridge<LD::CT::TypeList<Options...>> ) noexcept
        {
            return true;
        }
    }
}
#endif //LANDESSDEVCORE_ISNETWORKBRIDGE_H
