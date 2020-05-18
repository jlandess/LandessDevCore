//
//  NoSuchElementException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#include <Exceptions/NoSuchElementException.h>
namespace PDP
{
    namespace DataStructures
    {
        NoSuchElementException::NoSuchElementException(const char * message, const Integer code):UnderFlowException(message,code)
        {
            
        }
        NoSuchElementException::NoSuchElementException(const char * message):UnderFlowException(message)
        {
            
        }
    }
}