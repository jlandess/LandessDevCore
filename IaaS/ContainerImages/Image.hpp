//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_OCI_IMAGE_HPP
#define LANDESSDEVCORE_OCI_IMAGE_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace OCI
    {
        template<typename T>
        class Image;

        template<LD::UInteger Size>
        class Image<LD::ImmutableString<Size>>
        {
        private:
            LD::ImmutableString<Size> mName;
        };

    }
}
#endif //LANDESSDEVCORE_IMAGE_HPP
