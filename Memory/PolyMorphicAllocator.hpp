//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
#define LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
#include "MemoryResource.hpp"
#include "TypeTraits/Limit.hpp"
#include "TypeTraits/UsesAlloc.h"
#include "TypeTraits/IsBaseOf.hpp"
namespace LD
{
    namespace Mem
    {
        class AllocatorArg
        {

        };
        template<typename T>
        class PolymorphicAllocator
        {
        private:
            LD::Mem::MemoryResource * mResource;
            using __uses_alloc1_ = __uses_alloc1<LD::Mem::PolymorphicAllocator<T>>;
        public:
            PolymorphicAllocator(LD::Mem::MemoryResource * resource) noexcept:mResource{resource}
            {

            }

            PolymorphicAllocator(LD::Mem::MemoryResource & resource) noexcept:mResource{&resource}
            {

            }
            MemoryResource * Resource() noexcept
            {
                return this->mResource;
            }
            template<typename _Up>
            PolymorphicAllocator(const LD::Mem::PolymorphicAllocator<_Up>& __x) noexcept
                    : mResource(__x.Resource())
            { }

            PolymorphicAllocator& operator=(const PolymorphicAllocator&) = delete;
            T * allocate(LD::UInteger amount) noexcept
            {
                if (amount > (LD::Limit<size_t>::GetMax() / sizeof(T)))
                {
                    return nullptr;
                }
                return static_cast<T*>(this->mResource->allocate(amount * sizeof(T),alignof(T)));
            }
            void deallocate(T * p, LD::UInteger n) noexcept
            {
                this->mResource->deallocate(p, n * sizeof(T), alignof(T));
            }
            template<typename _Tp1, typename ... Args>
            void construct(_Tp1 * pointer, Args && ... args) noexcept
            {
                using __use_tag = LD::__uses_alloc_t<_Tp1, LD::Mem::PolymorphicAllocator<T>, Args...>;
                if constexpr (LD::Detail::IsBaseOf_V<__uses_alloc0, __use_tag>)
                {
                    ::new(pointer) _Tp1(LD::Forward<Args>(args)...);
                }
                else if constexpr (LD::Detail::IsBaseOf_V<__uses_alloc1_, __use_tag>)
                {
                    ::new(pointer) _Tp1(AllocatorArg{}, *this,LD::Forward<Args>(args)...);
                }
                else
                {
                    ::new(pointer) _Tp1(LD::Forward<Args>(args)..., *this);
                }
            }
            template<typename U>
            void destroy(U* __p) noexcept
            { __p->~U(); }
        };
    }
}
#endif //LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
