//
// Created by phoenixflower on 5/19/20.
//
#ifndef LANDESSDEVCORE_CONTEXTUALVARIANT_H
#define LANDESSDEVCORE_CONTEXTUALVARIANT_H
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/Context.h"
namespace LD
{
    template<typename Variance>
    class ContextualVariant;

    template<typename ... Variance, typename ... Cntx>
    class ContextualVariant<LD::Variant<Variance...>(Cntx...)>: public LD::Variant<LD::Context<Variance,Cntx...>...>
    {
    public:
        ContextualVariant() = default;

        template<typename ... Args>
        ContextualVariant( const LD::Variant<LD::Context<Variance,Cntx...>...> & var) noexcept
        {
            (*this) = var;
        }


        ContextualVariant & operator = (const LD::Variant<LD::Context<Variance,Cntx...>...> & var) noexcept
        {
            LD::Variant<LD::Context<Variance,Cntx...>...>::operator=(var);
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_CONTEXTUALVARIANT_H