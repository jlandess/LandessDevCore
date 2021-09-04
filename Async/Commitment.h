//
//  LongTermPromise.h
//  DataStructures
//
//  Created by James Landess on 6/14/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef __DataStructures__LongTermPromise__
#define __DataStructures__LongTermPromise__

#include "Async/Atomic/Atomic.h"
#include "Memory/AutomaticReferenceCounting.h"
#include "Async/Locks/SpinLock.h"
#include "Future.h"
#include "concurrentqueue.h"
#include "Functor/LightWeightDelegate.h"
#include "Memory/Optional.h"
#include "Primitives/General/mapboxvariant.hpp"


namespace LD
{
    /**
     @tparam T - The expected value to be streamed through the committed future
     @brief - The implementation of PDP::Commitment
     */
    template<typename T>
    class SharedCommittedFuture : public PDP::Memory::AutomaticReferencedCountingObject
    {
    private:
        LD::Atomic<PDP::UInteger> BrokenStatus;//indicates weather the indicated commitment has been broken with the calling thread
        LD::Atomic<PDP::UInteger> Workload;//due to the fact that a singular commitment can be shared across multiple asyncrhonous functions we need a way to indicate when the commitment has been fufilled
        LD::Atomic<PDP::UInteger> ExceptionType;//to dynamically detect, set, and prepare to re-throw exceptions created on spawned threads to the threads which invoked them.  Multi-Core Exception Technology
        //one pointer will be required for each type exception - std::exception, PDP::Exception, and so on.  Those two will probably be the only supported types
        moodycamel::ConcurrentQueue<T> * DataToStream;
    public:
        
        
        inline SharedCommittedFuture();
        inline ~SharedCommittedFuture();
        
        inline void AddItem(const T & item);
        
        
        inline void SetExceptionOccured();
        
        inline void IndicateFufillment()
        {
            ++this->Workload;
        }
        
        inline void IndicateRequestToBeFufilled()
        {
            --this->Workload;
        }
        
        const bool HasBeenFufilled() const
        {
            return this->Workload.load(LD::AcquireRelease) == 0;
        }
        
        inline const bool WasBroken() const;
        
        
        inline const PDP::Optional<T> GetItem() const;
        
    };
    
    template<>
    class SharedCommittedFuture<void> : public PDP::Memory::AutomaticReferencedCountingObject
    {
    private:
        LD::Atomic<PDP::UInteger> BrokenStatus;//indicates weather the indicated commitment has been broken with the calling thread
        LD::Atomic<PDP::UInteger> Workload;//due to the fact that a singular commitment can be shared across multiple asyncrhonous functions we need a way to indicate when the commitment has been fufilled
        LD::Atomic<PDP::UInteger> ExceptionType;//to dynamically detect, set, and prepare to re-throw exceptions created on spawned threads to the threads which invoked them.  Multi-Core Exception Technology
        //one pointer will be required for each type exception - std::exception, PDP::Exception, and so on.  Those two will probably be the only supported types
        moodycamel::ConcurrentQueue<bool> * DataToStream;
        
    public:
        
        inline SharedCommittedFuture();
        inline ~SharedCommittedFuture();
        
        inline void AddItem(const bool & item);
        
        
        inline void SetExceptionOccured();
        
        inline void IndicateFufillment()
        {
            ++this->Workload;
        }
        
        inline void IndicateRequestToBeFufilled()
        {
            --this->Workload;
        }
        
        const bool HasBeenFufilled() const
        {
            return this->Workload.load(LD::AcquireRelease) == 0;
        }
        
        inline const bool WasBroken() const;
        
        
        inline const PDP::Optional<bool> GetItem() const;
    };
    
    template<typename T>
    SharedCommittedFuture<T>::SharedCommittedFuture()
    {
        this->DataToStream = new moodycamel::ConcurrentQueue<T>();
        this->BrokenStatus.store(false,LD::AcquireRelease);
        this->Workload.store(0,LD::AcquireRelease);
    }
    
    template<typename T>
    SharedCommittedFuture<T>::~SharedCommittedFuture()
    {
        delete this->DataToStream;
    }
    
    template<typename T>
    void SharedCommittedFuture<T>::AddItem(const T &item)
    {
        //printf("being called\n");
        DataToStream->enqueue(item);//add the task to the queue which will be executed at some point in the future
    }
    
    template<typename T>
    inline void SharedCommittedFuture<T>::SetExceptionOccured()
    {
        this->BrokenStatus.store(true,LD::AcquireRelease);
    }
    
    template<typename T>
    inline const bool SharedCommittedFuture<T>::WasBroken() const
    {
        return this->BrokenStatus.load(LD::AcquireRelease);
    }
    
