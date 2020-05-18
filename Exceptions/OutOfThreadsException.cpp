//
//  OutOfThreadsException.cpp
//  Multithreading
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//
#include <Exceptions/OutOfThreadsException.h>

namespace PDP
{
    namespace MultiThreading
    {
        OutOfThreadsException::OutOfThreadsException(const char * message, const Integer code):PDP::DataStructures::OverFlowException(message,code)
        {
            
        }
        
        OutOfThreadsException::OutOfThreadsException(const char * message): PDP::DataStructures::OverFlowException(message)
        {
            
        }
    }
}