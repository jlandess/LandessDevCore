//
// Created by phoenixflower on 9/9/21.
//

#ifndef LANDESSDEVCORE_ISSUBSCRIPTIONTOKEN_HPP
#define LANDESSDEVCORE_ISSUBSCRIPTIONTOKEN_H
#include "TypeTraits/Type.h"
namespace LD
{
    namespace CT
    {
        template<typename T>
        constexpr bool IsSubsriptionToken(LD::Type<T> ) noexcept
        {
            return false;
        }
    }
}
#endif //LANDESSDEVCORE_ISSUBSCRIPTIONTOKEN_HPP
