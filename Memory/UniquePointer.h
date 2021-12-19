//
//  UniquePointer.h
//  DataStructures
//
//  Created by James Landess on 6/21/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_UniquePointer_h
#define DataStructures_UniquePointer_h


#include "MemoryResource.hpp"
#include "PolyMorphicAllocator.hpp"
#include "TypeTraits/IsBaseOf.hpp"
//#include "Definitions/Common.hpp"
//#include <Types/MemoryBarriers.h>
#include <cstddef>      // NULL
#include <algorithm>    // std::swap
#include "Async/Atomic/Atomic.h"
#include "Functor/LightWeightDelegate.h"
#include "ElementReference.h"
#include "TypeTraits/RemoveExtent.hpp"
// can be replaced by other error mechanism
#include <cassert>
#define SHARED_ASSERT(x)    assert(x)
namespace LD
{
    template<typename _Tp>
    struct DefaultDelete
    {
        DefaultDelete() { }
        
        template<typename _Up>
        DefaultDelete(const DefaultDelete<_Up>&) { }
        
        void operator()(_Tp* __ptr) const
        {
            static_assert(sizeof(_Tp)>0, "can't delete pointer to incomplete type");
            delete __ptr;
            __ptr = nullptr;
            
        }
    };

    struct MemoryResourceDeleter
    {
        LD::Mem::MemoryResource * mResource;
        template<typename T>
        void operator()(T * __ptr) const
        {
            //LD::Mem::PolymorphicAllocator<T> resourceAllocatr[mResource};
            if(__ptr != nullptr)
            {
                LD::Mem::PolymorphicAllocator<T> poly{mResource};
                poly.destroy(__ptr);
                poly.deallocate(__ptr,1);
            }

        }
    };
    
    template<typename _Tp>
    struct DefaultDelete<_Tp[]>
    {
        void operator()(_Tp* __ptr) const
        {
            static_assert(sizeof(_Tp)>0,"can't delete pointer to incomplete type");
            delete [] __ptr;
            __ptr = nullptr;
        }
    };
    
    
    template<class T,typename DeleterType = DefaultDelete<T> >
    class UniquePointer
    {
    private:
        T* px; //!< Native pointer
        DeleterType CurrentDeleter;
    public:
        /// The type of the managed object, aliased as member type
        typedef T element_type;
        
        /// @brief Default constructor
        
        
        UniquePointer(void) noexcept: // never throws
        px(nullptr)
        {
        }
        /// @brief Constructor with the provided pointer to manage
        explicit UniquePointer(T* p, const DeleterType & deleterType = DeleterType()) noexcept : // never throws
        px(p),CurrentDeleter(deleterType)
        {
        }


        UniquePointer(const UniquePointer& ptr) = delete;
        UniquePointer& operator=(const UniquePointer & ptr) = delete;

        template<typename U, class = LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf_V<T,U>>>>
        UniquePointer(const UniquePointer<U,DeleterType>& ptr) = delete;
        /// @brief Copy constructor to convert from another pointer type
        /* TODO MSVC error C2248: 'unique_ptr<B>::px' : unique_ptr<A> cannot access private member declared in class 'unique_ptr<B>'
         template <class U>
         unique_ptr(const unique_ptr<U>& ptr) throw() : // never throws
         px(static_cast<typename unique_ptr<T>::element_type*>(ptr.px))
         {
         const_cast<unique_ptr<U>&>(ptr).px = NULL; // const-cast to force ownership transfer!
         }
         */
        /// @brief Copy constructor (used by the copy-and-swap idiom)
        UniquePointer( UniquePointer&& ptr) noexcept : // never throws
        px(ptr.px)
        {
            ptr.px = nullptr; // const-cast to force ownership transfer!
        }

        template<typename U, class = LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf_V<T,U>>>>
        UniquePointer(UniquePointer<U,DeleterType>&& ptr) noexcept : // never throws
                px(ptr.px)
        {
            ptr.px = nullptr; // const-cast to force ownership transfer!
        }
        /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
        //UniquePointer& operator=(UniquePointer && ptr) noexcept// never throws
        //{
            //swap(ptr);
            //return *this;
        //}

