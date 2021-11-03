//
// Created by phoenixflower on 9/27/21.
//

#ifndef LANDESSDEVCORE_RINGBUFFER_H
#define LANDESSDEVCORE_RINGBUFFER_H
#include "TypeTraits/IsAssignable.hpp"
#include "Definitions/Integer.hpp"
#include "Atomic/Atomic.h"
#include "TypeTraits/Iterable.h"
#include "Memory/ElementReference.h"
#include "Memory/Optional.h"
#include "TypeTraits/FunctionalReflection.hpp"
namespace LD
{
    /*!
     * \brief Lock free, with no wasted slots ringbuffer implementation
     *
     * \tparam T Type of buffered elements
     * \tparam buffer_size Size of the buffer. Must be a power of 2.
     * \tparam fake_tso Omit generation of explicit barrier code to avoid unnecesary instructions in tso scenario (e.g. simple microcontrollers/single core)
     * \tparam cacheline_size Size of the cache line, to insert appropriate padding in between indexes and buffer
     * \tparam index_t Type of array indexing type. Serves also as placeholder for future implementations.
     */
    template<typename T, LD::UInteger buffer_size = 16, bool fake_tso = false, LD::UInteger cacheline_size = 0, typename index_t = LD::UInteger>
    class Ringbuffer
    {
    public:
        /*!
         * \brief Default constructor, will initialize head and tail indexes
         */
        Ringbuffer() : head(0), tail(0) {}

        /*!
         * \brief Special case constructor to premature out unnecessary initialization code when object is
         * instatiated in .bss section
         * \warning If object is instantiated on stack, heap or inside noinit section then the contents have to be
         * explicitly cleared before use
         * \param dummy Ignored
         */
        Ringbuffer(int dummy) { (void)(dummy); }

        /*!
         * \brief Clear buffer from producer side
         * \warning function may return without performing any action if consumer tries to read data at the same time
         */
        void producerClear(void) {
            // head modification will lead to underflow if cleared during consumer read
            // doing this properly with CAS is not possible without modifying the consumer code
            consumerClear();
        }

        /*!
         * \brief Clear buffer from consumer side
         */
        void consumerClear(void) {
            tail.store(head.load(LD::MemoryOrder::Relaxed), LD::MemoryOrder::Relaxed);
        }

        /*!
         * \brief Check if buffer is empty
         * \return True if buffer is empty
         */
        bool isEmpty(void) const {
            return readAvailable() == 0;
        }

        /*!
         * \brief Check if buffer is full
         * \return True if buffer is full
         */
        bool isFull(void) const {
            return writeAvailable() == 0;
        }

        /*!
         * \brief Check how many elements can be read from the buffer
         * \return Number of elements that can be read
         */
        index_t readAvailable(void) const {
            return head.load(index_acquire_barrier) - tail.load(LD::MemoryOrder::Relaxed);
        }

        /*!
         * \brief Check how many elements can be written into the buffer
         * \return Number of free slots that can be be written
         */
        index_t writeAvailable(void) const {
            return buffer_size - (head.load(LD::MemoryOrder::Relaxed) - tail.load(index_acquire_barrier));
        }

        /*!
         * \brief Inserts data into internal buffer, without blocking
         * \param data element to be inserted into internal buffer
         * \return True if data was inserted
         */
        bool insert(T data)
        {

            index_t tmp_head = head.load(LD::MemoryOrder::Relaxed);

            if((tmp_head - tail.load(index_acquire_barrier)) == buffer_size)
                return false;
            else
            {
                data_buff[tmp_head++ & buffer_mask] = data;
                turf_signalFenceRelease();
                //std::atomic_signal_fence(std::memory_order_release);
                head.store(tmp_head, index_release_barrier);
            }
            return true;
        }

        /*!
         * \brief Inserts data into internal buffer, without blocking
         * \param[in] data Pointer to memory location where element, to be inserted into internal buffer, is located
         * \return True if data was inserted
         */
        bool insert(const T* data)
        {
            index_t tmp_head = head.load(LD::MemoryOrder::Relaxed);

            if((tmp_head - tail.load(index_acquire_barrier)) == buffer_size)
                return false;
            else
            {
                data_buff[tmp_head++ & buffer_mask] = *data;
                turf_signalFenceRelease();
                //std::atomic_signal_fence(std::memory_order_release);
                head.store(tmp_head, index_release_barrier);
            }
            return true;
        }

