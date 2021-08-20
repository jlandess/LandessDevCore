//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_ROOTFS_HPP
#define LANDESSDEVCORE_ROOTFS_HPP
#include "SystemHeurestics/DiskSize.hpp"
namespace LD
{
    namespace PVE
    {
        template<typename StringType>
        class RootFS
        {
        private:
            LD::DiskSize<LD::GigaByte<LD::UInteger>> mSize;
            StringType mVolumeLocation;
        public:
            RootFS() = default;
            RootFS(LD::DiskSize<LD::GigaByte<LD::UInteger>> diskSize, StringType && volumeLocation) noexcept: mSize{diskSize},mVolumeLocation{LD::Forward<StringType>(volumeLocation)}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"rootfs"};
            }
            auto Value() const noexcept
            {
                return this->mVolumeLocation+LD::ImmutableString{":"}+LD::ToImmutableString(LD::UInteger(mSize.Size().NativeRepresentation().Value()));
            }
        };

        template<typename StringType> RootFS(LD::DiskSize<LD::GigaByte<LD::UInteger>> diskSize, StringType && volumeLocation) -> RootFS<StringType>;
    }
}
#endif //LANDESSDEVCORE_ROOTFS_HPP
