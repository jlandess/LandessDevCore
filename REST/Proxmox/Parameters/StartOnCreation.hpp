//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_STARTONCREATION_HPP
#define LANDESSDEVCORE_STARTONCREATION_HPP
namespace LD
{
    namespace PVE
    {
        class StartOnCreation
        {
        private:
            bool mStatus;
        public:
            StartOnCreation() noexcept:mStatus{false}
            {

            }
            StartOnCreation(bool status)  noexcept:mStatus{status}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"start"};
            }
            auto Value() const noexcept
            {
                return LD::ToImmutableString(LD::UInteger(this->mStatus));
            }
            bool Status() const noexcept
            {
                return this->mStatus;
            }
        };
    }
}
#endif //LANDESSDEVCORE_STARTONCREATION_HPP
