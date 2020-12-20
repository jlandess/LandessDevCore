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
            Function(tExpressions && ... expression) noexcept:mExpressions{LD::Forward<tExpressions>(expression)...}
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

        template <typename ... Components> Function(Components && ... components) -> Function<LD::CT::TypeList<Components...>>;
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
        constexpr LD::UInteger NumberOfTabs = sizeof...(tExpressions);
        constexpr LD::UInteger NumberOfNewlines = sizeof...(tExpressions)+3;
        constexpr LD::UInteger maxAllocation = LD::GetRequiredSizeForExpressions(LD::CT::TypeList<tExpressions...>{})+sizeof("function(){}")+23+NumberOfNewlines+NumberOfTabs+1;
        LD::StaticArray<char,maxAllocation> output;
        LD::BackInserter outputInserter{output};
        using Output = decltype(outputInserter);
        LD::StaticArray<LD::QueryResult<LD::Variant<LD::Detail::Decay_T<tExpressions>...>(Output&)>,sizeof...(tExpressions)> expressions;
        const auto & functionExpressions = function.Expressions();
        LD::For<sizeof...(tExpressions)>([](
                auto I,
                const auto & functionExpressions,
                LD::StaticArray<LD::QueryResult<LD::Variant<LD::Detail::Decay_T<tExpressions>...>(Output&)>,sizeof...(tExpressions)> & expressions,
                Output & output)
        {
            auto abc = LD::Get(LD::Get<I>(functionExpressions));
            LD::QueryResult<LD::Variant<LD::Detail::Decay_T<tExpressions>...>(Output&)> current= LD::MakeContext(LD::TransactionResult{},decltype(abc){abc},output);
            expressions.PushBack(current);
            return true;
        },functionExpressions,expressions,outputInserter);


        LD::QuickSort(expressions,[](const LD::QueryResult<LD::Variant<LD::Detail::Decay_T<tExpressions>...>(Output&)> & a, const LD::QueryResult<LD::Variant<LD::Detail::Decay_T<tExpressions>...>(Output&)> & b) -> bool
        {

            auto onError = [](const LD::Context<LD::TransactionError> & a,const LD::Context<LD::TransactionError> & b) noexcept
            {
                return false;
            };

            auto onComparison = [](auto && a, auto &&b) noexcept
            {
                if constexpr (LD::TupleSize<LD::Detail::Decay_T<decltype(a)>>::value > 2 && LD::TupleSize<LD::Detail::Decay_T<decltype(b)>>::value > 2)
                {
                    return LD::Get(LD::Get<1>(a)).Order() < LD::Get(LD::Get<1>(b)).Order();
                }
                return false;
            };

            return LD::MultiMatch(LD::Overload{onComparison,onError},a,b);
        });
        const char * initialName = "function";
        for(LD::UInteger n = 0;n<sizeof(initialName);++n)
        {
            outputInserter = initialName[n];
        }
        outputInserter = '(';
        outputInserter = ')';
        outputInserter = '\n';
        outputInserter = '{';
        outputInserter = '\n';
        for(LD::UInteger n = 0;n< sizeof...(tExpressions);++n)
        {
            //auto immutableString = LD::ToImmutableString(expressions[n]);
            const auto & expression = expressions[n];
            auto writeToBuffer = [](auto && query) noexcept
            {
                if constexpr (LD::TupleSize<LD::Detail::Decay_T<decltype(query)>>::value > 2)
                {
                    auto expression = LD::Get(LD::Get<1>(query));
                    auto & output = LD::Get(LD::Get<2>(query));
                    auto expressionAsImmutableString = LD::ToImmutableString(expression);
                    const char * data = expressionAsImmutableString.Data();
                    output = '\t';
                    for(LD::UInteger n = 0;n<expressionAsImmutableString.GetSize();++n)
                    {
                        output = expressionAsImmutableString[n];
                    }
                    output= '\n';
                }

            };

            LD::MultiMatch(LD::Overload{writeToBuffer},expression);

        }

        outputInserter = '}';
        outputInserter = '\0';
        std::cout << output.GetData() << std::endl;

    }
}
#endif //LANDESSDEVCORE_FUNCTION_HPP
