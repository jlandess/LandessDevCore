//
//  OverFlowException.h
//  DataStructures
//
//  Created by James Landess on 10/30/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__OverFlowException__
#define __DataStructures__OverFlowException__

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
        class OverFlowException: public Exception
        {
        public:
            OverFlowException(const char * message, const Integer code);
            OverFlowException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__OverFlowException__) */
