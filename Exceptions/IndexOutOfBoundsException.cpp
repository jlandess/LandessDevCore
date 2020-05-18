//
//  IndexOutOfBoundsException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
#include <Exceptions/IndexOutOfBoundsException.h>
namespace PDP
{
    namespace DataStructures
    {
        IndexOutOfBoundsException::IndexOutOfBoundsException(const char * message, const Integer code):Exception(message,code)
        {
            
        }
        IndexOutOfBoundsException::IndexOutOfBoundsException(const char * message):Exception(message)
        {
            
        }
    }
}