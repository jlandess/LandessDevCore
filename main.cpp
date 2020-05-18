#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Chrono/Timer.h"


#include <iostream>



int main()
{

    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();
    LD::Example::ReflectionExample();
    currentTimer.Stop();

    std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}