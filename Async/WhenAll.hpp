//
// Created by phoenixflower on 9/14/21.
//

#ifndef LANDESSDEVCORE_WHENALL_HPP
#define LANDESSDEVCORE_WHENALL_HPP
#include "Primitives/General/Tuple.hpp"
#include "Algorithms/Forward.hpp"
#include "WhenPredicate.hpp"
namespace LD
{
    namespace Async
    {

        /*
        template<typename ... A>
        auto WhenAll(A && ... senders)
        {
            return LD::Tuple<A...>{LD::Forward<A>(senders)...};
        }
         */


        template<typename ... A>
        auto WhenAll(A && ... senders) noexcept
        {
            return LD::Async::WhenAllPredicate<A...>{LD::Tuple<A...>{LD::Forward<A>(senders)...}};
        }
    }
}
#endif //LANDESSDEVCORE_WHENALL_HPP
