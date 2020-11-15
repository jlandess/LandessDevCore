//
// Created by phoenixflower on 11/14/20.
//

#ifndef LANDESSDEVCORE_HTMLEMITTER_HPP
#define LANDESSDEVCORE_HTMLEMITTER_HPP
#include "TypeTraits/Detection.hpp"
#include "Reflection/Reflection.hpp"
#include "IO/XML/tinyxml2.h"
#include "Primitives/General/StaticArray.hpp"
#include "Unicode/UTF8.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "Primitives/General/Span.hpp"
#include "SPA/Text.hpp"
#include "SPA/IsAttribute.hpp"
namespace LD
{
    class HTMLEmitter
    {
    private:

    public:
        tinyxml2::XMLDocument xmlDoc;
        template<typename T>
        LD::Enable_If_T<LD::Require<LD::CT::IsReflectable<T>()>,void> Print(T && object) noexcept
        {
            return this->__Print(xmlDoc.GetDocument(),LD::Forward<T>(object));
        }
        template<typename T>
        LD::Enable_If_T<LD::Require<LD::CT::IsReflectable<T>()>,void> __Print(tinyxml2::XMLNode * rootNode,T && object, const bool shouldGenerateNewNode = true) noexcept
        {

            constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<T>>();
            constexpr auto className = LD::CT::GetClassName(traits);
            constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
            constexpr auto members = traits.Members;

            LD::StaticArray<char,1024> classNameBuffer;
            bool classNameIsValid = true;

            tinyxml2::XMLElement * node = (tinyxml2::XMLElement *)rootNode;
            if (shouldGenerateNewNode)
            {
                LD::BackInserter<LD::StaticArray<char,1024>> classNameBackInserter{classNameBuffer};
                auto classNameLength = LD::UTF8::Distance(
                        LD::UTF8::Begin(className),
                        LD::UTF8::End(className),classNameBackInserter,className);
                using Ctxt = LD::Context<LD::TransactionResult,LD::UInteger,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(className)&>;
                //make sure we have a UTF-8 validated string
                classNameIsValid = LD::MultiMatch(LD::Overload{
                        [](auto)
                        {
                            return false;
                        },
                        [](const Ctxt & context) noexcept
                        {
                            const auto & length = LD::Get(LD::Get<1>(context));
                            const auto & className = LD::Get(LD::Get<3>(context));
                            using IT = LD::Detail::Decay_T<decltype(classNameBackInserter)>;
                            auto & inserter = LD::Get(LD::Get<2>(context));
                            LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});

                            for(LD::UInteger n = 0;n<length && LD::IsTransactionalQuery(result);++n)
                            {
                                result =  LD::UTF8::Append(className[n],inserter);
                            }
                            result =  LD::UTF8::Append('\0',inserter);

                            return LD::IsTransactionalQuery(result);
                        }
                },classNameLength);
                node = xmlDoc.NewElement(classNameBuffer.GetData());
                rootNode->InsertFirstChild(node);
            }
            if (classNameIsValid)
            {
                LD::For<NumberOfMembers>([&](
                        auto I,
                        auto members,
                        LD::ElementReference<tinyxml2::XMLElement> classNode,
                        T && object,
                        LD::ElementReference<HTMLEmitter> instance)
                                         {
                                             constexpr auto memberInfo = LD::Get<I>(members);
                                             constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                                             using ExtractedClass = typename decltype(type)::type;
                                             constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                                             auto memberAccessor = memberInfo(LD::Forward<T>(object));
                                             LD::StaticArray<char,1024> memberNameBuffer;
                                             LD::BackInserter<LD::StaticArray<char,1024>> memberNameBackInserter{memberNameBuffer};
                                             using MemberCntxt = LD::Context<LD::TransactionResult,LD::UInteger,LD::BackInserter<LD::StaticArray<char,1024>>&,decltype(memberName)&>;
                                             auto memberNameLength = LD::UTF8::Distance(
                                                     LD::UTF8::Begin(memberName),
                                                     LD::UTF8::End(memberName),memberNameBackInserter,memberName);

                                             bool memberNameIsValid = LD::MultiMatch(LD::Overload{
                                                     [](auto)
                                                     {
                                                         return false;
                                                     },
                                                     [](const MemberCntxt & context) noexcept
                                                     {
                                                         const auto & length = LD::Get(LD::Get<1>(context));
                                                         const auto & memberName = LD::Get(LD::Get<3>(context));
                                                         using IT = LD::Detail::Decay_T<decltype(memberNameBackInserter)>;
                                                         auto & inserter = LD::Get(LD::Get<2>(context));
                                                         LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});

                                                         for(LD::UInteger n = 0;n<length && LD::IsTransactionalQuery(result);++n)
                                                         {
                                                             result =  LD::UTF8::Append(memberName[n],inserter);
                                                         }
                                                         result =  LD::UTF8::Append('\0',inserter);

                                                         return LD::IsTransactionalQuery(result);
                                                     }
                                             },memberNameLength);

                                             if (memberNameIsValid)
                                             {
                                                 if constexpr(LD::SPA::IsText(LD::CT::RemoveQualifiers(LD::Type<T>{})))
                                                 {


                                                     if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))
                                                     {
                                                         classNode->SetText(LD::ToImmutableString(memberAccessor()).Data());
                                                     }else if constexpr (LD::CT::Spannable(LD::CT::RemoveQualifiers(type)))
                                                     {
                                                         LD::Span currentSpan{memberAccessor()};
                                                         using SpanType =LD::Type<decltype(currentSpan)>;
                                                         constexpr auto SpanInnerType = LD::CT::SpanType(SpanType{});
                                                         //todo add support for unicode
                                                         if constexpr (LD::CT::IsSame(SpanInnerType,LD::Type<char>{}))
                                                         {
                                                             classNode->SetText(currentSpan.Data());
                                                         }
                                                     }
                                                 }
                                                 else if constexpr (LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(type)))
                                                 {
                                                     //printf("being called \n");
                                                     classNode->SetAttribute(memberNameBuffer.GetData(),LD::ToImmutableString(memberAccessor()).Data());
                                                 }else if constexpr(LD::CT::IsTuple(LD::CT::RemoveQualifiers(type)))
                                                 {
                                                     const auto & tupleMember = memberAccessor();
                                                     constexpr auto tupleSize = LD::CT::TupleSize(LD::CT::RemoveQualifiers(LD::Type<decltype(tupleMember)>{}));
                                                     if constexpr(tupleSize > 0)
                                                     {
                                                         LD::For<tupleSize>([](
                                                                 auto I,
                                                                 const auto & tuple,
                                                                 LD::ElementReference<HTMLEmitter> instance,
                                                                 LD::ElementReference<tinyxml2::XMLElement> classNode)
                                                                            {
                                                                                const auto & memberOfTuple = LD::Get(LD::Get<I>(tuple));
                                                                                using MemberType = LD::Detail::Decay_T<decltype(memberOfTuple)>;
                                                                                //todo when this gets generalized, ask the backend if it supporst the idea of an "attribute"
                                                                                const bool backendSupportsAttributes = true;


                                                                                constexpr bool usesNewNode = LD::SPA::IsAttribute(LD::Type<MemberType>{}) || LD::SPA::IsText(LD::Type<MemberType>{});
                                                                                //std::cout << usesNewNode << std::endl;
                                                                                instance->__Print((tinyxml2::XMLNode*)classNode.GetPointer(),memberOfTuple,backendSupportsAttributes &&  not usesNewNode);


                                                                                return true;
                                                                            },tupleMember,instance,classNode);
                                                     }
                                                 }
                                                 else if constexpr (LD::CT::Spannable(LD::CT::RemoveQualifiers(type)))
                                                 {
                                                     LD::Span currentSpan{memberAccessor()};
                                                     using SpanType =LD::Type<decltype(currentSpan)>;
                                                     constexpr auto SpanInnerType = LD::CT::SpanType(SpanType{});
                                                     //if the span happens to represent a string we can just use it directly
                                                     //todo add support for unicode
                                                     if constexpr (LD::CT::IsSame(SpanInnerType,LD::Type<char>{}))
                                                     {
                                                         classNode->SetAttribute(memberNameBuffer.GetData(),currentSpan.Data());
                                                     }else
                                                     {
                                                     }
                                                 }
                                                 else if constexpr (LD::CT::IsReflectable(type))
                                                 {
                                                     instance->__Print((tinyxml2::XMLNode*)classNode.GetPointer(),memberAccessor());
                                                 }
                                             }
                                             return true;
                                         },LD::Forward<decltype(members)>(members),LD::ElementReference<tinyxml2::XMLElement>{node},LD::Forward<T>(object),LD::ElementReference<HTMLEmitter>{this});

            }
        }
    };
}
#endif //LANDESSDEVCORE_HTMLEMITTER_HPP
