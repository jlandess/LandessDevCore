//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_NESTING_HPP
#define LANDESSDEVCORE_NESTING_HPP
namespace LD
{
    namespace PVE
    {
        class Nesting
        {
        private:
            bool mStatus;
        public:
            Nesting() noexcept:mStatus{false}
            {

            }
            Nesting(bool status) noexcept: mStatus{status}
            {

            }
            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_NESTING_HPP
