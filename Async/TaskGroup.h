//
//  TaskGroup.h
//  DataStructures
//
//  Created by James Landess on 5/6/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_TaskGroup_h
#define DataStructures_TaskGroup_h

#include <Primitives/Threads/ThreadPool.h>
#include <Memory/ElementReference.h>

namespace PDP
{
    class TaskGroup
    {
    private:
        PDP::ElementReference<PDP::ThreadPool> ThreadPoolReference;
    public:
    };
}

#endif
