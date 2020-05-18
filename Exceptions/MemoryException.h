//
//  MemoryException.h
//  DataStructures
//
//  Created by James Landess on 10/3/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __DataStructures__MemoryException__
#define __DataStructures__MemoryException__

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
        class MemoryException: public Exception
        {
        private:
            bool MemoryAllocationError;
            
        public:
            MemoryException(const char * message, const Integer code);
            MemoryException(const char * message);
        };
        /**
         @}
         */
        /**
         @}
         */
    }
}


#endif /* defined(__DataStructures__MemoryException__) */
