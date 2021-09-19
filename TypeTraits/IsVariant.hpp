//
// Created by phoenixflower on 3/6/21.
//

#ifndef LANDESSDEVCORE_ISVARIANT_HPP
#define LANDESSDEVCORE_ISVARIANT_HPP
#include "Type.h"
#include "SumTypes/MPark/variant.hpp"
namespace LD
{
    namespace CT
    {
        template<typename T>
        constexpr bool IsVariant(LD::Type<T> ) noexcept
        {
            return false;
        }
        template<typename ... T>
        constexpr bool IsVariant(LD::Type<LD::Variant<T...>> ) noexcept
        {
            return true;
        }

        template<typename ... T>
        constexpr bool IsVariant(LD::Type<mpark::variant<T...>> ) noexcept
        {
            return true;
        }
    }
}
#endif //LANDESSDEVCORE_ISVARIANT_HPP
