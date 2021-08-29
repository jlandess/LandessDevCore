//
// Created by phoenixflower on 8/25/21.
//
#include <variant>
#include <iostream>
#include "Primitives/General/mapboxvariantvisitor.h"
namespace LD
{
    namespace Example
    {
        class LightItem {};
        class HeavyItem {};
        class Fluid{};
        class FragileItem{};
        extern void Visitation()
        {
            std::variant<LightItem,HeavyItem> basicPackA;
            std::variant<LightItem,HeavyItem> basicPackB;
            std::visit(LD::Overload{
                    [](LightItem&, LightItem& ) { std::cout << "2 light items\n"; },
                    [](LightItem&, HeavyItem& ) { std::cout << "light & heavy items\n"; },
                    [](HeavyItem&, LightItem& ) { std::cout << "heavy & light items\n"; },
                    [](HeavyItem&, HeavyItem& ) { std::cout << "2 heavy items\n"; },
            }, basicPackA, basicPackB);
            std::variant<Fluid, LightItem, HeavyItem, FragileItem> package;
        }
    }
}
