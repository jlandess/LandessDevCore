//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
#define LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
#include "MemoryResource.hpp"
#include "TypeTraits/Limit.hpp"
#include "TypeTraits/UsesAlloc.h"
namespace LD
{
    namespace Mem
    {
        template<typename T>
        class PolymorphicAllocator
        {
        private:
            LD::Mem::MemoryResource * mResource;
            using __uses_alloc1_ = __uses_alloc1<LD::PolymorphicAllocator>;
        public:
            PolymorphicAllocator(LD::Mem::MemoryResource * resource) noexcept:mResource{resource}
            {

            }

            MemoryResource * Resource() noexcept
            {
                return this->mResource;
            }
            template<typename _Up>
            PolymorphicAllocator(const LD::PolymorphicAllocator<_Up>& __x) noexcept
                    : mResource(__x.Resource())
            { }

            PolymorphicAllocator& operator=(const polymorphic_allocator&) = delete;
            T * allocate(LD::UInteger amount) noexcept
            {
                if (amount > (LD::Limit<size_t>::GetMax() / sizeof(_Tp)))
                {
                    return nullptr;
                }
                return static_cast<T*>(this->mResource->allocate(amount * sizeof(T),alignof(T)));
            }
            void deallocate(T * p, LD::UInteger n) noexcept
            {
                this->mResource->deallocate(__p, __n * sizeof(_Tp), alignof(_Tp));
            }
            template<typename _Tp1, typename ... Args>
            void construct(_Tp1 * pointer, Args && ... args) noexcept
            {
                using __use_tag = LD::__uses_alloc_t<_Tp1, LD::PolymorphicAllocator, _Args...>;
                if constexpr (std::is_base_of_v<__uses_alloc0, __use_tag>)
                {
                    ::new(__p) _Tp1(std::forward<_Args>(__args)...);
                }
                else if constexpr (std::is_base_of_v<__uses_alloc1_, __use_tag>)
                {
                    ::new(__p) _Tp1(allocator_arg, *this,std::forward<_Args>(__args)...);
                }
                else
                {
                    ::new(__p) _Tp1(std::forward<_Args>(__args)..., *this);
                }
            }
            template<typename U>
            void destroy(_Up* __p)
            { __p->~_Up(); }
        };
    }
}
#endif //LANDESSDEVCORE_POLYMORPHICALLOCATOR_HPP
