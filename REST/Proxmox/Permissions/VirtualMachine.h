//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_VIRTUALMACHINE_HPP
#define LANDESSDEVCORE_VIRTUALMACHINE_H
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        class VMPermission
        {
        private:
            LD::UInteger mVMSnapShot;
            LD::UInteger mVMConfigOptions;
            LD::UInteger mVMConfigCDRom;
            LD::UInteger mVMConfigCPU;
            LD::UInteger mVMConfigNetwork;
            LD::UInteger mVMConfigHardwareType;
            LD::UInteger mVMMigrate;
            LD::UInteger mVMConfigMemory;
            LD::UInteger mVMConfigCloudInit;
            LD::UInteger mVMAudit;
            LD::UInteger mVMPowerManagement ;
            LD::UInteger mVMPermissionsModify;
            LD::UInteger mVMAllocate ;
            LD::UInteger mVMConfigDisk ;
            LD::UInteger mVMSnapshotRollback ;
            LD::UInteger mVMBackup ;
            LD::UInteger mVMConsole ;
            LD::UInteger mVMClone ;
            LD::UInteger mVMMonitor;
        public:
            VMPermission() noexcept:
                    mVMSnapShot{0},
                    mVMConfigOptions{0},
                    mVMConfigCDRom{0},
                    mVMConfigCPU{0},
                    mVMConfigNetwork{0},
                    mVMConfigHardwareType{0},
                    mVMMigrate{0},
                    mVMConfigMemory{0},
                    mVMConfigCloudInit{0},
                    mVMAudit{0},
                    mVMPowerManagement{0},
                    mVMPermissionsModify{0},
                    mVMAllocate{0},
                    mVMConfigDisk{0},
                    mVMSnapshotRollback{0},
                    mVMBackup{0},
                    mVMConsole{0},
                    mVMClone{0},
                    mVMMonitor{0}
            {

            }
            VMPermission(LD::UInteger vmSnapShot,
                         LD::UInteger vmConfigOptions,
                         LD::UInteger vmConfigCDRom,
                         LD::UInteger vmConfigCPU,
                         LD::UInteger vmConfigNetwork,
                         LD::UInteger vmConfigHardwareType,
                         LD::UInteger vmMigrate,
                         LD::UInteger vmConfigMemory,
                         LD::UInteger vmConfigCloudInit,
                         LD::UInteger vmAudit,
                         LD::UInteger vmPowerManagement,
                         LD::UInteger vmPermissionsModify,
                         LD::UInteger vmAllocate,
                         LD::UInteger vmConfigDisk,
                         LD::UInteger vmSnapshotRollback,
                         LD::UInteger vmBackup,
                         LD::UInteger vmConsole,
                         LD::UInteger vmClone,
                         LD::UInteger vmMonitor) noexcept:
                    mVMSnapShot{vmSnapShot},
                    mVMConfigOptions{vmConfigOptions},
                    mVMConfigCDRom{vmConfigCDRom},
                    mVMConfigCPU{vmConfigCPU},
                    mVMConfigNetwork{vmConfigNetwork},
                    mVMConfigHardwareType{vmConfigHardwareType},
                    mVMMigrate{vmMigrate},
                    mVMConfigMemory{vmConfigMemory},
                    mVMConfigCloudInit{vmConfigCloudInit},
                    mVMAudit{vmAudit},
                    mVMPowerManagement{vmPowerManagement},
                    mVMPermissionsModify{vmPermissionsModify},
                    mVMAllocate{vmAllocate},
                    mVMConfigDisk{vmConfigDisk},
                    mVMSnapshotRollback{vmSnapshotRollback},
                    mVMBackup{vmBackup},
                    mVMConsole{vmConsole},
                    mVMClone{vmClone},
                    mVMMonitor{vmMonitor}
            {

            }
            bool CanSnapShot() const
            {
                return this->mVMSnapShot == 1;
            }
            bool CanModifyConfiguration() const
            {
                return this->mVMConfigOptions ==1;
            }
            bool CanConfigCDROMs() const
            {
                return this->mVMConfigCDRom == 1;
            }
            bool CanConfigCPU() const
            {
                return this->mVMConfigCPU == 1;
            }
            bool CanConfigNetwork() const
            {
                return this->mVMConfigNetwork == 1;
            }
            bool CanConfigHardwareType() const
            {
                return this->mVMConfigHardwareType ==1;
            }
            bool CanMigrate() const
            {
                return this->mVMMigrate == 1;
            }
            bool CanConfigureMemory() const
            {
                return this->mVMConfigMemory == 1;
            }
            bool CanConfigCloudInit() const
            {
                return this->mVMConfigCloudInit == 1;
            }
            bool CanAudit() const
            {
                return this->mVMAudit == 1;
            }

            bool CanApplyPowerManagement() const
            {
                return this->mVMPowerManagement == 1;
            }
            bool CanModify() const
            {
                return this->mVMPermissionsModify == 1;
            }
            bool CanAllocate() const
            {
                return this->mVMAllocate == 1;
            }
            bool CanConfigDisk() const
            {
                return this->mVMConfigDisk == 1;
            }
            bool CanUtilizeSnapshotRollBack() const
            {
                return this->mVMSnapshotRollback == 1;
            }
            bool CanBackup() const
            {
                return this->mVMBackup == 1;
            }
            bool CanUtilizeConsole() const
            {
                return this->mVMConsole == 1;
            }
            bool CanClone() const
            {
                return this->mVMClone == 1;
            }
            bool CanMonitor() const
            {
                return this->mVMMonitor == 1;
            }
        };
    }
}
#endif //LANDESSDEVCORE_VIRTUALMACHINE_HPP
