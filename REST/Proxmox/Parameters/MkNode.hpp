//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_MKNODE_HPP
#define LANDESSDEVCORE_MKNODE_HPP
namespace LD
{
    namespace PVE
    {
        class MkNode
        {
        private:
            bool mStatus;
        public:
            MkNode() noexcept:mStatus{false}
            {

            }
            MkNode(bool status) noexcept:mStatus{status}
            {

            }

            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_MKNODE_HPP
