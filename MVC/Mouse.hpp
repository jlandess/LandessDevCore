//
// Created by phoenixflower on 6/15/20.
//

#ifndef LANDESSDEVCORE_MOUSE_HPP
#define LANDESSDEVCORE_MOUSE_HPP
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "TypeTraits/IntegralConstant.hpp"
namespace LD
{
    template<typename PositionType, LD::UInteger NumberOfInputs>
    class BasicMouse
    {
    private:
        LD::BasicVec2D<PositionType> mPosition;
        LD::StaticArray<LD::UInteger,NumberOfInputs> mInputs;
    public:
        inline BasicMouse() = default;

        inline explicit constexpr BasicMouse(const LD::BasicVec2D<PositionType> & position) noexcept : mPosition{position},mInputs{false}
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

        const LD::StaticArray<LD::UInteger,NumberOfInputs> & Inputs() const noexcept
        {
            return this->mInputs;
        }

        LD::StaticArray<LD::UInteger,NumberOfInputs> & Inputs() noexcept
        {
            return this->mInputs;
        }

    };

    namespace Detail
    {
        template<typename T>
        struct IsBasicMouse: public LD::Detail::IntegralConstant<bool,false>
        {

        };

        template<typename T, LD::UInteger Size>
        struct IsBasicMouse<LD::BasicMouse<T,Size>>: public LD::Detail::IntegralConstant<bool,true>
        {


        };
    }

    template<typename T>
    constexpr bool IsBasicMouse = LD::Detail::IsBasicMouse<T>::value;
}
#endif //LANDESSDEVCORE_MOUSE_HPP
