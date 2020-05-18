//
//  UnderFlowException.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
#include <Exceptions/UnderFlowException.h>
namespace PDP
{
    namespace DataStructures
    {
        UnderFlowException::UnderFlowException(const char * message, const Integer code):Exception(message,code)
        {
            
        }
        UnderFlowException::UnderFlowException(const char * message):Exception(message)
        {
            
        }
    }
}