        /*!
         * \brief Inserts data returned by callback function, into internal buffer, without blocking
         *
         * This is a special purpose function that can be used to avoid redundant availability checks in case when
         * acquiring data have a side effects (like clearing status flags by reading a peripheral data register)
         *
         * \param get_data_callback Pointer to callback function that returns element to be inserted into buffer
         * \return True if data was inserted and callback called
         */
        bool insertFromCallbackWhenAvailable(T (*get_data_callback)(void))
        {

            index_t tmp_head = head.load(LD::MemoryOrder::Relaxed);

            if((tmp_head - tail.load(index_acquire_barrier)) == buffer_size)
                return false;
            else
            {
                //execute callback only when there is space in buffer
                data_buff[tmp_head++ & buffer_mask] = get_data_callback();
                turf_signalFenceRelease();
                //std::atomic_signal_fence(std::memory_order_release);
                head.store(tmp_head, index_release_barrier);
            }
            return true;
        }

        /*!
         * \brief Removes single element without reading
         * \return True if one element was removed
         */
        bool remove()
        {
            index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);

            if(tmp_tail == head.load(LD::MemoryOrder::Relaxed))
                return false;
            else
                tail.store(++tmp_tail, index_release_barrier); // release in case data was loaded/used before

            return true;
        }

        /*!
         * \brief Removes multiple elements without reading and storing it elsewhere
         * \param cnt Maximum number of elements to remove
         * \return Number of removed elements
         */
        LD::UInteger remove(LD::UInteger cnt) {

            index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);
            index_t avail = head.load(LD::MemoryOrder::Relaxed) - tmp_tail;

            cnt = (cnt > avail) ? avail : cnt;

