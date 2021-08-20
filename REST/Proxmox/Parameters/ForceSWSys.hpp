//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_FORCESWSYS_HPP
#define LANDESSDEVCORE_FORCESWSYS_HPP
namespace LD
{
    namespace PVE
    {
        class ForceSWSys
        {
        private:
            bool mStatus;
        public:
            ForceSWSys() noexcept:mStatus{false}
            {

            }
            ForceSWSys(bool status) noexcept:mStatus{status}
            {

            }
            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_FORCESWSYS_HPP
