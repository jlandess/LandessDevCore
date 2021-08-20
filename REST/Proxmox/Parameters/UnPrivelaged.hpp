//
// Created by phoenixflower on 2/1/21.
//

#ifndef LANDESSDEVCORE_UNPRIVELAGED_HPP
#define LANDESSDEVCORE_UNPRIVELAGED_HPP
namespace LD
{
    namespace PVE
    {
        class UnPrivelaged
        {
        private:
            bool mValue;
        public:
            UnPrivelaged() noexcept:mValue{false}
            {

            }
            UnPrivelaged(bool value) noexcept:mValue{value}
            {

            }
            bool Status() const noexcept
            {
                return this->mValue;
            }
        };
    }
}
#endif //LANDESSDEVCORE_UNPRIVELAGED_HPP
