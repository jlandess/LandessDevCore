//
//  OutOfThreadsException.h
//  Multithreading
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __Multithreading__OutOfThreadsException__
#define __Multithreading__OutOfThreadsException__

#include <stdio.h>
#include "OverFlowException.h"
//#include "Definitions/Common.hpp"

namespace PDP
{
    namespace MultiThreading
    {

        class OutOfThreadsException: public PDP::DataStructures::OverFlowException
        {
        public:
            OutOfThreadsException(const char * message, const Integer code);
            OutOfThreadsException(const char * message);
        };
    }
}
#endif /* defined(__Multithreading__OutOfThreadsException__) */
