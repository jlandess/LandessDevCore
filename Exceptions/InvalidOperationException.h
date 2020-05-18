//
//  InvalidOperationException.h
//  DataStructures
//
//  Created by James Landess on 10/6/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __DataStructures__InvalidOperationException__
#define __DataStructures__InvalidOperationException__

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
        class InvalidOperationException: public Exception
        {
        public:
            InvalidOperationException(const char * message, const Integer code);
            InvalidOperationException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}
#endif /* defined(__DataStructures__InvalidOperationException__) */
