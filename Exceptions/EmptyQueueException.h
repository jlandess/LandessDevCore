//
//  EmptyQueueException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__EmptyQueueException__
#define __DataStructures__EmptyQueueException__

#include <Exceptions/UnderFlowException.h>

namespace PDP
{
    namespace DataStructures
    {
        /**
         @addtogroup DataStructures
         *@{
         */
        /**
         @addtogroup Exceptions
         @{
         */
        class EmptyQueueException: public UnderFlowException
        {
        public:
            EmptyQueueException(const char * message, const Integer code);
            EmptyQueueException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__EmptyQueueException__) */
