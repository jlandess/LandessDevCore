//
//  MemoryException.cpp
//  DataStructures
//
//  Created by James Landess on 10/3/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#include <Exceptions/MemoryException.h>

namespace PDP
{
    namespace DataStructures
    {
        MemoryException::MemoryException(const char * message, const Integer code):PDP::DataStructures::Exception(message,code)
        {
            
        }
        
        MemoryException::MemoryException(const char * message):PDP::DataStructures::Exception(message)
        {
            
        }
    }
}