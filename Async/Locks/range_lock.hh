/*
 * Copyright (C) 2016 Raphael S. Carvalho
 *
 * This program can be distributed under the terms of the GNU GPL.
 * See the file COPYING.
 */

#pragma once

#if __cplusplus < 201103L
#error This file requires compiler and library support for the \
ISO C++ 2011 standard. This support is currently experimental, and must be \
enabled with the -std=c++11 or -std=gnu++11 compiler options.
#endif

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <assert.h>
#if (__cplusplus >= 201402L)
#include <shared_mutex>
#else
#include <mutex>
#endif

/// \brief Range lock class
///
/// Utility created to control access to specific regions of a shared resource,
/// such as a buffer or a file. Think of it as byte-range locking mechanism.
///
/// This implementation works by virtually dividing the shared resource into N
/// regions of the same size, and associating an id with each region.
/// A region is the unit to be individually protected from concurrent access.
///
/// Choosing an optimal region size:
/// The smaller the region size, the more regions exists.
/// The more regions exists, the finer grained the locking is.
/// If in doubt, use range_lock::create_range_lock(). It will choose a region
/// size for you.
///
/// How locking does work with range_lock?
/// A lock request may cover more than one region, so there is a need to wait
/// for each covered region to be available. Deadlock is avoided by always
/// locking regions in sequential order.
///
/// This implementation is resource efficient because it will only keep alive
/// data for the regions being used at the moment. That's done with a simple
/// reference count management.


#include "RWLock.h"
namespace LD
{
    struct RangedLock
    {
    private:
        struct region
        {
            uint64_t refcount = 0;
#if (__cplusplus >= 201402L)
            std::shared_timed_mutex mutex;
#else
            std::mutex mutex;
#endif
        };
        std::unordered_map<uint64_t, std::unique_ptr<region>> _regions;
        std::mutex _regions_lock;
        const uint64_t _region_size;
    public:
        RangedLock() = delete;
        RangedLock& operator=(const RangedLock&) = delete;
        RangedLock(const RangedLock&) = delete;
        RangedLock(RangedLock&&) = default;

        // NOTE: Please make sure that region_size is greater than zero and power
        // of two. Use std::pow(2, exp) to generate a proper region size.


        RangedLock(uint64_t region_size) : _region_size(region_size)
        {
            assert(region_size > 0);
            assert((region_size & (region_size - 1)) == 0);
        }

        // Create a range_lock with a region size, which is calculated based on the
        // size of resource to be protected.
        // For example, if you want to protect a file, call create_range_lock()
        // with the size of that file.


        static std::unique_ptr<RangedLock> create_range_lock(uint64_t resource_size)
        {
            auto res = std::ceil(std::log2(resource_size) * 0.5);
            auto exp = std::max(uint64_t(res), uint64_t(10));
            uint64_t region_size = uint64_t(std::pow(2, exp));
            return static_cast<std::unique_ptr<RangedLock>>(new RangedLock(region_size));
        }
    private:

        region& get_locked_region(uint64_t region_id)
        {


            std::lock_guard<std::mutex> lock(_regions_lock);


            auto it = _regions.find(region_id);


            assert(it != _regions.end()); // assert region exists


            region& r = *(it->second);


            assert(r.refcount > 0); // assert region is locked


            return r;
        }

        region& get_and_lock_region(uint64_t region_id)
        {
            std::lock_guard<std::mutex> lock(_regions_lock);


            auto it = _regions.find(region_id);


            if (it == _regions.end())
            {

                std::unique_ptr<region> r(new region);


                auto ret = _regions.insert(std::make_pair(region_id, std::move(r)));


                it = ret.first;
            }


            region& r = *(it->second);


            r.refcount++;


            return r;
        }

        void unlock_region(uint64_t region_id)
        {

            std::lock_guard<std::mutex> lock(_regions_lock);


            auto it = _regions.find(region_id);


            assert(it != _regions.end());


            region& r = *(it->second);


            if (--r.refcount == 0)
            {
                _regions.erase(it);
            }


        }

        inline uint64_t get_region_id(uint64_t offset) const
        {
            return offset / _region_size;
        }

        enum class stop_iteration { no, yes };

        inline void do_for_each_region(uint64_t offset, uint64_t length, std::function<stop_iteration(uint64_t)> f)
        {

            auto assert_alignment = [] (uint64_t v, uint64_t alignment)
            {
                assert((v & (alignment - 1)) == 0);
            };


            assert_alignment(offset, _region_size);


            assert_alignment(length, _region_size);


            auto regions = length / _region_size;


            assert(length % _region_size == 0);


            for (auto i = 0; i < regions; i++)
            {
                auto current_offset = offset + (i * _region_size);
                stop_iteration stop = f(get_region_id(current_offset));
                if (stop == stop_iteration::yes)
                {
                    return;
                }
            }
        }

