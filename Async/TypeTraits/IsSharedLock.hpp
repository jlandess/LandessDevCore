//
// Created by phoenixflower on 9/6/21.
//

#ifndef LANDESSDEVCORE_ISSHAREDLOCK_HPP
#define LANDESSDEVCORE_ISSHAREDLOCK_HPP
namespace LD
{
    namespace CT
    {
        template<typename T>
        constexpr bool IsSharedLock(LD::Type<T> ) noexcept
        {
            return false;
        }
    }
}
#endif //LANDESSDEVCORE_ISSHAREDLOCK_HPP
