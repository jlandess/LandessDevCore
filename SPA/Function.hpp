//
// Created by phoenixflower on 11/12/20.
//

#ifndef LANDESSDEVCORE_FUNCTION_HPP
#define LANDESSDEVCORE_FUNCTION_HPP
#include "TypeTraits/Detection.hpp"
#include "Reflection/Reflection.hpp"
#include "Variable.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/StaticArray.hpp"
#include "Algorithms/Sorting.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename Expressions, class = void>
        class Function;



        template<typename ... tExpressions>
        class Function<LD::CT::TypeList<tExpressions...>,LD::Enable_If_T<
                LD::Require<
                LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<tExpressions>{})...)
                >>>
        {
        private:
            LD::Tuple<tExpressions...> mExpressions;
        public:
            Function(tExpressions && ... expression) noexcept:mExpressions{LD::Forward<Expressions>(expression)...}
            {

            }
            const LD::Tuple<tExpressions...> & Expressions() const noexcept
            {
                return this->mExpressions;
            }

            LD::Tuple<tExpressions...> & Expressions() noexcept
            {
                return this->mExpressions;
            }


        };
    }
}
template<typename ... tExpressions>
struct LD::CT::TypeDescriptor<LD::SPA::Function<LD::CT::TypeList<tExpressions...>>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("expressions");
public:
    static constexpr auto ClassName = ctll::fixed_string{"function"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::Tuple<tExpressions...> & (LD::SPA::Function<LD::CT::TypeList<tExpressions...>>::*)(),&LD::SPA::Function<LD::CT::TypeList<tExpressions...>>::Expressions>(),LD::CT::SelectOverload<const LD::Tuple<tExpressions...> & (LD::SPA::Function<LD::CT::TypeList<tExpressions...>>::*)() const,&LD::SPA::Function<LD::CT::TypeList<tExpressions...>>::Expressions>()>
    >;


    static constexpr MemberList Members{  };

};

namespace LD
{
    template<typename ... tExpressions>
    auto ToImmutableString(const LD::SPA::Function<LD::CT::TypeList<tExpressions...>> & function) noexcept
    {
        //todo sort the expressions based on order
        LD::StaticArray<LD::Variant<tExpressions...>,sizeof...(tExpressions)> expressions;
        const auto & functionExpressions = function.Expressions();
        LD::For<sizeof...(tExpressions)>([](
                auto I,
                const auto & functionExpressions,
                LD::StaticArray<LD::Variant<tExpressions...>,sizeof...(tExpressions)> & expressions)
        {
            expressions.PushBack(LD::Get(LD::Get<I>(functionExpressions)));
            return true;
        },functionExpressions);


    }
}
#endif //LANDESSDEVCORE_FUNCTION_HPP
