//
//  AutomaticReferenceCounting.cpp
//  Memory
//
//  Created by James Landess on 7/16/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//
#include <Memory/AutomaticReferenceCounting.h>
namespace PDP
{
    namespace Memory
    {
        
        AutomaticReferencedCountingObject::~AutomaticReferencedCountingObject()
        {
            
        }
        
        
        void AutomaticReferencedCountingObject::Release(bool dealloc) const noexcept
        {
            --m_refCount;
            if (m_refCount.load(LD::AcquireRelease) == 0 && dealloc)
            {
                this->OnFinalize();
                delete this;
            } else if (m_refCount.load(LD::AcquireRelease) < 0) {
                fprintf(stderr, "Internal error: Object reference count < 0!\n");
                abort();
            }
        }
    }
}
