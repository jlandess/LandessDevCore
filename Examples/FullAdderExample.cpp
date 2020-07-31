//
// Created by phoenixflower on 7/15/20.
//
#include <iostream>
#include "Primitives/General/BitSet.hpp"
#include "Primitives/General/Hexademical.hpp"
#include "Algorithms/FullAdder.hpp"
namespace LD
{
    namespace Example
    {
        void FullAdderExample()
        {
            LD::BitSet<8> five1{0,0,0,0,0,1,0,1};
            LD::BitSet<8> three1{0,0,0,0,0,0,1,1};

            LD::FullAdder<8> adder;

            auto setResult = adder(five1,three1);
            std::cout << "The Addition of : ";
            for(LD::UInteger n = 0;n<8;++n)
            {
                std::cout << five1[n];
            }
            std::cout << "\n";
            std::cout << "and  ";
            for(LD::UInteger n = 0;n<8;++n)
            {
                std::cout << three1[n];
            }
            std::cout << "\n";
            std::cout << "is ";
            for(LD::UInteger n = 0;n<8;++n)
            {
                std::cout << setResult[n];
            }
            std::cout << std::endl;

            LD::Hexademical<2> address{LD::BitSet<8>{setResult}};
        }
    }
}