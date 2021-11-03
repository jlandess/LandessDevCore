//
// Created by phoenixflower on 9/21/21.
//

#ifndef LANDESSDEVCORE_ASYNCRINGBUFFER_HPP
#define LANDESSDEVCORE_ASYNCRINGBUFFER_HPP
#pragma once
///////////////////////////////////////////////////////////////////////////////
//
// Implements an allocation free (after initial construction), lock-free,
// parallel, multi-producer, multi-consumer ring buffer. Any data
// type T is supported, so long as it's default constructible.
//
// The type doesn't even need to be movable / copyable, as the TryProduce
// and TryConsume methods do not force the values to be copied or moved,
// although that is a likely enough case that the TryPushBack and TryPopFront
// helpers are included. Values are typically consumed from the same address
// they're produced in.
//
// Items added to the end of the buffer and guaranteed to be popped out in
// order, or, more strictly, consumption is guaranteed to commence in the
// same order that production commenced.
//
// Eg (single consumer):
//
//  T 1: |---Produce A--------|   |---Produce B--------|
//  T 2:  |--Produce C -----|   |----produce D------|
//  T 3: idle.................|Consume A|Consume C|.|Consume D|Consume B|
//
//  Or (multiple consumers):
//  T 3: idle.................|Consume A|............|Consume D|
//  T 4: idle..................|Consume C|.............|Consume B|
//
// LGPL - (c) Ashley.Harris@Maptek.com.au.
//
///////////////////////////////////////////////////////////////////////////////

#include <atomic>
#include <memory>
#include "TypeTraits/Iterable.h"
namespace LD
{
    namespace Async
    {
        template<class T, uint8_t BitsForItemCount> class RingBuffer
        {
        public:
            static_assert(std::is_default_constructible_v<T>);
            static_assert(BitsForItemCount > 0);
            static constexpr uint32_t ItemCount = 1 << BitsForItemCount;
            enum class State : uint8_t;

            struct Consumer;

            struct Producer;

            RingBuffer();

            // Will attempt to read from the buffer. If the buffer was empty, the
            // Consumer will return false in boolean context. While the Consumer is in
            // scope it is guaranteed that the memory your reading will remain
            // untouched and will not be re-used by any producers.
            Consumer TryConsume();

            // Will attempt to reserve a spot in the buffer for you, and if it
            // succeeded, you can take your time populating it (eg with a complex
            // assignment move operation) in safety knowing that no reader will access
            // it until the Producer goes out of scope.
            //
            // If no space in the buffer was available, the producer will
            // return false in a boolean context.
            //
            // There is no way to cancel production once this method has been called,
            // if this is an issue for you A) Don't call it until you know for sure or
            // B) set up a no-op value of T that your consumers will safely skip over.
            //
            // If you try to cancel by letting an exception escape (e.g. your move
            // assignment operator throws), that is very bad, as the T may be left in
            // an invalid state and then submitted to a consumer. This results in an
            // assertion failure in debug if attempted.
            Producer TryProduce();

            // ------------------

            // Simple helpers - it's quite common to want to use this buffer with a
            // type that implements fast move:

            // Moves Item into the write pointer of the ring buffer, if it'll fit.
            // Returns whether the move occurred.
            bool TryPushBack(T&& Item);

            // Moves an item from the front of the buffer to ItemDestination, if there
            // is one to read. Returns whether the move occurred.
            bool TryPopFront(T& ItemDestination);

            enum class State : uint8_t
            {
                Empty,
                Populating,
                Queued,
                Reading
            };

            struct Consumer
            {
                explicit operator bool() const { return myValue; }

                auto& Get() const { return *myValue; }
                auto& Get() { return *myValue; }

                operator const T&() const { return Get(); }
                operator T&() { return Get(); }

                T& operator*() { return Get(); }
                const T& operator*() const { return Get(); }

                T* operator->() { return &Get(); }
                const T* operator->() const { return &Get(); }

                Consumer(const Consumer& DontCopy) = delete;
                Consumer(Consumer&& Move);

                Consumer& operator=(const Consumer& DontCopy) = delete;
                Consumer& operator=(Consumer&& Move);

                void Release();

                ~Consumer() { Release(); }
                Consumer() = default;
                Consumer(T* Data, std::atomic_uint8_t* State)
                        : myValue(Data), myState(State){};

            private:

                T* myValue = nullptr;
                std::atomic_uint8_t* myState = nullptr;
            };

            struct Producer
            {
                explicit operator bool() const { return myValue; }

                auto& Get() const { return *myValue; }
                auto& Get() { return *myValue; }

                operator const T&() const { return Get(); }
                operator T&() { return Get(); }

                T& operator*() { return Get(); }
                const T& operator*() const { return Get(); }

                T* operator->() { return &Get(); }
                const T* operator->() const { return &Get(); }

                Producer(const Producer& DontCopy) = delete;
                Producer(Producer&& Move);

                Producer& operator=(const Producer& DontCopy) = delete;
                Producer& operator=(Producer&& Move);

                void Release();

                ~Producer();
                Producer() = default;
                Producer(T* Data, std::atomic_uint8_t* State)
                        : myValue(Data), myState(State){};

            private:

                T* myValue = nullptr;
                std::atomic_uint8_t* myState = nullptr;
            };
        private:

            static constexpr uint32_t ItemDiv =
                    (uint64_t(uint32_t(-1)) + 1) / ItemCount;

            static_assert(
                    (ItemDiv & (ItemDiv - 1)) == 0,
                    "ItemDiv should always be a power of 2. Otherwise won't wrap correctly.");






            std::unique_ptr<T[]> myData;
            std::unique_ptr<std::atomic_uint8_t[]> myStates;

            std::atomic_uint32_t myNextRead = 0;
            std::atomic_uint32_t myNextWrite = 0;
        };