        //template<typename U, class = LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf_V<T,U>>>>
        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<LD::Detail::IsBaseOf_V<V,U>>,UniquePointer&> operator=(UniquePointer<U,DeleterType> && ptr) noexcept// never throws
        {
            swap(ptr);
            return *this;
        }
        /// @brief the destructor releases its ownership and destroy the object
        inline ~UniquePointer(void) noexcept // never throws
        {
            destroy();
        }
        /// @brief this reset releases its ownership and destroy the object
        inline void reset(void) noexcept // never throws
        {
            destroy();
        }
        /// @brief this reset release its ownership and re-acquire another one
        void reset(T* p) noexcept // never throws
        {
            SHARED_ASSERT((NULL == p) || (px != p)); // auto-reset not allowed
            destroy();
            px = p;
        }
        
        /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
        void swap(UniquePointer& lhs) noexcept // never throws
        {
            std::swap(px, lhs.px);
        }
        
        /// @brief release the ownership of the px pointer without destroying the object!
        inline void release(void) noexcept // never throws
        {
            px = NULL;
        }
        
        // reference counter operations :
        inline operator bool() const noexcept // never throws
        {
            return (nullptr != px); // TODO nullptr
        }
        
        // underlying pointer operations :
        inline T& operator*()  const noexcept// never throws
        {
            SHARED_ASSERT(NULL != px);
            return *px;
        }
        inline T* operator->() const noexcept// never throws
        {
            SHARED_ASSERT(NULL != px);
            return px;
        }
        inline T* get(void)  const noexcept // never throws
        {
            // no assert, can return NULL
            return px;
        }
        
    private:
        /// @brief release the ownership of the px pointer and destroy the object
        inline void destroy(void) noexcept// never throws
        {
            CurrentDeleter(px);
            //delete px;
            //px = NULL;
        }
        
        /// @brief hack: const-cast release the ownership of the px pointer without destroying the object!
        inline void release(void) const noexcept // never throws
        {
            px = NULL;
        }
        

        //template<class T,typename DeleterType = DefaultDelete<T> >

        template<class X, typename Z> friend class UniquePointer;
    };
    
    template <typename T>
    void swap(UniquePointer<T>& a, UniquePointer<T>& b) noexcept
    {
        a.Swap(b);
    }
    
    template<class T> struct _Unique_if
    {
        
        typedef UniquePointer<T> _Single_object;
        
    };
    
    
    template<class T> struct _Unique_if<T[]>
    {
        
        typedef UniquePointer<T[]> _Unknown_bound;
        
    };
    
    
    template<class T, size_t N> struct _Unique_if<T[N]>
    {
        
        typedef void _Known_bound;
        
    };
    
    
    
    template<class T, class... Args>
    typename _Unique_if<T>::_Single_object MakeUnique(Args&&... args)
    {
        
        return UniquePointer<T>(new T(LD::Forward<Args>(args)...));
    }

    template<typename T>
   LD::Enable_If_T<LD::Require<LD::IsDefaultConstructible<T>>,LD::UniquePointer<T,LD::MemoryResourceDeleter>> AllocateUnique(LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator{resource};
        T * buffer = allocator.allocate(1);
        allocator.construct(buffer);

        return LD::UniquePointer<T, MemoryResourceDeleter>{buffer,MemoryResourceDeleter{resource}};
    }

    template<typename T, typename ... A>
    LD::Enable_If_T<true,LD::UniquePointer<T,LD::MemoryResourceDeleter>> AllocateUnique(LD::Mem::MemoryResource * resource, A && ... args) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator{resource};
        T * buffer = allocator.allocate(1);
        allocator.construct(buffer,LD::Forward<A>(args)...);

