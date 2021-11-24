//
// Created by phoenixflower on 11/17/21.
//

#ifndef LANDESSDEVCORE_VIEW_HPP
#define LANDESSDEVCORE_VIEW_HPP
#include "Primitives/General/StringView.hpp"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/IsConstructible.hpp"
#include "Forward.hpp"
namespace LD
{
    template<typename T>
    LD::Enable_If_T<LD::Require<LD::IsConstructible<LD::StringView,T>::value>,LD::StringView> ViewAsStringView(T && object) noexcept
    {
        return LD::StringView{LD::Forward<T>(object)};
    }
}
#endif //LANDESSDEVCORE_VIEW_HPP
