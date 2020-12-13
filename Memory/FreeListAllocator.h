//
// Created by phoenixflower on 12/12/20.
//

#ifndef LANDESSDEVCORE_FREELISTALLOCATOR_H
#define LANDESSDEVCORE_FREELISTALLOCATOR_H
#include <new>
#include "Primitives/General/StaticArray.hpp"
#include "CorePrimitiveAllocator.hpp"
namespace LD
{
    namespace Mem
    {
        namespace Detail
        {
            struct FreeListAllocationHeader
            {
                LD::UInteger mRow;
            };

            class FreeListAllocator
            {
            private:
                struct AllocationHeader
                {
                    size_t size;
                    u8     adjustment;
                };

                struct FreeBlock
                {
                    size_t     size;
                    FreeBlock* next;
                };

                FreeListAllocator(const FreeListAllocator&); //Prevent copies because it might cause errors
                FreeListAllocator& operator=(const FreeListAllocator&);

                FreeBlock* _free_blocks;

            public:
                FreeListAllocator(size_t size, void* start);
                ~FreeListAllocator();

                void* allocate(size_t size, u8 alignment)
                {
                    LD::Mem::Detail::FreeListAllocator::FreeBlock* prev_free_block = nullptr;
                    LD::Mem::Detail::FreeListAllocator::FreeBlock* free_block     = _free_blocks;
                    while(free_block != nullptr)
                    {
                        u8 adjustment = pointer_math::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));

                        size_t total_size = size + adjustment;

                        if(free_block->size < total_size)
                        {
                            prev_free_block = free_block;
                            free_block = free_block->next;
                            continue;
                        }

                        if(free_block->size - total_size <= sizeof(AllocationHeader))
                        {
                            //Increase allocation size instead of creating a new FreeBlock
                            total_size = free_block->size;

                            if(prev_free_block != nullptr)
                                prev_free_block->next = free_block->next;
                            else
                                _free_blocks = free_block->next;
                        }
                        else
                        {
                            //Else create a new FreeBlock containing remaining memory
                            LD::Mem::Detail::FreeListAllocator::FreeBlock* next_block = (LD::Mem::Detail::FreeListAllocator::FreeBlock*)( pointer_math::add(free_block, total_size) );
                            next_block->size = free_block->size - total_size;
                            next_block->next = free_block->next;

                            if(prev_free_block != nullptr)
                                prev_free_block->next = next_block;
                            else
                                _free_blocks = next_block;
                        }

                        uptr aligned_address = (uptr)free_block + adjustment;

                        LD::Mem::Detail::FreeListAllocator::AllocationHeader* header = (LD::Mem::Detail::FreeListAllocator::AllocationHeader*)(aligned_address - sizeof(LD::Mem::Detail::FreeListAllocator::AllocationHeader));
                        header->size             = total_size;
                        header->adjustment       = adjustment;

                        _used_memory += total_size;
                        _num_allocations++;

                        //ASSERT(pointer_math::alignForwardAdjustment((void*)aligned_address, alignment) == 0);

                        return (void*)aligned_address;
                    }
                    return nullptr;
                }

