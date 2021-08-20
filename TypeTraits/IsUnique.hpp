//
// Created by phoenixflower on 2/1/21.
//

#ifndef LANDESSDEVCORE_ISUNIQUE_HPP
#define LANDESSDEVCORE_ISUNIQUE_HPP
#include "IntegralConstant.hpp"
#include "IsSame.hpp"
namespace LD
{
    template <typename...>
    inline constexpr auto IsUnique = LD::TrueType{};

    template <typename T, typename... Rest>
    inline constexpr auto IsUnique<T, Rest...> = LD::Detail::IntegralConstant<bool,(!LD::Detail::IsSame<T, Rest>::value && ...) && IsUnique<Rest...>
    >{};

    namespace CT
    {
        template<typename ... T>
        constexpr bool IsUnique = LD::IsUnique<T...>;
    }
}
#endif //LANDESSDEVCORE_ISUNIQUE_HPP
