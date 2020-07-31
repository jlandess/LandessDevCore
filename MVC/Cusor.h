//
// Created by phoenixflower on 6/15/20.
//

#ifndef LANDESSDEVCORE_CUSOR_H
#define LANDESSDEVCORE_CUSOR_H
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "TypeTraits/IntegralConstant.hpp"
namespace LD
{
    template<typename PositionType>
    class Cursor
    {
    private:
        LD::BasicVec2D<PositionType> mPosition;
    public:
        inline Cursor() = default;

        inline explicit constexpr Cursor(const LD::BasicVec2D<PositionType> & position) noexcept : mPosition{position}
        {

        }

        inline explicit constexpr Cursor(const PositionType & x, const PositionType & y) noexcept :mPosition{x,y}
        {

        }

        const LD::BasicVec2D<PositionType> & Position() const noexcept
        {
            return this->mPosition;
        }

        LD::BasicVec2D<PositionType> & Position() noexcept
        {
            return this->mPosition;
        }




    };

    namespace Detail
    {
        template<typename T>
        struct IsCursor: public LD::Detail::IntegralConstant<bool,false>
        {

        };

        template<typename T>
        struct IsCursor<LD::Cursor<T>>: public LD::Detail::IntegralConstant<bool,true>
        {


        };
    }

    template<typename T>
    constexpr bool IsCursor = LD::Detail::IsCursor<T>::value;
}
#endif //LANDESSDEVCORE_CUSOR_H
