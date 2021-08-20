//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_DATACENTER_H
#define LANDESSDEVCORE_DATACENTER_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class DCPermission
        {
        private:
            LD::UInteger mSystemAudit;
            LD::UInteger mSDNAudit;
            LD::UInteger mSDNAllocate;
        public:
            DCPermission() noexcept: mSystemAudit{0},mSDNAudit{0},mSDNAllocate{0}
            {

            }
            DCPermission(LD::UInteger systemAudit, LD::UInteger sdnAudit, LD::UInteger sdnAllocate) noexcept: mSystemAudit{systemAudit},mSDNAudit{sdnAudit},mSDNAllocate{sdnAllocate}
            {

            }
            bool CanAuditSystem() const noexcept
            {
                return this->mSystemAudit == 1;
            }

            bool CanAuditSDN() const noexcept
            {
                return this->mSDNAudit == 1;
            }
            bool CanAllocateSDN() const noexcept
            {
                return this->mSDNAllocate == 1;
            }
        };
    }
}
#endif //LANDESSDEVCORE_DATACENTER_H
