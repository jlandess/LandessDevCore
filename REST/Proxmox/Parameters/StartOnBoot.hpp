//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_STARTONBOOT_HPP
#define LANDESSDEVCORE_STARTONBOOT_HPP
namespace LD
{
    namespace PVE
    {
        class StartOnBoot
        {
        private:
            bool mStartStatus;
        public:
            StartOnBoot() noexcept:mStartStatus{false}
            {

            }
            StartOnBoot(bool status) noexcept:mStartStatus{status}
            {

            }

            auto Key() const noexcept
            {
                return LD::ImmutableString{"onboot"};
            }
            bool Status() const noexcept
            {
                return this->mStartStatus;
            }
            auto Value() const noexcept
            {
                return LD::ToImmutableString(LD::UInteger(this->mStartStatus));
            }
        };
    }
}
#endif //LANDESSDEVCORE_STARTONBOOT_HPP
