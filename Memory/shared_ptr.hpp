/**
 * @file  shared_ptr.hpp
 * @brief shared_ptr is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * Copyright (c) 2013-2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cstddef>      // NULL
#include <algorithm>    // std::swap
// can be replaced by other error mechanism
#include <cassert>
//#include "Definitions/Common.hpp"
#include "Memory/Optional.h"
#include "Async/Atomic/Atomic.h"
#include "TypeTraits/Detection.hpp"
#include "Memory/MemoryResource.hpp"
#include "Memory/PolyMorphicAllocator.hpp"
#define SHARED_ASSERT(x)    assert(x)

namespace LD
{
    /**
     * @brief implementation of reference counter for the following minimal smart pointer.
     *
     * shared_ptr_count is a container for the allocated pn reference counter.
     */
    class shared_ptr_count
    {
    private:
        LD::Mem::MemoryResource * mResource;
        LD::Mem::PolymorphicAllocator<LD::Atomic<PDP::Integer>> mAllocator;
    public:
        shared_ptr_count(LD::Mem::MemoryResource * resource) :
        pn(NULL),
        mResource{resource},
        mAllocator(this->mResource)
        {
        }
        shared_ptr_count(const shared_ptr_count& count) :
        pn(count.pn),mResource{count.mResource}, mAllocator{this->mResource}
        {

        }
        /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
        void swap(shared_ptr_count& lhs) noexcept // never throws
        {
            std::swap(pn, lhs.pn);
            std::swap(mResource,lhs.mResource);
        }
        /// @brief getter of the underlying reference counter
        PDP::Integer use_count(void) const noexcept // never throws
        {
            long count = 0;
            if (nullptr != pn)
            {
                count = pn->load(LD::AcquireRelease);
            }
            return count;
        }
        /// @brief acquire/share the ownership of the pointer, initializing the reference counter
        template<class U>
        void acquire(U* p) // may throw std::bad_alloc
        {
            if (nullptr != p)
            {
                if (nullptr == pn)
                {
                    try
                    {
                        pn = this->mAllocator.allocate(1);
                        this->mAllocator.construct(pn,LD::Atomic<LD::Integer>{1});
                        //pn1 = this->mAllocator.allocate(1);
                        //this->mResource->allocate(32,8);
                        //LD::Mem::GetNewDeleteResource()->allocate(16,8);
                        //pn = new LD::Atomic<PDP::Integer>(1); // may throw std::bad_alloc
                    }
                    catch (std::bad_alloc&)
                    {
                        delete p;
                        throw; // rethrow the std::bad_alloc
                    }
                }
                else
                {
                    ++(*pn);
                }
            }
        }
        /// @brief release the ownership of the px pointer, destroying the object when appropriate
        template<class U>
        void release(U* p) throw() // never throws
        {
            if (nullptr != pn)
            {
                --(*pn);
                if (0 == pn->load(LD::AcquireRelease))
                {
                    delete p;
                    this->mAllocator.destroy(pn);
                    this->mAllocator.deallocate(pn,1);
                    //delete pn;
                    pn = nullptr;
                }
                //pn = nullptr;
            }
        }
        
    public:
        LD::Atomic<PDP::Integer> *   pn; //!< Reference counter

        //LD::Atomic<PDP::Integer> *   pn1;
        template<typename U>
        friend class WeakPointer;
        
        template<typename U>
        friend class SharedPointer;
    };
    
    
    /**
     * @brief minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
     *
     * shared_ptr is a smart pointer retaining ownership of an object through a provided pointer,
     * and sharing this ownership with a reference counter.
     * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
     */
    template<class T>
    class SharedPointer
    {
    public:
        /// The type of the managed object, aliased as member type
        typedef T element_type;
        
        
        
        /// @brief Default constructor
        SharedPointer(void) throw() : // never throws
        px(nullptr),
        mResource(LD::Mem::GetNewDeleteResource()),
        pn(LD::Mem::GetNewDeleteResource())
        {
        }
        /// @brief Constructor with the provided pointer to manage
        explicit SharedPointer(T* p) noexcept: px(p), mResource{LD::Mem::GetNewDeleteResource()},pn{LD::Mem::GetNewDeleteResource()} //SharedPointer() // may throw std::bad_alloc
        //px(p)//, would be unsafe as acquire() may throw, which would call release() in destructor
        {
            acquire(p);   // may throw std::bad_alloc
        }

        explicit SharedPointer(T* p, LD::Mem::MemoryResource * resource) noexcept: px(p), mResource{resource},pn{resource} //SharedPointer() // may throw std::bad_alloc
        //px(p)//, would be unsafe as acquire() may throw, which would call release() in destructor
        {
            acquire(p);   // may throw std::bad_alloc
        }


        /// @brief Constructor to share ownership. Warning : to be used for pointer_cast only ! (does not manage two separate <T> and <U> pointers)
        template <class U>
        SharedPointer(const SharedPointer<U>& ptr, T* p) :
        //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
        pn(ptr.pn)
        {
            acquire(p);   // may throw std::bad_alloc
        }
        /// @brief Copy constructor to convert from another pointer type
        template <class U>
        SharedPointer(const SharedPointer<U>& ptr) throw() : // never throws (see comment below)
        //px(ptr.px),
        pn(ptr.pn)
        {
            SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count())); // must be coherent : no allocation allowed in this path
            acquire(static_cast<typename SharedPointer<T>::element_type*>(ptr.px));   // will never throw std::bad_alloc
        }
        /// @brief Copy constructor (used by the copy-and-swap idiom)
        SharedPointer(const SharedPointer& ptr) throw() : // never throws (see comment below)
        //px(ptr.px),
        pn(ptr.pn)
        {
            SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count())); // must be cohérent : no allocation allowed in this path
            acquire(ptr.px);   // will never throw std::bad_alloc
        }
        /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
        SharedPointer& operator=(SharedPointer ptr) throw() // never throws
        {
            swap(ptr);
            return *this;
        }
        /// @brief the destructor releases its ownership
        inline ~SharedPointer(void) throw() // never throws
        {
            release();
        }
        /// @brief this reset releases its ownership
        inline void reset(void) throw() // never throws
        {
            release();
        }
        /// @brief this reset release its ownership and re-acquire another one
        void reset(T* p) // may throw std::bad_alloc
        {
            SHARED_ASSERT((nullptr == p) || (px != p)); // auto-reset not allowed
            release();
            acquire(p); // may throw std::bad_alloc
        }
        
        /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
        void swap(SharedPointer& lhs) throw() // never throws
        {
            std::swap(px, lhs.px);
            pn.swap(lhs.pn);
        }
        
        const bool IsShared() const
        {
            return pn.pn && pn.use_count() > 1;
        }
        
        // reference counter operations :
        inline operator bool() const throw() // never throws
        {
            return (0 < pn.use_count());
        }
        inline bool unique(void)  const throw() // never throws
        {
            return (1 == pn.use_count());
        }
        PDP::Integer use_count(void)  const throw() // never throws
        {
            return pn.use_count();
        }
        
        // underlying pointer operations :
        inline T& operator*()  const throw() // never throws
        {
            //SHARED_ASSERT(NULL != px);
            return *px;
        }
        inline T* operator->() const throw() // never throws
        {
            //SHARED_ASSERT(NULL != px);
            return px;
        }
        inline T* get(void)  const throw() // never throws
        {
            // no assert, can return NULL
            return px;
        }
        
        
        const bool IsValid() const
        {
            return px != nullptr && pn.use_count() >=1;
        }
        
        inline T * Get() const throw()
        {
            return px;
        }
        
    private:
        /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
        inline void acquire(T* p) // may throw std::bad_alloc
        {
            pn.acquire(p); // may throw std::bad_alloc
            px = p; // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
        }
        
        /// @brief release the ownership of the px pointer, destroying the object when appropriate
        inline void release(void) throw() // never throws
        {
            pn.release(px);
            //px = NULL;
        }
        
    private:
        // This allow pointer_cast functions to share the reference counter between different shared_ptr types
        
        
        template<class U>
        friend class WeakPointer;
        friend class shared_ptr_count;


    private:
        T*                  px; //!< Native pointer
        shared_ptr_count    pn; //!< Reference counter
        LD::Mem::MemoryResource * mResource;
    public:
        template<typename Z>
        friend class SharedPointer;
    };
    
    
    // comparaison operators
    template<class T, class U> inline bool operator==(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() == r.get());
    }
    template<class T, class U> inline bool operator!=(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() != r.get());
    }
    template<class T, class U> inline bool operator<=(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() <= r.get());
    }
    template<class T, class U> inline bool operator<(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() < r.get());
    }
    template<class T, class U> inline bool operator>=(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() >= r.get());
    }
    template<class T, class U> inline bool operator>(const SharedPointer<T>& l, const SharedPointer<U>& r) throw() // never throws
    {
        return (l.get() > r.get());
    }
    
    
    
    // static cast of shared_ptr
    template<class T, class U>
    SharedPointer<T> static_pointer_cast(const SharedPointer<U>& ptr)  noexcept// never throws
    {
        return SharedPointer<T>(ptr, static_cast<typename SharedPointer<T>::element_type*>(ptr.get()));
    }
    
    // dynamic cast of shared_ptr
    template<class T, class U>
    SharedPointer<T> dynamic_pointer_cast(const SharedPointer<U>& ptr) noexcept// never throws
    {
        T* p = dynamic_cast<typename SharedPointer<T>::element_type*>(ptr.get());
        if (nullptr != p)
        {
            return SharedPointer<T>(ptr, p);
        }
        else
        {
            return SharedPointer<T>();
        }
    }
    
    template<typename T>
    class WeakPointer
    {
    private:
        T * ObjectPointer;
        LD::Atomic<PDP::Integer> * AtomicCounter;
    public:

        template<typename U>
        friend class SharedPointer;

        friend class shared_ptr_count;

        
        inline WeakPointer() noexcept
        {
            
        }
        inline WeakPointer(const LD::SharedPointer<T> & sharedPointer):AtomicCounter(sharedPointer.pn.pn),ObjectPointer(sharedPointer.px)
        {
        }
        inline WeakPointer & operator = (const LD::SharedPointer<T> & sharedPointer) noexcept
        {
            this->ObjectPointer = sharedPointer.px;
            this->AtomicCounter = sharedPointer.pn.pn;
            return (*this);
        }
        inline WeakPointer & operator = (const WeakPointer<T> & weakPointer) noexcept
        {
            this->ObjectPointer = weakPointer.ObjectPointer;
            this->AtomicCounter = weakPointer.AtomicCounter;
            return (*this);
        }
        
        const bool IsValid() const noexcept
        {
            return AtomicCounter != nullptr && AtomicCounter->load(LD::AcquireRelease) >=1;
        }
        
        const bool IsExpired() const noexcept
        {
            return AtomicCounter == nullptr || AtomicCounter->load(LD::AcquireRelease) == 0;
        }
        
        SharedPointer<T> Lock() const noexcept
        {
            if (IsValid())
            {
                return SharedPointer<T>(new T(*ObjectPointer));
            }
            
            return SharedPointer<T>();
        }
        
        inline T * operator->() noexcept
        {
            return IsValid()?ObjectPointer:nullptr;
        }
        inline const T * operator ->() const noexcept
        {
            return IsValid()?ObjectPointer:nullptr;
        }
        
        inline T * Get() noexcept
        {
            return IsValid()?ObjectPointer:nullptr;
        }
        
        inline const T * Get() const noexcept
        {
            return IsValid()?ObjectPointer:nullptr;
        }
        
        /*
         T & operator*();
         const T & operator*() const;
         */
        
        
    };

    template<typename T, typename ... Pack>
    LD::SharedPointer<T> MakeShared(Pack && ... arguements) noexcept
    {
                //new (noexcept(T(LD::Forward<Pack>(arguements)...))) T(LD::Forward<Pack>(arguements)...);
        T * p = new (std::nothrow) T(LD::Forward<Pack>(arguements)...);


        if(p)
        {
            return LD::SharedPointer<T>(p);
        }
        return LD::SharedPointer<T>(nullptr);
    }

    template<typename T, typename Allocator ,typename ... Pack>
    LD::Enable_If_T<LD::Require<
            LD::IsConstructible<T,LD::Decay<Pack>...>::value
    >,LD::SharedPointer<T>> MakeShared(Allocator && allocator,Pack && ... arguements) noexcept
    {
        return LD::SharedPointer<T>(new T{LD::Forward<Pack>(arguements)...});
    }

    template<typename T, typename U>
    LD::SharedPointer<T> MakeSharedOne(LD::Mem::MemoryResource & memoryResource,U && object) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator(&memoryResource);
        T * allocation = allocator.allocate(1);
        if (allocation != nullptr)
        {
            allocator.construct(allocation,LD::Forward<U>(object));
            return LD::SharedPointer<T>{allocation,allocator.Resource()};
        }
        return LD::SharedPointer<T>();
    }

    template<typename T, typename Allocator>
    LD::SharedPointer<LD::Detail::Decay_T<T>> AllocateShared(Allocator & memoryResource, T && object) noexcept
    {
        LD::Mem::PolymorphicAllocator<LD::Detail::Decay_T<T>> allocator(&memoryResource);
        LD::Detail::Decay_T<T> * allocation = allocator.allocate(1);

        if (allocation == nullptr)
        {
            allocator.construct(allocation,LD::Forward<T>(object));
            //return LD::SharedPointer<T>{allocation,allocator.Resource()};
        }
        return LD::SharedPointer<T>{nullptr};
    }

    template<typename T, typename Allocator>
    LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf<Allocator,LD::Mem::MemoryResource>::value>,LD::SharedPointer<T>> AllocateShared(Allocator * memoryResource, T && object) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator(memoryResource);
        T * allocation = allocator.allocate(1);
        allocator.construct(allocation,LD::Forward<T>(object));
        if (allocation == nullptr)
        {
            return LD::SharedPointer<T>{allocation,allocator.Resource()};
        }
        return LD::SharedPointer<T>{nullptr};
    }
    template<typename T, typename Allocator>
    LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf<Allocator,LD::Mem::MemoryResource>::value>,LD::SharedPointer<T>> AllocateShared(Allocator * memoryResource) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator(memoryResource);
        T * allocation = allocator.allocate(1);
        if (allocation == nullptr)
        {
            return LD::SharedPointer<T>{allocation,allocator.Resource()};
        }
        return LD::SharedPointer<T>{nullptr};
    }
}

