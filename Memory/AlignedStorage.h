//
//  AlignedStorage.h
//  DataStructures
//
//  Created by James Landess on 5/3/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_AlignedStorage_h
#define DataStructures_AlignedStorage_h
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace Mem
    {
        inline void* alignedMalloc(size_t size, size_t alignment)
        {
            const int pointerSize = sizeof(void*);
            const long requestedSize = size + alignment - 1 + pointerSize;
            void* raw = std::malloc(requestedSize);

            if (!raw)
            {
                return 0;
            }

            void* start = (uint8_t*)raw + pointerSize;
            void* aligned = (void*)(((uintptr_t)((uint8_t*)start+alignment-1)) & ~(alignment-1));
            *(void**)((uint8_t*)aligned-pointerSize) = raw;
            return aligned;
        }

        /**
         * \brief Free memory allocated through alignedMalloc()
         * \param aligned The memory to free
         */
        inline void alignedFree(void* aligned)
        {
            if (!aligned)
            {
                return;
            }

            void* raw = *(void**)((uint8_t*)aligned - sizeof(void*));
            std::free(raw);
        }

        template<typename T, PDP::UInteger Alignment>
        class AlignedStorage
        {
        private:


        public:
        };

        template<typename T>
        class CacheAlignedAllocator
        {
        private:

        public:

            typedef T ValueType;
            typedef ValueType* Pointer;
            typedef const ValueType* ConstPointer;
            typedef ValueType& Reference;
            typedef const ValueType& ConstReference;
            typedef UInteger SizeType;

            inline Pointer Allocate (SizeType n, ConstPointer  hint=0);

            inline void Deallocate (Pointer p, SizeType n);

            inline void DeAllocate(Pointer p, SizeType n);

            inline void Construct(Pointer p, const T& t) const;

            inline void Destroy(Pointer p);


            inline Pointer Address(Reference r);

            inline ConstPointer ConstAddress(ConstReference r);

            template <class U>
            struct Rebind { typedef CacheAlignedAllocator<U> Other; };
        };


        template<typename T> typename CacheAlignedAllocator<T>::Pointer CacheAlignedAllocator<T>::Allocate(SizeType n, ConstPointer hint)
        {
            return alignedMalloc(n, PDP::CacheLineSize());
        }

        template<typename T> void CacheAlignedAllocator<T>::Deallocate(Pointer p, SizeType n)
        {
            alignedFree(p);
        }


        template<typename T> void CacheAlignedAllocator<T>::Construct(Pointer p, const T &t) const
        {

            *p = t;
        }

        template<typename T> void CacheAlignedAllocator<T>::Destroy(Pointer p)
        {
            //p->~T();
        }

        template<typename T> typename CacheAlignedAllocator<T>::Pointer CacheAlignedAllocator<T>::Address(Reference r)
        {
            return &r;
        }

        template<typename T> typename CacheAlignedAllocator<T>::ConstPointer CacheAlignedAllocator<T>::ConstAddress(ConstReference r)
        {
            return &r;
        }

        template<typename T>
        class CacheAlignedStorage
        {
        private:
            T * Object;
        public:
            inline CacheAlignedStorage();
            inline CacheAlignedStorage(const T & object);
            inline CacheAlignedStorage(const CacheAlignedStorage & storage);
            inline CacheAlignedStorage(CacheAlignedStorage && storage);
            ~CacheAlignedStorage();

            inline CacheAlignedStorage & operator = (const T & object);
            inline CacheAlignedStorage & operator = (const CacheAlignedStorage & storage);
            inline CacheAlignedStorage & operator = (CacheAlignedStorage && storage);


            inline T * operator ->();
            inline const T * operator ->() const;

            inline T & operator*();
            inline const T & operator*() const;

        };


        template<typename T>
        CacheAlignedStorage<T>::CacheAlignedStorage():Object(nullptr)
        {

        }

        template<typename T>
        CacheAlignedStorage<T>::CacheAlignedStorage(const T & object):Object(nullptr)
        {
            (*this) = object;
        }

        template<typename T>
        CacheAlignedStorage<T>::CacheAlignedStorage(const CacheAlignedStorage<T> & storage):Object(nullptr)
        {
            (*this) = storage;
        }

        template<typename T>
        CacheAlignedStorage<T>::CacheAlignedStorage(CacheAlignedStorage<T> && storage):Object(nullptr)
        {
            (*this) = storage;
        }

        template<typename T>
        CacheAlignedStorage<T>::~CacheAlignedStorage<T>()
        {
            alignedFree(this->Object);
        }


        template<typename T>
        CacheAlignedStorage<T> & CacheAlignedStorage<T>::operator=(const T &object)
        {
            if (this->Object == nullptr)
            {
                this->Object = (T*)alignedMalloc(sizeof(T), PDP::CacheLineSize());
            }


            *(this->Object) = object;
            return (*this);
        }

        template<typename T>
        CacheAlignedStorage<T> & CacheAlignedStorage<T>::operator=(const CacheAlignedStorage<T> &storage)
        {
            if (&storage == this || storage.Object == nullptr)
            {
                return (*this);
            }


            alignedFree(this->Object);

            this->Object = (T*)alignedMalloc(sizeof(T), PDP::CacheLineSize());

            *(this->Object) = *(storage.Object);

            return (*this);
        }


        template<typename T>
        CacheAlignedStorage<T> & CacheAlignedStorage<T>::operator=(CacheAlignedStorage<T> &&storage)
        {
            if (&storage == this || storage.Object == nullptr)
            {
                return (*this);
            }

            alignedFree(this->Object);
            this->Object = storage.Object;
            storage.Object = nullptr;
            return (*this);
        }

        template<typename T>
        T * CacheAlignedStorage<T>::operator->()
        {
            return this->Object;
        }

        template<typename T>
        const T * CacheAlignedStorage<T>::operator->() const
        {
            return this->Object;
        }

        template<typename T>
        T & CacheAlignedStorage<T>::operator*()
        {
            return *this->Object;
        }



        template<typename T>
        const T & CacheAlignedStorage<T>::operator*() const
        {
            return *this->Object;
        }
    }
}
#endif
