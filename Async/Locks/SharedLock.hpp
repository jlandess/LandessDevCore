//
// Created by phoenixflower on 9/4/21.
//

#ifndef LANDESSDEVCORE_SHAREDLOCK_HPP
#define LANDESSDEVCORE_SHAREDLOCK_HPP
#include "Memory/shared_ptr.hpp"
#include "Async/TypeTraits/IsLock.hpp"
#include "Async/TypeTraits/IsSharedLock.hpp"
namespace LD
{
    template<typename T, class = void>
    class SharedLock;
    template<typename T>
    class SharedLock<T,LD::Enable_If_T<LD::Require<LD::CT::IsLock(LD::Type<T>{})>>>
    {
    private:
        LD::SharedPointer<T> mSharedLock;
    public:
        SharedLock() noexcept:mSharedLock{LD::MakeShared<T>()}{}

        void Lock() noexcept{this->mSharedLock->Lock();}
        void UnLock() noexcept{return this->mSharedLock->UnLock();}
    };
}

namespace LD
{
    namespace CT
    {
        template<typename T>
        constexpr bool IsSharedLock(LD::Type<LD::SharedLock<T>> ) noexcept
        {
            return true;
        }
    }
}
#endif //LANDESSDEVCORE_SHAREDLOCK_HPP
