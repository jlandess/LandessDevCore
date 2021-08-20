//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_ABS_HPP
#define LANDESSDEVCORE_ABS_HPP
namespace LD
{
    template<typename T>
    inline constexpr T Abs(const T & integer) noexcept
    {
        //return (integer<0)?integer*-1:integer;

        return (integer < 0)*(integer*-1) + (integer>=0)*integer;
    }
}
#endif //LANDESSDEVCORE_ABS_HPP
