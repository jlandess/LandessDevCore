//
//  OverFlowException.cpp
//  DataStructures
//
//  Created by James Landess on 10/30/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//
#include <Exceptions/OverFlowException.h>
namespace PDP
{
    namespace DataStructures
    {
        OverFlowException::OverFlowException(const char * message, const Integer code):Exception(message,code)
        {
            
        }
        OverFlowException::OverFlowException(const char * message):Exception(message)
        {
            
        }
    }
}