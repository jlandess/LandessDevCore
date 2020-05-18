//
//  UnderFlowException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__UnderFlowException__
#define __DataStructures__UnderFlowException__

#include <Exceptions/Exception.h>

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
        class UnderFlowException: public Exception
        {
        public:
            UnderFlowException(const char * message, const Integer code);
            UnderFlowException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__UnderFlowException__) */
