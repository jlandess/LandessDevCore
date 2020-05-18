//
//  ConcurrentVector.h
//  DataStructures
//
//  Created by James Landess on 6/18/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_ConcurrentVector_h
#define DataStructures_ConcurrentVector_h

#include <Memory/ElementReference.h>
#include <atomic>
#include <Definitions/Integer.h>
namespace PDP
{
    template<typename T>
    class ConcurrentVectorPair
    {
    private:
        std::atomic<bool> IsCurrentlyOwned;
        T CurrentObject;
        
        template<typename U> friend class ConcurrentVector;
        
    };
    
    template<typename T>
    class ConcurrentVectorPairReference
    {
    private:
        PDP::ElementReference<std::atomic<bool>> CurrentAtomicReference;
        T * Object;;
    public:
        ConcurrentVectorPairReference(std::atomic<bool> * atomicReference, T * object):CurrentAtomicReference(atomicReference),Object(object)
        {
            
        }
        
        ~ConcurrentVectorPairReference()
        {
            CurrentAtomicReference->store(false);
        }
        
        
        
    };
    
    
    template<typename T>
    class ConstConcurrentVectorPairReference
    {
    private:
        PDP::ElementReference<std::atomic<bool>> CurrentAtomicReference;
        const T * Object;
    public:
        ConstConcurrentVectorPairReference(std::atomic<bool> * atomicReference, const T * object):CurrentAtomicReference(atomicReference),Object(object)
        {
            
        }
        
        ~ConstConcurrentVectorPairReference()
        {
            if(CurrentAtomicReference.IsValid())
            {
                CurrentAtomicReference->store(false);
            }
            
        }
        
        
        
    };
    
    template<typename T>
    class ConcurrentVector
    {
    private:
        ConcurrentVectorPair<T> * Buffer;
        std::atomic<PDP::UInteger> Size;
        std::atomic<bool> OwnerShip;
    public:
        
        ConcurrentVector()
        {
            OwnerShip.store(false);
        }
        
        ConcurrentVectorPairReference<T>  operator[](const PDP::UInteger & index)
        {
            //someone already owns the index
            if (Buffer[index].IsCurrentlyOwned.load() == true || OwnerShip.load() == true)
            {
                return ConcurrentVectorPairReference<T>(nullptr,nullptr);
                
            }else
            {
                Buffer[index].IsCurrentlyOwned.store(true);
                
                
                
                
                return ConcurrentVectorPairReference<T>(&Buffer[index].IsCurrentlyOwned,&Buffer[index]);
            }
            
        }
        
        void PushBack(const T & object)
        {
            
        }
        
        void Reserve(const PDP::UInteger & amount)
        {
            
        }
        
        void Remove(const PDP::UInteger & index)
        {
            
        }
        
        const ConstConcurrentVectorPairReference<T>  operator[](const PDP::UInteger & index) const
        {
            if (Buffer[index].IsCurrentlyOwned.load() == true || OwnerShip.load() == true)
            {
                return ConstConcurrentVectorPairReference<T>(nullptr,nullptr);
                
            }else
            {
                Buffer[index].IsCurrentlyOwned.store(true);
                
                
                
                
                return ConstConcurrentVectorPairReference<T>(&Buffer[index].IsCurrentlyOwned,&Buffer[index]);
            }
            
        }
        
        const PDP::UInteger GetSize() const
        {
            return Size.load();
        }
        
        void Resize(const PDP::UInteger & size)
        {
            
        }
        
        
    };
}
#endif
