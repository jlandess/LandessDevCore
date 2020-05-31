//
// Created by phoenixflower on 5/18/20.
//

#ifndef LANDESSDEVCORE_CONTEXT_H
#define LANDESSDEVCORE_CONTEXT_H
#include "Memory/ElementReference.h"
namespace LD
{
    template<typename ... Arguements>
    using Context = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>;

    template<typename ... Arguements,
            typename R = LD::Tuple<LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<Arguements>::value),LD::ElementReference <LD::Detail::Decay_T<Arguements>>,LD::Detail::Decay_T<Arguements>>...>>
    constexpr auto MakeContext(Arguements && ... arguements) noexcept -> R
    {
        return R{LD::Forward<Arguements>(arguements)...};
    }
}
#endif //LANDESSDEVCORE_CONTEXT_H