    template<typename T>
    inline const PDP::Optional<T> SharedCommittedFuture<T>::GetItem() const
    {
        //DataToStream->try_dequeue(possibleItem);

        T possibleItem;
        
        if(DataToStream->try_dequeue(possibleItem))
        {
            return PDP::Optional<T>(possibleItem);
        }

        return PDP::Optional<T>();
    }
    class BrokenCommitment
    {

    };
    /**
     @tparam T - The expected type you want to stream between threads
     @brief This class is expected to be utilized when you want to stream data between threads.  For instance when you're downloading a file or performing an intensive task which may block your main thread.
     */
    template<typename T>
    class Commitment
    {
    private:
        //PDP::Memory::AutomaticReferenceCounter<SharedCommittedFuture<T>> CurrentCommittedFuture;

        std::shared_ptr<moodycamel::ConcurrentQueue<T>> CurrentCommittedFuture;

        LD::Atomic<PDP::UInteger> BrokenStatus;
    public:
        
        inline Commitment() noexcept:CurrentCommittedFuture{new moodycamel::ConcurrentQueue<T>{}}
        {

        }
        
        
        inline void AddItem(const T & item)
        {
            this->CurrentCommittedFuture->enqueue(item);
        }
        
        /**
         @brief This provides the thread executing the code a mechanism to indicate that something has gone wrong and communicate it to the calling thread
         */
        inline void SetExceptionOccured()
        {

        }
        
        /**
         @brief This provides the calling thread a mechanism to be informed that something has gone wrong and allows for you to be able to handle it in the calling thread
         */
        inline const bool WasBroken() const
        {

        }
        
        /**
         @return PDP::Optional - The possible value that may return from the function, in the case that nothing is ready to be streaming, you'll simply get an empty PDP::Optional object
         @brief This is necessary because due to the how multi-core IPC works it is not always on time and may take a few extra cycles to execute in order to get the necessary information transferred from one core to the other.
         */
        inline const PDP::Optional<T> GetItem() const
        {
            //return this->CurrentCommittedFuture->GetItem();
            //return {};
            T possibleItem;


            if(this->CurrentCommittedFuture->try_dequeue(possibleItem))
            {
                return PDP::Optional<T>(possibleItem);
            }


            return PDP::Optional<T>();
        }

        explicit  operator mapbox::util::variant<LD::NullClass,PDP::Optional<T>,LD::BrokenCommitment> () const
        {
            mapbox::util::variant<LD::NullClass,PDP::ElementReference<T>,LD::BrokenCommitment> currentState;
            if(this->HasCompleted())
            {
                currentState = this->CurrentCommittedFuture->GetItem();

            }else if(this->WasBroken())
            {
                currentState = LD::BrokenCommitment{};
            }
            return currentState;
        }

        
        inline void IndicateFufillment()
        {
            return this->CurrentCommittedFuture->IndicateFufillment();
        }
        
        inline void IndicateRequestToBeFufilled()
        {
            return this->CurrentCommittedFuture->IndicateRequestToBeFufilled();
        }
        
        const bool HasBeenFufilled() const
        {
            return this->CurrentCommittedFuture->HasBeenFufilled();
        }
        
    };

    template<>
    class Commitment<void>
    {
    private:
        PDP::Memory::AutomaticReferenceCounter<SharedCommittedFuture<bool>> CurrentCommittedFuture;
    public:
        
        
        
        
        inline Commitment(){}
        
        
        inline void AddItem(const bool & item){return this->CurrentCommittedFuture->AddItem(item);}
        
        /**
         @brief This provides the thread executing the code a mechanism to indicate that something has gone wrong and communicate it to the calling thread
         */
        inline void SetExceptionOccured();
        
        /**
         @brief This provides the calling thread a mechanism to be informed that something has gone wrong and allows for you to be able to handle it in the calling thread
         */
        inline const bool WasBroken() const;
        
        /**
         @return PDP::Optional - The possible value that may return from the function, in the case that nothing is ready to be streaming, you'll simply get an empty PDP::Optional object
         @brief This is necessary because due to the how multi-core IPC works it is not always on time and may take a few extra cycles to execute in order to get the necessary information transferred from one core to the other.
         */
        inline const PDP::Optional<bool> GetItem() const;
        
        
        
        inline void IndicateFufillment()
        {
            return this->CurrentCommittedFuture->IndicateFufillment();
        }
        
        inline void IndicateRequestToBeFufilled()
        {
            return this->CurrentCommittedFuture->IndicateRequestToBeFufilled();
        }
        
        const bool HasBeenFufilled() const
        {
            return this->CurrentCommittedFuture->HasBeenFufilled();
        }
    };

    /*
    template<typename T>
    void Commitment<T>::AddItem(const T & item)
    {
        return this->CurrentCommittedFuture->AddItem(item);
    }
    
    template<typename T>
    void Commitment<T>::SetExceptionOccured()
    {
        return this->CurrentCommittedFuture->SetExceptionOccured();
    }
    
    
    template<typename T>
    const bool Commitment<T>::WasBroken() const
    {
        return this->CurrentCommittedFuture->WasBroken();
    }
     */
    

    
    
    
}

#endif /* defined(__DataStructures__LongTermPromise__) */
