//
// Created by phoenixflower on 8/11/21.
//

#ifndef LANDESSDEVCORE_FROMJSON_HPP
#define LANDESSDEVCORE_FROMJSON_HPP
#include <simdjson/simdjson.h>
#include "Primitives/General/Immutable.hpp"
#include "Reflection/NamedTuple.h"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "TypeTraits/IsVariant.hpp"
#include "Unicode/UTF8.hpp"
#include "Primitives/General/Span.hpp"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include <simdjson/simdjson.h>
#include "Reflection/Reflection.hpp"
#include "Memory/OptionalReference.h"
namespace LD
{

    template<typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::Type<V>{})>,LD::Optional<V&>> FromJSON(V & object,const simdjson::dom::element & json) noexcept;
    template<typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsImmutableString(LD::Type<V>{})>,LD::Optional<V&>> FromJSON(V & object,const simdjson::dom::element & json) noexcept;
    template<typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsTuple(LD::Type<V>{})>,LD::Optional<V&>> FromJSON(V & object,const simdjson::dom::element & json) noexcept;
    template<typename V,typename ... A>
    LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,LD::RequestResponse<V(A...)>> FromJSON(V & object, const simdjson::dom::element & json,A && ... args) noexcept;

    template<typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::Type<V>{})>,LD::Optional<V&>> FromJSON(V & object,const simdjson::dom::element & document) noexcept
    {
        simdjson::simdjson_result<V> possibleRes = document.template get<V>();
        if(!possibleRes.error())
        {
            object = possibleRes.value();
            return LD::Optional<V&>{object};
        }
        return LD::Optional<V&>{};
    }
    template<typename V>
    LD::Enable_If_T<LD::Require<LD::CT::IsImmutableString(LD::Type<V>{})>,LD::Optional<V&>> FromJSON(V & object,const simdjson::dom::element & document) noexcept
    {
        simdjson::simdjson_result<const char*> possibleRes = document.get_c_str();
        if(!possibleRes.error())
        {
            LD::UInteger stringLength = strlen(possibleRes.value());
            constexpr LD::UInteger maxCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
            LD::BackInserter inserter{object};
            for(LD::UInteger n = 0;n<stringLength && n<maxCapacity;++n)
            {
                inserter = possibleRes.value()[n];
            }
            return LD::Optional<V&>{object};
        }
        return LD::Optional<V&>{};
    }
    template<typename V,typename ... A>
    LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,LD::RequestResponse<V(A...)>> FromJSON(V & object, const simdjson::dom::element & json,A && ... args) noexcept
    {
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
        constexpr auto members = traits.Members;
        constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);

        LD::For<NumberOfMembers>([](
                auto I,
                auto members,
                const simdjson::dom::element & document,
                V & object) noexcept
        {


            constexpr auto memberInfo = LD::Get<I>(members);

            constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);


            constexpr auto type = LD::CT::RemoveQualifiers(LD::CT::GetDescriptorType(memberInfo));


            auto memberAccessor = memberInfo(object);



            auto memberNameAsBuffer = LD::ToImmutableString(memberName);


            if (memberNameAsBuffer.GetSize() > 0)
            {
                if constexpr(LD::CT::IsPrimitive(type))
                {
                    simdjson::simdjson_result<LD::GetType<decltype(type)>> possibleRes = document[memberNameAsBuffer.Data()].template get<LD::GetType<decltype(type)>>();


                    if (!possibleRes.error())
                    {

                        memberAccessor = possibleRes.value();
                    }
                }
                else if constexpr(LD::CT::IsImmutableString(type))
                {

                    simdjson::simdjson_result<const char*> possibleRes = document[memberNameAsBuffer.Data()].get_c_str();
                    if(!possibleRes.error())
                    {
                        LD::GetType<decltype(type)> immutableString;
                        constexpr LD::UInteger immutableStringSize = LD::CT::ImmutableStringCapacity(type);
                        LD::UInteger stringLength = strlen(possibleRes.value());

                        if (stringLength <= immutableStringSize)
                        {
                            for(LD::UInteger n = 0;n<stringLength;++n)
                            {
                                immutableString[n] = possibleRes.value()[n];
                            }
                        }

                        memberAccessor = immutableString;
                    }



                }
                else if constexpr(LD::CT::IsReflectable(type))
                {
                    auto ReflectiveType = LD::CT::RemoveQualifiers(LD::Type<decltype(memberAccessor())>{});
                    using ReflectypeTypeAsType = decltype(ReflectiveType);
                    using ActualType = typename ReflectypeTypeAsType::type;
                    //LD::CT::DebugTemplate<ActualType>{};
                    ActualType type;
                    //LD::CT::DebugTemplate<decltype(ReflectiveType)>{};
                    auto  response  = LD::FromJSON(type,document[memberNameAsBuffer.Data()],memberAccessor);

                    auto onSuccessfulQuery = [](auto object, auto member) noexcept
                    {
                        //LD::CT::DebugTemplate<decltype(object)>{};
                        (*member) = object;
                    };



                    auto onError = [](LD::TransactionError, auto) noexcept
                    {

                    };


                    LD::InvokeVisitation(LD::Overload{onSuccessfulQuery,onError},response);
                    /*
                    auto ReflectiveType = LD::CT::RemoveQualifiers(LD::Type<decltype(memberAccessor())>{});
                    auto  response = LD::FromJSON(
                            ReflectiveType,
                            document[memberNameAsBuffer.Data()],
                            memberNameAsBuffer,
                            memberAccessor);


                    auto onSuccessfulQuery = [](auto object, auto member) noexcept
                    {
                        (*member) = object;
                    };



                    auto onError = [](LD::TransactionError , auto member) noexcept
                    {

                    };


                    LD::InvokeVisitation(LD::Overload{onSuccessfulQuery,onError},response);
                     */
                }
            }
            return true;
        },members,json,object);


        return LD::CreateResponse(LD::Type<V>{},V{object},LD::Forward<A>(args)...);
    }

    template<typename V,typename ... A>
    LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::CT::RemoveQualifiers(LD::Type<V>{}))>,LD::RequestResponse<V(A...)>> FromJSON(LD::Type<V>, const simdjson::dom::element & json,A && ... args) noexcept
    {
        V object;
        return LD::FromJSON(object,json,LD::Forward<A>(args)...);
    }

}
#endif //LANDESSDEVCORE_FROMJSON_HPP
