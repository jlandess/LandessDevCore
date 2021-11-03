//
// Created by phoenixflower on 10/22/21.
//

#ifndef LANDESSDEVCORE_SERVICELOCATOR_HPP
#define LANDESSDEVCORE_SERVICELOCATOR_HPP
#include "Algorithms/Forward.hpp"
#include "Primitives/General/Tuple.hpp"
namespace LD
{
    template<typename ... A>
    class BasicServiceRepository
    {
    private:
        LD::Tuple<A...> mServices;
    public:


        BasicServiceRepository() noexcept
        {

        }

        BasicServiceRepository(A && ... args) noexcept:mServices{LD::Forward<A>(args)...}
        {

        }

        template<typename T>
        auto & GetService() noexcept
        {
            return LD::Get(LD::Get<T>(this->mServices));
        }

        LD::Tuple<A...> & Services() noexcept
        {
            return this->mServices;
        }
    };
}
#endif //LANDESSDEVCORE_SERVICELOCATOR_HPP
