//
// Created by phoenixflower on 11/5/21.
//
#include <limits>
#include "Core/CompileTimeMap.hpp"
namespace LD
{
    namespace Example
    {
        struct Color {
            constexpr inline Color() {
            }
            constexpr inline Color(unsigned char r_, unsigned char g_, unsigned char b_, float a_)
                    : r(r_), g(g_), b(b_), a(a_ > 1 ? 1 : a_ < 0 ? 0 : a_) {
            }
            unsigned char r = 0, g = 0, b = 0;
            float a = 1.0f;

            constexpr bool operator==(const Color& rhs) const {
                return r == rhs.r && g == rhs.g && b == rhs.b &&
                       (a >= rhs.a ? a - rhs.a : rhs.a - a) < std::numeric_limits<float>::epsilon();
            }
        };
        constexpr auto const multi_colors = mapbox::eternal::map<mapbox::eternal::string, Color>({
                                                                                                         { "red", { 255, 0, 0, 1 } },
                                                                                                         { "yellow", { 255, 255, 0, 1 } },
                                                                                                         { "white", { 255, 255, 255, 1 } }, // comes before yellow!
                                                                                                         { "yellow", { 255, 220, 0, 1 } },  // a darker yellow
                                                                                                 });



        extern void ConstexprMapExample()
        {
            multi_colors.find("red");
        }
    }
}