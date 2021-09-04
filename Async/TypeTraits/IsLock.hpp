//
// Created by phoenixflower on 9/4/21.
//

#ifndef LANDESSDEVCORE_ISLOCK_HPP
#define LANDESSDEVCORE_ISLOCK_HPP
#include "TypeTraits/Type.h"

namespace LD
{
    namespace CT
    {

        template<typename T>
        constexpr bool IsLock(LD::Type<T> ) noexcept
        {
            return false;
        }

    }
}
#endif //LANDESSDEVCORE_ISLOCK_HPP
