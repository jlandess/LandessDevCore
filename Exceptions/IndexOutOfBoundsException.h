//
//  IndexOutOfBoundsException.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__IndexOutOfBoundsException__
#define __DataStructures__IndexOutOfBoundsException__

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
        class IndexOutOfBoundsException: public Exception
        {
        public:
            IndexOutOfBoundsException(const char * message, const Integer code);
            IndexOutOfBoundsException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}


#endif /* defined(__DataStructures__IndexOutOfBoundsException__) */
