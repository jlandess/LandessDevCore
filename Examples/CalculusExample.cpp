//
// Created by phoenixflower on 6/9/20.
//
#include <iostream>
#include "Algorithms/Calculus.h"
//#include "Random/GS_BasicRNG.hpp"
namespace LD
{

    namespace Example
    {
        void CalculusExample()
        {
            std::cout << LD::RectangularIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,LD::MidPointRule{},0.001) << std::endl;

            std::cout << LD::TrapezoidIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::SimpsonIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::QuadratureIntegrate([](const LD::Float & x){return x*x;},1.0,5.0,0.0001) << std::endl;

            std::cout << LD::RungaKutta4Integrate([](const LD::Float & x, const LD::Float & y){return x*x;},1.0,5.0,0.0001) << std::endl;
        }
    }
}