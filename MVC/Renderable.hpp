//
// Created by James Landess on 2/12/20.
//

#ifndef LANDESSDEVCORE_RENDERABLE_HPP
#define LANDESSDEVCORE_RENDERABLE_HPP

#include "Primitives/General/Vec2.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/Immutable.hpp"
#include "Definitions/Common.hpp"
#include "Primitives/General/StringView.hpp"
namespace LD
{

    class RenderableDimensionEvent{};
    template<typename T, typename Context>
    class Renderer2D{};


    template<typename T>
    class Rendera2DTypeTraits;


    namespace Detail
    {
        template<typename T>
        inline  constexpr LD::Enable_If_T<
                LD::Require<
                        !LD::IsClass<T>>
                ,PDP::Detail::tVec2<LD::UInteger >> GetRenderableDimensions(const T & primitive) noexcept
        {
            auto immutableString = LD::ToImmutableString(primitive);
            return PDP::Detail::tVec2<LD::UInteger >{immutableString.GetSize(),1};
        }


        template<LD::UInteger Size>
        inline constexpr PDP::Detail::tVec2<LD::UInteger> GetRenderableDimensions(const LD::ImmutableString<Size> & object) noexcept
        {
            return PDP::Detail::tVec2<LD::UInteger >{object.GetSize(),1};
        }


        inline constexpr PDP::Detail::tVec2<LD::UInteger> GetRenderableDimensions(const LD::StringView & view) noexcept
        {
            return PDP::Detail::tVec2<LD::UInteger>{view.size(),1};
        }

        template<char ... Characters>
        inline  constexpr PDP::Detail::tVec2<LD::UInteger> GetRenderableDimensions(const LD::TypeString<Characters...> & object) noexcept
        {
            return PDP::Detail::tVec2<LD::UInteger >{sizeof...(Characters),1};
        }


        template<typename T>
        LD::Enable_If_T<
                LD::Require<
                        LD::IsClass<T>,
                        LD::ConvertiblyCallable<T,PDP::Detail::tVec2<LD::Integer >(const RenderableDimensionEvent &)>::Value()
                >
                ,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept
        {
            return object(LD::RenderableDimensionEvent{});
        }

        template<typename T>
        constexpr LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<LD::Rendera2DTypeTraits<LD::Decay_T<T>>,PDP::Detail::tVec2<LD::UInteger>(const LD::RenderableDimensionEvent, const T &)>::Value()
                        >
        ,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept (noexcept(LD::Declval<LD::Rendera2DTypeTraits<LD::Decay_T<T>>>()(LD::Declval<LD::RenderableDimensionEvent>(),LD::Declval<T>())))
        {
            LD::Rendera2DTypeTraits<LD::Decay_T<T>> renderableTraits;


            return renderableTraits(LD::RenderableDimensionEvent{},LD::Forward<T>(object));
        }


    }



    template<typename T>
    auto GetRenderableDimensions(T && object) noexcept(noexcept(LD::Detail::GetRenderableDimensions(LD::Declval<T>()))) -> decltype(LD::Detail::GetRenderableDimensions(LD::Declval<T>()))
    {
        return LD::Detail::GetRenderableDimensions(LD::Forward<T>(object));
    }






    template<typename T>
    using ObjectHasGetRenderableDimensions = decltype(LD::Declval<T>().GetRenderableDimensions());


    template<typename T>
    inline LD::Enable_If_T<LD::Require<LD::Exists<ObjectHasGetRenderableDimensions,LD::Decay_T<T>>>,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept (noexcept(LD::Declval<T>().GetRenderableDimensions()))
    {
        return object.GetRenderableDimensions();
    }
    //template<typename T, typename Context>
    //using Renderable2D = decltype(LD::Declval<T>(LD::Declval<Context>()),LD::Declval<PDP::Detail::tVec2<LD::Integer>>());
    template<typename T, typename Context>
    constexpr bool Renderable2DX = (LD::ConvertiblyCallable<T,Context(Context,const PDP::Detail::tVec2<LD::Integer>)>::Value() ||
    LD::ConvertiblyCallable<LD::Renderer2D<T,Context>,Context(Context,const PDP::Detail::tVec2<LD::Integer>&,const T &)>::Value());
    template<typename T, typename Context>
    constexpr bool IsRenderable2D = (LD::IsImmutableString<T> || LD::IsPrimitive<T> || LD::IsSame<T,LD::StringView>|| (Renderable2DX<T,Context> && true));

}
#endif //LANDESSDEVCORE_RENDERABLE_HPP
