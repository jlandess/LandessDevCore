//
//  OptionalReference.h
//  DataStructures
//
//  Created by James Landess on 3/20/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_OptionalReference_h
#define DataStructures_OptionalReference_h

#include "Optional.h"
#include "ElementReference.h"

namespace LD
{
    template<typename T>
    class Optional<T&>
    {
    private:
        LD::ElementReference<T> mReference;
    public:
        Optional() noexcept:mReference{nullptr}{}
        Optional(T * object) noexcept:mReference{object}{}
        Optional(T & object) noexcept:mReference{object}{}

        T & operator*() noexcept{ return *mReference;}

        const T & operator*() const noexcept{ return *mReference;}
    };
    template<typename T>
    class OptionalReference: public LD::Optional<PDP::ElementReference<T>>
    {
    public:
        
        inline OptionalReference(): LD::Optional<PDP::ElementReference<T>>()
        {
            
        }
        
        inline OptionalReference(const PDP::ElementReference<T> & elementReference): LD::Optional<PDP::ElementReference<T>>(elementReference)
        {
            
        }
    };


}
#endif
