//
// Created by phoenixflower on 12/6/20.
//

#include "StackAllocator.hpp"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    namespace Mem
    {
        namespace Detail
        {
            StackAllocator::StackAllocator(size_t size, void* start) noexcept:_size{size},_current_pos{start}
            {
            }

            StackAllocator::~StackAllocator()
            {

            }
        }


        StackAllocator::~StackAllocator() noexcept
        {
            for(LD::UInteger n = 0;n<8;++n)
            {
                using MType = LD::Mem::Detail::StackAllocator;
                //the header to indicate what row this memory allocator came from
                LD::Mem::Detail::StackAllocator * memorySequence =(LD::Mem::Detail::StackAllocator*)(pointer_math::subtract(this->mBackend[n].mHandle, sizeof(LD::Mem::Detail::StackAllocator::HeaderSize)));
                LD::UInteger managedSize = memorySequence->Size();
                memorySequence->~MType();
                this->mUpstream->deallocate(memorySequence,managedSize+sizeof(LD::Mem::Detail::StackAllocator)+sizeof(LD::Mem::Detail::StackAllocationHeader),__alignof(LD::Mem::Detail::StackAllocator));


            }
        }
        void * LD::Mem::StackAllocator::do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept
        {
            if (this->mCurrentBackend == 8)
            {
                return nullptr;
            }
            if (this->mCurrentBackend < 8)
            {
                if (this->mBackend[this->mCurrentBackend].mHandle == nullptr)
                {
                    this->mBackend[this->mCurrentBackend].mHandle =  LD::Mem::Detail::allocator::newStackAllocator(this->mBlockSize,this->mUpstream,this->mCurrentBackend);
                    void * userPointer = this->mBackend[this->mCurrentBackend].mHandle->allocate(size,alignment);
                    return userPointer;
                }else if(this->mBackend[this->mCurrentBackend].mHandle != nullptr)
                {
                    void * userPointer = this->mBackend[this->mCurrentBackend].mHandle->allocate(size,alignment);
                    if (userPointer == nullptr)
                    {
                        ++this->mCurrentBackend;
                        return do_allocate(size,alignment);
                    }
                    return userPointer;
                }
            }
            return nullptr;
        }

        void StackAllocator::do_deallocate(void *p, LD::UInteger size, LD::UInteger alignment) noexcept
        {

            LD::Mem::Detail::StackAllocationHeader* header = (LD::Mem::Detail::StackAllocationHeader*)(pointer_math::subtract(p, sizeof(LD::Mem::Detail::StackAllocationHeader)));
            LD::UInteger row = header->mRow;
            if (row < 8)
            {
                this->mBackend[row].mHandle->deallocate(p,size,alignment);
            }


        }
    }
}