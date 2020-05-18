//
//  AutomaticReferenceCounting.h
//  Memory
//
//  Created by James Landess on 7/16/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __Memory__AutomaticReferenceCounting__
#define __Memory__AutomaticReferenceCounting__
#include <stdlib.h>
#include <stdio.h>
#include <Definitions/Common.hpp>
#include "Async/Atomic/Atomic.h"
#include "ElementReference.h"
//#include <Memory/ElementReference.h>
//#include "ElementReference.h"
namespace PDP
{
    namespace Memory
    {
        class  AutomaticReferencedCountingObject
        {
        public:
            /// Default constructor
            AutomaticReferencedCountingObject() { }
            
            /// Copy constructor
            AutomaticReferencedCountingObject(const AutomaticReferencedCountingObject &) : m_refCount(0) {}
            
            /// Return the current reference count
            int getRefCount() const { return m_refCount.load(PDP::AcquireRelease); };
            
            /// Increase the object's reference count by one
            void Retain() const { ++m_refCount; }
            
            virtual void OnFinalize() const {}
            
            /** \brief Decrease the reference count of
             * the object and possibly deallocate it.
             *
             * The object will automatically be deallocated once
             * the reference count reaches zero.
             */
            virtual void Release(bool dealloc = true) const noexcept;
        protected:
            /** \brief Virtual protected deconstructor.
             * (Will only be called by \ref ref)
             */
            virtual ~AutomaticReferencedCountingObject();
        private:
            mutable PDP::Atomic<int> m_refCount { 0 };
        };
        
        
        template<typename T, class = void> class AutomaticReferenceCounter{};
        
        
        template <typename T> class AutomaticReferenceCounter<T,PDP::Enable_If_T<PDP::IsKindOf<PDP::Memory::AutomaticReferencedCountingObject, T>::value>>
        {
        public:
            
            
            /// Create a <tt>nullptr</tt>-valued reference
            AutomaticReferenceCounter() { }
            
            /// Construct a reference from a pointer
            AutomaticReferenceCounter(T *ptr) : m_ptr(ptr)
            {
                if (m_ptr)
                    ((AutomaticReferencedCountingObject *) m_ptr)->Retain();
            }
            
            /// Copy constructor
            /**
             @tparam DerivedType - The type of PDP::AutomaticReferenceCounter you would like to get an assignment from.
             @tparam class - An anymous paramater to help satisfy the needs of SFINAE and only activate this constructor which are the same type as T or a derived type of T
             @param arcShared - The instansiation of a PDP::Memory::AutomaticReferenceCounter which satisfies the requirement of holding an object which is either of type of T or a derived class of type T
             @brief The SFINAE of the constructor allows the PDP::Memory::AutomaticReferenceCounter allows polymorphic assignments between instances.
             */
            template <typename DerivedType, class = PDP::Enable_If_T<PDP::IsKindOf<T, DerivedType>::value>>
            explicit AutomaticReferenceCounter(const PDP::Memory::AutomaticReferenceCounter<DerivedType> & arcShared): m_ptr((T*)arcShared.Get())
            {
                if (m_ptr)
                    ((AutomaticReferencedCountingObject *) m_ptr)->Retain();
            }
            
            /// Copy constructor
            /*
            AutomaticReferenceCounter(const AutomaticReferenceCounter &r) : m_ptr(r.m_ptr)
            {
                if (m_ptr)
                    ((AutomaticReferencedCountingObject *) m_ptr)->Retain();
            }
             */
            
            /// Move constructor
            /*
            AutomaticReferenceCounter(AutomaticReferenceCounter &&r) noexcept : m_ptr(r.m_ptr)
            {
                r.m_ptr = nullptr;
            }
             */
            
            
            /// Move constructor
            /**
             @tparam DerivedType - The type of PDP::AutomaticReferenceCounter you would like to get an assignment from.
             @tparam class - An anymous paramater to help satisfy the needs of SFINAE and only activate this constructor which are the same type as T or a derived type of T
             @param arcShared - The instansiation of a PDP::Memory::AutomaticReferenceCounter which satisfies the requirement of holding an object which is either of type of T or a derived class of type T
             @brief The SFINAE of the constructor allows the PDP::Memory::AutomaticReferenceCounter allows polymorphic assignments between instances.
             */
            template <typename DerivedType, class = PDP::Enable_If_T<PDP::IsKindOf<T, DerivedType>::value>>
            explicit AutomaticReferenceCounter(PDP::Memory::AutomaticReferenceCounter<DerivedType> && arcShared): m_ptr(arcShared.m_ptr)
            {
                arcShared.m_ptr = nullptr;
            }
            
            /// Destroy this reference
            ~AutomaticReferenceCounter()
            {
                if (m_ptr)
                    ((AutomaticReferencedCountingObject *) m_ptr)->Release();
            }
            
            /// Move another reference into the current one
            /**
             @tparam DerivedType - The type of PDP::AutomaticReferenceCounter you would like to get an assignment from.
             @tparam class - An anymous paramater to help satisfy the needs of SFINAE and only activate this constructor which are the same type as T or a derived type of T
             @param arcShared - The instansiation of a PDP::Memory::AutomaticReferenceCounter which satisfies the requirement of holding an object which is either of type of T or a derived class of type T
             @brief The SFINAE of the constructor allows the PDP::Memory::AutomaticReferenceCounter allows polymorphic assignments between instances.
             */
            template <typename DerivedType, class = PDP::Enable_If_T<PDP::IsKindOf<T, DerivedType>::value>>
            AutomaticReferenceCounter<T> operator=(PDP::Memory::AutomaticReferenceCounter<DerivedType> && r) noexcept
            {
                if (&r != this)
                {
                    if (m_ptr)
                        ((AutomaticReferencedCountingObject *) m_ptr)->Release();
                        m_ptr = r.m_ptr;
                        r.m_ptr = nullptr;
                        }
                return *this;
            }
            /// Move another reference into the current one
            
            /*
            AutomaticReferenceCounter& operator=(AutomaticReferenceCounter&& r) noexcept {
                if (&r != this)
                {
                    if (m_ptr)
                        ((AutomaticReferencedCountingObject *) m_ptr)->Release();
                        m_ptr = r.m_ptr;
                    r.m_ptr = nullptr;
                }
                return *this;
            }
             */
            
            /// Overwrite this reference with another reference
            /**
             @tparam DerivedType - The type of PDP::AutomaticReferenceCounter you would like to get an assignment from.
             @tparam class - An anymous paramater to help satisfy the needs of SFINAE and only activate this constructor which are the same type as T or a derived type of T
             @param arcShared - The instansiation of a PDP::Memory::AutomaticReferenceCounter which satisfies the requirement of holding an object which is either of type of T or a derived class of type T
             @brief The SFINAE of the constructor allows the PDP::Memory::AutomaticReferenceCounter allows polymorphic assignments between instances.
             */
            template <typename DerivedType, class = PDP::Enable_If_T<PDP::IsKindOf<T, DerivedType>::value>>
            AutomaticReferenceCounter<T> operator=(const PDP::Memory::AutomaticReferenceCounter<DerivedType> & r) noexcept
            {
                if (m_ptr != r.m_ptr)
                {
                    if (r.m_ptr)
                        ((AutomaticReferencedCountingObject *) r.m_ptr)->Retain();
                        if (m_ptr)
                            ((AutomaticReferencedCountingObject *) m_ptr)->Release();
                            m_ptr = r.m_ptr;
                            
                }
                return *this;
            }
            /// Overwrite this reference with another reference
            /*
            AutomaticReferenceCounter& operator=(const AutomaticReferenceCounter& r) noexcept
            {
                if (m_ptr != r.m_ptr)
                {
                    if (r.m_ptr)
                        ((AutomaticReferencedCountingObject *) r.m_ptr)->Retain();
                        if (m_ptr)
                            ((AutomaticReferencedCountingObject *) m_ptr)->Release();
                            m_ptr = r.m_ptr;
                }
                return *this;
            }
             */
            
            /// Overwrite this reference with a pointer to another object
            AutomaticReferenceCounter& operator=(T *ptr) noexcept
            {
                if (m_ptr != ptr)
                {
                    if (ptr)
                        ((AutomaticReferencedCountingObject *) ptr)->Retain();
                        if (m_ptr)
                            ((AutomaticReferencedCountingObject *) m_ptr)->Release();
                            m_ptr = ptr;
                }
                return *this;
            }
            
            /// Compare this reference with another reference
            bool operator==(const AutomaticReferenceCounter &r) const { return m_ptr == r.m_ptr; }
            
            /// Compare this reference with another reference
            bool operator!=(const AutomaticReferenceCounter &r) const { return m_ptr != r.m_ptr; }
            
            /// Compare this reference with a pointer
            bool operator==(const T* ptr) const { return m_ptr == ptr; }
            
            /// Compare this reference with a pointer
            bool operator!=(const T* ptr) const { return m_ptr != ptr; }
            
            /// Access the object referenced by this reference
            T* operator->() { return m_ptr; }
            
            /// Access the object referenced by this reference
            const T* operator->() const { return m_ptr; }
            
            /// Return a C++ reference to the referenced object
            T& operator*() { return *m_ptr; }
            
            /// Return a const C++ reference to the referenced object
            const T& operator*() const { return *m_ptr; }
            
            /// Return a pointer to the referenced object
            operator T* () { return m_ptr; }
            
            /// Return a const pointer to the referenced object
            T* Get() { return m_ptr; }
            
            /// Return a pointer to the referenced object
            const T* Get() const { return m_ptr; }
            
            /// Check if the object is defined
            operator bool() const { return m_ptr != nullptr; }
        private:
            T *m_ptr = nullptr;
        };

        
        template<typename T>
        PDP::Enable_If_T<PDP::IsKindOf<PDP::Memory::AutomaticReferencedCountingObject, T>::value,PDP::Memory::AutomaticReferenceCounter<T>>
         MakeARCShared(const T & object)
        {
            return PDP::Memory::AutomaticReferenceCounter<T>(new T(object));
        }
    }
}
#endif /* defined(__Memory__AutomaticReferenceCounting__) */