                void deallocate(void* p)
                {
                    //ASSERT(p != nullptr);

                    LD::Mem::Detail::FreeListAllocator::AllocationHeader* header = (LD::Mem::Detail::FreeListAllocator::AllocationHeader*)pointer_math::subtract(p, sizeof(LD::Mem::Detail::FreeListAllocator::AllocationHeader));

                    uptr   block_start = reinterpret_cast<uptr>(p) - header->adjustment;
                    size_t block_size  = header->size;
                    uptr   block_end   = block_start + block_size;

                    LD::Mem::Detail::FreeListAllocator::FreeBlock* prev_free_block = nullptr;
                    LD::Mem::Detail::FreeListAllocator::FreeBlock* free_block = _free_blocks;

                    while(free_block != nullptr)
                    {
                        if( (uptr) free_block >= block_end )
                            break;

                        prev_free_block = free_block;
                        free_block = free_block->next;
                    }

                    if(prev_free_block == nullptr)
                    {
                        prev_free_block = (LD::Mem::Detail::FreeListAllocator::FreeBlock*) block_start;
                        prev_free_block->size = block_size;
                        prev_free_block->next = _free_blocks;

                        _free_blocks = prev_free_block;
                    } else if((uptr) prev_free_block + prev_free_block->size == block_start)
                    {
                        prev_free_block->size += block_size;
                    } else
                    {
                        LD::Mem::Detail::FreeListAllocator::FreeBlock* temp = (LD::Mem::Detail::FreeListAllocator::FreeBlock*) block_start;
                        temp->size = block_size;
                        temp->next = prev_free_block->next;
                        prev_free_block->next = temp;

                        prev_free_block = temp;
                    }

                    if( free_block != nullptr && (uptr) free_block == block_end)
                    {
                        prev_free_block->size += free_block->size;
                        prev_free_block->next = free_block->next;
                    }

                    _num_allocations--;
                    _used_memory -= block_size;
                }
            };

            inline LD::Mem::Detail::FreeListAllocator* newFreeListAllocator(size_t size, LD::Mem::MemoryResource * allocator, LD::UInteger row)
            {
                void* p = allocator->allocate(size+sizeof(LD::Mem::Detail::FreeListAllocator)+sizeof(LD::Mem::Detail::FreeListAllocationHeader), __alignof(LD::Mem::Detail::StackAllocator));
                LD::Mem::Detail::FreeListAllocator * s =  new (p) LD::Mem::Detail::FreeListAllocator{size, LD::Mem::pointer_math::add(p,sizeof(LD::Mem::Detail::FreeListAllocator)+sizeof(LD::Mem::Detail::FreeListAllocationHeader))};
                new (LD::Mem::pointer_math::add(p, sizeof(LD::Mem::Detail::FreeListAllocator))) LD::UInteger {row};
                return s;
            }
        }
        class FreeListAllocator: public LD::Mem::MemoryResource
        {
        private:
            LD::StaticArray<LD::Mem::PoolAllocatorBlock,8> mMemoryBlocks;
            LD::UInteger mCurrentBlock
            LD::UInteger mBlockSize;
        public:
            FreeListAllocator(LD::UInteger blockSize) noexcept: LD::Mem::MemoryResource{}
            {
                this->mBlockSize = blockSize;
                this->mCurrentBlock = 0;
            }
            FreeListAllocator(const FreeListAllocator &) = delete;

            ~FreeListAllocator()
            {

            }

            void* do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept override
            {
                if (this->mCurrentBlock == 8)
                {
                    return nullptr;
                }
                if (this->mMemoryBlocks[this->mCurrentBlock].mHandle == nullptr)
                {
                    this->mMemoryBlocks[this->mCurrentBlock].mHandle = LD::Mem::Detail::newFreeListAllocator(this->mObjectSize,this->mObjectAlignment,this->mBlockSize,this->mUpstream,this->mCurrentBlock);
                    void * userPointer = this->mMemoryBlocks[this->mCurrentBlock].mHandle->allocate(size,alignment);
                    return userPointer;
                }else if(this->mMemoryBlocks[this->mCurrentBlock].mHandle != nullptr)
                {
                    void * userPointer = this->mMemoryBlocks[this->mCurrentBlock].mHandle->allocate(size,alignment);
                    if (userPointer == nullptr)
                    {
                        ++this->mCurrentBlock;
                        return do_allocate(size,alignment);
                    }
                    return userPointer;
                }
                return nullptr;
            }
            void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept override
            {
                LD::Mem::Detail::FreeListAllocationHeader* header = (LD::Mem::Detail::FreeListAllocationHeader*)(pointer_math::subtract(p, sizeof(LD::Mem::Detail::FreeListAllocationHeader)));
                this->mMemoryBlocks[header->mRow].mHandle->deallocate(p);
            }

            bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept override
            {
                return  this == &other; ;
            }

        };
    }
}
#endif //LANDESSDEVCORE_FREELISTALLOCATOR_H
