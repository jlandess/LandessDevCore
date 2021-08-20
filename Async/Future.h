//
//  Future.h
//  DataStructures
//
//  Created by James Landess on 6/11/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Future_h
#define DataStructures_Future_h
#include <atomic>
#include <Memory/Optional.h>
#include <Memory/AutomaticReferenceCounting.h>
namespace LD
{
    
    template<typename T>
    class Future
    {
    private:
        //PDP::Optional<T> OptionalData;
        //T * Object;
        
        PDP::Optional<T> OptionalData;
    public:
        Future()//:Object(nullptr)
        {
            
        }
        
        
        Future(const T  value)
        {
            OptionalData = PDP::Optional<T>(value);
            //Object = (T*)value;
        }
        
        const bool IsValid() const
        {
            return OptionalData.has_value();
        }

        
        inline T &  Get()
        {
            //return *this->Object;
            return *OptionalData;
        }
        
        inline const T &  Get() const
        {
            //return *this->Object;
            
            return *OptionalData;
        }
        
        
        
        
        template<typename U> friend class Promise;
    };
}
#endif