        void for_each_region(uint64_t offset, uint64_t length, std::function<stop_iteration(uint64_t)> f)
        {

            uint64_t aligned_down_offset = offset & ~(_region_size - 1);

            uint64_t aligned_up_length = (length + _region_size - 1) & ~(_region_size - 1);

            do_for_each_region(aligned_down_offset, aligned_up_length, std::move(f));
        }

        static inline void validate_parameters(uint64_t offset, uint64_t length)
        {
            assert(length > 0);
            assert(offset < (offset + length)); // check for overflow
        }

        bool generic_try_lock(uint64_t offset, uint64_t length,
                              std::function<bool(uint64_t)> try_lock, std::function<void(uint64_t)> unlock)

        {


            std::vector<uint64_t> locked_region_ids;


            bool failed_to_lock_region = false;


            validate_parameters(offset, length);


            for_each_region(offset, length, [&] (uint64_t region_id)
            {


                bool acquired = try_lock(region_id);


                if (acquired)
                {
                    locked_region_ids.push_back(region_id);
                } else
                {
                    failed_to_lock_region = true;
                    return stop_iteration::yes;
                }
                return stop_iteration::no;
            });

            if (failed_to_lock_region)
            {
                for (auto region_id : locked_region_ids)
                {
                    unlock(region_id);
                }
            }
            return !failed_to_lock_region;
        }
    public:
        uint64_t region_size() const { return _region_size; }

        // Lock range [offset, offset+length) for exclusive ownership.
        void lock(uint64_t offset, uint64_t length)
        {
            validate_parameters(offset, length);


            for_each_region(offset, length, [this] (uint64_t region_id)
            {

                region& r = this->get_and_lock_region(region_id);

                r.mutex.lock();


                return stop_iteration::no;

            });
        }

        // Tries to lock the range [offset, offset+length) for exclusive ownership.
        // This function returns immediately.
        // On successful range acquisition returns true, otherwise returns false.
        bool try_lock(uint64_t offset, uint64_t length)
        {
            auto try_lock_f = [this] (uint64_t region_id)
            {
                region& r = this->get_and_lock_region(region_id);
                return r.mutex.try_lock();
            };

            auto unlock_f = [this] (uint64_t region_id)
            {
                region& r = this->get_locked_region(region_id);
                r.mutex.unlock();
                this->unlock_region(region_id);
            };

            return generic_try_lock(offset, length, try_lock_f, unlock_f);
        }

        // Unlock range [offset, offset+length) from exclusive ownership.
        void unlock(uint64_t offset, uint64_t length) {
            validate_parameters(offset, length);
            for_each_region(offset, length, [this] (uint64_t region_id) {
                region& r = this->get_locked_region(region_id);
                r.mutex.unlock();
                this->unlock_region(region_id);
                return stop_iteration::no;
            });
        }

        // Execute an operation with range [offset, offset+length) locked for exclusive ownership.
        template <typename Func>
        void with_lock(uint64_t offset, uint64_t length, Func&& func)
        {
            lock(offset, length);
            func();
            unlock(offset, length);
        }

#if (__cplusplus >= 201402L)
        // Lock range [offset, offset+length) for shared ownership.
        void lock_shared(uint64_t offset, uint64_t length)
        {
            validate_parameters(offset, length);


            for_each_region(offset, length, [this] (uint64_t region_id)
            {
                region& r = this->get_and_lock_region(region_id);
                r.mutex.lock_shared();
                return stop_iteration::no;
            });
        }

        // Tries to lock the range [offset, offset+length) for shared ownership.
        // This function returns immediately.
        // On successful range acquisition returns true, otherwise returns false.
        bool try_lock_shared(uint64_t offset, uint64_t length)
        {
            auto try_lock_f = [this] (uint64_t region_id)
            {
                region& r = this->get_and_lock_region(region_id);
                return r.mutex.try_lock_shared();
            };


            auto unlock_f = [this] (uint64_t region_id)
            {
                region& r = this->get_locked_region(region_id);
                r.mutex.unlock_shared();
                this->unlock_region(region_id);
            };

            return generic_try_lock(offset, length, try_lock_f, unlock_f);
        }

        // Unlock range [offset, offset+length) from shared ownership.
        void unlock_shared(uint64_t offset, uint64_t length)
        {
            validate_parameters(offset, length);


            for_each_region(offset, length, [this] (uint64_t region_id)
            {
                region& r = this->get_locked_region(region_id);
                r.mutex.unlock_shared();
                this->unlock_region(region_id);
                return stop_iteration::no;
            });
        }

        // Execute an operation with range [offset, offset+length) locked for shared ownership.
        template <typename Func>
        void with_lock_shared(uint64_t offset, uint64_t length, Func&& func)
        {

            lock_shared(offset, length);

            func();

            unlock_shared(offset, length);
        }
#else
        /// If __cplusplus < 201402L, lock functions for shared ownership will call
/// corresponding lock functions for exclusive ownership.
/// That's because std::shared_timed_mutex is only available from C++14 on.
/// This decision is also important to not break compilation of programs (when
/// __cplusplus < 201402L) that use these functions.

#warning __cplusplus < 201402L, so lock for shared ownership will lock a range \
for exclusive ownership instead. That can be changed by using the -std=c++14 \
or -std=gnu++14 compiler options.