            tail.store(tmp_tail + cnt, index_release_barrier);
            return cnt;
        }

        /*!
         * \brief Reads one element from internal buffer without blocking
         * \param[out] data Reference to memory location where removed element will be stored
         * \return True if data was fetched from the internal buffer
         */
        bool remove(T& data) {
            return remove(&data); // references are anyway implemented as pointers
        }

        /*!
         * \brief Reads one element from internal buffer without blocking
         * \param[out] data Pointer to memory location where removed element will be stored
         * \return True if data was fetched from the internal buffer
         */
        bool remove(T* data) {
            index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);

            if(tmp_tail == head.load(index_acquire_barrier))
                return false;
            else
            {
                *data = data_buff[tmp_tail++ & buffer_mask];
                turf_signalFenceRelease();
                //std::atomic_signal_fence(std::memory_order_release);
                tail.store(tmp_tail, index_release_barrier);
            }
            return true;
        }

        /*!
         * \brief Gets the first element in the buffer on consumed side
         *
         * It is safe to use and modify item contents only on consumer side
         *
         * \return Pointer to first element, nullptr if buffer was empty
         */
        T* peek() {
            index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);

            if(tmp_tail == head.load(index_acquire_barrier))
                return nullptr;
            else
                return &data_buff[tmp_tail & buffer_mask];
        }

        /*!
         * \brief Gets the n'th element on consumed side
         *
         * It is safe to use and modify item contents only on consumer side
         *
         * \param index Item offset starting on the consumed side
         * \return Pointer to requested element, nullptr if index exceeds storage count
         */
        T* at(LD::UInteger index) {
            index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);

            if((head.load(index_acquire_barrier) - tmp_tail) <= index)
                return nullptr;
            else
                return &data_buff[(tmp_tail + index) & buffer_mask];
        }

        /*!
         * \brief Gets the n'th element on consumed side
         *
         * Unchecked operation, assumes that software already knows if the element can be used, if
         * requested index is out of bounds then reference will point to somewhere inside the buffer
         * The isEmpty() and readAvailable() will place appropriate memory barriers if used as loop limiter
         * It is safe to use and modify T contents only on consumer side
         *
         * \param index Item offset starting on the consumed side
         * \return Reference to requested element, undefined if index exceeds storage count
         */
        T& operator[](LD::UInteger index) noexcept {
            return data_buff[(tail.load(LD::MemoryOrder::Relaxed) + index) & buffer_mask];
        }

        /*!
         * \brief Insert multiple elements into internal buffer without blocking
         *
         * This function will insert as much data as possible from given buffer.
         *
         * \param[in] buff Pointer to buffer with data to be inserted from
         * \param count Number of elements to write from the given buffer
         * \return Number of elements written into internal buffer
         */
        LD::UInteger writeBuff(const T* buff, LD::UInteger count);

        /*!
         * \brief Insert multiple elements into internal buffer without blocking
         *
         * This function will continue writing new entries until all data is written or there is no more space.
         * The callback function can be used to indicate to consumer that it can start fetching data.
         *
         * \warning This function is not deterministic
         *
         * \param[in] buff Pointer to buffer with data to be inserted from
         * \param count Number of elements to write from the given buffer
         * \param count_to_callback Number of elements to write before calling a callback function in first loop
         * \param execute_data_callback Pointer to callback function executed after every loop iteration
         * \return Number of elements written into internal  buffer
         */
        LD::UInteger writeBuff(const T* buff, LD::UInteger count, LD::UInteger count_to_callback, void (*execute_data_callback)(void));

        /*!
         * \brief Load multiple elements from internal buffer without blocking
         *
         * This function will read up to specified amount of data.
         *
         * \param[out] buff Pointer to buffer where data will be loaded into
         * \param count Number of elements to load into the given buffer
         * \return Number of elements that were read from internal buffer
         */
        LD::UInteger readBuff(T* buff, LD::UInteger count);

        /*!
         * \brief Load multiple elements from internal buffer without blocking
         *
         * This function will continue reading new entries until all requested data is read or there is nothing
         * more to read.
         * The callback function can be used to indicate to producer that it can start writing new data.
         *
         * \warning This function is not deterministic
         *
         * \param[out] buff Pointer to buffer where data will be loaded into
         * \param count Number of elements to load into the given buffer
         * \param count_to_callback Number of elements to load before calling a callback function in first iteration
         * \param execute_data_callback Pointer to callback function executed after every loop iteration
         * \return Number of elements that were read from internal buffer
         */
        LD::UInteger readBuff(T* buff, LD::UInteger count, LD::UInteger count_to_callback, void (*execute_data_callback)(void));

    private:
        constexpr static index_t buffer_mask = buffer_size-1; //!< bitwise mask for a given buffer size
        constexpr static LD::MemoryOrder index_acquire_barrier = fake_tso ?
                                                                 LD::MemoryOrder::Relaxed
                                                                          : LD::MemoryOrder::Acquire; // do not load from, or store to buffer before confirmed by the opposite side
        constexpr static LD::MemoryOrder index_release_barrier = fake_tso ?
                                                                 LD::MemoryOrder::Relaxed
                                                                          : LD::MemoryOrder::Release; // do not update own side before all operations on data_buff committed

        alignas(cacheline_size) LD::Atomic<index_t> head; //!< head index
        alignas(cacheline_size) LD::Atomic<index_t> tail; //!< tail index

        // put buffer after variables so everything can be reached with short offsets
        alignas(cacheline_size) T data_buff[buffer_size]; //!< actual buffer

        // let's assert that no UB will be compiled in
        static_assert((buffer_size != 0), "buffer cannot be of zero size");
        static_assert((buffer_size & buffer_mask) == 0, "buffer size is not a power of 2");
        static_assert(sizeof(index_t) <= sizeof(size_t),
                      "indexing type size is larger than size_t, operation is not lock free and doesn't make sense");

        //static_assert(std::numeric_limits<index_t>::is_integer, "indexing type is not integral type");
        //static_assert(!(std::numeric_limits<index_t>::is_signed), "indexing type shall not be signed");
        //static_assert(buffer_mask <= ((std::numeric_limits<index_t>::max)() >> 1),
                     // "buffer size is too large for a given indexing type (maximum size for n-bit type is 2^(n-1))");
    };

    template<typename T, size_t buffer_size, bool fake_tso, size_t cacheline_size, typename index_t>
    size_t Ringbuffer<T, buffer_size, fake_tso, cacheline_size, index_t>::writeBuff(const T* buff, LD::UInteger count)
    {
        index_t available = 0;
        index_t tmp_head = head.load(LD::MemoryOrder::Relaxed);
        LD::UInteger to_write = count;

        available = buffer_size - (tmp_head - tail.load(index_acquire_barrier));

        if(available < count) // do not write more than we can
            to_write = available;

        // maybe divide it into 2 separate writes
        for(size_t i = 0; i < to_write; i++)
            data_buff[tmp_head++ & buffer_mask] = buff[i];

        //std::atomic_signal_fence(std::memory_order_release);
        turf_signalFenceRelease();
        head.store(tmp_head, index_release_barrier);

        return to_write;
    }

    template<typename T, size_t buffer_size, bool fake_tso, size_t cacheline_size, typename index_t>
    size_t Ringbuffer<T, buffer_size, fake_tso, cacheline_size, index_t>::writeBuff(const T* buff, LD::UInteger count,
                                                                                    LD::UInteger count_to_callback, void(*execute_data_callback)())
    {
        LD::UInteger written = 0;
        index_t available = 0;
        index_t tmp_head = head.load(LD::MemoryOrder::Relaxed);
        LD::UInteger to_write = count;

        if(count_to_callback != 0 && count_to_callback < count)
            to_write = count_to_callback;

        while(written < count)
        {
            available = buffer_size - (tmp_head - tail.load(index_acquire_barrier));

            if(available == 0) // less than ??
                break;

            if(to_write > available) // do not write more than we can
                to_write = available;

            while(to_write--)
                data_buff[tmp_head++ & buffer_mask] = buff[written++];

            //std::atomic_signal_fence(std::memory_order_release);
            turf_signalFenceRelease();
            head.store(tmp_head, index_release_barrier);

            if(execute_data_callback != nullptr)
                execute_data_callback();

            to_write = count - written;
        }

        return written;
    }

    template<typename T, size_t buffer_size, bool fake_tso, size_t cacheline_size, typename index_t>
    size_t Ringbuffer<T, buffer_size, fake_tso, cacheline_size, index_t>::readBuff(T* buff, LD::UInteger count)
    {
        index_t available = 0;
        index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);
        LD::UInteger to_read = count;

        available = head.load(index_acquire_barrier) - tmp_tail;

        if(available < count) // do not read more than we can
            to_read = available;

        // maybe divide it into 2 separate reads
        for(LD::UInteger i = 0; i < to_read; i++)
            buff[i] = data_buff[tmp_tail++ & buffer_mask];

        //std::atomic_signal_fence(std::memory_order_release);
        turf_signalFenceRelease();
        tail.store(tmp_tail, index_release_barrier);

        return to_read;
    }

    template<typename T, size_t buffer_size, bool fake_tso, size_t cacheline_size, typename index_t>
    size_t Ringbuffer<T, buffer_size, fake_tso, cacheline_size, index_t>::readBuff(T* buff, size_t count,
                                                                                   size_t count_to_callback, void(*execute_data_callback)())
    {
        LD::UInteger read = 0;
        index_t available = 0;
        index_t tmp_tail = tail.load(LD::MemoryOrder::Relaxed);
        LD::UInteger to_read = count;

        if(count_to_callback != 0 && count_to_callback < count)
            to_read = count_to_callback;

        while(read < count)
        {
            available = head.load(index_acquire_barrier) - tmp_tail;

            if(available == 0) // less than ??
                break;

            if(to_read > available) // do not write more than we can
                to_read = available;

            while(to_read--)
                buff[read++] = data_buff[tmp_tail++ & buffer_mask];

            //std::atomic_signal_fence(std::memory_order_release);
            turf_signalFenceRelease();
            tail.store(tmp_tail, index_release_barrier);

            if(execute_data_callback != nullptr)
                execute_data_callback();

            to_read = count - read;
        }

        return read;
    }

} // namespace

