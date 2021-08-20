//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_SDN_H
#define LANDESSDEVCORE_SDN_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class SDNPermission
        {
        private:
            LD::UInteger mSDNAudit;
            LD::UInteger mSDNAllocate;
            LD::UInteger mSDNPermissionsModify;
        public:
            SDNPermission() noexcept:mSDNAudit{0},mSDNAllocate{0},mSDNPermissionsModify{0}
            {

            }
            SDNPermission(LD::UInteger sdnAudit,
                          LD::UInteger sdnAllocate,
                          LD::UInteger sdnPermissionsModify) noexcept:
                    mSDNAudit{sdnAudit},
                    mSDNAllocate{sdnAllocate},
                    mSDNPermissionsModify{sdnPermissionsModify}
            {

            }


            bool CanAuditSDN() const noexcept
            {
                return this->mSDNAudit;
            }

            bool CanAllocateSDN() const noexcept
            {
                return this->mSDNAllocate;
            }

            bool CanModifySDN() const noexcept
            {
                return this->mSDNPermissionsModify;
            }
        };
    }
}
#endif //LANDESSDEVCORE_SDN_H
