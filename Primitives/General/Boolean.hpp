//
// Created by phoenixflower on 11/2/20.
//

#ifndef LANDESSDEVCORE_BOOLEAN_HPP
#define LANDESSDEVCORE_BOOLEAN_HPP
#include "Immutable.hpp"
#include "ctre.hpp"
namespace LD
{
    enum YesNo{No = 0,Yes =1};

    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,LD::YesNo>::value>,LD::ImmutableString<3>> ToImmutableString(const T & booleanValue) noexcept
    {
        LD::StaticArray<LD::ImmutableString<3>,2> results;

        LD::ImmutableString<3> trueResult;
        trueResult[0] = 'y';
        trueResult[1] = 'e';
        trueResult[2] = 's';

        LD::ImmutableString<3> falseResult;
        falseResult[0] = 'n';
        falseResult[1] = 'o';


        results[1] = trueResult;
        results[0] = falseResult;
        return results[booleanValue];
    }
}





#endif //LANDESSDEVCORE_BOOLEAN_HPP
