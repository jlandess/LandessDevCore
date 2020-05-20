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
    class ContextualVariant<LD::TypeList<Variance...>(Cntx...)>: public LD::Variant<LD::Context<Variance,Cntx...>...>
    {
        //LD::Variant<LD::Context<Variance,Cntx...>...> mVariant;
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

    template<typename ... Variance, typename ... Cntx>
    class ContextualVariant<LD::Variant<Variance...>(Cntx...)>
    {
    private:
        LD::Variant<LD::Context<Variance,Cntx...>...> mVariant;
    public:
        ContextualVariant() = default;
        ContextualVariant(const LD::Variant<LD::Context<Variance,Cntx...>...> & var) noexcept
        {
            (*this) = var;
        }

        ContextualVariant & operator = (const LD::Variant<LD::Context<Variance,Cntx...>...> & var) noexcept
        {
            this->mVariant = var;
            return (*this);
        }
        LD::Variant<LD::Context<Variance,Cntx...>...> & Variant() noexcept {return this->mVariant;}
    };
}
#endif //LANDESSDEVCORE_CONTEXTUALVARIANT_H