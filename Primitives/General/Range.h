//
// Created by phoenixflower on 6/22/20.
//

#ifndef LANDESSDEVCORE_RANGE_H
#define LANDESSDEVCORE_RANGE_H
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
namespace LD
{
    template<typename T>
    class Range
    {
    private:
        T mMin;
        T mMax;
    public:

        inline constexpr Range(const T & min, const T & max) noexcept :mMin(min),mMax(max){}

        const T & Max() const noexcept {return this->mMax;}
        const T & Min() const noexcept {return this->mMin;}

        T & Max() noexcept {return this->mMax;}
        T & Min() noexcept {return this->mMin;}

        const T Span() const noexcept {return this->mMax-this->mMin;}
    };
}
#endif //LANDESSDEVCORE_RANGE_H
