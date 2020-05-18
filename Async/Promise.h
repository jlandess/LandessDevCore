//
//  Promise.h
//  DataStructures
//
//  Created by James Landess on 6/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Promise_h
#define DataStructures_Promise_h
#include <atomic>
#include "Memory/AutomaticReferenceCounting.h"
#include "Async/Future.h"
#include "Async/Locks/SpinLock.h"
#include "Async/Atomic/Atomic.h"
#include "Memory/shared_ptr.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Memory/ElementReference.h"
#include <memory>
//#include <memory>
namespace PDP
{
    /**
     @tparam T - The type to utilize - By default an atomic spin lock will be used but in certain cases where the underlying class only has atomic operations - a Spinlock will not be cessary
     */
    template<typename T, class = void>
    class SharedFuture: public PDP::Memory::AutomaticReferencedCountingObject
    {
    private:
        //Future<T> ReturnValue;
        T * ObjectBuffer;
        
        //bool CompletedStatus;
        //bool IsReady;
        
        
        //PDP::Atomic<PDP::UInteger> ExceptionType;//to dynamically detect, set, and prepare to re-throw exceptions created on spawned threads to the threads which invoked them.  Multi-Core Exception Technology
        //one pointer will be required for each type exception - std::exception, PDP::Exception, and so on.  Those two will probably be the only supported types
        
        
        PDP::Atomic<PDP::UInteger> IsReadyStatus;
        
        mutable PDP::SpinLock CurrentLock;
    public:
        
        
        inline SharedFuture()
        {
            
            
            IsReadyStatus.store(false,PDP::AcquireRelease);

            CurrentLock.Lock();
            this->ObjectBuffer = new T();
            CurrentLock.Unlock();


            
            /*
             CurrentLock.Lock();
             {
             IsReady = false;
             }
             CurrentLock.Unlock();
             */
        }
        
        
        inline ~SharedFuture()
        {

            CurrentLock.Lock();
            delete this->ObjectBuffer;
            CurrentLock.Unlock();

        }
        
        //const Future<T> & GetFuture() const {return this->ReturnValue;}
        Future<T>  GetFuture() const
        {
            Future<T> stuffings;
            // bool readyIndicator = false;


            {
             //readyIndicator = IsReady;
             if(IsReadyStatus.load(PDP::AcquireRelease) == true)
             {
                 CurrentLock.Lock();
                 stuffings = Future<T>(*ObjectBuffer);
                 CurrentLock.Unlock();
             }


            }


            /*
            
            if (IsReadyStatus.load(PDP::AcquireRelease) == true)
            {
                return PDP::Future<T>(*ObjectBuffer);
            }
            //return Future<T>();
             */

            return stuffings;
        }


        
        inline void SetValue(const T & value)
        {
            /*
             CurrentLock.Lock();
             {
             IsReady = true;
             this->ReturnValue = PDP::Future<T>(value);
             }
             CurrentLock.Unlock();
             */
            
            
            IsReadyStatus.store(true,PDP::AcquireRelease);
            this->CurrentLock.Lock();
            *this->ObjectBuffer = value;
            this->CurrentLock.Unlock();
            
            
        }
        
        /*
         inline void SetReadyStatus(const bool & status)
         {
         CurrentLock.Lock();
         {
         IsReady = status;
         }
         CurrentLock.Unlock();
         
         }
         */
        
        const bool GetReadyStatus() const
        {
            /*
             bool readyIndicator = false;
             CurrentLock.Lock();
             {
             readyIndicator = IsReady;
             }
             CurrentLock.Unlock();
             */
            //std::cout << "IsReadyStatus: " <<  IsReadyStatus.load() << std::endl;
            return IsReadyStatus.load(PDP::AcquireRelease);
        }
        
        
        template<typename U> friend class Promise;
    };
    
    class BrokenPromise
    {

    };
    /**
     @tparam T - The type to utilize - By default an atomic spin lock will be used but in certain cases where the underlying class only has atomic operations - a Spinlock will not be cessary
     */
    template<typename T>
    class Promise
    {
    private:
        PDP::SharedPointer<SharedFuture<T>> CurrentSharedFuture;

        //PDP::Memory::AutomaticReferenceCounter<SharedFuture<T>> CurrentSharedFuture;

        //std::shared_ptr<SharedFuture<T>> CurrentSharedFuture;

        PDP::Atomic<PDP::UInteger> BrokenStatus;
        //PDP::Atomic<PDP::UInteger > Ready;
    public:
        
        inline Promise()
        {
            //this->CurrentSharedFuture =  PDP::SharedPointer<SharedFuture<T>>(new SharedFuture<T>());

            this->CurrentSharedFuture = PDP::MakeShared<SharedFuture<T>>();
            //this->CurrentSharedFuture = PDP::Memory::AutomaticReferenceCounter<SharedFuture<T>>( new SharedFuture<T>());

            //this->CurrentSharedFuture = std::make_shared<SharedFuture<T>>();
            this->BrokenStatus.store(false,PDP::AcquireRelease);
        }
        
        
        inline void SetValue(const T & value)
        {

            //this->Shared = std::make_shared<T>(value);
            this->CurrentSharedFuture->SetValue(value);
        }
        
        inline void ExceptionWasThrown()
        {
            this->BrokenStatus.store(true,PDP::AcquireRelease);
        }

        explicit  operator LD::Variant<LD::NullClass,T,PDP::BrokenPromise> () const
        {
            mapbox::util::variant<LD::NullClass,T,PDP::BrokenPromise> currentState;
            if(this->HasCompleted())
            {
                currentState = this->GetFuture().Get();
            }else if(this->WasBroken())
            {
                currentState = PDP::BrokenPromise{};
            }
            return currentState;
        }

        LD::Variant<LD::NullClass,T,PDP::BrokenPromise> GetVariant() const
        {
            mapbox::util::variant<LD::NullClass,T,PDP::BrokenPromise> currentState;
            if(this->HasCompleted())
            {
                currentState = this->GetFuture().Get();
            }else if(this->WasBroken())
            {
                currentState = PDP::BrokenPromise{};
            }
            return currentState;
        }
        inline const bool WasBroken() const
        {
            return this->BrokenStatus.load(PDP::AcquireRelease);
        }


        const bool HasCompleted() const
        {
            return  this->CurrentSharedFuture->GetReadyStatus();
            //return this->Shared != nullptr;
        }

        
        const PDP::Future<T> GetFuture() const
        {
            PDP::Future<T> currentFuture = CurrentSharedFuture->GetFuture();
            return currentFuture;
            
        }


        
    };
    
    template<>
    class Promise<void>
    {
    private:
        PDP::SharedPointer<SharedFuture<PDP::UInteger>> CurrentSharedFuture;
        PDP::Atomic<PDP::UInteger> BrokenStatus;
    public:
        
        inline void ExceptionWasThrown()
        {
            this->BrokenStatus.store(true,PDP::AcquireRelease);
        }
        
        inline const bool WasBroken() const
        {
            return this->BrokenStatus.load(PDP::AcquireRelease);
        }
        
        
        const bool HasCompleted() const
        {
            return  this->CurrentSharedFuture->GetReadyStatus();
        }
        
        const PDP::Future<PDP::UInteger> GetFuture() const
        {
            PDP::Future<PDP::UInteger> currentFuture = CurrentSharedFuture->GetFuture();
            return currentFuture;
            
        }
    };
}
#endif

