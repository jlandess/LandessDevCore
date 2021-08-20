//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_NODE_H
#define LANDESSDEVCORE_NODE_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class NodePermission
        {
        private:
            LD::UInteger mNodeSysLog;
            LD::UInteger mNodePermissionsModify;
            LD::UInteger mNodeSysPowerManagment ;
            LD::UInteger mNodeSystemModify;
            LD::UInteger mNodeSystemAudit ;
            LD::UInteger mNodeSystemConsole;
        public:
            NodePermission() noexcept:mNodeSysLog{0},mNodePermissionsModify{0},mNodeSysPowerManagment{0},mNodeSystemModify{0},mNodeSystemAudit{0},mNodeSystemConsole{0}
            {

            }

            NodePermission(
                    LD::UInteger nodeSysLog,
                    LD::UInteger nodePermissionsModify,
                    LD::UInteger sysPowerManagement,
                    LD::UInteger nodeSystemModify,
                    LD::UInteger nodeSystemAudit,
                    LD::UInteger nodeSystemConsole) noexcept:
                    mNodeSysLog{nodeSysLog},
                    mNodePermissionsModify{nodePermissionsModify},
                    mNodeSystemModify{nodeSystemModify},
                    mNodeSysPowerManagment{sysPowerManagement},
                    mNodeSystemAudit{nodeSystemAudit},
                    mNodeSystemConsole{nodeSystemConsole}
            {

            }
        };
    }
}
#endif //LANDESSDEVCORE_NODE_H
