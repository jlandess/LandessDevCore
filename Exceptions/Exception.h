//
//  Exception.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__Exception__
#define __DataStructures__Exception__

#include "Definitions/Integer.hpp"
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
        class Exception
        {
        private:
            const char * Message;
            Integer Code;
        public:
            Exception();
            Exception(const char * message, const Integer code);
            Exception(const char * message);
            const char*  What() const;
            const Integer & ErrorCode() const;
            
        };
        /**
         @}
         */
        /**
         @}
         */
    }
    
}

#endif /* defined(__DataStructures__Exception__) */
