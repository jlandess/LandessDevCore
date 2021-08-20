//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_STORAGE_H
#define LANDESSDEVCORE_STORAGE_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class StoragePermission
        {
        private:
            LD::UInteger mDataStoreAudit;
            LD::UInteger mDataStoreAllocationTemplate;
            LD::UInteger mDataStoreAllocateSpace;
            LD::UInteger mDataStorePermissionsModify;
            LD::UInteger mDataStoreAllocate;
        public:

            StoragePermission() noexcept:mDataStoreAudit{0},mDataStoreAllocationTemplate{0},mDataStoreAllocateSpace{0},mDataStorePermissionsModify{0},mDataStoreAllocate{0}
            {

            }
            StoragePermission(LD::UInteger dataStoreAudit,
                              LD::UInteger dataStoreAllocationTemplate,
                              LD::UInteger dataStoreAllocateSpace,
                              LD::UInteger dataStorePermissionsModify,
                              LD::UInteger dataStoreAllocate) noexcept:
                    mDataStoreAudit{dataStoreAudit},
                    mDataStoreAllocationTemplate{dataStoreAllocationTemplate},
                    mDataStoreAllocateSpace{dataStoreAllocateSpace},
                    mDataStorePermissionsModify{dataStorePermissionsModify},
                    mDataStoreAllocate{dataStoreAllocate}
            {

            }


            bool CanAudit() const
            {
                return this->mDataStoreAudit == 1;
            }

            bool CanCreateTemplate() const
            {
                return this->mDataStoreAllocationTemplate == 1;
            }

            bool CanAllocateSpace() const
            {
                return this->mDataStoreAllocateSpace == 1;
            }

            bool CanModify() const
            {
                return this->mDataStorePermissionsModify == 1;
            }

            bool CanAllocate() const
            {
                return this->mDataStoreAllocate;
            }
        };
    }
}
#endif //LANDESSDEVCORE_STORAGE_H
