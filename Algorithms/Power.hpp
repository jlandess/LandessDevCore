//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_POWER_HPP
#define LANDESSDEVCORE_POWER_HPP
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
namespace LD
{
    inline constexpr LD::UInteger Power(const LD::UInteger & x, const LD::UInteger & y) noexcept
    {
        LD::UInteger temp = 0;
        if( y == 0)
            return 1;
        temp = LD::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
            return x*temp*temp;
    }
    inline constexpr LD::Float Power(const LD::Float & x, const LD::Integer & y) noexcept
    {
        LD::Float temp = 0;
        if( y == 0)
            return 1;
        temp = LD::Power(x, y/2);
        if (y%2 == 0)
            return temp*temp;
        else
        {
            if(y > 0)
                return x*temp*temp;
            else
                return (temp*temp)/x;
        }
    }
}
#endif //LANDESSDEVCORE_POWER_HPP
