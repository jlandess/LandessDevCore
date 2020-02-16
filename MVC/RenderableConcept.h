//
// Created by James Landess on 2/12/20.
//

#ifndef LANDESSDEVCORE_RENDERABLECONCEPT_H
#define LANDESSDEVCORE_RENDERABLECONCEPT_H

#include "Definitions/Common.h"
#include "Primitives/General/Immutable.h"
#include "Primitives/General/Vec2.h"
namespace LD
{
    template<typename T>
    inline  constexpr LD::Enable_If_T<
            LD::Require<
                    !LD::IsClass<T>>
            ,PDP::Detail::tVec2<LD::UInteger >> GetRenderableDimensions(const T & object) noexcept
    {
        auto immutableString = LD::ToImmutableString(object);
        return PDP::Detail::tVec2<LD::UInteger >{1,1};
    }


    template<LD::UInteger Size>
    inline constexpr PDP::Detail::tVec2<LD::UInteger> GetRenderableDimensions(const LD::ImmutableString<Size> & object) noexcept
    {
        return PDP::Detail::tVec2<LD::UInteger >{object.GetSize(),1};
    }


    template<char ... Characters>
    inline  constexpr PDP::Detail::tVec2<LD::UInteger> GetRenderableDimensions(const LD::TypeString<Characters...> & object) noexcept
    {
        return PDP::Detail::tVec2<LD::UInteger >{sizeof...(Characters),1};
    }

    template<typename T>
    using ObjectHasGetRenderableDimensions = decltype(LD::Declval<T>().GetRenderableDimensions());


    template<typename T>
    inline LD::Enable_If_T<LD::Require<LD::Exists<ObjectHasGetRenderableDimensions,LD::Decay_T<T>>>,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept (noexcept(LD::Declval<T>().GetRenderableDimensions()))
    {
        return object.GetRenderableDimensions();
    }

}
#endif //LANDESSDEVCORE_RENDERABLECONCEPT_H
