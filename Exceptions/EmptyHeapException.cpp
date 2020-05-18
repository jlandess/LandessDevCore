//
//  EmptyHeapException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#include <Exceptions/EmptyHeapException.h>
namespace PDP
{
    namespace DataStructures
    {
        EmptyHeapException::EmptyHeapException(const char * message, const Integer code):UnderFlowException(message,code)
        {
            
        }
        EmptyHeapException::EmptyHeapException(const char * message):UnderFlowException(message)
        {
            
        }
    }
}