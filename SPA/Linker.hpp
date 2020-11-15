//
// Created by phoenixflower on 10/26/20.
//

#ifndef LANDESSDEVCORE_LINKER_HPP
#define LANDESSDEVCORE_LINKER_HPP
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Detection.hpp"
#include "Backend.hpp"
#include "Compositor.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename BackendGround, class = void>
        class Linker;


        template<typename ... Backends>
        class Linker<LD::CT::TypeList<Backends...>,LD::Requires<LD::SPW::IsBackend<Backends>...>>
        {
        private:
            constexpr static LD::StringView DOCTYPE = "<!DOCTYPE html>\n";
        public:
            Linker(LD::SPA::Compositor<LD::CT::TypeList<Backends...>> & compositor) noexcept
            {

            }


        };
    }
}
#endif //LANDESSDEVCORE_LINKER_HPP
