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
#include "TypeTraits/IsString.hpp"
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
            LD::UInteger mID;
        public:
            template<typename IntegerGenerator, typename = LD::Enable_If_T<LD::Require<LD::ConvertiblyCallable<IntegerGenerator,LD::UInteger()>::Value()>>>
            Function(IntegerGenerator && generator,tExpressions && ... expression) noexcept:mExpressions{LD::Forward<tExpressions>(expression)...}
            {
                this->mID = generator();
            }
            const LD::Tuple<tExpressions...> & Expressions() const noexcept
            {
                return this->mExpressions;
            }
            LD::UInteger ID() const noexcept
            {
                return this->mID;
            }

            LD::Tuple<tExpressions...> & Expressions() noexcept
            {
                return this->mExpressions;
            }


        };

        template <typename IntegerGenerator,typename ... Components> Function(IntegerGenerator &&,Components && ... components) -> Function<LD::CT::TypeList<Components...>>;
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
    namespace SPA
    {
        namespace Detail
        {
            template<typename T,typename Op,typename OpType,typename ... A, typename ElementType,typename ArrayType, typename ... B>
            void Inject(LD::SPA::VarExpression<T,Op,OpType,A...> expression, ArrayType & array, LD::Type<ElementType>,B && ... arguments) noexcept
            {
                auto composition = expression.Composition();
                using CompositionType = decltype(composition);
                constexpr LD::UInteger SizeOfComposition = LD::TupleSize<CompositionType>{};
                LD::For<SizeOfComposition>([](auto I, auto & composition, ArrayType & array,LD::SPA::VarExpression<T,Op,OpType,A...> expression ,B && ... args) noexcept
                {
                    auto currentVariable = LD::Get(LD::Get<I>(composition));
                    constexpr auto currentVariableType = LD::CT::RemoveQualifiers(LD::Type<decltype(currentVariable)>{});
                    if constexpr (LD::SPA::IsVarExpression(currentVariableType))
                    {
                        Inject(currentVariable,array,LD::Type<ElementType>{},LD::Forward<B>(args)...);

                    }else if constexpr (LD::SPA::IsVar(currentVariableType))
                    {
                        ElementType current= LD::MakeContext(LD::TransactionResult{},decltype(currentVariable){currentVariable},LD::Forward<B>(args)...);
                        array.PushBack(current);
                    }
                    return true;
                },composition,array,expression,LD::Forward<B>(arguments)...);
                //LD::CT::DebugTemplate<CompositionType>{};
            }
        }
    }
}
namespace LD
{
    template<typename ... tExpressions>
    auto ToImmutableString(const LD::SPA::Function<LD::CT::TypeList<tExpressions...>> & function) noexcept
    {
        using ExpressionCompositions = LD::CT::Tranform<typename LD::SPA::CompositionsOfExpressions<LD::Detail::Decay_T<tExpressions>...>::type,LD::Detail::Decay_T>;
        using NumberOfVariablesInExpressions = LD::CT::Tranform<typename LD::SPA::CompositionsOfExpressions<LD::Detail::Decay_T<tExpressions>...>::alltypes ,LD::Detail::Decay_T>;
        //LD::CT::DebugTemplate<NumberOfVariablesInExpressions>{};
        using ExpressionDefinitions = LD::CT::TypeList<LD::Detail::Decay_T<tExpressions>...>;
        using TotalDefinition = LD::CT::Concatenate<ExpressionCompositions,ExpressionDefinitions>;
        using Rebound = LD::CT::RebindList<TotalDefinition,LD::Variant>;
        constexpr LD::UInteger NumberOfTabs = sizeof...(tExpressions);
        constexpr LD::UInteger NumberOfNewlines = sizeof...(tExpressions)+3;
        constexpr LD::UInteger AmountRequiredForVariables = (... + GetRequiredSizeForVariablesInsideExpression(LD::CT::RemoveQualifiers(LD::Type<tExpressions>{})));
        constexpr LD::UInteger maxAllocation = LD::GetRequiredSizeForExpressions(LD::CT::TypeList<tExpressions...>{})+sizeof("function(){}")+sizeof("function ")+AmountRequiredForVariables+23+NumberOfNewlines+NumberOfTabs+1;
        LD::StaticArray<char,maxAllocation> output;
        LD::BackInserter outputInserter{output};
        using Output = decltype(outputInserter);
        using ElementType = LD::QueryResult<Rebound(Output&)>;
        constexpr LD::UInteger NumberOfVariables = sizeof...(tExpressions)+NumberOfVariablesInExpressions::size();
        LD::StaticArray<ElementType ,NumberOfVariables> expressions;
        const auto & functionExpressions = function.Expressions();
        LD::For<sizeof...(tExpressions)>([](
                auto I,
                const auto & functionExpressions,
                LD::StaticArray<ElementType ,NumberOfVariables> & expressions,
                Output & output)
        {
            auto abc = LD::Get(LD::Get<I>(functionExpressions));
            ElementType current= LD::MakeContext(LD::TransactionResult{},decltype(abc){abc},output);
            LD::SPA::Detail::Inject(abc,expressions,LD::Type<ElementType>{},output);
            expressions.PushBack(current);
            return true;
        },functionExpressions,expressions,outputInserter);


        LD::QuickSort(expressions,[](const ElementType & a, const ElementType & b) -> bool
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
        outputInserter = ' ';
        for(LD::UInteger n = 0;n<sizeof(initialName);++n)
        {
            outputInserter = initialName[n];
        }

        auto functionIDAsImmutableString = LD::ToImmutableString(function.ID());
        for(LD::UInteger n = 0;n<functionIDAsImmutableString.GetSize();++n)
        {
            outputInserter = functionIDAsImmutableString[n];
        }
        outputInserter = '(';
        outputInserter = ')';
        outputInserter = '\n';
        outputInserter = '{';
        outputInserter = '\n';
        for(LD::UInteger n = 0;n< NumberOfVariables;++n)
        {
            const auto & expression = expressions[n];
            auto writeToBuffer = [](auto && query) noexcept
            {
                if constexpr (LD::TupleSize<LD::Detail::Decay_T<decltype(query)>>::value > 2)
                {
                    auto expression = LD::Get(LD::Get<1>(query));
                    constexpr auto expressionType = LD::Type<decltype(expression)>{};

                    auto & output = LD::Get(LD::Get<2>(query));
                    auto expressionAsImmutableString = LD::ToImmutableString(expression);
                    output = '\t';
                    if constexpr (LD::SPA::IsVar(expressionType))
                    {
                        output = 'l';
                        output = 'e';
                        output = 't';
                        output = ' ';
                    }
                    for(LD::UInteger n = 0;n<expressionAsImmutableString.GetSize();++n)
                    {
                        output = expressionAsImmutableString[n];
                    }
                    if constexpr (LD::SPA::IsVar(expressionType))
                    {
                        output='=';
                        const auto & variable = expression.DefaultValue();
                        auto variableValue = LD::ToImmutableString(variable);

                        if constexpr (LD::CT::IsString(LD::CT::RemoveQualifiers(LD::Type<decltype(variable)>{})))
                        {
                            output = '\"';
                        }
                        //std::cout << "Express variable :" << variableValue.Data() << std::endl;

                        for(LD::UInteger n = 0;n<variableValue.GetSize();++n)
                        {
                            output = variableValue[n];
                        }
                        if constexpr (LD::CT::IsString(LD::CT::RemoveQualifiers(LD::Type<decltype(variable)>{})))
                        {
                            output = '\"';
                        }
                    }
                    output = ';';
                    output= '\n';
                }
            };
            LD::MultiMatch(LD::Overload{writeToBuffer},expression);
        }
        outputInserter = '}';
        outputInserter = '\0';
        return LD::ImmutableString{output};
    }
}
#endif //LANDESSDEVCORE_FUNCTION_HPP
