//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_FLOOR_HPP
#define LANDESSDEVCORE_FLOOR_HPP
#include "Definitions/Float.hpp"
#include "Definitions/Integer.hpp"
namespace LD
{
    inline constexpr LD::Integer Floor(LD::Float x) noexcept
    {
        if(x>0)return (LD::Integer)x;
        return (LD::Integer)(x-((LD::Float)0.9999999999999999));
    }
}
#endif //LANDESSDEVCORE_FLOOR_HPP
