//
//  HazardPointer.h
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

// Hazard Pointer C++11 implem based on Michael (2004) article

// TODO:
// * cleaning, renaming ...
// * correct iterator for hprecord list
// * try to make hazard pointers look like smart pointers
// * more coherent use of inline/noexcept ...

#include <atomic>
#include <vector>
#include <algorithm>
#include <thread>
#include <unordered_set>
#include <unordered_map>

#ifndef LOCKFREEEXPERIMENT_HAZARDPOINTERS_HAZARD_POINTER_HH_
#define LOCKFREEEXPERIMENT_HAZARDPOINTERS_HAZARD_POINTER_HH_

// hazard_pointer<T> a kind of smart pointer.
// Normaly, you don't need to build hp yourself, use hazard_pointer_manager
// and/or hprecord_guard

namespace PDP
{
    namespace MultiThreading
    {
        template <typename T>
        class HazardPointer {
        public:
            
            // Constructors
            
            // Default constructor: initialized with a null pointer
            HazardPointer() noexcept : ptr(0) {}
            // Normally no other constructors are needed
            // and we should forbid explicitely copy and move constructors.
            HazardPointer(const HazardPointer& hp) = delete;
            HazardPointer(HazardPointer&& hp) = delete;
            
            // We are not doing the memory management here, so no need for a
            // specific destructor.
            
            // Operations
            
            // As most smart pointer, we should be able to load and store pointers
            inline T*   load() noexcept     { return ptr; }
            inline void store(T* p)     noexcept     { ptr = p; }
            // reset() is an important operation since we should set the hp back to
            // null to indicate that we want to release the pointer.
            inline void reset()         noexcept     { ptr = 0; }
            
            // Our hazard pointers can be seen as special local variables
            // So here are some convenient operators
            T& operator*()  const noexcept { return *ptr; }
            T* operator->() const noexcept { return ptr; }
            T* operator=(T* p) noexcept {
                ptr = p;
                return p;
            }
            
            // As for copy we should never do assignement directly
            HazardPointer& operator=(const HazardPointer& r) = delete;
            
        private:
            T*   ptr;
        };
        
        // hprecord<T,K> a "block" of K hazard_pointer<T>
        // Normaly provides through hazard_pointer_manager and hprecord_guard as a
        // stores for managed shared pointers.
        template <typename T, unsigned K>
        class HazardPointerRecord {
        public:
            
            // default constructor
            HazardPointerRecord() : next(0), active(ATOMIC_FLAG_INIT) {}
            
            // A kind of try lock: if the record is available return true and lock it
            // for futher test.
            bool try_acquire() noexcept { return !active.test_and_set(); };
            
            // just release the active flag don't unset pointers
            void release() noexcept { active.clear(); }
            
            // give back the record and reset each stored pointers (making them
            // deletable)
            void Retire() noexcept {
                for (HazardPointer<T> *h = hp; h != hp+K; ++h)
                    h->reset();
                    active.clear();
                    }
            
            inline void reset() noexcept {
                for (HazardPointer<T> *h = hp; h != hp+K; ++h)
                    h->reset();
                    }
            
            // Those element should be accessible (at least through the guard.)
            HazardPointer<T>     hp[K];
            std::vector<T*>       rlist;
            HazardPointerRecord             *next;
            
        private:
            std::atomic_flag      active;
        };
        
#ifdef TLS_AVAILABLE
        // keep track of last usage record
        static thread_local void*       mine = 0;
#endif
        
        // hazard_pointer_manager<T,K> manage a set of hprecord<T,K>
        // responsible for the creation of records and their assignment
        template <typename T, unsigned K>
        class HazrdPointerManager {
        public:
            
            // Default constructor
            HazrdPointerManager() :  H(0), head(0) {}
            
