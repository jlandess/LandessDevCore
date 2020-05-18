//
//  EmptyHeapException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__EmptyHeapException__
#define __DataStructures__EmptyHeapException__

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
        class EmptyHeapException: public UnderFlowException
        {
        public:
            EmptyHeapException(const char * message, const Integer code);
            EmptyHeapException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__EmptyHeapException__) */
