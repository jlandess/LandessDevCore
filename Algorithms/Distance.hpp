//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_DISTANCE_HPP
#define LANDESSDEVCORE_DISTANCE_HPP
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/Vec3.hpp"
#include "Primitives/General/Vec4.hpp"
#include "Exponential.hpp"
namespace LD
{

    template<typename T>
    static const LD::Float Distance(const LD::BasicVec4D<T> & first, const LD::BasicVec4D<T> & second)
    {
        LD::Float result = {};
        const T deltaX = LD::Abs(second.x - first.x);
        const T deltaY = LD::Abs(second.y - first.y);
        const T deltaZ = LD::Abs(second.z - first.z);
        const T deltaW = LD::Abs(second.w - first.w);

        const T smallestDelta = LD::Min(deltaX, deltaY, deltaZ, deltaW);

        result += smallestDelta * 1.4142135623730950488016887242097;
        result += (deltaX - smallestDelta) + (deltaY - smallestDelta) + (deltaZ - smallestDelta) + (deltaW - smallestDelta);
        return result;
    }
    template<typename T>
    static const LD::Float Distance(const LD::BasicVec3D<T> & first, const LD::BasicVec3D<T> & second) noexcept
    {
        LD::Float result = T{};

        const T deltaX = LD::Abs(second.x - first.x);
        const T deltaY = LD::Abs(second.y - first.y);
        const T deltaZ = LD::Abs(second.z - first.z);

        const T smallestDelta = LD::Min(deltaX, deltaY, deltaZ);

        result += smallestDelta * 1.4142135623730950488016887242097;
        result += (deltaX - smallestDelta) + (deltaY - smallestDelta) + (deltaZ - smallestDelta);

        return result;
    }

    template<typename T>
    static const LD::Float Distance(const LD::BasicVec2D<T> & first, const LD::BasicVec2D<T> & second) noexcept
    {
        LD::Float result = {};

        const T deltaX = LD::Abs(second.x - first.x);
        const T deltaY = LD::Abs(second.y - first.y);

        const T smallestDelta = LD::Min(deltaX, deltaY);

        result += smallestDelta * 1.4142135623730950488016887242097;
        result += (deltaX - smallestDelta) + (deltaY - smallestDelta);

        return result;
    }

    template<typename T>
    static LD::BasicVec3D<T> CrossProduct(const LD::BasicVec3D<T> & first, const LD::BasicVec3D<T> & second) noexcept
    {
        LD::BasicVec3D<T> result;
        result.X() = (first.Y() * second.Z()) - (second.Y()*first.Z());
        result.Y() = -1 * ((first.X()*second.Z()) - (second.X() * first.Z()));
        result.Z() = (first.X()*second.Y()) - (second.X()*first.Y());
        return result;
    }


    template<typename T>
    static const LD::Float Magnitude(const LD::BasicVec4D<T> & vec) noexcept
    {
        return Distance({ 0,0,0,0 }, vec);
    }
    template<typename T>
    static const LD::Float Magnitude(const LD::BasicVec3D<T> & vec) noexcept
    {
        return Distance({ 0,0,0 }, vec);
    }

    template<typename T>
    static const LD::Float Magnitude(const LD::BasicVec2D<T> & vec) noexcept
    {
        return Distance({ 0,0 }, vec);
    }

    template<typename T>
    static LD::BasicVec4D<T> Normalize(const LD::BasicVec4D<T> & first) noexcept
    {
        LD::BasicVec4D<T> result;
        LD::Float magnitude = Distance({ 0,0,0,0 }, first);
        result.X() = first.X() / magnitude;
        result.Y() = first.Y() / magnitude;
        result.Z() = first.Z() / magnitude;
        result.W() = first.W() / magnitude;
        return result;
    }

    template<typename T>
    static LD::BasicVec4D<T> Normalize(const LD::BasicVec3D<T> & first) noexcept
    {
        LD::BasicVec3D<T> result;
        LD::Float magnitude = Distance({ 0,0,0}, first);
        result.X() = first.X() / magnitude;
        result.Y() = first.Y() / magnitude;
        result.Z() = first.Z() / magnitude;
        return result;
    }

    template<typename T>
    static LD::BasicVec4D<T> Normalize(const LD::BasicVec2D<T> & first) noexcept
    {
        LD::BasicVec2D<T> result;
        LD::Float magnitude = Distance({ 0,0}, first);
        result.X() = first.X() / magnitude;
        result.Y() = first.Y() / magnitude;
        return result;
    }

    template<typename T>
    static LD::Float DotProduct(const LD::BasicVec4D<T> & first, const LD::BasicVec4D<T> & second) noexcept
    {
        return (first.X() * second.X()) + (first.Y() * second.Y()) + (first.Z() * second.Z() + (first.W() * second.W()));
    }
    template<typename T>
    static LD::Float DotProduct(const LD::BasicVec3D<T> & first, const LD::BasicVec3D<T> & second) noexcept
    {
        return (first.X() * second.X()) + (first.Y() * second.Y()) + (first.Z() * second.Z());
    }

    template<typename T>
    static LD::Float DotProduct(const LD::BasicVec2D<T> & first, const LD::BasicVec3D<T> & second) noexcept
    {
        return (first.X() * second.X()) + (first.Y() * second.Y());
    }

}
#endif //LANDESSDEVCORE_DISTANCE_HPP