namespace LD
{
    template<typename T, size_t buffer_size , bool fake_tso , size_t cacheline_size , typename index_t>
    class BackInserter<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>>
    {
    private:
        LD::ElementReference<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>> mRingBuffer;
    public:
        BackInserter(LD::ElementReference<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>> ringBuffer) noexcept:mRingBuffer{ringBuffer}
        {

        }

        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<LD::IsConstructible<V,U>::value>,BackInserter> & operator = (const U && value) noexcept
        {
            while(this->mRingBuffer->insert(value) == false);
            return (*this);
        }
    };

    template<typename T, size_t buffer_size , bool fake_tso , size_t cacheline_size , typename index_t>
    class ExtractionIterator<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>>
    {
    private:
        LD::ElementReference<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>> mBuffer;
    public:
        ExtractionIterator() noexcept:mBuffer{nullptr} {}

        ExtractionIterator(LD::ElementReference<LD::Ringbuffer<T,buffer_size,fake_tso,cacheline_size,index_t>> buffer) noexcept:mBuffer{buffer}
        {

        }

        operator T ()
        {
            T returnable;

            while(this->mBuffer->remove(returnable) == false);

            return returnable;
        }

        operator LD::Optional<T> ()
        {
            T returnable;

            if(this->mBuffer->remove(returnable) == true)
            {
                return LD::Optional<T>{returnable};
            }
            return LD::Optional<T>{};
        }

        const T  operator*()  noexcept
        {
            T returnable;
            while(this->mBuffer->remove(returnable) == false);
            return returnable;
        }
        LD::Optional<T> operator()() noexcept
        {
            T returnable;
            if(this->mBuffer->remove(returnable) == true)
            {
                return LD::Optional<T>{returnable};
            }
            return LD::Optional<T>{};
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
#endif //LANDESSDEVCORE_RINGBUFFER_H
