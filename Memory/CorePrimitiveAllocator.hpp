//
// Created by phoenixflower on 12/5/20.
//

#ifndef LANDESSDEVCORE_COREPRIMITIVEALLOCATOR_HPP
#define LANDESSDEVCORE_COREPRIMITIVEALLOCATOR_HPP

#include <cstdint>
#include "Definitions/Integer.hpp"
#include "MemoryResource.hpp"
namespace LD
{
    namespace Mem
    {
        typedef unsigned int uint;

        typedef uint8_t u8;
        typedef uint16_t u16;
        typedef uint32_t u32;
        typedef uint64_t u64;

        typedef uintptr_t uptr;

        namespace pointer_math
        {
            inline void* alignForward(void* address, u8 alignment)
            {
                return (void*)( ( reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment-1) ) & static_cast<uptr>(~(alignment-1)) );
            }

            inline const void* alignForward(const void* address, u8 alignment)
            {
                return (void*)( ( reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment-1) ) & static_cast<uptr>(~(alignment-1)) );
            }

            inline void* alignBackward(void* address, u8 alignment)
            {
                return (void*)( reinterpret_cast<uptr>(address) & static_cast<uptr>(~(alignment-1)) );
            }

            inline const void* alignBackward(const void* address, u8 alignment)
            {
                return (void*)( reinterpret_cast<uptr>(address) & static_cast<uptr>(~(alignment-1)) );
            }

            inline u8 alignForwardAdjustment(const void* address, u8 alignment)
            {
                u8 adjustment =  alignment - ( reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment-1) );

                if(adjustment == alignment)
                    return 0; //already aligned

                return adjustment;
            }

            inline u8 alignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize)
            {
                u8 adjustment =  alignForwardAdjustment(address, alignment);

                u8 neededSpace = headerSize;

                if(adjustment < neededSpace)
                {
                    neededSpace -= adjustment;

                    //Increase adjustment to fit header
                    adjustment += alignment * (neededSpace / alignment);

                    if(neededSpace % alignment > 0)
                        adjustment += alignment;
                }

                return adjustment;
            }

            inline u8 alignBackwardAdjustment(const void* address, LD::UInteger alignment)
            {
                u8 adjustment =  reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment-1);

                if(adjustment == alignment)
                    return 0; //already aligned

                return adjustment;
            }

            inline void* add(void* p, LD::UInteger x)
            {
                return (void*)( reinterpret_cast<uptr>(p) + x);
            }

            inline const void* add(const void* p, LD::UInteger x)
            {
                return (const void*)( reinterpret_cast<uptr>(p) + x);
            }

            inline void* subtract(void* p, LD::UInteger x)
            {
                return (void*)( reinterpret_cast<uptr>(p) - x);
            }

            inline const void* subtract(const void* p, LD::UInteger x)
            {
                return (const void*)( reinterpret_cast<uptr>(p) - x);
            }
        }


        class Allocator: public LD::Mem::MemoryResource
        {
        public:
            Allocator(LD::UInteger size, void* start)
            {
                _start          = start;
                _size           = size;

                _used_memory     = 0;
                _num_allocations = 0;
            }

            virtual ~Allocator()
            {

                _start = nullptr;
                _size   = 0;
            }

            virtual void* do_allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept = 0;

            virtual void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept = 0;

            void* getStart() const
            {
                return _start;
            }

            size_t getSize() const
            {
                return _size;
            }

            size_t getUsedMemory() const
            {
                return _used_memory;
            }

            size_t getNumAllocations() const
            {
                return _num_allocations;
            }

        protected:
            void*         _start;
            LD::UInteger       _size;

            LD::UInteger       _used_memory;
            LD::UInteger       _num_allocations;
        };

        inline void* alignForward(void* address, PDP::UInteger alignment)
        {
            return (void*)( ( reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment-1) ) & static_cast<uptr>(~(alignment-1)) );
        }

        inline PDP::UInteger alignForwardAdjustment(const void* address, PDP::UInteger alignment)
        {
            PDP::UInteger adjustment =  alignment - ( reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment-1) );

            if(adjustment == alignment)
                return 0; //already aligned

            return adjustment;
        }
        /*
        namespace allocator
        {
            template <class T> T* allocateNew(Allocator& allocator)
            {
                return new (allocator.allocate(sizeof(T), __alignof(T))) T;
            }

            template <class T> T* allocateNew(Allocator& allocator, const T& t)
            {
                return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
            }

            template<class T> void deallocateDelete(Allocator& allocator, T& object)
            {
                object.~T();
                allocator.deallocate(&object);
            }

            template<class T> T* allocateArray(Allocator& allocator, size_t length)
            {

                PDP::UInteger headerSize = sizeof(size_t)/sizeof(T);

                if(sizeof(size_t)%sizeof(T) > 0)
                    headerSize += 1;

                //Allocate extra space to store array length in the bytes before the array
                T* p = ( (T*) allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T)) ) + headerSize;

                *( ((size_t*)p) - 1 ) = length;

                for(size_t i = 0; i < length; i++)
                    new (&p[i]) T;

                return p;
            }

            template<class T> void deallocateArray(Allocator& allocator, T* array)
            {

                size_t length = *( ((size_t*)array) - 1 );

                for(size_t i = 0; i < length; i++)
                    array[i].~T();

                //Calculate how much extra memory was allocated to store the length before the array
                PDP::UInteger headerSize = sizeof(size_t)/sizeof(T);

                if(sizeof(size_t)%sizeof(T) > 0)
                    headerSize += 1;

                allocator.deallocate(array - headerSize);
            }
        };
        */
    }
}
#endif //LANDESSDEVCORE_COREPRIMITIVEALLOCATOR_HPP
