//
// Created by phoenixflower on 11/5/20.
//

#ifndef LANDESSDEVCORE_STYLESHEET_HPP
#define LANDESSDEVCORE_STYLESHEET_HPP
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Detection.hpp"
#include "Reflection/Reflection.hpp"
#include "Definitions/Common.hpp"
#include "Unicode/UTF8.hpp"
#include "Primitives/General/Span.hpp"
#include "Primitives/General/Immutable.hpp"
#include "CssBlock.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename B, class = void>
        class StyleSheet;
        template<typename ... B>
        class StyleSheet<LD::CT::TypeList<B...>,LD::Enable_If_T<LD::Require<true>>>
        {
        private:
            LD::Context<B...> mContext;
        public:
            StyleSheet(B && ... objects) noexcept:mContext{LD::Forward<B>(objects)...}
            {

            }

            template<typename T, typename BackInserter>
            static void Serialize(T && object, BackInserter inserter) noexcept
            {
                using Type = LD::Type<LD::Detail::Decay_T<T>>;

                constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<T>>();
                constexpr auto className = LD::CT::GetClassName(traits);
                constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
                constexpr auto members = traits.Members;

                LD::For<NumberOfMembers>([](
                        auto I,
                        auto members,
                        T && object,
                        BackInserter inserter)
                                         {
                                             constexpr auto memberInfo = LD::Get<I>(members);
                                             constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                                             constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                                             auto memberAccessor = memberInfo(LD::Forward<T>(object));

                                             auto memberNameLength = LD::UTF8::Distance(
                                                     LD::UTF8::Begin(memberName),
                                                     LD::UTF8::End(memberName),inserter,memberName);

                                             using MemberCntxt = LD::Context<LD::TransactionResult,LD::UInteger,BackInserter&,decltype(memberName)&>;

                                             bool memberNameIsValid = LD::MultiMatch(LD::Overload{
                                                     [](auto)
                                                     {
                                                         return false;
                                                     },
                                                     [](const MemberCntxt & context) noexcept
                                                     {
                                                         const auto & length = LD::Get(LD::Get<1>(context));
                                                         const auto & memberName = LD::Get(LD::Get<3>(context));
                                                         using IT = BackInserter;
                                                         auto & inserter = LD::Get(LD::Get<2>(context));
                                                         LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});

                                                         for(LD::UInteger n = 0;n<length && LD::IsTransactionalQuery(result);++n)
                                                         {
                                                             result =  LD::UTF8::Append(memberName[n],inserter);
                                                         }
                                                         result =  LD::UTF8::Append(':',inserter);
                                                         result =  LD::UTF8::Append(' ',inserter);
                                                         return LD::IsTransactionalQuery(result);
                                                     }
                                             },memberNameLength);

                                             if (memberNameIsValid)
                                             {
                                                 if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))
                                                 {
                                                     auto stringifiedPrimitive = LD::ToImmutableString(memberAccessor());
                                                     for(LD::UInteger n = 0;n<stringifiedPrimitive.GetSize();++n)
                                                     {
                                                         LD::UTF8::Append(stringifiedPrimitive[n],inserter);
                                                     }
                                                 }else if constexpr (LD::CT::Spannable(LD::CT::RemoveQualifiers(type)))
                                                 {
                                                     LD::Span currentSpan{memberAccessor()};
                                                     using SpanType =LD::Type<decltype(currentSpan)>;
                                                     constexpr auto SpanInnerType = LD::CT::SpanType(SpanType{});
                                                     //if the span happens to represent a string we can just use it directly
                                                     //todo add support for unicode
                                                     if constexpr (LD::CT::IsSame(SpanInnerType,LD::Type<char>{}))
                                                     {
                                                         for(LD::UInteger n = 0;n<currentSpan.Size();++n)
                                                         {
                                                             LD::UTF8::Append(currentSpan[n],inserter);
                                                         }
                                                     }else
                                                     {
                                                     }

                                                 }
                                             }
                                             LD::UTF8::Append(';',inserter);
                                             LD::UTF8::Append('\n',inserter);
                                             return true;
                                         },members,LD::Forward<T>(object),inserter);

            }
            LD::StaticArray<char,1024> ToString() const
            {

                LD::StaticArray<char,1024> cssPageBuffer;
                using CssPageBackInserterType = LD::BackInserter<LD::StaticArray<char,1024>>;
                CssPageBackInserterType cssPageBackInserter{cssPageBuffer};
                LD::For<sizeof...(B)>([](
                        auto I,
                        LD::Ref<StyleSheet> instance,
                        CssPageBackInserterType backInserter)
                                      {
                                          const auto & object = LD::Get(LD::Get<I>(instance->mContext));
                                          using T = LD::Detail::Decay_T<decltype(object)>;
                                          constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<T>>();
                                          constexpr auto className = LD::CT::GetClassName(traits);
                                          constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
                                          constexpr auto members = traits.Members;


                                          using ClassNameContext = LD::Context<LD::TransactionResult,LD::UInteger,CssPageBackInserterType& ,decltype(className)&>;

                                          //we're going to use a different classname if it happens to be a CssBlock

                                          auto appendClassName = [](auto className, auto backInserter) noexcept
                                          {
                                              auto classNameLength = LD::UTF8::Distance(
                                                      LD::UTF8::Begin(className),
                                                      LD::UTF8::End(className),backInserter,className);
                                              bool classNameIsValid = LD::MultiMatch(LD::Overload{
                                                      [](auto)
                                                      {
                                                          return false;
                                                      },
                                                      [](const ClassNameContext & context) noexcept
                                                      {
                                                          const auto & length = LD::Get(LD::Get<1>(context));
                                                          const auto & memberName = LD::Get(LD::Get<3>(context));
                                                          using IT = CssPageBackInserterType ;
                                                          auto & inserter = LD::Get(LD::Get<2>(context));
                                                          LD::QueryResult<IT()> result = LD::MakeContext(LD::TransactionResult{},IT{inserter});

                                                          for(LD::UInteger n = 0;n<length && LD::IsTransactionalQuery(result);++n)
                                                          {
                                                              result =  LD::UTF8::Append(memberName[n],inserter);
                                                          }


                                                          return LD::IsTransactionalQuery(result);
                                                      }
                                              },classNameLength);
                                              return classNameIsValid;
                                          };
                                          bool classNameIsValid = false;
                                          if constexpr (LD::SPA::IsCSSBlock(LD::Type<T>{}))
                                          {
                                              //classNameIsValid = appendClassName(className,backInserter);
                                          }else
                                          {
                                              classNameIsValid = appendClassName(className,backInserter);
                                          }
                                          if (classNameIsValid)
                                          {
                                              LD::UTF8::Append(' ',backInserter);
                                              LD::UTF8::Append('{',backInserter);
                                              LD::For<NumberOfMembers>([](
                                                      auto I,
                                                      auto members,
                                                      CssPageBackInserterType backInserter,
                                                      const T & object)
                                                                       {

                                                                           LD::UTF8::Append('\n',backInserter);
                                                                           constexpr auto memberInfo = LD::Get<I>(members);
                                                                           constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                                                                           constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                                                                           auto memberAccessor = memberInfo(object);
                                                                           if constexpr(LD::CT::IsTuple(LD::CT::RemoveQualifiers(type)))
                                                                           {
                                                                               const auto & tupleMember = memberAccessor();
                                                                               constexpr auto tupleSize = LD::CT::TupleSize(LD::CT::RemoveQualifiers(LD::Type<decltype(tupleMember)>{}));

                                                                               if constexpr(tupleSize > 0)
                                                                               {
                                                                                   LD::For<tupleSize>([](
                                                                                           auto I,
                                                                                           CssPageBackInserterType backInserter,
                                                                                           const auto & tuple)
                                                                                                      {
                                                                                                          const auto & memberOfTuple = LD::Get(LD::Get<I>(tuple));
                                                                                                          //instance->__Print((tinyxml2::XMLNode*)classNode.GetPointer(),memberOfTuple,backendSupportsAttributes &&  not usesNewNode);
                                                                                                          StyleSheet::Serialize(memberOfTuple,backInserter);
                                                                                                          return true;
                                                                                                      },backInserter,tupleMember);
                                                                               }

                                                                           }else
                                                                           {
                                                                               StyleSheet::Serialize(memberAccessor(),backInserter);
                                                                           }
                                                                           return true;

                                                                       },members,backInserter,object);
                                          }

                                          LD::UTF8::Append('}',backInserter);

                                          return true;
                                      },LD::Ref<StyleSheet>{(StyleSheet*)this},cssPageBackInserter);

                LD::UTF8::Append('\0',cssPageBackInserter);
                return cssPageBuffer;
            }
        };


        template<typename ... T> StyleSheet(T && ...) -> StyleSheet<LD::CT::TypeList<T...>>;
    }
}
#endif //LANDESSDEVCORE_STYLESHEET_HPP
