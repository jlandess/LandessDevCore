//
// Created by phoenixflower on 10/21/21.
//

#ifndef LANDESSDEVCORE_FLYWEIGHT_HPP
#define LANDESSDEVCORE_FLYWEIGHT_HPP
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/IsConstructible.hpp"
#include "Memory/UniquePointer.h"
#include "Memory/shared_ptr.hpp"
namespace LD
{
    template<typename T>
    class FlyWeight
    {
    private:
        T * mShared;
    public:
        FlyWeight(T * shared) noexcept
        {

        }
        T * operator->() noexcept
        {
            return this->mShared;
        }

        const T * operator->() const noexcept
        {
            return this->mShared;
        }
    };
    template<typename T>
    class FlyWeightFactory
    {
    private:
        //need an associate array
        //need a way to coorelate objects with keys
        //we need a factory
    public:

        template<typename ... A>
        LD::Enable_If_T<LD::Require<LD::IsConstructible<T,A...>::value>,FlyWeight<T>> operator()(A && ... args) noexcept
        {
            return FlyWeight<T>{};
        }


        LD::SharedPointer<FlyWeight<T>> MakeShared() noexcept
        {

        }
        //LD::UniquePointer<T,LD::MemoryResourceDeleter>
        LD::UniquePointer<FlyWeight<T>,LD::MemoryResourceDeleter> MakeUnique(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
        {

        }

        FlyWeight<T> Make() noexcept
        {

        }
    };
}
#endif //LANDESSDEVCORE_FLYWEIGHT_HPP