        return LD::UniquePointer<T, MemoryResourceDeleter>{buffer,MemoryResourceDeleter{resource}};
    }

    template<typename T, typename ... A>
    LD::UniquePointer<T,LD::MemoryResourceDeleter> AllocateUnique(T && object,LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept
    {
        LD::Mem::PolymorphicAllocator<T> allocator{resource};
        T * buffer = allocator.allocate(1);
        allocator.construct(buffer,LD::Forward<T>(object));

        return LD::UniquePointer<T, MemoryResourceDeleter>{buffer,MemoryResourceDeleter{resource}};
    }
    
    
    template<class T, class... Args, typename Allocator>
    typename _Unique_if<T>::_Single_object MakeUnique(Args&&... args,const PDP::ElementReference<Allocator> & currentAllocator)
    {
        
        return UniquePointer<T>(new T(LD::Forward<Args>(args)...));
    }
    
    
    template<class T>
    typename _Unique_if<T>::_Unknown_bound MakeUnique(const PDP::UInteger & n)
    {
        
        typedef typename LD::Detail::RemoveExtent<T>::type U;
        
        return UniquePointer<T>(new U[n]());
        
    }
    
    template<class T, typename Allocator>
    typename _Unique_if<T>::_Unknown_bound MakeUnique(const PDP::UInteger &  n,const PDP::ElementReference<Allocator> & currentAllocator)
    {
        
        typedef typename LD::Detail::RemoveExtent<T>::type U;
        
        return UniquePointer<T>(new U[n]());
        
    }
    
    // comparaison operators
    template<class T, class U> inline bool operator==(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() == r.get());
    }
    template<class T, class U> inline bool operator!=(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() != r.get());
    }
    template<class T, class U> inline bool operator<=(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() <= r.get());
    }
    template<class T, class U> inline bool operator<(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() < r.get());
    }
    template<class T, class U> inline bool operator>=(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() >= r.get());
    }
    template<class T, class U> inline bool operator>(const UniquePointer<T>& l, const UniquePointer<U>& r) throw() // never throws
    {
        return (l.get() > r.get());
    }
}

