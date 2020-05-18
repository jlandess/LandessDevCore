//
//  NoSuchElementException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__NoSuchElementException__
#define __DataStructures__NoSuchElementException__

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
        class NoSuchElementException: public UnderFlowException
        {
        public:
            NoSuchElementException(const char * message, const Integer code);
            NoSuchElementException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__NoSuchElementException__) */
