#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Chrono/Timer.h"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"


int main()
{

    //auto bar = LD::ImmutableString<9>{'_'};
    //auto edge = LD::ToImmutableString("abc");
    //std::cout << (bar+edge).Data() << std::endl;
    //std::cout << edge.Data() << std::endl;
    //std::cout << bar.GetSize() << std::endl;
    //std::cout << edge.GetSize() << std::endl;
    LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();

    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();

    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}