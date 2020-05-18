//
//  Atomic.h
//  DataStructures
//
//  Created by James Landess on 6/14/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Atomic_h
#define DataStructures_Atomic_h

#include "AtomicUnderlyingCore.h"
#include "AtomicPrimitiveImplementation.h"
#include "AtomicCore.h"
//#include "Thread.h"
namespace PDP
{
    
    // clang-format off
    
    inline void signalFenceConsume() { turf_signalFenceConsume(); }
    inline void signalFenceAcquire() { turf_signalFenceAcquire(); }
    inline void signalFenceRelease() { turf_signalFenceRelease(); }
    inline void signalFenceSeqCst() { turf_signalFenceSeqCst(); }
    
    inline void threadFenceConsume() { turf_threadFenceConsume(); }
    inline void threadFenceAcquire() { turf_threadFenceAcquire(); }
    inline void threadFenceRelease() { turf_threadFenceRelease(); }
    inline void threadFenceSeqCst() { turf_threadFenceSeqCst(); }
    //memory_order_seq_cst
    enum MemoryOrder
    {
        Relaxed = TURF_MEMORY_ORDER_RELAXED,
        Consume = TURF_MEMORY_ORDER_ACQUIRE,
        Acquire = TURF_MEMORY_ORDER_ACQUIRE,
        Release = TURF_MEMORY_ORDER_RELEASE,
        ConsumeRelease = TURF_MEMORY_ORDER_ACQ_REL,
        AcquireRelease = TURF_MEMORY_ORDER_ACQ_REL,
    };
    
    template <typename T> class Atomic
    {
        /*
         private:
         mutable PDP::Mutex CurrentMutex;
         T CurrentValue;
         public:
         
         Atomic()
         {
         
         }
         
         Atomic(const Atomic & atomic)
         {
         atomic.CurrentMutex.Lock();
         
         
         CurrentMutex.Lock();
         
         CurrentValue = atomic.CurrentValue;
         
         CurrentMutex.UnLock();
         
         
         atomic.CurrentMutex.Lock();
         }
         
         
         ~Atomic()
         {
         CurrentMutex.Lock();
         
         CurrentMutex.UnLock();
         }
         
         Atomic & operator = (const Atomic & atomic)
         {
         atomic.CurrentMutex.Lock();
         
         
         CurrentMutex.Lock();
         
         CurrentValue = atomic.CurrentValue;
         
         CurrentMutex.UnLock();
         
         
         atomic.CurrentMutex.Lock();
         
         return (*this);
         }
         
         
         
         Atomic(const T & value)
         {
         CurrentMutex.Lock();
         {
         CurrentValue = value;
         }
         CurrentMutex.UnLock();
         }
         
         T loadNonatomic() const
         {
         return CurrentValue;
         }
         
         
         T load(MemoryOrder memoryOrder) const
         {
         T returnableValue;
         
         CurrentMutex.Lock();
         {
         returnableValue = CurrentValue;
         }
         CurrentMutex.UnLock();
         return returnableValue;
         }
         
         void store(const T & value, MemoryOrder memoryOrder)
         {
         CurrentMutex.Lock();
         {
         CurrentValue = value;
         }
         CurrentMutex.UnLock();
         }
         
         bool compareExchange(const T & expected, const T & desired, MemoryOrder memoryOrder)
         {
         if (CurrentMutex.TryLock())
         {
         if (CurrentValue == expected)
         {
         CurrentValue = desired;
         }
         return true;
         }
         
         return false;
         }
         
         bool compareExchangeStrong(const T & expected, const T & desired, MemoryOrder memoryOrder)
         {
         if (CurrentMutex.TryLock())
         {
         if (CurrentValue == expected)
         {
         CurrentValue = desired;
         }
         return true;
         }
         
         return false;
         }
         
         bool compareExchangeWeak(const T & expected, AtomicTypes::u8 desired, MemoryOrder success, MemoryOrder failure)
         {
         if (CurrentMutex.TryLock())
         {
         if (CurrentValue == expected)
         {
         CurrentValue = desired;
         }
         return true;
         
         CurrentMutex.UnLock();
         }
         
         return false;
         }
         
         
         T exchange(AtomicTypes::u8 desired, MemoryOrder memoryOrder)
         {
         T returnableValue;
         CurrentMutex.Lock();
         returnableValue = CurrentValue;
         CurrentValue = desired;
         
         CurrentMutex.UnLock();
         return returnableValue;
         }
         */
        
    };
    
    
    
#define TURF_ATOMIC_INC_TYPE char
    template <>
    class Atomic<char>
    {
    private:
        turf_atomic8_t m_value;
        
