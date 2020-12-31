//
// Created by phoenixflower on 12/30/20.
//

#ifndef LANDESSDEVCORE_ISEXTERNALOBJECTSYMBOL_HPP
#define LANDESSDEVCORE_ISEXTERNALOBJECTSYMBOL_HPP
#include "Type.h"
namespace LD
{
    template<typename T>
    constexpr bool IsExternalObjectSymbol(LD::Type<T> ) noexcept
    {
        return false;
    }
}
#endif //LANDESSDEVCORE_ISEXTERNALOBJECTSYMBOL_HPP
