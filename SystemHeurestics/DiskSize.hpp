//
// Created by phoenixflower on 1/23/21.
//

#ifndef LANDESSDEVCORE_DISKSIZE_HPP
#define LANDESSDEVCORE_DISKSIZE_HPP
#include "Primitives/General/Unit.hpp"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    template<typename T,class = void>
    class DiskSize;
    template<typename T>
    class DiskSize<T,LD::Enable_If_T<
            LD::Require<
                    true
                    >>>
    {
    private:
        T mSize;
    public:
        DiskSize() noexcept:mSize(0ul)
        {

        }
        DiskSize(T size) noexcept:mSize{size}
        {

        }

        T Size() const noexcept
        {
            return this->mSize;
        }
    };


    template<typename T> DiskSize(T && object) -> DiskSize<T>;
}
#endif //LANDESSDEVCORE_DISKSIZE_HPP
