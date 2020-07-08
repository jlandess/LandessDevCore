//
// Created by phoenixflower on 6/8/20.
//

#ifndef LANDESSDEVCORE_TYPE_H
#define LANDESSDEVCORE_TYPE_H
namespace LD
{
    template<typename T>
    struct Type
    {
        using type = T;
    };

    template<typename T>
    using GetType = typename T::type ;


}

namespace LD
{
    template<LD::UInteger Index, typename ... A>
    constexpr LD::Enable_If_T<LD::Require<(LD::CT::TypeList<A...>::size() > 0)>,LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>> Get(const LD::CT::TypeList<A...> & tl) noexcept //-> LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>
    {
        //LD::Enable_If_T<LD::Require<(LD::CT::TypeList<A...>::size() > 0)>,LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>>;
        return {};
    }

    template<LD::UInteger Index>
    constexpr void Get(const LD::CT::TypeList<> & tl) noexcept
    {

    }

    namespace CT
    {
        template<LD::UInteger Index, typename ... A>
        constexpr LD::Enable_If_T<LD::Require<(LD::CT::TypeList<A...>::size() > 0)>,LD::Type<LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>>> GetType( LD::CT::TypeList<A...>  tl) noexcept //-> LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>
        {
            //LD::Enable_If_T<LD::Require<(LD::CT::TypeList<A...>::size() > 0)>,LD::CT::TypeAtIndex<Index,LD::CT::TypeList<A...>>>;
            return {};
        }

        template<LD::UInteger Index>
        constexpr LD::Type<void> GetType( LD::CT::TypeList<>  tl) noexcept
        {
            return {};
        }

    }


}
#endif //LANDESSDEVCORE_TYPE_H
