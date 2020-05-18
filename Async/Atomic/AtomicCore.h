//
//  AtomicCore.h
//  DataStructures
//
//  Created by James Landess on 7/10/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_AtomicCore_h
#define DataStructures_AtomicCore_h

#include <Definitions/platform_detect.h>

namespace AtomicTypes
{
    typedef char s8;
    typedef unsigned char u8;
    typedef short s16;
    typedef unsigned short u16;
    typedef int s32;
    typedef unsigned int u32;
    // FIXME: Define s64/u64 on compilers without long long
//#if TURF_HAS_LONG_LONG
    typedef long long s64;
    typedef unsigned long long u64;
//#endif
#if TURF_PTR_SIZE == 4
    //typedef s32 sptr;
    //typedef u32 uptr;
    //typedef s32 sreg;
    //typedef u32 ureg;
#else
    typedef s64 sptr;
    typedef u64 uptr;
    typedef s64 sreg;
    typedef u64 ureg;
#endif
}
#endif
