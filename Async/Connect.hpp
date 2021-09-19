//
// Created by phoenixflower on 9/11/21.
//

#ifndef LANDESSDEVCORE_CONNECT_HPP
#define LANDESSDEVCORE_CONNECT_HPP
#include "Then.hpp"
namespace LD
{
    namespace Async
    {
        template<typename S, typename R>
        auto Connect(S && sender, R && receiver) noexcept
        {
            return LD::Async::Then(LD::Forward<S>(sender),LD::Forward<R>(receiver));
        }
    }
}
#endif //LANDESSDEVCORE_CONNECT_HPP
