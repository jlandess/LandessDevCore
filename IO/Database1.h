//
// Created by phoenixflower on 3/23/21.
//

#ifndef LANDESSDEVCORE_DATABASE1_H
#define LANDESSDEVCORE_DATABASE1_H
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/IsFunction.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Core/RequestResponse.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/StringView.hpp"
#include "Reflection/Reflection.hpp"
#include "Memory/Optional.h"
#include "Algorithms/FromString.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
#include "TypeTraits/CanBeMadeFromStringView.hpp"
namespace LD
{
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept;



    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    !LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))

            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key,V && object,Args && ... args) noexcept;







    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept
    {
        auto immutableRepresentation = LD::ToImmutableString(LD::Forward<V>(object));
        auto response = document.Insert(
                LD::StringView{key.Data(),key.GetSize()},
                LD::StringView{immutableRepresentation.Data(),immutableRepresentation.GetSize()});

        if (LD::IsTransactionalResponse(response))
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(args)...);
        }

        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    !LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))

            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept
    {
        auto immutableRepresentation = LD::ToImmutableString(LD::Forward<V>(object));
        auto response = document.Insert(
                LD::StringView{key.Data(),key.GetSize()},
                LD::StringView{immutableRepresentation.Data(),immutableRepresentation.GetSize()});

        if (LD::IsTransactionalResponse(response))
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(args)...);
        }

        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept
    {
        constexpr LD::UInteger CurrentTupleSize = LD::CT::TupleSize(LD::Type<LD::Detail::Decay_T<V>>{});
        bool didInsert = true;
        LD::For<CurrentTupleSize>([](auto I, Key && key, V && object, T & document, bool & insertStatus)
        {
            auto response = LD::Insert(
                    document,
                    key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger (I)),
                    LD::Get(LD::Get<I>(LD::Forward<V>(object))));
            insertStatus = (insertStatus && LD::IsTransactionalResponse(response));


            return insertStatus;
        },LD::Forward<Key>(key),LD::Forward<V>(object),document,didInsert);

        if (didInsert == true)
        {
            return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<bool(Args...)>> Insert(
                    T & document,
                    Key && key,
                    V && object,
                    Args && ... args) noexcept
    {
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
        constexpr auto members = traits.Members;
        constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
        constexpr auto className = LD::CT::GetClassName(traits);
        auto classBuffer = LD::ToImmutableString(className);
        bool status = true;
        if (classBuffer.GetSize() > 0)
        {
            LD::For<NumberOfMembers>([](
                    auto I,
                    auto members,
                    V && object,
                    auto classBuffer,
                    T & document,
                    bool & status,
                    Key && key)
            {

                constexpr auto memberInfo = LD::Get<I>(members);
                constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                auto memberAccessor = memberInfo(LD::Forward<V>(object));
                constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                auto memberBuffer = LD::ToImmutableString(memberName);

                auto currentKey = key+LD::ImmutableString{"."}+memberBuffer;
                if (memberBuffer.GetSize() > 0)
                {
                    const auto & member = memberAccessor();

                    if constexpr(!LD::CT::IsReflectable(type))
                    {
                        auto response = Insert(document,currentKey,member);


                        auto onResponse = [](auto object) noexcept
                        {
                            return true;
                        };

                        auto onError = [](LD::TransactionError ) noexcept
                        {
                            return false;
                        };
                        bool responseValue = LD::InvokeVisitation(LD::Overload{onResponse,onError},response);
                        if (!responseValue)
                        {
                            //status = handler();
                        }


                    }
                    else if constexpr (LD::CT::IsReflectable(type))
                    {
                        auto response = Insert(document,currentKey,memberAccessor());

                        auto onResponse = [](auto object) noexcept
                        {
                            return true;
                        };

                        auto onError = [](LD::TransactionError ) noexcept
                        {
                            return false;
                        };

                        bool responseValue = LD::InvokeVisitation(LD::Overload{onResponse,onError},response);

                        if (!responseValue)
                        {
                            //status = handler();
                        }
                    }

                }
                return true;

            },members,LD::Forward<V>(object),classBuffer,document,status,LD::Forward<Key>(key));
        }
        if (status)
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }



    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key,V & object, Args && ... args) noexcept;
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key,LD::Type<V>, Args && ... args) noexcept;


    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;



    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;


    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept
    {

        auto responseFunction = [](auto a, auto b, V & primitiveType)
        {
            auto response1 = LD::FromString(LD::Type<V>{},b,primitiveType);
            auto onValue = [](V value, V & assignment) noexcept
            {
                assignment = value;
                return LD::Optional<bool>{true};
            };
            auto onError1 = [](LD::TransactionError, V & assignment ) noexcept
            {
                return LD::Optional<bool>{};
            };
            return LD::InvokeVisitation(LD::Overload{onValue,onError1},response1);
        };
        auto parsingResponse =  document.Query(LD::StringView{LD::ToImmutableString(LD::Forward<Key>(key)).Data()},responseFunction,primitiveType);

        auto onParse = [](LD::Optional<bool> object, V &) noexcept
        {
            return LD::Optional<bool>{true};
        };

        auto onParsingError = [](LD::TransactionError, V &) noexcept
        {
            return LD::Optional<bool>{};
        };

        LD::Optional<bool> parsedResult = LD::InvokeVisitation(LD::Overload{onParse,onParsingError},parsingResponse);


        if (parsedResult.HasValue())
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(args)...);
        }

        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        LD::RequestResponse<LD::Optional<V>()> parsingResponse =  document.Query(LD::StringView{LD::ToImmutableString(LD::Forward<Key>(key)).Data()},[](auto a, auto b)
        {
            auto response1 = LD::FromString(LD::Type<V>{},b);
            auto onValue = [](V value) noexcept
            {
                return LD::Optional<V>{value};
            };
            auto onError1 = [](LD::TransactionError ) noexcept
            {
                return LD::Optional<V>{};
            };
            return LD::InvokeVisitation(LD::Overload{onValue,onError1},response1);
        });

        auto onParse = [](LD::Optional<V> object) noexcept
        {
            return object;
        };

        auto onParsingError = [](LD::TransactionError) noexcept
        {
            return LD::Optional<V>{};
        };
        LD::Optional<V> parsedResult = LD::InvokeVisitation(LD::Overload{onParse,onParsingError},parsingResponse);
        if (parsedResult)
        {
            return LD::CreateResponse(LD::Type<V>{},*parsedResult,LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }


    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & immutableString, Args && ... args) noexcept
    {

        auto responseFunction = [](auto key, auto value, V & immutableString) -> bool
        {
            constexpr LD::UInteger ImmutableStringCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
            if (ImmutableStringCapacity < value.size() && value.size() > 0)
            {
                for(LD::UInteger n = 0;n<value.size();++n)
                {
                    immutableString[n] = value[n];
                }
                return true;
            }
            return false;
        };
        auto parsingResponse =  document.Query(LD::StringView{LD::ToImmutableString(LD::Forward<Key>(key)).Data()},responseFunction,immutableString);


        auto onParse = [](bool status, V & ) noexcept
        {
            return LD::Optional<bool>{true};
        };

        auto onTransactionError = [](LD::TransactionError, V &) noexcept
        {
            return LD::Optional<bool>{};
        };

        LD::Optional<bool> status = LD::InvokeVisitation(LD::Overload{onParse,onTransactionError},parsingResponse);
        if (status.HasValue())
        {
            return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        auto responseFunction = [](auto key, auto value) -> LD::Optional<V>
        {
            constexpr LD::UInteger ImmutableStringCapacity = LD::CT::ImmutableStringCapacity(LD::Type<V>{});
            V immutableString;
            if (ImmutableStringCapacity < value.size() && value.size() > 0)
            {
                for(LD::UInteger n = 0;n<value.size();++n)
                {
                    immutableString[n] = value[n];
                }
                return LD::Optional<V>{immutableString};
            }
            return LD::Optional<V>{};
        };
        auto parsingResponse =  document.Query(LD::StringView{LD::ToImmutableString(LD::Forward<Key>(key)).Data()},responseFunction);


        auto onParse = [](LD::Optional<V> string) noexcept -> LD::Optional<V>
        {
            return string;
        };

        auto onError = [](LD::TransactionError) noexcept -> LD::Optional<V>
        {
            return LD::Optional<V>{};
        };

        LD::Optional<V> immutableString = LD::InvokeVisitation(LD::Overload{onParse,onError},parsingResponse);

        if (immutableString.HasValue())
        {
            return LD::CreateResponse(LD::Type<V>{},V{*immutableString},LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }
    /*
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    !LD::CT::IsPrimitive(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    LD::CT::CanBeMadeFromStringView(LD::Type<V>{})
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        LD::RequestResponse<LD::Optional<V>()> parsingResponse =  document.Query(LD::StringView{LD::ToImmutableString(LD::Forward<Key>(key)).Data()},[](auto a, auto b)
        {
            auto response1 = LD::FromString(LD::Type<V>{},b);
            auto onValue = [](V value) noexcept
            {
                return LD::Optional<V>{value};
            };
            auto onError1 = [](LD::TransactionError ) noexcept
            {
                return LD::Optional<V>{};
            };
            return LD::InvokeVisitation(LD::Overload{onValue,onError1},response1);
        });

        auto onParse = [](LD::Optional<V> object) noexcept
        {
            return object;
        };

        auto onParsingError = [](LD::TransactionError) noexcept
        {
            return LD::Optional<V>{};
        };
        LD::Optional<V> parsedResult = LD::InvokeVisitation(LD::Overload{onParse,onParsingError},parsingResponse);
        if (parsedResult)
        {
            return LD::CreateResponse(LD::Type<V>{},*parsedResult,LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }
     */

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key,V & object, Args && ... args) noexcept
    {
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
        constexpr auto members = traits.Members;
        constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
        constexpr auto className = LD::CT::GetClassName(traits);
        auto classBuffer = LD::ToImmutableString(className);
        bool status = true;

        if (classBuffer.GetSize() > 0)
        {
            LD::For<NumberOfMembers>([](
                    auto I,
                    auto members,
                    V & object,
                    auto classBuffer,
                    T & document,
                    bool & status,
                    Key && key)
            {

                constexpr auto memberInfo = LD::Get<I>(members);
                constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                auto memberAccessor = memberInfo(object);
                constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                auto memberBuffer = LD::ToImmutableString(memberName);
                auto currentKey = key+LD::ImmutableString{"."}+memberBuffer;
                if (memberBuffer.GetSize() > 0)
                {
                    if constexpr (!LD::CT::IsReflectable(type))
                    {
                        auto fetchResult = Fetch(document,currentKey,LD::CT::RemoveQualifiers(type),memberAccessor);
                        auto onFetch = [](auto value, auto  member) noexcept
                        {
                            (*member) = value;
                            return true;
                        };
                        auto onError = [](LD::TransactionError, auto  member) noexcept
                        {
                            return false;
                        };
                        bool successfulFetch =  LD::InvokeVisitation(LD::Overload{onFetch,onError},fetchResult);
                    }else if constexpr (LD::CT::IsReflectable(type))
                    {
                        auto ReflectiveType = LD::CT::RemoveQualifiers(LD::Type<decltype(memberAccessor())>{});
                        LD::RequestResponse<typename decltype(ReflectiveType)::type(decltype(memberAccessor)&)> response =  Fetch(document,currentKey,ReflectiveType,memberAccessor);
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
            },members,object,classBuffer,document,status,LD::Forward<Key>(key));
        }
        if (status)
        {
            return LD::CreateResponse(LD::Type<bool>{},true,LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key,LD::Type<V>, Args && ... args) noexcept
    {
        V object;
        auto response = LD::Fetch(document,LD::Forward<Key>(key),object,object,LD::Forward<Args>(args)...);

        auto onObject = [](bool status, V & object, auto && ... args) noexcept
        {
            return LD::CreateResponse(LD::Type<V>{},V{object},LD::Forward<Args>(args)...);
        };
        auto onError = [](LD::TransactionError, V & object, auto && ... args) noexcept
        {
            return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
        };
        return LD::InvokeVisitation(LD::Overload{onObject,onError},response);
    }
    /*
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsReflectable<LD::Detail::Decay_T<V>>()
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key,LD::Type<V>, Args && ... args) noexcept
    {
        constexpr auto traits = LD::CT::Reflect<LD::Detail::Decay_T<V>>();
        constexpr auto members = traits.Members;
        constexpr auto NumberOfMembers = LD::CT::GetNumberOfMembers(traits);
        constexpr auto className = LD::CT::GetClassName(traits);
        auto classBuffer = LD::ToImmutableString(className);
        bool status = true;
        V object;
        if (classBuffer.GetSize() > 0)
        {
            LD::For<NumberOfMembers>([](
                    auto I,
                    auto members,
                    V & object,
                    auto classBuffer,
                    T & document,
                    bool & status,
                    Key && key)
            {

                constexpr auto memberInfo = LD::Get<I>(members);
                constexpr auto type = LD::CT::GetDescriptorType(memberInfo);
                auto memberAccessor = memberInfo(object);
                constexpr auto memberName = LD::CT::GetMemberDescriptorName(memberInfo);
                auto memberBuffer = LD::ToImmutableString(memberName);
                auto currentKey = key+LD::ImmutableString{"."}+memberBuffer;
                if (memberBuffer.GetSize() > 0)
                {
                    if constexpr (!LD::CT::IsReflectable(type))
                    {
                        auto fetchResult = Fetch(document,currentKey,LD::CT::RemoveQualifiers(type),memberAccessor);

                        auto onFetch = [](auto value, auto  member) noexcept
                        {
                            (*member) = value;
                            return true;
                        };

                        auto onError = [](LD::TransactionError, auto  member) noexcept
                        {
                            return false;
                        };

                        bool successfulFetch =  LD::InvokeVisitation(LD::Overload{onFetch,onError},fetchResult);

                    }else if constexpr (LD::CT::IsReflectable(type))
                    {
                        auto ReflectiveType = LD::CT::RemoveQualifiers(LD::Type<decltype(memberAccessor())>{});
                        LD::RequestResponse<typename decltype(ReflectiveType)::type(decltype(memberAccessor)&)> response =  Fetch(document,currentKey,ReflectiveType,memberAccessor);
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

            },members,object,classBuffer,document,status,LD::Forward<Key>(key));
        }
        if (status)
        {
            return LD::CreateResponse(LD::Type<V>{},object,LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }
     */
}

#endif //LANDESSDEVCORE_DATABASE1_H