        template<class T, uint8_t BC>
        RingBuffer<T, BC>::RingBuffer()
                : myData(std::make_unique<T[]>(ItemCount)),
                  myStates(std::make_unique<std::atomic_uint8_t[]>(ItemCount)),
                  myNextRead(0),
                  myNextWrite(0)
        {
        }

        template<class T, uint8_t BC>
        typename RingBuffer<T, BC>::Consumer RingBuffer<T, BC>::TryConsume()
        {
            uint8_t timeout = 1;
            while (timeout++)
            {
                auto toRead = myNextRead.load(std::memory_order_acquire);
                auto toWrite = myNextWrite.load(std::memory_order_acquire);

                if (toRead == toWrite)
                {
                    // Buffer is empty.
                    return Consumer(nullptr, nullptr);
                }

                auto readPtr = myData.get() + (toRead / ItemDiv);
                auto statePtr = myStates.get() + (toRead / ItemDiv);

                auto oldState = uint8_t(State::Queued);
                if (statePtr->compare_exchange_strong(
                        oldState, uint8_t(State::Reading), std::memory_order_release))
                {
                    // We've marked it as reading successfully.

                    // Advance the read pointer for the next read. We do it by a large
                    // power of two so that it wraps around at the size of the buffer -
                    // otherwise we end up having to do a compare exchange if the counter
                    // is at end.
                    myNextRead.fetch_add(ItemDiv, std::memory_order_release);

                    return Consumer(readPtr, statePtr);
                }

                // We were unable to mark the item for "reading" from "queued", that
                // means it was:
                //  - still being populated by a writer.
                //  - given to another consumer on a different thread and the
                //    myNextRead value was incremented by the other thread.
                //
                // Loop back around and try again a few hundred times - otherwise
                // we fail.
            }
            return Consumer(nullptr, nullptr);
        }

        template<class T, uint8_t BC>
        typename RingBuffer<T, BC>::Producer RingBuffer<T, BC>::TryProduce()
        {
            uint8_t timeout = 1;
            while (timeout++)
            {
                auto toRead = myNextRead.load(std::memory_order_acquire);
                auto toWrite = myNextWrite.load(std::memory_order_acquire);

                if (toRead == toWrite + ItemDiv)
                {
                    // Buffer is full.
                    return Producer(nullptr, nullptr);
                }

                auto writePtr = myData.get() + (toWrite / ItemDiv);
                auto statePtr = myStates.get() + (toWrite / ItemDiv);

                auto oldState = uint8_t(State::Empty);
                if (statePtr->compare_exchange_strong(
                        oldState, uint8_t(State::Populating), std::memory_order_release))
                {
                    // We've marked it as populating successfully.

                    // Advance the write pointer for the next write. We do it by a a
                    // large power of two so that it wraps around at the size of the
                    // buffer - otherwise we end up having to do a compare exchange if
                    // the counter is at end.
                    myNextWrite.fetch_add(ItemDiv, std::memory_order_release);

                    return Producer(writePtr, statePtr);
                }

                // We were unable to mark the item for "writing" from "empty", that
                // means it was:
                //  - still being read by a reader.
                //  - given to another producer on a different thread and the toWrite
                //    value is about to increment.
                //
                // Loop back around and try again a few hundred times - otherwise
                // we fail.
            }
            return Producer(nullptr, nullptr);
        }


        // Moves Item into the write pointer of the ring buffer, if it'll fit.
        // Returns whether the move occurred.
        template<class T, uint8_t BitsForItemCount>
        inline bool RingBuffer<T, BitsForItemCount>::TryPushBack(T&& Item)
        {
            auto producer = TryProduce();
            if (!producer) return false;
            producer.Get() = std::move(Item);
            return true;
        }

        // Moves an item from the front of the buffer to ItemDestination, if there
        // is one to read. Returns whether the move occurred.
        template<class T, uint8_t BitsForItemCount>
        inline bool RingBuffer<T, BitsForItemCount>::TryPopFront(T& ItemDestination)
        {
            auto consumer = TryConsume();
            if (!consumer) return false;
            ItemDestination = std::move(consumer.Get());
            return true;
        }


