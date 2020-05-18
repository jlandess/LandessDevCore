#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"



#include <iostream>



int main()
{

    /*
    static constexpr auto Pattern = ctll::fixed_string{ "[^\\.]+" };
    auto result = LD::StringAsNumber<LD::UInteger >("1234");

    LD::Match(result,[](const LD::UInteger & number){std::cout << number << std::endl;},[](auto &&){});

    auto signedResult = LD::StringAsNumber<LD::Integer >("-1234");

    LD::Match(signedResult,[](const LD::Integer & number){std::cout << number << std::endl;},[](auto &&){});

    auto floatingPointresult = LD::StringAsNumber<LD::Float >("-921.73");

    LD::Match(floatingPointresult,[](const LD::Float & number){std::cout << number << std::endl;},[](auto &&){});
     */
    //LD::Example::TermBoxMenuExample();
    LD::Example::ReflectionExample();
    return 0;
}