//
//  Integer.h
//  DataStructures
//
//  Created by James Landess on 10/8/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef DataStructures_Integer_h
#define DataStructures_Integer_h


namespace PDP
{
#if defined(_WIN64) || defined(__LP64__)
    
    typedef long Integer;
    typedef unsigned long UInteger;
    
#else
    typedef long Integer;
    typedef unsigned int UInteger;
    
#endif
    typedef short Short;
    typedef unsigned short UShort;
    typedef unsigned int UInt;
    typedef int Int;
}

namespace LD
{
#if defined(_WIN64) || defined(__LP64__)

    typedef long Integer;
    typedef unsigned long UInteger;

#else
    typedef long Integer;
    typedef unsigned int UInteger;

#endif
    typedef short Short;
    typedef unsigned short UShort;
    typedef unsigned int UInt;
    typedef int Int;
}

#endif
