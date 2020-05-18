//
//  Exception.cpp
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#include <Exceptions/Exception.h>
namespace PDP
{
    namespace DataStructures
    {
        Exception::Exception()
        {
            this->Code = 0;
        }
        Exception::Exception(const char * message, const Integer code):Message(message),Code(code)
        {
        }
        Exception::Exception(const char * message):Message(message),Code(0)
        {
            
        }
        const char*  Exception::What() const
        {
            return this->Message;
        }
        const Integer & Exception::ErrorCode() const
        {
            return this->Code;
        }
    }
}