//
// Created by phoenixflower on 10/26/20.
//

#ifndef LANDESSDEVCORE_COMPOSITOR_HPP
#define LANDESSDEVCORE_COMPOSITOR_HPP
#include "Backend.hpp"
#include "Random/MarsagliaRandomNumberGenerator.hpp"
#include <map>
namespace LD
{
    namespace SPA
    {
        template<typename BackendGround, class = void>
        class Compositor;

        template<typename ... Backends>
        class Compositor<LD::CT::TypeList<Backends...>,LD::Requires<LD::SPW::IsBackend<Backends>...>>
        {
        private:
            LD::MarsagliaMultiplyWithCarryGenerator mGenerator;
        public:
            Compositor() noexcept
            {
                this->mGenerator.ReSseed();
            }
            auto GetNextValidComponentID() noexcept
            {
                return this->mGenerator.NextInteger();
            }
        };
    }
}
#endif //LANDESSDEVCORE_COMPOSITOR_HPP
