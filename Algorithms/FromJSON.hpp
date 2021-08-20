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
#include "Algorithms/CompileTimeControlFlow.hpp"
#include <simdjson/simdjson.h>
#include "Reflection/Reflection.hpp"
namespace LD
{


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
                    LD::RequestResponse<typename decltype(ReflectiveType)::type(decltype(memberAccessor)&)> response = FromJSON(
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