/**
 template<typename _Tp>
 struct default_delete
 {
 default_delete() { }
 
 template<typename _Up>
 default_delete(const default_delete<_Up>&) { }
 
 void operator()(_Tp* __ptr) const
 {
 static_assert(sizeof(_Tp)>0, "can't delete pointer to incomplete type");
 delete __ptr;
 
 }
 };
 
 
 template<typename _Tp>
 struct default_delete<_Tp[]>
 {
 void operator()(_Tp* __ptr) const
 {
 static_assert(sizeof(_Tp)>0,"can't delete pointer to incomplete type");
 delete [] __ptr;
 }
 };
 
 template <typename _Tp, typename _Tp_Deleter = default_delete<_Tp> >
 class UniquePointer
 {
 private:
 //typedef PDP::Tuple<_Tp*, _Tp_Deleter>  __tuple_type;
 //typedef __tuple_type UniquePointer::*     __unspecified_bool_type;
 typedef _Tp* UniquePointer::*             __unspecified_pointer_type;
 
 
 _Tp * Object;
 _Tp_Deleter CurrentDeleter;
 //__tuple_type _M_t;
 public:
 typedef _Tp*               pointer;
 typedef _Tp                element_type;
 typedef _Tp_Deleter        deleter_type;
 
 
 // Constructors.
 UniquePointer(): Object(pointer()),CurrentDeleter(deleter_type())
 {
 static_assert(!PDP::IsPointer<deleter_type>::value,"constructed with null function pointer deleter");
 
 }
 
 
 
 explicit UniquePointer(pointer __p): Object(__p),CurrentDeleter(deleter_type())
 {
 static_assert(!PDP::IsPointer<deleter_type>::value,"constructed with null function pointer deleter");
 
 }
 
 
 
 
 UniquePointer(pointer __p,typename PDP::Conditonal<PDP::IsReference<deleter_type>::value,deleter_type, const deleter_type&>::type __d): Object(__p),CurrentDeleter(__d)
 {
 
 }
 
 
 
 UniquePointer(pointer __p,typename std::remove_reference<deleter_type>::type && __d): Object(PDP::Move(__p)),CurrentDeleter(PDP::Move(__d))
 {
 static_assert(!PDP::IsReference<deleter_type>::value, "rvalue deleter bound to reference");
 
 }
 
 
 // Move constructors.
 UniquePointer(UniquePointer&& __u):Object(__u.release()),CurrentDeleter(__u.get_deleter())//_M_t(__u.release(), PDP::Forward<deleter_type>(__u.get_deleter()))
 {
 
 }
 
 
 template<typename _Up, typename _Up_Deleter>
 UniquePointer(UniquePointer<_Up, _Up_Deleter>&& __u):Object(__u.release()),CurrentDeleter(__u.get_deleter())
 {
 
 }
 
 
 
 ~UniquePointer()
 {
 reset();
 
 }
 
 
 UniquePointer& operator=(__unspecified_pointer_type)
 {
 reset();
 return *this;
 
 }
 
 pointer get() const { return this->Object; }
 
 
 typename PDP::AddLValueReference<deleter_type>::type get_deleter() { return CurrentDeleter; }
 
 
 
 typename PDP::AddLValueReference<typename PDP::AddConst<deleter_type>::type>::type get_deleter() const
 {
 return CurrentDeleter;
 
 }
 
 
 
 const bool IsValid() const
 {
 return !(this->get() == 0);
 }
 
 operator __unspecified_bool_type () const
 {
 return get() == 0 ? 0 : &UniquePointer::_M_t;
 
 }
 

pointer release()
{
    
    pointer __p = get();
    
    Object = 0;
    
    //PDP::get<0>(_M_t) = 0;
    
    return __p;
}

void swap(UniquePointer&& __u)
{
    //using std::swap;
    // swap(_M_t, __u._M_t);
    
    PDP::Swap(Object, __u.Object);
    PDP::Swap(CurrentDeleter, __u.CurrentDeleter);
    
}

// Disable copy from lvalue.
UniquePointer(const UniquePointer&) = delete;
UniquePointer& operator=(const UniquePointer&) = delete;


// Disable construction from convertible pointer types.
// (N2315 - 20.6.5.3.1)
template<typename _Up>
UniquePointer(_Up*, typename PDP::Conditonal<PDP::IsReference<deleter_type>::value,
              deleter_type, const deleter_type&>::type,
              typename PDP::EnableIf<PDP::IsConvertible<_Up*,
              pointer>::value>::type* = 0) = delete;


template<typename _Up>
UniquePointer(_Up*, typename PDP::RemoveReference<deleter_type>::type&&,
              typename PDP::EnableIf<PDP::IsConvertible<_Up*,
              pointer>::value>::type* = 0) = delete;


template<typename _Up>
UniquePointer unique_ptr(_Up*, typename PDP::EnableIf<PDP::IsConvertible<_Up*,
                         pointer>::value>::type* = 0) = delete;


void reset(pointer __p = pointer())
{
    if (__p != get())
    {
        
        get_deleter()(get());
        
        
        Object = __p;
    }
}
};



template<class T> struct _Unique_if
{
    
    typedef UniquePointer<T> _Single_object;
    
};


template<class T> struct _Unique_if<T[]>
{
    
    typedef UniquePointer<T[]> _Unknown_bound;
    
};


template<class T, size_t N> struct _Unique_if<T[N]>
{
    
    typedef void _Known_bound;
    
};



template<class T, class... Args>
typename _Unique_if<T>::_Single_object MakeUnique(Args&&... args)
{
    
    return UniquePointer<T>(new T(PDP::Forward<Args>(args)...));
}


template<class T, class... Args, typename Allocator>
typename _Unique_if<T>::_Single_object MakeUnique(Args&&... args,const PDP::ElementReference<Allocator> & currentAllocator)
{
    
    return UniquePointer<T>(new T(PDP::Forward<Args>(args)...));
}


template<class T>
typename _Unique_if<T>::_Unknown_bound MakeUnique(size_t n)
{
    
    typedef typename PDP::RemoveExtent<T>::type U;
    
    return UniquePointer<T>(new U[n]());
    
}

template<class T, typename Allocator>
typename _Unique_if<T>::_Unknown_bound MakeUnique(size_t n,const PDP::ElementReference<Allocator> & currentAllocator)
{
    
    typedef typename PDP::RemoveExtent<T>::type U;
    
    return UniquePointer<T>(new U[n]());
    
}



template<typename _Tp, typename _Tp_Deleter,
typename _Up, typename _Up_Deleter>
inline bool operator==(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return __x.get() == __y.get(); }

template<typename _Tp, typename _Tp_Deleter,
typename _Up, typename _Up_Deleter>
inline bool operator!=(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return !(__x.get() == __y.get()); }

template<typename _Tp, typename _Tp_Deleter,
typename _Up, typename _Up_Deleter>
inline bool operator<(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return __x.get() < __y.get(); }

template<typename _Tp, typename _Tp_Deleter,typename _Up, typename _Up_Deleter>
inline bool operator<=(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return !(__y.get() < __x.get()); }

template<typename _Tp, typename _Tp_Deleter,typename _Up, typename _Up_Deleter>
inline bool operator>(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return __y.get() < __x.get(); }

template<typename _Tp, typename _Tp_Deleter,typename _Up, typename _Up_Deleter>
inline bool operator>=(const UniquePointer<_Tp, _Tp_Deleter>& __x,const UniquePointer<_Up, _Up_Deleter>& __y)
{ return !(__x.get() < __y.get()); }
 */
#endif
