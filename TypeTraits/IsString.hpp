//
// Created by phoenixflower on 12/30/20.
//

#ifndef LANDESSDEVCORE_ISSTRING_HPP
#define LANDESSDEVCORE_ISSTRING_HPP
#include "Type.h"
namespace LD
{
    namespace CT
    {
        template<typename T>
        constexpr bool IsString(LD::Type<T> )  noexcept
        {
            return false;
        }
    }
}
#endif //LANDESSDEVCORE_ISSTRING_HPP
