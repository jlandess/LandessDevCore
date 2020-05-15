//
// Created by James Landess on 2/17/20.
//

#ifndef LANDESSDEVCORE_LINEARALGEBRARENDERABLE_HPP
#define LANDESSDEVCORE_LINEARALGEBRARENDERABLE_HPP

#include "Renderable.hpp"
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/Vec3.hpp"
#include "Primitives/General/Vec4.hpp"
#include "Primitives/General/Mat2x2.hpp"
#include "Primitives/General/Mat2x3.hpp"
#include "Primitives/General/Mat2x4.hpp"
#include "Primitives/General/Mat3x2.hpp"
#include "Primitives/General/Mat3x3.hpp"
#include "Primitives/General/Mat3x4.hpp"
#include "Primitives/General/Mat4x2.hpp"
#include "Primitives/General/Mat4x3.hpp"
#include "Primitives/General/Mat4x4.hpp"
#include "TermBoxRenderingContext.hpp"
namespace LD
{

    template<typename T>
    class Renderer2D<PDP::Detail::tVec2<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        const LD::TermBoxRenderContext& operator()(const LD::TermBoxRenderContext& context, const PDP::Detail::tVec2<LD::Integer> & translation, const PDP::Detail::tVec2<T> & object)
        {
            auto leftBrace = LD::ToImmutableString('{');
            auto comma = LD::ToImmutableString(',');
            auto rightBrace = LD::ToImmutableString('}');
            auto firstComponent = LD::ToImmutableString(object.X());
            auto secondComponent = LD::ToImmutableString(object.Y());
            auto phrasing = leftBrace+firstComponent+comma+secondComponent+comma+rightBrace;

            return context.Render(phrasing,translation);
        }


        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{2,1};
        }
    };

    template<typename T>
    class Renderer2D<PDP::Detail::tVec3<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{3,1};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tVec4<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{4,1};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat2x2<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{2,2};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat2x3<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{2,3};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat2x4<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{2,4};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat3x2<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{3,2};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat3x3<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{3,3};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat3x4<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{3,4};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat4x2<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{4,2};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat4x3<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{4,3};
        }

    };

    template<typename T>
    class Renderer2D<PDP::Detail::tMat4x4<T>,const LD::TermBoxRenderContext&>
    {
    private:

    public:

        PDP::Detail::tVec2<LD::UInteger> operator()(const LD::RenderableDimensionEvent &) noexcept
        {

            return PDP::Detail::tVec2<LD::UInteger>{4,4};
        }

    };
}
#endif //LANDESSDEVCORE_LINEARALGEBRARENDERABLE_HPP
