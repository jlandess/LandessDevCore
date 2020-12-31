//
// Created by phoenixflower on 5/18/20.
//

#ifndef LANDESSDEVCORE_CONTEXT_H
#define LANDESSDEVCORE_CONTEXT_H
#include "Memory/ElementReference.h"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/Type.h"
namespace LD
{
    namespace Detail
    {
        template<typename T, class = void>
        struct ContextTransform
        {
            using type = T;
        };

        template<typename T>
        struct ContextTransform<T,LD::Enable_If_T<
                LD::Require<
                LD::Detail::IsLValueReference<T>
                >>>
        {
            using type = LD::ElementReference<T>;
        };

    }
    template<typename ... Arguements>
    using Context = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>;

    template<typename ... Arguements,
            typename R = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>>
    constexpr auto MakeContext(Arguements && ... arguements) noexcept -> R
    {
        return R{LD::Forward<Arguements>(arguements)...};
    }

    namespace CT
    {
        template<typename T>
        constexpr bool IsElementReference(LD::Type<T>)  noexcept
        {
            return false;
        }

        template<typename T>
        constexpr bool IsElementReference(LD::Type<LD::ElementReference<T>> ) noexcept
        {
            return true;
        }
    }
}
#endif //LANDESSDEVCORE_CONTEXT_H
