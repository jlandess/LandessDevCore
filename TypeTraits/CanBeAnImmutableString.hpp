//
// Created by phoenixflower on 11/12/20.
//

#ifndef LANDESSDEVCORE_CANBEANIMMUTABLESTRING_HPP
#define LANDESSDEVCORE_CANBEANIMMUTABLESTRING_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace CT
    {
        namespace Detail
        {
            template<typename T>
            using Fartz1234 = decltype(LD::ToImmutableString(LD::Declval<T>()));
        }


        template<typename T>
        constexpr bool CanBeAnImmutableString(LD::Type<T> ) noexcept
        {

            //LD::CT::DebugTemplate<Fartz1234<LD::Detail::Decay_T<T>>>{};
            return LD::Exists<LD::CT::Detail::Fartz1234 ,T>;
        }
    }

}
#endif //LANDESSDEVCORE_CANBEANIMMUTABLESTRING_HPP
