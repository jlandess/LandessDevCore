//
// Created by phoenixflower on 9/6/21.
//

#ifndef LANDESSDEVCORE_DUMMYSHAREDLOCK_HPP
#define LANDESSDEVCORE_DUMMYSHAREDLOCK_HPP
namespace LD
{
    template<typename T>
    class DummySharedLock
    {
    public:
        DummySharedLock() noexcept{}

        void Lock() noexcept{}
        void UnLock() noexcept{}
    };
}
#endif //LANDESSDEVCORE_DUMMYSHAREDLOCK_HPP
