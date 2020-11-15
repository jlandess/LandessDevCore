//
// Created by phoenixflower on 11/12/20.
//

#ifndef LANDESSDEVCORE_SORTING_HPP
#define LANDESSDEVCORE_SORTING_HPP
#include "Definitions/Integer.hpp"
#include "Swap.hpp"
#include "Definitions/Common.hpp"
#include "TypeTraits/Iterable.h"
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Span.hpp"
namespace LD
{
    template<typename T>
    class Less
    {
    public:
        bool operator()(const T & a, const T & b) noexcept
        {
            return a < b;
        }
    };

    template<typename T>
    class Ascending
    {
    public:
        bool operator()(const T & a, const T & b) noexcept
        {
            return a > b;
        }
    };

    template<typename T>
    class Descending
    {
    public:
        bool operator()(const T & a, const T & b) noexcept
        {
            return a < b;
        }
    };

    template<typename T>
    class Greater
    {
    public:
        bool operator()(const T & a, const T & b) noexcept
        {
            return a > b;
        }
    };
    template<typename T, typename Comparator>
    LD::UInteger Pivot(T * v,  LD::UInteger  start, LD::UInteger  stop,  LD::UInteger  position, Comparator && comparator) noexcept
    {
        LD::Swap(*(v+start), *(v+position));

        LD::UInteger low = start+1;
        LD::UInteger high = stop;
        while(low < high)
        {
            if (comparator(v[low], v[start]))
            {
                low++;
            }else if(comparator(v[--high], v[start]))
            {
                LD::Swap(*(v+low), *(v+high));
            }
        }
        LD::Swap(*(v+start), *(v+(--low)));
        return low;
    }


    template<typename T, typename Comparator>
    void QuickSort(T * v,  LD::UInteger  low,  LD::UInteger  high,Comparator && comparator) noexcept
    {
        if (low >= high)
        {
            return;
        }

        LD::UInteger pivotIndex = (low+high)/2;
        pivotIndex = Pivot(v, low, high, pivotIndex,LD::Forward<Comparator>(comparator));

        if (low < pivotIndex)
        {
            QuickSort(v, low, pivotIndex,LD::Forward<Comparator>(comparator));
        }
        if (pivotIndex < high)
        {
            QuickSort(v, pivotIndex+1, high,LD::Forward<Comparator>(comparator));
        }
    }

    template<typename T, typename Comparator>
    LD::Enable_If_T<LD::Require<LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<T>{}))>,T&> QuickSort(T & vector, Comparator && comparator) noexcept
    {
        LD::Span span{vector};
        LD::QuickSort(span.Data(),0,span.Size(),LD::Forward<Comparator>(comparator));
        return vector;
    }


}
#endif //LANDESSDEVCORE_SORTING_HPP
