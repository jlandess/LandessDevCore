//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_ACCESS_H
#define LANDESSDEVCORE_ACCESS_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class AccessPermission
        {

        private:
            LD::UInteger mAccessGroupAllocate;
            LD::UInteger mAccessUserModify;
            LD::UInteger mAccessPermissionsModify;
        public:
            AccessPermission() noexcept: mAccessGroupAllocate{0},mAccessUserModify{0},mAccessPermissionsModify{0}
            {

            }

            AccessPermission(LD::UInteger accessGroupAllocate,
                             LD::UInteger accessUserModify,
                             LD::UInteger accessPermissionsModify) noexcept:
                    mAccessGroupAllocate{accessGroupAllocate},
                    mAccessUserModify{accessUserModify},
                    mAccessPermissionsModify{accessPermissionsModify}
            {

            }

            bool CanAllocateGroup() const noexcept
            {
                return this->mAccessGroupAllocate == 1;
            }

            bool CanModifyUser() const noexcept
            {
                return this->mAccessUserModify == 1;
            }

            bool CanModifyPermissions() const noexcept
            {
                return this->mAccessPermissionsModify == 1;
            }
        };
    }
}
#endif //LANDESSDEVCORE_ACCESS_H
