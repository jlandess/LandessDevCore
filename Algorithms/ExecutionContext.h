//
// Created by phoenixflower on 9/8/21.
//

#ifndef LANDESSDEVCORE_EXECUTIONCONTEXT_H
#define LANDESSDEVCORE_EXECUTIONCONTEXT_H
#include "Memory/ElementReference.h"
namespace LD
{
    template<typename T>
    class ExecutionContext
    {
    private:
        LD::ElementReference<T> mExecution;
    public:
        ExecutionContext(T & context) noexcept:mExecution{context}
        {

        }


        void Dispatch();


        void Post();
    };
}

#endif //LANDESSDEVCORE_EXECUTIONCONTEXT_H
