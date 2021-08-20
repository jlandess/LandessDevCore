//
// Created by phoenixflower on 3/19/21.
//

#ifndef LANDESSDEVCORE_ISNAN_HPP
#define LANDESSDEVCORE_ISNAN_HPP
namespace LD
{
    inline bool IsNan(const float & f)
    {
        const unsigned int u = *(unsigned int*)&f;
        return (u&0x7F800000) == 0x7F800000 && (u&0x7FFFFF);    // Both NaN and qNan.
    }

    inline bool IsNan(const double & d)
    {
        const unsigned long u = *(unsigned int*)&d;
        return (u&0x7FF0000000000000ULL) == 0x7FF0000000000000ULL && (u&0xFFFFFFFFFFFFFULL);
    }
}
#endif //LANDESSDEVCORE_ISNAN_HPP
