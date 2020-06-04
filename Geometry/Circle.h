//
// Created by phoenixflower on 6/2/20.
//

#ifndef LANDESSDEVCORE_CIRCLE_H
#define LANDESSDEVCORE_CIRCLE_H
#include "Primitives/General/Vec2.hpp"
namespace LD
{
    //todo replace assumed Precision type to be primitive type to be of some type of unit (eg meters, miles, feet etc)
    template<typename Precision> class BasicCircle
    {
    private:

        LD::BasicVec2D<Precision> mCenter;
        LD::BasicVec2D<Precision> mRadius;
    public:
        inline constexpr BasicCircle() noexcept ;
        inline constexpr BasicCircle(const LD::BasicVec2D<Precision> & center, const LD::BasicVec2D<Precision> & radius) noexcept ;
        constexpr  Precision GetVolume() const noexcept ;
        constexpr  Precision GetArea() const noexcept ;
    };

    template<typename Precision> constexpr BasicCircle<Precision>::BasicCircle() noexcept :mCenter(0),mRadius(0)
    {

    }
    template<typename Precision>  constexpr BasicCircle<Precision>::BasicCircle(
            const LD::BasicVec2D<Precision> & center,
            const LD::BasicVec2D<Precision> & radius) noexcept :mCenter(center),mRadius(radius)
    {

    }
    template<typename Precision> constexpr  Precision BasicCircle<Precision>::GetVolume() const noexcept
    {
        return 0;
    }
    template<typename Precision> constexpr  Precision BasicCircle<Precision>::GetArea() const noexcept
    {
        return Precision(3.141529265359)*(this->Radius*this->Radius);
    }
}
#endif //LANDESSDEVCORE_CIRCLE_H
