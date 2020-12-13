//
// Created by phoenixflower on 12/5/20.
//
#include "LinearAllocator.h"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    namespace Mem
    {
        LinearAllocator::LinearAllocator(LD::UInteger blockSize) noexcept:LD::Mem::MemoryResource{}
        {

            this->mBlockSize = blockSize;
            this->mCurrentBackend = 0;
            this->_current_pos = this->mBackend[this->mCurrentBackend].mBlock;
        }
        LinearAllocator::LinearAllocator(LD::UInteger blockSize, LD::Mem::MemoryResource * upstream) noexcept:LD::Mem::MemoryResource{upstream}
                //: Allocator(size, start), _current_pos(start)
        {
            this->mBlockSize = blockSize;
            this->mCurrentBackend = 0;
            this->_current_pos = this->mBackend[this->mCurrentBackend].mBlock;
        }

        LinearAllocator::~LinearAllocator() noexcept
        {
            _current_pos   = nullptr;
            for(LD::UInteger n = 0;n<7;++n)
            {
                this->mUpstream->deallocate(this->mBackend[n].mBlock,this->mBackend[n].mBlockSize,this->mBackend[n].mAlignment);
            }
        }

        void* LinearAllocator::do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept
        {
            if (this->mCurrentBackend < 8)
            {
                const bool hasAllocated = this->mBackend[this->mCurrentBackend].mHasAllocated;
                if (!hasAllocated)
                {
                    LD::UInteger requestedAllocationBlock = LD::Max(this->mBlockSize,size);
                    void * requestedBlock = this->mUpstream->allocate(requestedAllocationBlock,alignment);
                    if (requestedBlock == nullptr)
                    {
                        return nullptr;
                    }
                    this->mBackend[this->mCurrentBackend].mBlock = requestedBlock;
                    this->mBackend[this->mCurrentBackend].mBlockSize = requestedAllocationBlock;
                    this->mBackend[this->mCurrentBackend].mNumberOfAllocations = 0;
                    this->mBackend[this->mCurrentBackend].mHasAllocated = true;
                    this->mBackend[this->mCurrentBackend].mAlignment = alignment;
                    this->_current_pos = this->mBackend[this->mCurrentBackend].mBlock;

                }
                PDP::UInteger adjustment =  pointer_math::alignForwardAdjustment(_current_pos, alignment);
                const bool isFull = (this->mBackend[this->mCurrentBackend].mUsedMemory + adjustment + size) >this->mBackend[this->mCurrentBackend].mBlockSize;
                if(hasAllocated && isFull)
                {
                    ++this->mCurrentBackend;

                    LD::UInteger requestedAllocationBlock = LD::Max(this->mBlockSize,size);
                    void * requestedBlock = this->mUpstream->allocate(requestedAllocationBlock,alignment);
                    if (requestedBlock == nullptr)
                    {
                        return nullptr;
                    }
                    this->mBackend[this->mCurrentBackend].mBlock = requestedBlock;
                    this->mBackend[this->mCurrentBackend].mBlockSize = requestedAllocationBlock;
                    this->mBackend[this->mCurrentBackend].mNumberOfAllocations = 0;
                    this->mBackend[this->mCurrentBackend].mHasAllocated = true;
                    this->mBackend[this->mCurrentBackend].mAlignment = alignment;
                    this->_current_pos = this->mBackend[this->mCurrentBackend].mBlock;
                }
                uptr aligned_address = (uptr)_current_pos + adjustment;
                _current_pos = (void*)(aligned_address + size);
                this->mBackend[this->mCurrentBackend].mUsedMemory += (size + adjustment);
                this->mBackend[this->mCurrentBackend].mNumberOfAllocations++;
                return (void*)aligned_address;
            }
            //todo handle what happens when you exhaust the first 8 layers
            else
            {

            }
            return nullptr;
        }
        void LinearAllocator::do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept
        {

        }

        void LinearAllocator::Clear()
        {
            for(LD::UInteger n = 0;n<8;++n)
            {
                this->mBackend[n].mHasAllocated = false;
                this->mBackend[n].mBlockSize = 0;
                this->mBackend[n].mUsedMemory = 0;
                this->mBackend[n].mNumberOfAllocations = 0;

            }
            this->mCurrentBackend = 0;
            this->_current_pos = this->mBackend[this->mCurrentBackend].mBlock;
        }
    }
}