//
// Created by phoenixflower on 7/19/20.
//

#ifndef LANDESSDEVCORE_SCOPEDSTATE_HPP
#define LANDESSDEVCORE_SCOPEDSTATE_HPP
#include "Reflection/Reflection.hpp"

namespace LD
{
    template<typename Reflectable, class = void>
    class ScopedState;


    template<typename Reflectable>
    class ScopedState<Reflectable,LD::Enable_If_T<
            LD::Require<
            LD::CT::IsReflectable<Reflectable>()
            >>>
    {

    };
}
#endif //LANDESSDEVCORE_SCOPEDSTATE_HPP
