//
// Created by phoenixflower on 2/24/21.
//

#ifndef LANDESSDEVCORE_TOJSON_HPP
#define LANDESSDEVCORE_TOJSON_HPP
#include "Primitives/General/Immutable.hpp"
#include "Reflection/NamedTuple.h"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "TypeTraits/IsVariant.hpp"
#include "Unicode/UTF8.hpp"
#include "Primitives/General/Span.hpp"
#include "Reflection/Reflection.hpp"
#include "IO/json.hpp"
namespace LD
{
    template<typename T, LD::UInteger N>
    LD::ImmutableString<N*4> ToImmutableString(const ctll::basic_fixed_string<T,N> & string) noexcept
    {
        LD::StaticArray<char,N*4> buffer;
        LD::BackInserter<LD::StaticArray<char,N*4>> bufferInserter{buffer};
        for(LD::UInteger n = 0;n<string.size();++n)
        {
            LD::UTF8::Append(string[n],bufferInserter);
        }
        LD::UTF8::Append('\0',bufferInserter);
        return LD::ImmutableString{buffer};
    }
    template<typename DocumentType, typename V>
    LD::Enable_If_T<LD::Require<LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,DocumentType &> ToJson(DocumentType & map, V && object) noexcept
    {
        map = nlohmann::json::array();
        LD::Span span{object};
        for(LD::UInteger n = 0;n<span.Size();++n)
        {
            const auto & object = span[n];
            constexpr auto UsableType = LD::CT::RemoveQualifiers(LD::Type<decltype(object)>{});

            if constexpr (LD::CT::IsVariant(UsableType))
            {
                auto funciton = [&](auto && abc)
                {
                    auto UsableType = LD::CT::RemoveQualifiers(LD::Type<decltype(abc)>{});
                    if constexpr (LD::CT::IsPrimitive(UsableType))
                    {
                        map.push_back(abc);
                    }else if constexpr (LD::CT::IsImmutableString(UsableType))
                    {
                        map.push_back(LD::ToImmutableString(abc).Data());
                    }
                };
                //auto variant = object[n];
                mpark::visit(funciton,object);
            }
        }
        return map;
    }
    template<typename DocumentType, typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,DocumentType &> ToJson(DocumentType & map, V && object) noexcept
    {
        map = object;
        return map;
    }
    template<typename DocumentType, typename V>
    LD::Enable_If_T<LD::Require<!LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<V>{})),LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{})),!LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,DocumentType &> ToJson(DocumentType & map, V && object) noexcept
    {
        map = LD::ToImmutableString(LD::Forward<V>(object)).Data();
        return map;
    }
    template<typename DocumentType, typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,DocumentType &> ToJson(DocumentType & map, V && object) noexcept
    {
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
        constexpr auto members = traits.Members;
        constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);


        LD::For<NumberOfMembers>([](
                auto I,
                auto members,
                DocumentType & document,
                V && object) noexcept
        {


            constexpr auto memberInfo = LD::Get<I>(members);

            constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);


            constexpr auto type = LD::CT::RemoveQualifiers(LD::CT::GetDescriptorType(memberInfo));


            auto memberAccessor = memberInfo(LD::Forward<V>(object));


            auto memberNameAsBuffer = LD::ToImmutableString(memberName);


            if (memberNameAsBuffer.GetSize() > 0)
            {

                auto memberNameRepresentation = memberNameAsBuffer;
                if constexpr (LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(type)))
                {
                    auto primtiveValue = memberAccessor();
                    document[memberNameRepresentation.Data()] = primtiveValue;
                }
                else if constexpr(LD::CT::IsImmutableString(type))
                {
                    document[memberNameAsBuffer.Data()] = memberAccessor().Data();
                }
                else if constexpr (LD::CT::IsNamedTuple(type))
                {
                    const auto & tuple = memberAccessor();
                    constexpr LD::UInteger TupleSize = LD::CT::TupleSize(type);

                    LD::For<TupleSize>([](

                            auto I,
                            const auto & tuple,
                            DocumentType & map,
                            auto memberName) noexcept

                    {
                        const auto & namedObject = LD::Get(LD::Get<I>(tuple));
                        const auto & name = namedObject.Name();
                        const auto & object = namedObject.Object();
                        //LD::CT::DebugTemplate<decltype(object)>{};
                        ToJson(map[name.Data()],object);
                        return true;

                    },tuple,document,memberNameAsBuffer);
                }
                else if constexpr(LD::CT::IsTuple(type))
                {
                    const auto & tuple = memberAccessor();
                    constexpr LD::UInteger TupleSize = LD::CT::TupleSize(type);

                    LD::For<TupleSize>([](

                            auto I,
                            const auto & tuple,
                            DocumentType & map,
                            auto name) noexcept

                    {

                        //ToJson(map[name.Data()],LD::Get(LD::Get<I>(tuple)));

                        return true;

                    },tuple,document,memberNameAsBuffer);


                }

                else if constexpr(LD::CT::Spannable(type))
                {
                    LD::Span span{memberAccessor()};
                }
                else if constexpr (LD::CT::IsReflectable(type))
                {
                    ToJson(document[memberNameRepresentation.Data()],memberAccessor());
                }
            }
            return true;
        },members,map,LD::Forward<V>(object));
        return map;
    }
}
#endif //LANDESSDEVCORE_TOJSON_HPP
