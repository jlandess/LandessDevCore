//
// Created by phoenixflower on 12/12/20.
//

#ifndef LANDESSDEVCORE_POOLALLOCATOR_H
#define LANDESSDEVCORE_POOLALLOCATOR_H
#include <memory>
#include "CorePrimitiveAllocator.hpp"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{
    namespace Mem
    {
        namespace Detail
        {
            struct PoolAllocationHeader {
                LD::UInteger mRow;
            };

            class PoolAllocator
            {
            private:
                size_t     _objectSize;
                u8         _objectAlignment;
                LD::UInteger       _size;
                LD::UInteger       _used_memory;
                LD::UInteger       _num_allocations;
                void**     _free_list;
            public:
                PoolAllocator(size_t objectSize, u8 objectAlignment, size_t size, void *mem) noexcept:_objectSize{objectSize},_objectAlignment{objectAlignment},_size{size}
                {
                    //Calculate adjustment needed to keep object correctly aligned
                    u8 adjustment = pointer_math::alignForwardAdjustment(mem, objectAlignment);

                    _free_list = (void**)pointer_math::add(mem, adjustment);

                    size_t numObjects = (size-adjustment)/objectSize;

                    void** p = _free_list;

                    //Initialize free blocks list
                    for(size_t i = 0; i < numObjects-1; i++)
                    {
                        *p = pointer_math::add(p, objectSize );
                        p = (void**) *p;
                    }

                    *p = nullptr;
                }
                LD::UInteger Size() const noexcept
                {
                    return this->_size;
                }
                LD::UInteger Alignment() const noexcept
                {
                    return this->_objectAlignment;
                }

                ~PoolAllocator()
                {
                    _free_list = nullptr;
                }

                void *allocate(size_t size, u8 alignment)
                {
                    //ASSERT(size == _objectSize && alignment == _objectAlignment);

                    if(_free_list == nullptr || _used_memory == _size)
                        return nullptr;

                    void* p = _free_list;

                    _free_list = (void**)(*_free_list);

                    _used_memory += _objectSize;
                    _num_allocations++;

                    return p;
                }

                void deallocate(void *p)
                {
                    *((void**)p) = _free_list;

                    _free_list = (void**)p;

                    _used_memory -= _objectSize;
                    _num_allocations--;
                }

            private:
                PoolAllocator(const PoolAllocator &); //Prevent copies because it might cause errors
                PoolAllocator &operator=(const PoolAllocator &);
            };

            inline LD::Mem::Detail::PoolAllocator *
            newPoolAllocator(size_t objectSize, u8 objectAlignment, size_t size, LD::Mem::MemoryResource *allocator,
                             LD::UInteger inputRow) {
                void *p = allocator->allocate(
                        size + sizeof(LD::Mem::Detail::PoolAllocator) + sizeof(LD::Mem::Detail::PoolAllocationHeader),
                        __alignof(LD::Mem::Detail::PoolAllocator));
                LD::UInteger *row = (LD::UInteger *) pointer_math::add(p, sizeof(PoolAllocator));
                (*row) = inputRow;
                return new(p) PoolAllocator(objectSize, objectAlignment, size, pointer_math::add(p,
                                                                                                 sizeof(PoolAllocator) +
                                                                                                 sizeof(LD::Mem::Detail::PoolAllocationHeader)));
            }

            inline void deletePoolAllocator(PoolAllocator  *poolAllocator, LD::Mem::MemoryResource *resource) {
                poolAllocator->~PoolAllocator();

                //allocator.deallocate(&poolAllocator);
            }
        }

        struct PoolAllocatorBlock
        {
            LD::Mem::Detail::PoolAllocator * mHandle;
            PoolAllocatorBlock() noexcept:mHandle{nullptr}
            {

            }
        };
        class PoolAllocator : public LD::Mem::MemoryResource
        {
        private:
            LD::StaticArray<LD::Mem::PoolAllocatorBlock,8> mMemoryBlocks;
            LD::UInteger mObjectSize;
            LD::UInteger mObjectAlignment;
            LD::UInteger mCurrentBlock;
            LD::UInteger mBlockSize;
        public:
            PoolAllocator(size_t objectSize, u8 objectAlignment) noexcept:LD::Mem::MemoryResource{},mObjectSize{objectSize},mObjectAlignment{objectAlignment},mCurrentBlock{0}
            {

            }


            PoolAllocator(size_t objectSize, u8 objectAlignment,LD::Mem::MemoryResource * resource) noexcept:LD::Mem::MemoryResource{resource},mObjectSize{objectSize},mObjectAlignment{objectAlignment},mCurrentBlock{0}
            {

            }
            ~PoolAllocator() noexcept
            {
                for(LD::UInteger n = 0;n<8;++n)
                {
                    using MType = LD::Mem::Detail::PoolAllocator;
                    //the header to indicate what row this memory allocator came from
                    //LD::Mem::Detail::PoolAllocator * memorySequence =(LD::Mem::Detail::PoolAllocator*)(pointer_math::subtract(this->mMemoryBlocks[n].mHandle, sizeof(LD::Mem::Detail::StackAllocator::HeaderSize)));
                    LD::UInteger size = this->mMemoryBlocks[n].mHandle->Size();
                    this->mMemoryBlocks[n].mHandle->~MType();
                    this->mUpstream->deallocate(this->mMemoryBlocks[n].mHandle,size+sizeof(LD::Mem::Detail::PoolAllocationHeader)+sizeof(LD::Mem::Detail::PoolAllocator),__alignof(LD::Mem::Detail::PoolAllocator));
                }
            }

            void* do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept override
            {
                if (this->mCurrentBlock == 8)
                {
                    return nullptr;
                }
                if (this->mMemoryBlocks[this->mCurrentBlock].mHandle == nullptr)
                {
                    this->mMemoryBlocks[this->mCurrentBlock].mHandle = LD::Mem::Detail::newPoolAllocator(this->mObjectSize,this->mObjectAlignment,this->mBlockSize,this->mUpstream,this->mCurrentBlock);
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
                LD::Mem::Detail::PoolAllocationHeader* header = (LD::Mem::Detail::PoolAllocationHeader*)(pointer_math::subtract(p, sizeof(LD::Mem::Detail::PoolAllocationHeader)));
                this->mMemoryBlocks[header->mRow].mHandle->deallocate(p);

            }

            bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept override
            {
                return  this == &other; ;
            }
        };
    }
}
#endif //LANDESSDEVCORE_POOLALLOCATOR_H
