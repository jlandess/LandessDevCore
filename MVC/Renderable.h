//
// Created by James Landess on 2/12/20.
//

#ifndef LANDESSDEVCORE_RENDERABLE_H
#define LANDESSDEVCORE_RENDERABLE_H

#include "Primitives/General/Vec2.h"
#include "Memory/ElementReference.h"
#include "Primitives/General/Immutable.h"
#include "Definitions/Common.h"
namespace LD
{
    template<typename T, typename Context, class sfinae = void>
    class Renderer2D;
    template<typename T, typename Context>
    class Renderer2D<T,Context,LD::Enable_If_T<
            LD::Require<
                    true
                    >>>
    {
    private:
    public:
        Context Render(Context context, const PDP::Detail::tVec2<LD::Integer> & translation, const T & object)
        {
            return context;
        }

        const LD::ElementReference<LD::Decay_T<Context>> & Render(const LD::ElementReference<LD::Decay_T<Context>> & context, const PDP::Detail::tVec2<LD::Integer> & translation, const T & object)
        {

            return context;
        }

    };


    template<typename T, typename Context>
    using Renderable2D = decltype(LD::Declval<T>(LD::Declval<Context>()),LD::Declval<PDP::Detail::tVec2<LD::Integer>>());


    template<typename T, typename Context>
    constexpr bool IsRenderable2D = (LD::IsImmutableString<T> || LD::IsPrimitive<T> || (LD::Exists<LD::Renderable2D ,T,Context> && true));

}
#endif //LANDESSDEVCORE_RENDERABLE_H
