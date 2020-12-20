//
//  EmptyStackException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__EmptyStackException__
#define __DataStructures__EmptyStackException__


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
        class EmptyStackException: public UnderFlowException
        {
        public:
            EmptyStackException(const char * message, const Integer code);
            EmptyStackException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}

#endif /* defined(__DataStructures__EmptyStackException__) */
