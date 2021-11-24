//
// Created by phoenixflower on 10/22/21.
//

#ifndef LANDESSDEVCORE_FACTORY_HPP
#define LANDESSDEVCORE_FACTORY_HPP
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Detection.hpp"
#include "Core/Tuple.hpp"
namespace LD
{
    template<typename Object, typename CreationMethod, class = void>
    class Factory;


    template<typename ... Objects, typename ... CreationMethods>
    class Factory<LD::CT::TypeList<Objects...>,LD::CT::TypeList<CreationMethods...>,
            LD::Enable_If_T<LD::Require<
            true
            >>>
    {
    private:

    };
}
#endif //LANDESSDEVCORE_FACTORY_HPP
