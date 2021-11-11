//
//  OptionalReference.h
//  DataStructures
//
//  Created by James Landess on 3/20/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_OptionalReference_h
#define DataStructures_OptionalReference_h

#include <Memory/Optional.h>
#include <Memory/ElementReference.h>

namespace LD
{
    template<typename T>
    class OptionalReference: public PDP::Optional<PDP::ElementReference<T>>
    {
    public:
        
        inline OptionalReference(): PDP::Optional<PDP::ElementReference<T>>()
        {
            
        }
        
        inline OptionalReference(const PDP::ElementReference<T> & elementReference): PDP::Optional<PDP::ElementReference<T>>(elementReference)
        {
            
        }
    };
}
#endif
