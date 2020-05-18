//
//  InvalidOperationException.cpp
//  DataStructures
//
//  Created by James Landess on 10/6/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#include <Exceptions/InvalidOperationException.h>

namespace PDP
{
    namespace DataStructures
    {
        InvalidOperationException::InvalidOperationException(const char * message, const Integer code):PDP::DataStructures::Exception(message,code)
        {
            
        }
        
        InvalidOperationException::InvalidOperationException(const char * message):PDP::DataStructures::Exception(message)
        {
            
        }
    }
}