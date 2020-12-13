//
// Created by phoenixflower on 12/5/20.
//

#ifndef LANDESSDEVCORE_LINEARALLOCATOR_H
#define LANDESSDEVCORE_LINEARALLOCATOR_H
#include "CorePrimitiveAllocator.hpp"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{
    namespace Mem
    {

        struct LinearAllocatorBlock
        {
            void* mBlock;
            LD::UInteger mUsedMemory;
            LD::UInteger mNumberOfAllocations;
            LD::UInteger mBlockSize;
            LD::UInteger mHasAllocated;
            LD::UInteger mAlignment;
            LinearAllocatorBlock():mBlock{nullptr},mUsedMemory{0},mNumberOfAllocations{0},mBlockSize{0},mHasAllocated{0},mAlignment{0}
            {

            }

            LD::UInteger NumberOfFreeBytes() const noexcept
            {
                return this->mBlockSize-this->mUsedMemory;
            }
        };
        class LinearAllocator : public LD::Mem::MemoryResource
        {
        public:
        LinearAllocator(LD::UInteger blockSize) noexcept;
            LinearAllocator(LD::UInteger blockSize, LD::Mem::MemoryResource * upstream) noexcept;
            ~LinearAllocator() noexcept;

            void* do_allocate(LD::UInteger size, LD::UInteger alignment) noexcept override;

            void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept override;

            bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept override
            {
                return  this == &other; ;
            }
            void Clear();
        private:
            LinearAllocator(const LinearAllocator&); //Prevent copies because it might cause errors
            LinearAllocator& operator=(const LinearAllocator&);
            LD::StaticArray<LD::Mem::LinearAllocatorBlock,8> mBackend;
            LD::UInteger mCurrentBackend;
            LD::UInteger mBlockSize;
            void* _current_pos;
        };
    }
}
#endif //LANDESSDEVCORE_LINEARALLOCATOR_H
