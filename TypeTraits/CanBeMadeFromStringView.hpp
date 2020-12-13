//
// Created by phoenixflower on 11/29/20.
//

#ifndef LANDESSDEVCORE_CANBEMADEFROMSTRINGVIEW_HPP
#define LANDESSDEVCORE_CANBEMADEFROMSTRINGVIEW_HPP
#include "Declval.hpp"
#include "Primitives/General/StringView.hpp"
#include "TypeList.hpp"
#include "Type.h"
#include "Algorithms/FromString.hpp"
namespace LD
{
    namespace CT
    {
        namespace Detail
        {
            template<typename T,typename ... A>
            using FromStringViewTest = decltype(LD::FromStringView(LD::Declval<LD::Type<T>>(),LD::Declval<LD::StringView>(),LD::Declval<A>()...));
        }
        template<typename T>
        constexpr bool CanBeMadeFromStringView(LD::Type<T>) noexcept
        {
            //LD::ConvertiblyCallable<T,LD::QueryResult<T()>()>;
            return LD::Exists<LD::CT::Detail::FromStringViewTest,T> || LD::ConvertiblyCallable<T,LD::QueryResult<T()>()>::Value();
        }
        template<typename T, typename ... A>
        constexpr bool CanBeMadeFromStringView(LD::Type<T>, LD::CT::TypeList<A...>) noexcept
        {
            return LD::Exists<LD::CT::Detail::FromStringViewTest,T,A...>;
        }
    }
}
#endif //LANDESSDEVCORE_CANBEMADEFROMSTRINGVIEW_HPP
