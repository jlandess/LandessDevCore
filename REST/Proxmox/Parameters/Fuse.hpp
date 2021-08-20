//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_FUSE_HPP
#define LANDESSDEVCORE_FUSE_HPP
namespace LD
{
    namespace PVE
    {
        class Fuse
        {
        private:
            bool mStatus;
        public:
            Fuse() noexcept:mStatus{false}
            {

            }
            Fuse(bool status) noexcept: mStatus{status}
            {

            }
            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_FUSE_HPP
