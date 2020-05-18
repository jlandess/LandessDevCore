//
//  ConcurrentStack.h
//  MultiThreading
//
//  Created by James Landess on 8/17/15.
//  Copyright (c) 2015 LandessDev. All rights reserved.
//

/*
 * Copyright (c) 2013, Marwan Burelle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Home Made Lock Free Stack (using hazard pointers)

#include <atomic>

#include "HazardPointer.h"

#ifndef LOCKFREEEXPERIMENT_LOCKFREEQUEUE_LOCK_FREE_QUEUE_HH_
#define LOCKFREEEXPERIMENT_LOCKFREEQUEUE_LOCK_FREE_QUEUE_HH_
namespace PDP
{
    namespace MultiThreading
    {
        template<typename T>
        class LockFreeStack
        {
        public:
            LockFreeStack() : Top(new node())
            {
                
            }
            
            void Push(const T & x)
            {
                auto n = new node(x);
                
                
                do
                {
                    node     *top = Top.load(std::memory_order_relaxed);
                    if (top != Top) continue;
                    n->next = top;
                    if (Top.compare_exchange_weak(top, n,
                                                    std::memory_order_release,
                                                    std::memory_order_relaxed))
                        return;
                } while (true);
            }
            
            bool Pop(T& rvalue)
            {
                HazardPointerGuard<node,1>      hp(mem_manager);
                
                node                       *top, *next;
                
                
                do
                {
                    top = Top.load(std::memory_order_relaxed);
                    
                    hp[0] = top;
                    
                    if (top != Top) continue;
                    
                    next = top->next;
                    
                    if (next == 0) return false;
                    
                    if (Top.compare_exchange_weak(top, next,
                                                    std::memory_order_release,
                                                    std::memory_order_relaxed))
                        break;
                } while (true);
                
                
                rvalue = top->value;
                
                hp.reset();
                
                hp.retire_node(top);
                
                return true;
            }
            
        private:
            struct node {
                std::atomic<node*>  next;
                T                   value;
                node()      : next(0)               {}
                node(T v)   : next(0), value(v)     {}
            };
            
            HazrdPointerManager<node,1>        mem_manager;
            mutable std::atomic<node*>            Top;
            
        };
    }
}

#endif
