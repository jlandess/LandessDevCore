//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_KEYCTL_HPP
#define LANDESSDEVCORE_KEYCTL_HPP
namespace LD
{
    namespace PVE
    {
        class KeyCTL
        {
        private:
            bool mStatus;
        public:
            KeyCTL() noexcept:mStatus{false}
            {

            }
            KeyCTL(bool status) noexcept:mStatus{status}
            {

            }
            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_KEYCTL_HPP