        // Hide operator=
        TURF_ATOMIC_INC_TYPE operator=(char value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u8 value)
        {
            m_value.nonatomic = (uint8_t) value;
        }
        char loadNonatomic() const {
            return (char) m_value.nonatomic;
        }
        char load(MemoryOrder memoryOrder) const {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return (char) turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u8 value) {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u8 value, MemoryOrder memoryOrder) {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store8(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        char compareExchange(AtomicTypes::u8 expected, AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(char& expected, AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            char previous =
            (char) turf_compareExchange8(&m_value, (uint8_t) expected, desired, (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(char& expected, AtomicTypes::u8 desired, MemoryOrder success, MemoryOrder failure) {
            return !!turf_compareExchangeWeak8(&m_value, (uint8_t*) &expected, desired, (turf_memoryOrder_t) success,
                                               (turf_memoryOrder_t) failure);
        }
        char exchange(AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange8(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        char fetchAdd(AtomicTypes::s8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        char fetchSub(AtomicTypes::s8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd8(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        char fetchAnd(AtomicTypes::u8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        char fetchOr(AtomicTypes::u8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(char(1), PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(char(1), PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(char(1), PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(char(1), PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
#define TURF_ATOMIC_INC_TYPE unsigned char
    template <>
    class Atomic<unsigned char>
    {
    private:
        turf_atomic8_t m_value;
        
        // Hide operator=
        unsigned char operator=(unsigned char value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u8 value) {
            m_value.nonatomic = (uint8_t) value;
        }
        unsigned char loadNonatomic() const
        {
            return (unsigned char) m_value.nonatomic;
        }
        unsigned char load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return (unsigned char) turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u8 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u8 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store8(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned char compareExchange(AtomicTypes::u8 expected, AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(unsigned char& expected, AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            unsigned char previous =
            (unsigned char) turf_compareExchange8(&m_value, (uint8_t) expected, desired, (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(unsigned char& expected, AtomicTypes::u8 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak8(&m_value, (uint8_t*) &expected, desired, (turf_memoryOrder_t) success,
                                               (turf_memoryOrder_t) failure);
        }
        unsigned char exchange(AtomicTypes::u8 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange8(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned char fetchAdd(AtomicTypes::s8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned char fetchSub(AtomicTypes::s8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd8(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned char fetchAnd(AtomicTypes::u8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned char fetchOr(AtomicTypes::u8 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr8(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
#define TURF_ATOMIC_INC_TYPE short
    template <>
    class Atomic<short>
    {
    private:
        turf_atomic16_t m_value;
        
        // Hide operator=
        short operator=(short value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u16 value)
        {
            m_value.nonatomic = value;
        }
        short loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        short load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load16(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u16 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u16 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store16(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        short compareExchange(AtomicTypes::u16 expected, AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange16(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(short& expected, AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            short previous = (short) turf_compareExchange16(&m_value, (uint16_t) expected, desired,
                                                            (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(short& expected, AtomicTypes::u16 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak16(&m_value, (uint16_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        short exchange(AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange16(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        short fetchAdd(AtomicTypes::s16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        short fetchSub(AtomicTypes::s16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd16(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        short fetchAnd(AtomicTypes::u16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        short fetchOr(AtomicTypes::u16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
#define TURF_ATOMIC_INC_TYPE unsigned short
    template <>
    class Atomic<unsigned short>
    {
    private:
        turf_atomic16_t m_value;
        
        // Hide operator=
        unsigned short operator=(unsigned short value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u16 value)
        {
            m_value.nonatomic = value;
        }
        unsigned short loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        unsigned short load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load16(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u16 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u16 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store16(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned short compareExchange(AtomicTypes::u16 expected, AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange16(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(unsigned short& expected, AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            unsigned short previous = (unsigned short) turf_compareExchange16(&m_value, (uint16_t) expected, desired,
                                                                              (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(unsigned short& expected, AtomicTypes::u16 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak16(&m_value, (uint16_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        unsigned short exchange(AtomicTypes::u16 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange16(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned short fetchAdd(AtomicTypes::s16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned short fetchSub(AtomicTypes::s16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd16(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned short fetchAnd(AtomicTypes::u16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned short fetchOr(AtomicTypes::u16 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr16(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
#define TURF_ATOMIC_INC_TYPE int
    template <>
    class Atomic<int>
    {
    private:
        turf_atomic32_t m_value;
        
        // Hide operator=
        int operator=(int value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        int loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        int load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load32(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u32 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store32(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        TURF_ATOMIC_INC_TYPE compareExchange(AtomicTypes::u32 expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange32(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(int& expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            int previous = (int) turf_compareExchange32(&m_value, (uint32_t) expected, desired,
                                                        (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(int& expected, AtomicTypes::u32 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak32(&m_value, (uint32_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        int exchange(AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange32(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        int fetchAdd(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        int fetchSub(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        int fetchAnd(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        int fetchOr(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
#define TURF_ATOMIC_INC_TYPE unsigned int
    template <>
    class Atomic<unsigned int>
    {
    private:
        turf_atomic32_t m_value;
        
        // Hide operator=
        unsigned int operator=(unsigned int value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        unsigned int loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        unsigned int load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load32(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u32 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store32(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned int compareExchange(AtomicTypes::u32 expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange32(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(unsigned int& expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            
            TURF_ATOMIC_INC_TYPE previous = (unsigned int) turf_compareExchange32(&m_value, (uint32_t) expected, desired,
                                                                                  (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(unsigned int& expected, AtomicTypes::u32 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak32(&m_value, (uint32_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        
        unsigned int exchange(AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange32(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned int fetchAdd(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned int fetchSub(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned int fetchAnd(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned int fetchOr(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
    
#if TURF_LONG_SIZE == 4
#define TURF_ATOMIC_INC_TYPE long
    
    template <>
    class Atomic<long>
    {
    private:
        turf_atomic32_t m_value;
        
        // Hide operator=
        long operator=(long value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        long loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        long load(MemoryOrder memoryOrder) const
        {
            TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load32(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u32 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store32(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        long compareExchange(AtomicTypes::u32 expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange32(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(long& expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            TURF_ATOMIC_INC_TYPE previous = (TURF_ATOMIC_INC_TYPE) turf_compareExchange32(&m_value, (uint32_t) expected, desired,
                                                                                          (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(long& expected, AtomicTypes::u32 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak32(&m_value, (uint32_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        long exchange(AtomicTypes::u32 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange32(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAdd(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchSub(AtomicTypes::s32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd32(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAnd(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchOr(AtomicTypes::u32 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
    
#endif
#undef TURF_ATOMIC_INC_TYPE
    
#define TURF_ATOMIC_INC_TYPE unsigned long
    template <>
    class Atomic<unsigned long>
    {
    private:
        turf_atomic32_t m_value;
        
        // Hide operator=
        unsigned long operator=(unsigned long value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u32 value)
        {
            m_value.nonatomic = value;
        }
        unsigned long loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        unsigned long load(MemoryOrder memoryOrder) const {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load32(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u32 value) {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u32 value, MemoryOrder memoryOrder) {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store32(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long compareExchange(AtomicTypes::u32 expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder) {
            return turf_compareExchange32(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(unsigned long& expected, AtomicTypes::u32 desired, MemoryOrder memoryOrder) {
            unsigned long previous = (unsigned long) turf_compareExchange32(&m_value, (uint32_t) expected, desired,
                                                                            (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(unsigned long& expected, AtomicTypes::u32 desired, MemoryOrder success, MemoryOrder failure) {
            return !!turf_compareExchangeWeak32(&m_value, (uint32_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        unsigned long exchange(AtomicTypes::u32 desired, MemoryOrder memoryOrder) {
            return turf_exchange32(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long fetchAdd(AtomicTypes::s32 operand, MemoryOrder memoryOrder) {
            return turf_fetchAdd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long fetchSub(AtomicTypes::s32 operand, MemoryOrder memoryOrder) {
            return turf_fetchAdd32(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long fetchAnd(AtomicTypes::u32 operand, MemoryOrder memoryOrder) {
            return turf_fetchAnd32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long fetchOr(AtomicTypes::u32 operand, MemoryOrder memoryOrder) {
            return turf_fetchOr32(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
    
#if TURF_LONG_SIZE == 8
#define TURF_ATOMIC_INC_TYPE long
    
    template <>
    class Atomic<long>
    {
    private:
        turf_atomic64_t m_value;
        
        // Hide operator=
        long operator=(long value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u64 value)
        {
            m_value.nonatomic = value;
        }
        long loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        long load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load64(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u64 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u64 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store64(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        long compareExchange(AtomicTypes::u64 expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange64(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(long& expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            long previous = (long) turf_compareExchange64(&m_value, (uint64_t) expected, desired,
                                                          (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(long& expected, AtomicTypes::u64 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak64(&m_value, (uint64_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        long exchange(AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange64(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAdd(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchSub(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAnd(AtomicTypes::u64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchOr(AtomicTypes::u64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchOr64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
    
#endif
#undef TURF_ATOMIC_INC_TYPE
    
    
#if TURF_HAS_LONG_LONG
#define TURF_ATOMIC_INC_TYPE long
    template <>
    class Atomic<long>
    {
    private:
        turf_atomic64_t m_value;
        
        // Hide operator=
        long operator=(long value);
        
    public:
        Atomic() {
        }
        Atomic(AtomicTypes::u64 value)
        {
            m_value.nonatomic = value;
        }
        long loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        long load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load64(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u64 value) {
            m_value.nonatomic = value;
        }
        void store(u64 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store64(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        long compareExchange(AtomicTypes::u64 expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange64(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            long previous = (long) turf_compareExchange64(&m_value, (uint64_t) expected, desired,
                                                          (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(long& expected, AtomicTypes::u64 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak64(&m_value, (uint64_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        long exchange(AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange64(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAdd(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchSub(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchAnd(AtomicTypes::u64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        long fetchOr(AtomicTypes::u64 operand, MemoryOrder memoryOrder) {
            return turf_fetchOr64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
    };
    
    
#define TURF_ATOMIC_INC_TYPE unsigned long long
    template <>
    class Atomic<unsigned long long>
    {
    private:
        turf_atomic64_t m_value;
        
        // Hide operator=
        unsigned long long operator=(unsigned long long value);
        
    public:
        Atomic()
        {
        }
        Atomic(AtomicTypes::u64 value)
        {
            m_value.nonatomic = value;
        }
        unsigned long long loadNonatomic() const
        {
            return m_value.nonatomic;
        }
        unsigned long long load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return turf_load64(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(AtomicTypes::u64 value)
        {
            m_value.nonatomic = value;
        }
        void store(AtomicTypes::u64 value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store64(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long long compareExchange(AtomicTypes::u64 expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_compareExchange64(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(unsigned long long& expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            unsigned long long previous = (unsigned long long) turf_compareExchange64(&m_value, (uint64_t) expected, desired,
                                                                                      (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(unsigned long long& expected, AtomicTypes::u64 desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeak64(&m_value, (uint64_t*) &expected, desired, (turf_memoryOrder_t) success,
                                                (turf_memoryOrder_t) failure);
        }
        unsigned long long exchange(AtomicTypes::u64 desired, MemoryOrder memoryOrder)
        {
            return turf_exchange64(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long long fetchAdd(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long long fetchSub(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAdd64(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long long fetchAnd(AtomicTypes::u64 operand, MemoryOrder memoryOrder)
        {
            return turf_fetchAnd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        unsigned long long fetchOr(AtomicTypes::u64 operand, MemoryOrder memoryOrder) {
            return turf_fetchOr64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
        }
        
        Atomic & operator ++ ()
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator ++(int)
        {
            this->fetchAdd(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --()
        {
            this->fetchSub(1, PDP::AcquireRelease);
            return (*this);
        }
        
        Atomic & operator --(int)
        {
            this->fetchSub(1, turf::AcquireRelease);
            return (*this);
        }
    };
    
#undef TURF_ATOMIC_INC_TYPE
    
    
#endif
#undef TURF_ATOMIC_INC_TYPE
    
    
    // Specialize for bool
    template <>
    class Atomic<bool>
    {
    private:
        turf_atomic8_t m_value;
        
        // Hide operator=
        bool operator=(bool value);
        
    public:
        Atomic()
        {
        }
        Atomic(bool value)
        {
            m_value.nonatomic = (uint8_t) value;
        }
        bool loadNonatomic() const
        {
            return !!m_value.nonatomic;
        }
        bool load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return !!turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(bool value)
        {
            m_value.nonatomic = (uint8_t) value;
        }
        void store(bool value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_store8(&m_value, (uint8_t) value, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchange(bool expected, bool desired, MemoryOrder memoryOrder)
        {
            return !!turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(bool& expected, bool desired, MemoryOrder memoryOrder)
        {
            uint8_t previous =
            turf_compareExchange8(&m_value, (uint8_t) expected, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == (uint8_t) expected);
            if (!result)
                expected = !!previous;
            return result;
        }
        bool compareExchangeWeak(bool& expected, AtomicTypes::u8 desired, MemoryOrder success, MemoryOrder failure)
        {
            uint8_t expected8 = (uint8_t) expected;
            bool result = !!turf_compareExchangeWeak8(&m_value, &expected8, (uint8_t) desired, (turf_memoryOrder_t) success,
                                                      (turf_memoryOrder_t) failure);
            if (!result)
                expected = !!expected8;
            return result;
        }
        bool exchange(bool desired, MemoryOrder memoryOrder)
        {
            return !!turf_exchange8(&m_value, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
        }
        
        bool TestAndTest(MemoryOrder memoryOrder)
        {
            return exchange(true,memoryOrder);
        }
        
        
        void Clear(MemoryOrder memoryOrder)
        {
            this->store(false,memoryOrder);
        }
        //__c11_atomic_store(&__a_, false, __m);
        //return __c11_atomic_exchange(&__a_, true, __m);
    };
    
    // Specialize for pointers
    template <typename T>
    class Atomic<T*>
    {
    private:
        turf_atomicPtr_t m_value;
        
        // Hide operator=
        T* operator=(T* value);
        
    public:
        Atomic()
        {
        }
        Atomic(T* value)
        {
            m_value.nonatomic = value;
        }
        T* loadNonatomic() const
        {
            return (T*) m_value.nonatomic;
        }
        T* load(MemoryOrder memoryOrder) const
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
            return (T*) turf_loadPtr(&m_value, (turf_memoryOrder_t) memoryOrder);
        }
        void storeNonatomic(T* value)
        {
            m_value.nonatomic = value;
        }
        void store(T* value, MemoryOrder memoryOrder)
        {
            //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
            turf_storePtr(&m_value, value, (turf_memoryOrder_t) memoryOrder);
        }
        T* compareExchange(T* expected, T* desired, MemoryOrder memoryOrder)
        {
            return (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
        }
        bool compareExchangeStrong(T*& expected, T* desired, MemoryOrder memoryOrder)
        {
            T* previous = (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
            bool result = (previous == expected);
            if (!result)
                expected = previous;
            return result;
        }
        bool compareExchangeWeak(T*& expected, T* desired, MemoryOrder success, MemoryOrder failure)
        {
            return !!turf_compareExchangeWeakPtr(&m_value, (void**) &expected, desired, (turf_memoryOrder_t) success,
                                                 (turf_memoryOrder_t) failure);
        }
        T* exchange(T* desired, MemoryOrder memoryOrder)
        {
            return (T*) turf_exchangePtr(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
        }
        // If you need other RMW operations on an atomic pointer, use
        // turf::Atomic<uptr> and cast argument/return values by hand
    };
    
    
    struct RaceDetector
    {
        PDP::Atomic<bool> entered;
        
        RaceDetector() : entered(false)
        {
        }
    };
    
    class RaceDetectGuard
    {
    private:
        
        RaceDetector& m_guard;
        
    public:
        RaceDetectGuard(RaceDetector& guard) : m_guard(guard)
        {
            if (m_guard.entered.exchange(true, PDP::Acquire) == true)
                TURF_DEBUG_BREAK();
        }
        ~RaceDetectGuard()
        {
            m_guard.entered.store(false, PDP::Release);
        }
    };
    
    
}
#endif

#define TURF_DEFINE_RACE_DETECTOR(name) turf::RaceDetector name;
#define TURF_RACE_DETECT_GUARD(name) turf::RaceDetectGuard TURF_UNIQUE_VARIABLE(raceDetectGuard)(name)
/*
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 #else
 #error "TURF_LONG_SIZE not defined!"
 #endif
 
 
 #define TURF_ATOMIC_INC_TYPE unsigned long long
 template <>
 class Atomic<TURF_ATOMIC_INC_TYPE>
 {
 private:
 turf_atomic64_t m_value;
 
 // Hide operator=
 TURF_ATOMIC_INC_TYPE operator=(TURF_ATOMIC_INC_TYPE value);
 
 public:
 Atomic()
 {
 }
 Atomic(AtomicTypes::u64 value)
 {
 m_value.nonatomic = value;
 }
 TURF_ATOMIC_INC_TYPE loadNonatomic() const
 {
 return m_value.nonatomic;
 }
 TURF_ATOMIC_INC_TYPE load(MemoryOrder memoryOrder) const
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
 return turf_load64(&m_value, (turf_memoryOrder_t) memoryOrder);
 }
 void storeNonatomic(AtomicTypes::u64 value)
 {
 m_value.nonatomic = value;
 }
 void store(AtomicTypes::u64 value, MemoryOrder memoryOrder)
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
 turf_store64(&m_value, value, (turf_memoryOrder_t) memoryOrder);
 }
 TURF_ATOMIC_INC_TYPE compareExchange(AtomicTypes::u64 expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
 {
 return turf_compareExchange64(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
 }
 bool compareExchangeStrong(TURF_ATOMIC_INC_TYPE& expected, AtomicTypes::u64 desired, MemoryOrder memoryOrder)
 {
 TURF_ATOMIC_INC_TYPE previous = (TURF_ATOMIC_INC_TYPE) turf_compareExchange64(&m_value, (uint64_t) expected, desired,
 (turf_memoryOrder_t) memoryOrder);
 bool result = (previous == expected);
 if (!result)
 expected = previous;
 return result;
 }
 bool compareExchangeWeak(TURF_ATOMIC_INC_TYPE& expected, AtomicTypes::u64 desired, MemoryOrder success, MemoryOrder failure)
 {
 return !!turf_compareExchangeWeak64(&m_value, (uint64_t*) &expected, desired, (turf_memoryOrder_t) success,
 (turf_memoryOrder_t) failure);
 }
 TURF_ATOMIC_INC_TYPE exchange(AtomicTypes::u64 desired, MemoryOrder memoryOrder)
 {
 return turf_exchange64(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
 }
 TURF_ATOMIC_INC_TYPE fetchAdd(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
 {
 return turf_fetchAdd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
 }
 TURF_ATOMIC_INC_TYPE fetchSub(AtomicTypes::s64 operand, MemoryOrder memoryOrder)
 {
 return turf_fetchAdd64(&m_value, -operand, (turf_memoryOrder_t) memoryOrder);
 }
 TURF_ATOMIC_INC_TYPE fetchAnd(AtomicTypes::u64 operand, MemoryOrder memoryOrder)
 {
 return turf_fetchAnd64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
 }
 TURF_ATOMIC_INC_TYPE fetchOr(AtomicTypes::u64 operand, MemoryOrder memoryOrder) {
 return turf_fetchOr64(&m_value, operand, (turf_memoryOrder_t) memoryOrder);
 }
 };
 
 #undef TURF_ATOMIC_INC_TYPE
 #endif
 
 // clang-format on
 
 // Specialize for bool
 template <>
 class Atomic<bool>
 {
 private:
 turf_atomic8_t m_value;
 
 // Hide operator=
 bool operator=(bool value);
 
 public:
 Atomic()
 {
 }
 Atomic(bool value)
 {
 m_value.nonatomic = (uint8_t) value;
 }
 bool loadNonatomic() const
 {
 return !!m_value.nonatomic;
 }
 bool load(MemoryOrder memoryOrder) const
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
 return !!turf_load8(&m_value, (turf_memoryOrder_t) memoryOrder);
 }
 void storeNonatomic(bool value)
 {
 m_value.nonatomic = (uint8_t) value;
 }
 void store(bool value, MemoryOrder memoryOrder)
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
 turf_store8(&m_value, (uint8_t) value, (turf_memoryOrder_t) memoryOrder);
 }
 bool compareExchange(bool expected, bool desired, MemoryOrder memoryOrder)
 {
 return !!turf_compareExchange8(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
 }
 bool compareExchangeStrong(bool& expected, bool desired, MemoryOrder memoryOrder)
 {
 uint8_t previous =
 turf_compareExchange8(&m_value, (uint8_t) expected, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
 bool result = (previous == (uint8_t) expected);
 if (!result)
 expected = !!previous;
 return result;
 }
 bool compareExchangeWeak(bool& expected, AtomicTypes::u8 desired, MemoryOrder success, MemoryOrder failure)
 {
 uint8_t expected8 = (uint8_t) expected;
 bool result = !!turf_compareExchangeWeak8(&m_value, &expected8, (uint8_t) desired, (turf_memoryOrder_t) success,
 (turf_memoryOrder_t) failure);
 if (!result)
 expected = !!expected8;
 return result;
 }
 bool exchange(bool desired, MemoryOrder memoryOrder)
 {
 return !!turf_exchange8(&m_value, (uint8_t) desired, (turf_memoryOrder_t) memoryOrder);
 }
 };
 
 // Specialize for pointers
 template <typename T>
 class Atomic<T*>
 {
 private:
 turf_atomicPtr_t m_value;
 
 // Hide operator=
 T* operator=(T* value);
 
 public:
 Atomic()
 {
 }
 Atomic(T* value)
 {
 m_value.nonatomic = value;
 }
 T* loadNonatomic() const
 {
 return (T*) m_value.nonatomic;
 }
 T* load(MemoryOrder memoryOrder) const
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Acquire);
 return (T*) turf_loadPtr(&m_value, (turf_memoryOrder_t) memoryOrder);
 }
 void storeNonatomic(T* value)
 {
 m_value.nonatomic = value;
 }
 void store(T* value, MemoryOrder memoryOrder)
 {
 //TURF_ASSERT(memoryOrder == Relaxed || memoryOrder == Release);
 turf_storePtr(&m_value, value, (turf_memoryOrder_t) memoryOrder);
 }
 T* compareExchange(T* expected, T* desired, MemoryOrder memoryOrder)
 {
 return (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
 }
 bool compareExchangeStrong(T*& expected, T* desired, MemoryOrder memoryOrder)
 {
 T* previous = (T*) turf_compareExchangePtr(&m_value, expected, desired, (turf_memoryOrder_t) memoryOrder);
 bool result = (previous == expected);
 if (!result)
 expected = previous;
 return result;
 }
 bool compareExchangeWeak(T*& expected, T* desired, MemoryOrder success, MemoryOrder failure)
 {
 return !!turf_compareExchangeWeakPtr(&m_value, (void**) &expected, desired, (turf_memoryOrder_t) success,
 (turf_memoryOrder_t) failure);
 }
 T* exchange(T* desired, MemoryOrder memoryOrder)
 {
 return (T*) turf_exchangePtr(&m_value, desired, (turf_memoryOrder_t) memoryOrder);
 }
 // If you need other RMW operations on an atomic pointer, use
 // turf::Atomic<uptr> and cast argument/return values by hand
 };
 
 } // namespace turf
 
 
 
 namespace turf
 {
 
 #if TURF_WITH_ASSERTS
 
 struct RaceDetector
 {
 turf::Atomic<bool> entered;
 
 RaceDetector() : entered(false) {
 }
 };
 
 class RaceDetectGuard
 {
 
 private:
 
 RaceDetector& m_guard;
 
 public:
 RaceDetectGuard(RaceDetector& guard) : m_guard(guard) {
 if (m_guard.entered.exchange(true, turf::Acquire) == true)
 TURF_DEBUG_BREAK();
 }
 ~RaceDetectGuard() {
 m_guard.entered.store(false, turf::Release);
 }
 };
 }
 
 // Alias it:
 namespace turf
 {
 
 template <typename T> class Atomic : public Atomic_Native<T>
 {
 public:
 Atomic()
 {
 }
 Atomic(T value) : Atomic_Native<T>(value)
 {
 
 }
 ~Atomic()
 {
 }
 };
 
 
 } // namespace turf
 */
/*
 #ifndef DataStructures_Atomic_h
 #define DataStructures_Atomic_h
 
 
 #include "Common.h"
 #include "MemoryBarriers.h"
 
 namespace PDP
 {
 
 namespace Internal
 {
 template<typename T>
 struct AtomicPointerType
 {
 
 };
 
 template<>
 struct AtomicPointerType<float>
 {
 typedef  mint_atomic32_t PointerType;
 
 };
 
 template<>
 struct AtomicPointerType<double>
 {
 typedef  mint_atomic64_t PointerType;
 
 };
 }
 
 template<typename T, class = void>
 class Atomic
 {
 private:
 //replace sizeof(T) with an appriopriate 4/8 byte aligned system
 PDP::Internal::AtomicPointerType<PDP::Float> CurrentObjectContainer[sizeof(T)];
 
 public:
 };
 
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>
 {
 private:
 mint_atomic32_t CurrentValue;
 public:
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator = ( const T & object);
 
 inline T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &) const;
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &);
 };
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Atomic()
 {
 mint_store_32_relaxed(&CurrentValue, 0);
 
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> && object)
 {
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&object.CurrentValue));
 mint_store_32_relaxed(&object.CurrentValue, 0);
 
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & object)
 {
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&object.CurrentValue));
 
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Atomic(const T & object)
 {
 mint_store_32_relaxed(&CurrentValue, object);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & atomic)
 {
 
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&atomic.CurrentValue));
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator=( Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> && atomic)
 {
 
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&atomic.CurrentValue));
 mint_store_32_relaxed(&atomic.CurrentValue, 0);
 
 return (*this);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator=( const T & object)
 {
 
 mint_store_32_relaxed(&CurrentValue, object);
 
 return (*this);
 }
 
 template<typename T>
 T Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Load() const
 {
 
 T value =  mint_load_32_relaxed(&CurrentValue);
 
 return value;
 }
 
 template<typename T>
 void Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::Store(const T &object)
 {
 
 mint_store_32_relaxed(&CurrentValue, object);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator++()
 {
 
 mint_fetch_add_32_relaxed(&CurrentValue, 1);
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator++(int)
 {
 
 mint_fetch_add_32_relaxed(&CurrentValue, 1);
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator--()
 {
 int32_t rawr = -1;
 mint_fetch_add_32_relaxed(&CurrentValue, rawr);
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator--(int)
 {
 
 int32_t rawr = -1;
 mint_fetch_add_32_relaxed(&CurrentValue, rawr);
 
 return (*this);
 }
 
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &value) const
 {
 mint_atomic32_t val;
 
 mint_store_32_relaxed(&val, mint_load_32_relaxed(&CurrentValue));
 
 
 mint_fetch_add_32_relaxed(&val, value.Load());
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value>>(val);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &value) const
 {
 
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&CurrentValue)-value.Load());
 
 return (*this);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &value) const
 {
 
 mint_atomic32_t val;
 
 mint_store_32_relaxed(&val, mint_load_32_relaxed(&CurrentValue));
 mint_store_32_relaxed(&val, mint_load_32_relaxed(&CurrentValue)*value.Load());
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value>>(val);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> &value) const
 {
 
 
 
 
 mint_atomic32_t val;
 
 mint_store_32_relaxed(&val, mint_load_32_relaxed(&CurrentValue));
 mint_store_32_relaxed(&val, mint_load_32_relaxed(&CurrentValue)*value.Load());
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value>>(val);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & value)
 {
 mint_fetch_add_32_relaxed(&CurrentValue, value.Load());
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & value)
 {
 int32_t integer = value.Load()*-1;
 mint_fetch_add_32_relaxed(&CurrentValue, integer);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & value)
 {
 
 
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&CurrentValue)*value.Load());
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>>::operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitUnsignedInteger<T>::value || PDP::IsUnsignedCharacter<T>::value>> & value)
 {
 
 
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&CurrentValue)/value.Load());
 
 
 return (*this);
 }
 
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>
 {
 private:
 mint_atomic64_t CurrentValue;
 public:
 
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator = ( const T & object);
 
 inline T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &) const;
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> &);
 };
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::Atomic()
 {
 mint_store_64_relaxed(&CurrentValue, 0);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::Atomic(const T & value)
 {
 mint_store_64_relaxed(&CurrentValue,value);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & atomic)
 {
 mint_store_64_relaxed(&CurrentValue,mint_load_64_relaxed(&atomic.CurrentValue));
 }
 
 template<typename T >
 T Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::Load() const
 {
 return mint_load_64_relaxed(&CurrentValue);
 }
 
 template<typename T>
 void Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::Store(const T & value)
 {
 mint_store_64_relaxed(&CurrentValue, value);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator++()
 {
 mint_fetch_add_64_relaxed(&CurrentValue, 1);
 return (*this);
 }
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator++(int)
 {
 mint_fetch_add_64_relaxed(&CurrentValue, 1);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator--()
 {
 
 mint_fetch_add_64_relaxed(&CurrentValue, -1);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator--(int)
 {
 mint_fetch_add_64_relaxed(&CurrentValue, -1);
 return (*this);
 }
 
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator+(const Atomic<T, PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value> > & atomic) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 mint_atomic64_t current;
 
 mint_store_64_relaxed(&current, currentValue);
 
 mint_fetch_add_64_relaxed(&current, atomic.Load());
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>(mint_load_64_relaxed(&current));
 }
 
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator-(const Atomic<T, PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value> > & atomic) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 mint_atomic64_t current;
 
 mint_store_64_relaxed(&current, currentValue);
 
 mint_fetch_add_64_relaxed(&current, -1*atomic.Load());
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>(mint_load_64_relaxed(&current));
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator*(const Atomic<T, PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value> > & atomic) const
 {
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>(mint_load_64_relaxed(&CurrentValue) * mint_load_64_relaxed(&atomic.CurrentValue));
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>::operator/(const Atomic<T, PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value> > & atomic) const
 {
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitUnsignedInteger<T>::value>>(mint_load_64_relaxed(&CurrentValue) / mint_load_64_relaxed(&atomic.CurrentValue));
 }
 
 //remember to store signed numbers by byte and retrieve them by byte in order to keep the sign without having to keep track of two atomic primitives
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>
 {
 private:
 mint_atomic32_t CurrentValue;
 public:
 
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator = ( const T & object);
 
 inline const T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &) const;
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &) const;
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &) const;
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &) const;
 
 
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &);
 
 
 
 };
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::Atomic()
 {
 mint_store_32_relaxed(&CurrentValue, 0);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & object)
 {
 mint_store_32_relaxed(&CurrentValue, object.Load());
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> && object)
 {
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&object.CurrentValue));
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator=(const T &object)
 {
 mint_store_32_relaxed(&CurrentValue, object);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & object)
 {
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&object.CurrentValue));
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator=(Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> && object)
 {
 mint_store_32_relaxed(&CurrentValue, mint_load_32_relaxed(&object.CurrentValue));
 mint_store_32_relaxed(&object.CurrentValue, 0);
 return (*this);
 }
 
 
 template<typename T>
 void Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::Store(const T & object)
 {
 int32_t value = object;
 
 uint32_t * atomicValue = (uint32_t*)&value;
 
 mint_store_32_relaxed(&CurrentValue, *atomicValue);
 
 
 }
 
 template<typename T>
 const T Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::Load() const
 {
 uint32_t atomicValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * value = (int32_t*)&atomicValue;
 
 
 return *value;
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator++()
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 ++(*signedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator++(int)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 ++(*signedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator--()
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 --(*signedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator--(int)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 --(*signedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 
 
 return (*this);
 }
 
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)const
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)+=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)const
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)-=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)const
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)*=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)const
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)/=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)+=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)-=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)*=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 return (*this);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> &  Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>>::operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitSignedInteger<T>::value || PDP::Is16BitSignedInteger<T>::value>> & atomicValue)
 {
 uint32_t currentValue = mint_load_32_relaxed(&CurrentValue);
 
 int32_t * signedValue = (int32_t*)&currentValue;
 
 uint32_t inputCurrentValue = mint_load_32_relaxed(&atomicValue.CurrentValue);
 
 int32_t * inputSignedValue = (int32_t*)&inputCurrentValue;
 
 
 (*signedValue)/=(*inputSignedValue);
 
 uint32_t * storeableValue = (uint32_t*)signedValue;
 
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 
 return (*this);
 }
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>
 {
 private:
 mint_atomic64_t CurrentValue;
 public:
 
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator = ( const T & object);
 
 inline const T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &) const;
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> &);
 };
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Atomic()
 {
 mint_store_64_relaxed(&CurrentValue, 0);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & object)
 {
 mint_store_64_relaxed(&CurrentValue, object.Load());
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> && object)
 {
 mint_store_64_relaxed(&CurrentValue, object.Load());
 mint_store_64_relaxed(&object.CurrentValue, 0);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Atomic(const T & object)
 {
 mint_store_64_relaxed(&CurrentValue, object);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue)
 {
 mint_store_64_relaxed(&CurrentValue, atomicValue.Load());
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> && atomicValue)
 {
 mint_store_64_relaxed(&CurrentValue, atomicValue.Load());
 mint_store_64_relaxed(&atomicValue.CurrentValue, 0);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator = ( const T & value)
 {
 mint_store_64_relaxed(&CurrentValue, value);
 return (*this);
 }
 
 
 template<typename T>
 const T Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Load() const
 {
 return mint_load_64_relaxed(&CurrentValue);
 }
 
 
 template<typename T>
 void Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::Store(const T & value)
 {
 mint_store_64_relaxed(&CurrentValue, value);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator++()
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 ++(*signedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator++(int)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 ++(*signedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator--()
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 --(*signedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator--(int)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 --(*signedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 mint_store_32_relaxed(&CurrentValue, *storeableValue);
 return (*this);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)+=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)-=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)*=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 template<typename T>
 const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue) const
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)/=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 return Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>(*storeableValue);
 }
 
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)+=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 
 mint_store_64_relaxed(&CurrentValue, *storeableValue);
 
 
 return (*this);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)-=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 
 mint_store_64_relaxed(&CurrentValue, *storeableValue);
 
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)*=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 
 mint_store_64_relaxed(&CurrentValue, *storeableValue);
 
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>>::operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitSignedInteger<T>::value>> & atomicValue)
 {
 uint64_t currentValue = mint_load_64_relaxed(&CurrentValue);
 
 uint64_t * signedValue = (uint64_t*)&currentValue;
 
 uint64_t inputCurrentValue = mint_load_64_relaxed(&atomicValue.CurrentValue);
 
 uint64_t * inputSignedValue = (uint64_t*)&inputCurrentValue;
 
 
 (*signedValue)/=(*inputSignedValue);
 
 uint64_t * storeableValue = (uint64_t*)signedValue;
 
 
 
 mint_store_64_relaxed(&CurrentValue, *storeableValue);
 
 
 return (*this);
 }
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>
 {
 private:
 mint_atomic32_t CurrentValue;
 public:
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & operator = ( const T & object);
 
 inline const T Load() const;
 
 
 inline void Store(const T & object);
 
 
 };
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Atomic()
 {
 mint_store_32_relaxed(&CurrentValue, 0);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Atomic(const T & value)
 {
 mint_store_32_relaxed(&CurrentValue, value);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Atomic(Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> && object)
 {
 mint_store_32_relaxed(&CurrentValue, object.Load());
 mint_store_32_relaxed(&object.CurrentValue, 0);
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Atomic(const Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & object)
 {
 mint_store_32_relaxed(&CurrentValue, object.Load());
 }
 
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::operator = (const Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & atomic)
 {
 mint_store_32_relaxed(&CurrentValue, atomic.Load());
 
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::operator = (Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> && atomic)
 {
 mint_store_32_relaxed(&CurrentValue, atomic.Load());
 mint_store_32_relaxed(&atomic.CurrentValue, 0);
 return (*this);
 }
 
 template<typename T>
 Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>> & Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::operator = (const T & object)
 {
 mint_store_32_relaxed(&CurrentValue, object);
 
 return (*this);
 }
 
 
 template<typename T>
 const T  Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Load() const
 {
 return mint_load_32_relaxed(&CurrentValue);
 }
 
 template<typename T>
 void Atomic<T,PDP::Enable_If_T<PDP::IsBoolean<T>::value>>::Store(const T & object)
 {
 mint_store_32_relaxed(&CurrentValue, object);
 }
 
 
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>>
 {
 private:
 mint_atomic64_t CurrentValue;
 public:
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator = ( const T & object);
 
 inline const T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &) const;
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is64BitFloatingPoint<T>::value>> &);
 };
 
 
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>>
 {
 private:
 mint_atomic32_t CurrentValue;
 public:
 
 
 inline Atomic();
 inline Atomic(const T & object);
 inline Atomic(Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> && object);
 inline Atomic(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator = (const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator = ( Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> && object);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator = ( const T & object);
 
 inline const T Load() const;
 
 
 inline void Store(const T & object);
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator++();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator++(int dummy);
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator--();
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator--(int dummy);
 
 
 
 
 
 
 
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> operator+(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> operator-(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> operator*(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &) const;
 inline const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> operator/(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &) const;
 
 
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator+=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator-=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator*=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &);
 inline Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> & operator/=(const Atomic<T,PDP::Enable_If_T<PDP::Is32BitFloatingPoint<T>::value>> &);
 };
 
 
 
 template<typename T>
 class Atomic<T,PDP::Enable_If_T<PDP::IsCharacter<T>::value>>
 {
 private:
 mint_atomic32_t CurrentValue;
 public:
 };
 
 }
 
 
 
 
 #endif
 */