            // Ask for a record
            HazardPointerRecord<T,K>* acquire() noexcept
            {
                HazardPointerRecord<T,K>*      cur;
#ifdef TLS_AVAILABLE
                if (mine != 0) {
                    cur = reinterpret_cast<hprecord<T,K>*>(mine);
                    if (cur->try_acquire()) return cur;
                }
#endif
                cur = head.load(std::memory_order_relaxed);
                for (; cur; cur = cur->next) {
                    if (!cur->try_acquire()) continue;
#ifdef TLS_AVAILABLE
                    mine = cur;
#endif
                    return cur;
                }
                H.fetch_add(K, std::memory_order_release);
                cur = new HazardPointerRecord<T,K>();
                cur->try_acquire();
#ifdef TLS_AVAILABLE
                mine = cur;
#endif
                HazardPointerRecord<T,K>      *oldhead = head.load(std::memory_order_relaxed);
                do
                    cur->next = oldhead;
                while (head.compare_exchange_strong(oldhead, cur,
                                                    std::memory_order_release,
                                                    std::memory_order_relaxed));
                return cur;
            }
            
            // Iterations: we shouldn't use a classic iterator scheme since we need
            // specific atomic pointer access.
            
            // Access to the first element of the records list
            HazardPointerRecord<T,K>* begin() noexcept {
                return head.load(std::memory_order_relaxed);
            }
            
            // how any pointer in the wait list before concrete deletion
            inline unsigned threshold() noexcept {
                unsigned    h = H.load(std::memory_order_relaxed);
                return 2*h + h/4;
            }
            
        private:
            // Already built records
            mutable std::atomic<unsigned>         H;
            // head of the records list
            mutable std::atomic<HazardPointerRecord<T,K>*>   head;
            
        };
        
        // hprecord_guard<T,K> is an object wrapper providing RAII style mechanism for
        // owning a hprecord
        template <typename T, unsigned K>
        class HazardPointerGuard {
        public:
            
            // Constructor: a hprecord_guard is always backed by a manager
            HazardPointerGuard(HazrdPointerManager<T,K>& _hpm)
            : ptr(_hpm.acquire()), hpm(_hpm) {}
            
            // No copy, no move !
            HazardPointerGuard(const HazardPointerGuard& r) = delete;
            HazardPointerGuard(HazardPointerGuard&& r) = delete;
            
            // Destructor: recycle the wrapped hprecord
            ~HazardPointerGuard() { ptr->Retire(); }
            
            // transparent access to the hp array
            HazardPointer<T>& operator[] (unsigned i) const noexcept {
                return ptr->hp[i];
            }
            
            // reset hp pointers
            inline void Reset() noexcept { ptr->Reset(); }
            
            // Try to delete a managed shared pointer.
            // This is the classic operation from original hazard pointer: we add the
            // pointer to a "wait list" and if the list is bigger than a given threshold
            // we try to concretely delete elements in the wait-list.
            void RetireNode(T* node) noexcept
            {
                ptr->rlist.push_back(node);
                if (ptr->rlist.size() >= hpm.threshold())
                {
                    Scan();
                    HelpScan();
                }
            }
            
            // try to delete all unused pointers
            // this is also a classical example from the original hazard pointers design
            void Scan() noexcept
            {
                std::unordered_set<T*>      plist;
                for (auto cur = hpm.begin(); cur != 0; cur = cur->next) {
                    for (unsigned i=0; i < K; ++i) {
                        T             *p = cur->hp[i].load();
                        if (p != 0) plist.insert(p);
                            }
                }
                ptr->rlist.erase
                (std::remove_if
                 (ptr->rlist.begin(), ptr->rlist.end(),
                  [&](T* p) -> bool {
                      if (plist.find(p) == plist.end()) {
                          delete p;
                          return true;
                      }
                      return false;
                  }),
                 ptr->rlist.end());
            }
            
            // try to delete unused pointers from free hprecords
            void HelpScan() noexcept {
                for (auto cur = hpm.begin(); cur != 0; cur = cur->next)
                {
                    if (!cur->try_acquire()) continue;
                    for (auto e : cur->rlist)
                        ptr->rlist.push_back(e);
                        cur->rlist.clear();
                        if (ptr->rlist.size() >= hpm.threshold())
                            Scan();
                            cur->release();
                            }
            }
            
        private:
            HazardPointerRecord<T,K>                *ptr;
            HazrdPointerManager<T,K>&  hpm;
        };
    }
}


#endif
