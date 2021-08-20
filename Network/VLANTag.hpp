//
// Created by phoenixflower on 1/26/21.
//

#ifndef LANDESSDEVCORE_VLANTAG_HPP
#define LANDESSDEVCORE_VLANTAG_HPP
#include "Definitions/Integer.hpp"

namespace LD
{
    class VLANTag
    {
    private:
        LD::UInteger mTag;
    public:
        VLANTag() noexcept:mTag{0}
        {

        }
        VLANTag(LD::UInteger tag) noexcept:mTag{tag}
        {

        }
    };
}
#endif //LANDESSDEVCORE_VLANTAG_HPP
