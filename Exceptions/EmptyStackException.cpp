//
//  EmptyStackException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
#include <Exceptions/EmptyStackException.h>
namespace PDP
{
    namespace DataStructures
    {
        EmptyStackException::EmptyStackException(const char * message, const Integer code):UnderFlowException(message,code)
        {
            
        }
        EmptyStackException::EmptyStackException(const char * message):UnderFlowException(message)
        {
            
        }
    }
}