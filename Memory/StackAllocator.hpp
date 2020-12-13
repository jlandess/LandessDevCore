//
// Created by phoenixflower on 12/6/20.
//

#ifndef LANDESSDEVCORE_STACKALLOCATOR_HPP
#define LANDESSDEVCORE_STACKALLOCATOR_HPP
#include <new>
#include "Primitives/General/StaticArray.hpp"
#include "CorePrimitiveAllocator.hpp"
namespace LD
{
    namespace Mem
    {
        namespace Detail
        {

            struct StackAllocationHeader
            {
                LD::UInteger mRow;
            };


            class StackAllocator
            {
            public:
                StackAllocator(size_t size, void* start) noexcept;
                ~StackAllocator();

                void* allocate(LD::UInteger size, LD::UInteger alignment) noexcept
                {
                    //ASSERT(size != 0);

                    u8 adjustment = pointer_math::alignForwardAdjustmentWithHeader(_current_pos, alignment, sizeof(LD::Mem::Detail::StackAllocator::AllocationHeader));

                    if(_used_memory + adjustment + size > _size)
                        return nullptr;

                    void* aligned_address = pointer_math::add(_current_pos, adjustment);

                    //Add Allocation Header
                    LD::Mem::Detail::StackAllocator::AllocationHeader* header = (LD::Mem::Detail::StackAllocator::AllocationHeader*)(pointer_math::subtract(aligned_address, sizeof(LD::Mem::Detail::StackAllocator::AllocationHeader)));

                    header->adjustment   = adjustment;
                    _current_pos = pointer_math::add(aligned_address, size);

                    _used_memory += size + adjustment;
                    _num_allocations++;

                    return aligned_address;
                }


                void deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept
                {
                    LD::Mem::Detail::StackAllocator::AllocationHeader* header = (LD::Mem::Detail::StackAllocator::AllocationHeader*)(pointer_math::subtract(p, sizeof(LD::Mem::Detail::StackAllocator::AllocationHeader)));

                    _used_memory -= (uptr)_current_pos - (uptr)p + header->adjustment;

                    _current_pos = pointer_math::subtract(p, header->adjustment);

#if _DEBUG
                    _prev_position = header->prev_address;
#endif

                    _num_allocations--;
                }

                bool CanAccomdateRequest(LD::UInteger size, LD::UInteger alignment) const noexcept
                {
                    u8 adjustment = LD::Mem::pointer_math::alignForwardAdjustmentWithHeader(_current_pos, alignment, sizeof(LD::Mem::Detail::StackAllocator::AllocationHeader));
                    //if(_used_memory + adjustment + size > _size)
                        //return nullptr;
                    return not (_used_memory + adjustment + size > _size);
                }
                bool do_is_equal(const StackAllocator & other) const noexcept
                {
                    return  this == &other; ;
                }

                LD::UInteger Size() const noexcept
                {
                    return this->_size;
                }

            private:
                StackAllocator(const StackAllocator&); //Prevent copies because it might cause errors
                StackAllocator& operator=(const StackAllocator&);
                void*         _start;
                LD::UInteger       _size;
                LD::UInteger       _used_memory;
                LD::UInteger       _num_allocations;


                struct AllocationHeader
                {
                    u8 mLine;
                    u8 adjustment;
                };
                void*  _current_pos;
            public:
                static constexpr auto HeaderSize = sizeof(LD::Mem::Detail::StackAllocator::AllocationHeader);
            };

            namespace allocator
            {
                inline LD::Mem::Detail::StackAllocator* newStackAllocator(size_t size, LD::Mem::MemoryResource * allocator, LD::UInteger row)
                {
                    void* p = allocator->allocate(size+sizeof(LD::Mem::Detail::StackAllocator)+sizeof(LD::Mem::Detail::StackAllocationHeader), __alignof(LD::Mem::Detail::StackAllocator));
                    StackAllocator * s =  new (p) LD::Mem::Detail::StackAllocator{size, LD::Mem::pointer_math::add(p,sizeof(LD::Mem::Detail::StackAllocator)+sizeof(LD::Mem::Detail::StackAllocationHeader))};
                    new (LD::Mem::pointer_math::add(p, sizeof(LD::Mem::Detail::StackAllocator))) LD::UInteger {row};
                    return s;
                }

                inline void deleteStackAllocator(StackAllocator * stackAllocaton, LD::Mem::MemoryResource *  allocator, LD::UInteger row)
                {
                    LD::UInteger size = stackAllocaton->Size();
                    stackAllocaton->~StackAllocator();
                    allocator->deallocate(stackAllocaton,size+sizeof(LD::Mem::Detail::StackAllocator)+sizeof(LD::UInteger),__alignof(LD::Mem::Detail::StackAllocator));
                }
            };
        }


        struct StackAllocatorBlock
        {
            LD::Mem::Detail::StackAllocator * mHandle;
            StackAllocatorBlock() noexcept:mHandle{nullptr}
            {

            }
        };



        class StackAllocator : public LD::Mem::MemoryResource
        {
        private:
            LD::UInteger mBlockSize;
            LD::StaticArray<LD::Mem::StackAllocatorBlock,8> mBackend;
            LD::UInteger mCurrentBackend;
            StackAllocator(const StackAllocator&); //Prevent copies because it might cause errors
            StackAllocator& operator=(const StackAllocator&);
        public:

            StackAllocator(StackAllocator && move) noexcept
            {
                for(LD::UInteger n = 0;n<8;++n)
                {
                    this->mBackend[n].mHandle = move.mBackend[n].mHandle;
                    move.mBackend[n].mHandle = nullptr;
                }
            }

            ~StackAllocator() noexcept;
            inline StackAllocator(LD::UInteger blockSize,LD::Mem::MemoryResource * upstream) noexcept:mBlockSize{blockSize},LD::Mem::MemoryResource{upstream},mCurrentBackend{0}
            {

            }
            void* do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept override;

            void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept override;

            bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept override
            {
                return  this == &other; ;
            }
        };
    }
}
#endif //LANDESSDEVCORE_STACKALLOCATOR_HPP
