//
// Created by phoenixflower on 12/4/20.
//

#ifndef LANDESSDEVCORE___OR_HPP
#define LANDESSDEVCORE___OR_HPP
#include "Conditional.hpp"
namespace LD
{
    namespace CT
    {
        template<typename...>
        struct Or;
        template<typename _B1, typename _B2>
        struct Or<_B1, _B2> : public LD::Conditonal<_B1::value, _B1, _B2>::type
        { };
    }

}
#endif //LANDESSDEVCORE___OR_HPP