    void lock_shared(uint64_t offset, uint64_t length)
    {
        lock(offset, length);
    }

    bool try_lock_shared(uint64_t offset, uint64_t length)
    {
        return try_lock(offset, length);
    }

    void unlock_shared(uint64_t offset, uint64_t length)
    {
        unlock(offset, length);
    }

    template <typename Func>
    void with_lock_shared(uint64_t offset, uint64_t length, Func&& func)
    {
        with_lock(offset, length, std::move(func));
    }
#endif
    };
}



/*
 
 ///
 /// Purpose of this program is to test range_lock implementation.
 ///
 /// Sincerely,
 /// Raphael Carvalho
 ///
 
 #include "range_lock.hh"
 #include <iostream>
 #include <thread>
 #include <assert.h>
 
 #define print_test_name() \
 std::cout << "\nRunning " << __FUNCTION__ << "...\n";
 
 static void basic_range_lock_test(range_lock& range_lock) {
 print_test_name();
 
 auto t = std::thread([&range_lock] {
 std::cout << "Trying to lock [0, 1024)\n";
 range_lock.with_lock(0, 1024, [] {
 std::cout << "Locked [0, 1024) properly, sleeping for 2 seconds...\n";
 std::this_thread::sleep_for(std::chrono::seconds(2));
 });
 std::cout << "Unlocked [0, 1024) properly\n";
 });
 
 std::this_thread::sleep_for(std::chrono::seconds(1));
 std::cout << "Trying to lock [0, 1024*1024)\n";
 range_lock.with_lock(0, 1024*1024, [] {
 std::cout << "Locked [0, 1024*1024) properly\n";
 });
 std::cout << "Unlocked [0, 1024*1024) properly\n";
 
 t.join();
 }
 
 static void basic_range_lock_shared_test(range_lock& range_lock) {
 #if (__cplusplus >= 201402L)
 print_test_name();
 
 auto print_message = [] (auto thread_id, auto message) {
 std::cout << "[thread " << thread_id << "] " << message << std::endl;
 };
 
 std::vector<std::thread> ts;
 for (auto i = 0; i < 5; i++) {
 auto t = std::thread([&range_lock, &print_message, i] {
 print_message(i, "Requiring immediate shared ownership from [0, 1024)");
 bool acquired = range_lock.try_lock_shared(0, 1024);
 assert(acquired);
 print_message(i, "Succeeded");
 std::this_thread::sleep_for(std::chrono::seconds(2));
 range_lock.unlock_shared(0, 1024);
 });
 ts.push_back(std::move(t));
 }
 
 std::this_thread::sleep_for(std::chrono::seconds(1));
 std::cout << "Checking that [0, 8192) cannot be acquired for exclusive ownership\n";
 bool acquired = range_lock.try_lock(0, 8192);
 assert(!acquired);
 std::cout << "Succeeded\n";
 
 for (auto& t : ts) {
 t.join();
 }
 std::cout << "All threads released their lock for shared ownership\n";
 
 std::cout << "Checking that [0, 8192) can be acquired for exclusive ownership\n";
 acquired = range_lock.try_lock(0, 8192);
 assert(acquired);
 range_lock.unlock(0, 8192);
 std::cout << "Succeeded\n";
 #endif
 }
 
 static void try_lock_test(range_lock& range_lock) {
 print_test_name();
 
 auto t = std::thread([&range_lock] {
 std::cout << "Trying to lock [4096, 8192)\n";
 range_lock.with_lock(4096, 8192, [] {
 std::cout << "Locked [4096, 8192) properly, sleeping for 2 second...\n";
 std::this_thread::sleep_for(std::chrono::seconds(2));
 });
 std::cout << "Unlocked [4096, 8192) properly\n";
 });
 
 std::this_thread::sleep_for(std::chrono::seconds(1));
 std::cout << "Checking that [0, 8192) cannot be immediately acquired\n";
 bool acquired = range_lock.try_lock(0, 8192);
 assert(!acquired);
 std::cout << "Succeeded\n";
 
 t.join();
 
 std::cout << "Checking that [0, 8192) can be immediately acquired\n";
 acquired = range_lock.try_lock(0, 8192);
 assert(acquired);
 range_lock.unlock(0, 8192);
 std::cout << "Succeeded\n";
 }
 
 int main(void) {
 auto range_lock = range_lock::create_range_lock(pow(2, 30));
 std::cout << "Range lock granularity (a.k.a. region size): " << range_lock->region_size() << std::endl;
 
 basic_range_lock_test(*range_lock);
 basic_range_lock_shared_test(*range_lock);
 try_lock_test(*range_lock);
 
 return 0;
 }

 */