        template<class T, uint8_t BitsForItemCount>
        typename RingBuffer<T, BitsForItemCount>::Producer::Producer&
        RingBuffer<T, BitsForItemCount>::Producer::operator=(Producer&& Move)
        {
            Release();
            myValue = Move.myValue;
            myState = Move.myState;
            Move.myValue = nullptr;
            Move.myState = nullptr;
            return *this;
        }


        template<class T, uint8_t BitsForItemCount>
        inline RingBuffer<T, BitsForItemCount>::Producer::Producer(Producer&& Move)
        {
            myValue = Move.myValue;
            myState = Move.myState;
            Move.myValue = nullptr;
            Move.myState = nullptr;
        }
        template<class T, uint8_t BitsForItemCount>
        inline void RingBuffer<T, BitsForItemCount>::Producer::Release()
        {
            if (myValue)
            {
                myState->store(uint8_t(State::Queued), std::memory_order_release);
                myValue = nullptr;
            }
        }
        template<class T, uint8_t BitsForItemCount>
        inline RingBuffer<T, BitsForItemCount>::Producer::~Producer()
        {

            /*
            if (dbgN::IsDebugFull() && myValue && std::uncaught_exception())
            {
                ASSERTF_UNREACHABLE(R"(
    Exception thrown during a buffer locked for production. Did a move constructor
    throw? Why would you do that? This will result in partial data being
    transmitted into the buffer and sent to consumers, which will probably
    cause issues. (no - we can't rewind the buffer, other producers may of already
    started on the next element and we can't break ordering guarentees) Don't use
    exceptions to leave the scope! if you really love exceptions and can't do
    without them for this tiny region of performance sensitive code - Catch, write
    a no-op to the buffer that your consumers will skip over safely, Release(),
    and then rethrow)");
            }
             */

            Release();
        }
        template<class T, uint8_t BitsForItemCount>
        inline RingBuffer<T, BitsForItemCount>::Consumer::Consumer(Consumer&& Move)
        {
            myValue = Move.myValue;
            myState = Move.myState;
            Move.myValue = nullptr;
            Move.myState = nullptr;
        }

        template<class T, uint8_t BitsForItemCount>
        typename RingBuffer<T, BitsForItemCount>::Consumer&
        RingBuffer<T, BitsForItemCount>::Consumer::operator=(Consumer&& Move)
        {
            Release();
            myValue = Move.myValue;
            myState = Move.myState;
            Move.myValue = nullptr;
            Move.myState = nullptr;
        }

        template<class T, uint8_t BitsForItemCount>
        inline void RingBuffer<T, BitsForItemCount>::Consumer::Release()
        {
            if (myValue)
            {
                myState->store(uint8_t(State::Empty), std::memory_order_release);
                myValue = nullptr;
            }
        }
    }

}

namespace LD
{
    template<typename T, LD::UInteger Size>
    class BackInserter<LD::Async::RingBuffer<T,Size>>
    {
    private:
        LD::ElementReference<LD::Async::RingBuffer<T,Size>> mBuffer;
    public:
        BackInserter() noexcept:mBuffer{nullptr} {}

        BackInserter(LD::ElementReference<LD::Async::RingBuffer<T,Size>> buffer) noexcept:mBuffer{buffer}
        {

        }

        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<LD::IsConstructible<V,U>::value>,BackInserter> & operator = (const U && value) noexcept
        {
            //while(this->mBuffer->TryPushBack(T{LD::Forward<U>(value)}) == false){};
            while(this->mBuffer->TryPushBack(T{value}) == true);
            return (*this);
        }
    };

    template<typename T, LD::UInteger Size>
    class ExtractionIterator<LD::Async::RingBuffer<T,Size>>
    {
    private:
        LD::ElementReference<LD::Async::RingBuffer<T,Size>> mBuffer;
    public:
        ExtractionIterator() noexcept:mBuffer{nullptr} {}

        ExtractionIterator(LD::ElementReference<LD::Async::RingBuffer<T,Size>> buffer) noexcept:mBuffer{buffer}
        {

        }

        operator T()
        {
            T object;
            while(this->mBuffer->TryPopFront(object) == false);
            return object;
        }


        operator LD::Optional<T>()
        {
            T object;
            if(this->mBuffer->remove(object))
            {
                return LD::Optional<T>{object};
            }
            return LD::Optional<T>{};
        }
        T operator()() noexcept
        {
            T returnable;
            while(this->mBuffer->TryPopFront(returnable) == false);
            return returnable;
        }

        T Value() noexcept
        {
            return T{};
        }

        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<U>{},LD::Type<V>{})>,void> operator()(U & object) noexcept
        {

            while(this->mBuffer->TryPopFront(object) == false);
        }


        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<
        !LD::CT::IsSame(LD::Type<U>{},LD::Type<V>{}),
        LD::IsAssignable<U,V>::value
        >,void> operator()(U & object) noexcept
        {
            T usableObject;
            while(this->mBuffer->TryPopFront(usableObject) == false);
            object = usableObject;
        }


    };
}
#endif //LANDESSDEVCORE_ASYNCRINGBUFFER_HPP
