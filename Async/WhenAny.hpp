//
// Created by phoenixflower on 9/15/21.
//

#ifndef LANDESSDEVCORE_WHENANY_HPP
#define LANDESSDEVCORE_WHENANY_HPP
#include "SumTypes/MPark/variant.hpp"
#include "WhenOnlyPredicate.hpp"
namespace LD
{
    namespace Async
    {
        template<typename ... A>
        auto WhenAny(A && ... senders) noexcept
        {
            return LD::Async::WhenAnyPredicate<A...>{LD::Tuple<A...>{LD::Forward<A>(senders)...}};
        }
    }
}
#endif //LANDESSDEVCORE_WHENANY_HPP
