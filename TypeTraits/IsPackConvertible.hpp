//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_ISPACKCONVERTIBLE_HPP
#define LANDESSDEVCORE_ISPACKCONVERTIBLE_HPP
#include "IntegralConstant.hpp"
#include "VariadicPack.hpp"
namespace LD
{
    template<typename P0, typename P1>
    struct IsPackConvertible;


    template <typename... T0s, typename T1s>
    struct IsPackConvertible<LD::VariadicPack<T0s...>, T1s>
            : LD::Detail::IntegralConstant<bool,
                    (LD::Detail::IsConvertible<T0s, T1s>::value && ...)>
    {
    };
}
#endif //LANDESSDEVCORE_ISPACKCONVERTIBLE_HPP
