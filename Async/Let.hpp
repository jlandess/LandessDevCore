//
// Created by phoenixflower on 9/18/21.
//

#ifndef LANDESSDEVCORE_LET_HPP
#define LANDESSDEVCORE_LET_HPP
#include "Then.hpp"
namespace LD
{
    namespace Async
    {
        template<typename Context,typename T, typename Task>
        auto LetValue(Context context,T  value, Task task) noexcept
        {
            return LD::Async::Then(LD::Async::Then(context,LD::Async::Just(value)),task);
        }
    }
}
#endif //LANDESSDEVCORE_LET_HPP
