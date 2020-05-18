//
//  EmptyQueueException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
#include <Exceptions/EmptyQueueException.h>
namespace PDP
{
    namespace DataStructures
    {
        EmptyQueueException::EmptyQueueException(const char * message, const Integer code):UnderFlowException(message,code)
        {
            
        }
        EmptyQueueException::EmptyQueueException(const char * message):UnderFlowException(message)
        {
            
        }
    }
}