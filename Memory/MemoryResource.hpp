//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE_MEMORYRESOURCE_HPP
#define LANDESSDEVCORE_MEMORYRESOURCE_HPP
#include <stdlib.h>
#include "Definitions/Integer.hpp"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{
    namespace Mem
    {
        class MemoryResource
        {
        protected:
            MemoryResource * mUpstream;
        protected:
            virtual void * do_allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept = 0;
            virtual void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept = 0;
            virtual bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept = 0;
        public:
            inline MemoryResource() noexcept;
            virtual ~MemoryResource() noexcept{}
            MemoryResource(MemoryResource * upstream) noexcept:mUpstream{upstream}{}
            void * allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept {return this->do_allocate(bytes,alignment);}
            void deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept{return;this->do_deallocate(p,bytes,alignment);}
            bool is_equal(const LD::Mem::MemoryResource & other) const noexcept{return this->do_is_equal(other);}
            MemoryResource * Upstream() const noexcept{return this->mUpstream;}
        };



        class NewDeleteResource: public LD::Mem::MemoryResource
        {
        protected:
            virtual void * do_allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return aligned_alloc(alignment,bytes);
            }
            virtual void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return free(p);
            }
            virtual bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept
            {
                return  this == &other; ;
            }
        public:
            NewDeleteResource(): LD::Mem::MemoryResource(nullptr){}
        };


        template<LD::UInteger Size>
        class StaticMemoryResource: public LD::Mem::MemoryResource
        {
        private:
            LD::StaticArray<unsigned char,Size> mStaticBuffer;
            LD::UInteger mCursor;
        public:
            virtual void * do_allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return nullptr;
            }
            virtual void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return ;
            }
            virtual bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept
            {
                return  this == &other;
            }
        };

        template<LD::UInteger Size>
        class ArenaMemoryResource: public LD::Mem::MemoryResource
        {
        private:
            LD::StaticArray<unsigned char,Size> mStaticBuffer;
            LD::UInteger mCursor;
        public:
            virtual void * do_allocate(LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return nullptr;
            }
            virtual void do_deallocate(void * p, LD::UInteger bytes, LD::UInteger alignment) noexcept
            {
                return ;
            }
            virtual bool do_is_equal(const LD::Mem::MemoryResource & other) const noexcept
            {
                return  this == &other;
            }
        };

        inline NewDeleteResource * GetNewDeleteResource() noexcept
        {
            static NewDeleteResource resource;
            return &resource;
        }



        MemoryResource::MemoryResource() noexcept :mUpstream{LD::Mem::GetNewDeleteResource()}
        {

        }
    }
}
#endif //LANDESSDEVCORE_MEMORYRESOURCE_HPP
