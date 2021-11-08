//
// Created by phoenixflower on 2/1/21.
//

#ifndef LANDESSDEVCORE_STARTUP_HPP
#define LANDESSDEVCORE_STARTUP_HPP
#include "Definitions/Integer.hpp"
#include "Core/Unit.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    namespace PVE
    {
        class StartupDelay
        {
        private:
            LD::Second<LD::UInteger> mDelay;
        public:

            LD::Second<LD::UInteger> Delay() const noexcept
            {
                return this->mDelay;
            }
        };

        class ShutdownDelay
        {
        private:
            LD::Second<LD::UInteger> mDelay;
        public:

            LD::Second<LD::UInteger> Delay() const noexcept
            {
                return this->mDelay;
            }
        };
        class Startup
        {
        private:
            LD::UInteger mOrder;
            LD::PVE::StartupDelay mStartUpDelay;
            LD::PVE::ShutdownDelay mShutDownDelay;
        public:

            Startup() noexcept:mOrder{0}
            {

            }

            template<typename ...A, typename = typename = LD::Enable_If_T<LD::Require<true>>>
            Startup(A && ... args) noexcept
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"startup"};
            }
            auto Value() const noexcept
            {
                auto startupDelay = this->mStartUpDelay.Delay();
                auto shutdownDelay = this->mShutDownDelay.Delay();
                return LD::ImmutableString{"order="}+LD::ToImmutableString(this->mOrder)+LD::ImmutableString{","}+
                LD::ImmutableString{"up="}+LD::ToImmutableString(startupDelay.NativeRepresentation().Value()) + LD::ImmutableString{","} +
                LD::ImmutableString{"down="}+LD::ToImmutableString(shutdownDelay.NativeRepresentation().Value());
            }
        };
    }
}
#endif //LANDESSDEVCORE_STARTUP_HPP
