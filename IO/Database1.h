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
#include "TypeTraits/IsVariant.hpp"
#include "TypeTraits/VariantTypes.h"
#include "Primitives/General/Span.hpp"
#include <iostream>
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
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept;


    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    !LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
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
        std::cout << "TupleSize: " << CurrentTupleSize << "\n";
        bool didInsert = true;
        LD::Insert(document,key+LD::ImmutableString{".Length"},CurrentTupleSize);
        LD::For<CurrentTupleSize>([](auto I, Key && key, V && object, T & document, bool & insertStatus)
        {
            std::cout << "Tuple insertion : " << (key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger (I))).Data() << "\n";
            auto response = LD::Insert(
                    document,
                    key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger (I)),
                    LD::Get(LD::Get<I>(LD::Forward<V>(object))));
            insertStatus = (insertStatus && LD::IsTransactionalResponse(response));


            return true;
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
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept
    {
        auto onObject = [&](const auto & object) noexcept
        {
            auto className = LD::CT::GetClassName(LD::CT::RemoveQualifiers(LD::Type<decltype(object)>{}));
            LD::Insert(document,key+LD::ImmutableString{".Type"},className);
            return LD::Insert(document,LD::Forward<Key>(key),object,LD::Forward<Args>(args)...);
        };
        return LD::MultiMatch(onObject,LD::Forward<V>(object));
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
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{})),
                    !LD::CT::CanBeAnImmutableString(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Insert(T & document, Key && key ,V && object, Args && ... args) noexcept
    {
        LD::Detail::Conditional_T<(LD::Detail::IsLValueReference<V>::value),V &,LD::Detail::Decay_T<V>> mappedObject{LD::Forward<V>(object)};
        LD::Span spannedObject{mappedObject} ;
        LD::UInteger spanSize = spannedObject.Size();
        LD::UInteger serializedAmount = 0;
        LD::Insert(
                document,
                key + LD::ImmutableString{"."} + LD::ToImmutableString("Length"),
                spanSize);
        for(LD::UInteger n = 0;n<spanSize;++n)
        {
            LD::RequestResponse<bool()> response =  LD::Insert(
                    document,
                    key + LD::ImmutableString{"."} + LD::ToImmutableString(n),
                            spannedObject[n]);
            auto onResponse = [](bool ) noexcept
            {
                return true;
            };

            auto onFailure = [](LD::TransactionError ) noexcept
            {
                return false;
            };

            serializedAmount +=  (LD::InvokeVisitation(LD::Overload{onResponse,onFailure},response));
        }
        if (serializedAmount == spanSize)
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
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & primitiveType, Args && ... args) noexcept;
    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept;




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

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & tuple, Args && ... args) noexcept
    {
        constexpr LD::UInteger CurrentTupleSize = LD::CT::TupleSize(LD::Type<LD::Detail::Decay_T<V>>{});

        LD::For<CurrentTupleSize>([](
                auto I,
                T & document,
                Key && key,
                V & tuple) noexcept
        {
            auto & objectAtIndex = LD::Get(LD::Get<I>(tuple));
            LD::RequestResponse<bool()> response =  LD::Fetch(document,key+LD::ImmutableString{"."}+LD::ToImmutableString(LD::UInteger(I)),objectAtIndex);

            auto onResponse = [](bool status) noexcept
            {
                return true;
            };


            auto onError = [](LD::TransactionError ) noexcept
            {
                return false;
            };

            return LD::InvokeVisitation(LD::Overload{onResponse,onError},response);
        },document,LD::Forward<Key>(key),tuple);
        return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        V tuple;
        LD::RequestResponse<bool()> request =  LD::Fetch(document,key,tuple);

        auto onFetch = [](bool status) noexcept
        {
            return true;
        };

        auto onError = [](LD::TransactionError) noexcept
        {
            return false;
        };
        bool isValid = LD::InvokeVisitation(LD::Overload{onFetch,onError},request);

        if(isValid)
        {
            return LD::CreateResponse(LD::Type<V>{},V{tuple},LD::Forward<Args>(args)...);
        }
        return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & variantType, Args && ... args) noexcept
    {
        auto TypesToCheck = LD::CT::VariantTypes(LD::Type<V>{});

        constexpr LD::UInteger NumberOfTypesToCheck = LD::CT::TypeListSize(LD::Type<decltype(TypesToCheck)>{});

        LD::For<NumberOfTypesToCheck>([](
                auto I) noexcept
        {


            return true;
        });
        auto onVariantType = [&](auto & object) noexcept
        {
            return LD::Fetch(document,LD::Forward<Key>(key),object,LD::Forward<Args>(args)...);
        };

        return LD::MultiMatch(LD::Overload{onVariantType},variantType);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsVariant(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        V variant;
        LD::RequestResponse<bool()> variantResponse = LD::Fetch(document,LD::Forward<Key>(key),variant);

        auto onFoundVariant = [&](bool status) noexcept
        {
            return LD::CreateResponse(LD::Type<V>{},V{variant},LD::Forward<Args>(args)...);
        };


        auto onError = [&](LD::TransactionError) noexcept
        {
            return LD::CreateResponse(LD::Type<V>{},LD::TransactionError{},LD::Forward<Args>(args)...);
        };

        return LD::InvokeVisitation(LD::Overload{onFoundVariant,onError},variantResponse);
    }


    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<bool(Args...)>> Fetch(T & document, Key && key ,V & spannableType, Args && ... args) noexcept
    {

        auto spannableSizeRequest = LD::Fetch(
                document,
                key+ LD::ImmutableString{"."} + LD::ToImmutableString("Length"),
                LD::Type<LD::UInteger>{},
                document,
                LD::Forward<Key>(key),
                spannableType,
                LD::Forward<Args>(args)...);
        auto onSpannableSize = [](
                LD::UInteger size,
                T & document,
                Key  key,
                V & spannableObject,
                Args  && ... args) noexcept
        {
            LD::BackInserter<V> inserter{spannableObject};


            LD::UInteger foundElements = 0;
            for(LD::UInteger n = 0;n<size;++n)
            {
                auto indexResponse =  LD::Fetch(
                        document,
                        key + LD::ImmutableString{"."} + LD::ToImmutableString(n),
                        LD::CT::SpanType(LD::Type<LD::Span<V>>{}),
                        inserter);

                auto onFoundElement = [](
                        auto Object,
                        LD::BackInserter<V> & inserter) noexcept
                {
                    inserter = Object;
                    return true;
                };

                auto onElementError = [](
                        LD::TransactionError ,
                        LD::BackInserter<V> & inserter) noexcept
                {
                    return false;
                };

                foundElements += (LD::InvokeVisitation(LD::Overload{onFoundElement,onElementError},indexResponse));
            }
            if (foundElements == size)
            {
                return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<Args>(args)...);
            }
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
        };

        auto onFailedToFetchSpanSize = [](
                LD::TransactionError,
                T &,
                Key  key,
                V &,
                Args && ... args  ) noexcept
        {
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(args)...);
        };

        return LD::InvokeVisitation(
                LD::Overload{onSpannableSize,onFailedToFetchSpanSize},
                spannableSizeRequest);
    }

    template<typename T, typename V, typename Key,typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::Spannable(LD::CT::RemoveQualifiers(LD::Type<V>{}))
            >,
            LD::RequestResponse<V(Args...)>> Fetch(T & document, Key && key ,LD::Type<V>, Args && ... args) noexcept
    {
        V spannableObject;
        auto arrayFetchResponse =  LD::Fetch(document,LD::Forward<Key>(key),spannableObject);

        auto onFetch = [](V array) noexcept -> LD::Optional<V>
        {
            return LD::Optional<V>{array};
        };


        auto onError = [](LD::TransactionError) noexcept -> LD::Optional<V>
        {
            return LD::Optional<V>{};
        };

        LD::Optional<V> object = LD::InvokeVisitation(LD::Overload{onFetch,onError},arrayFetchResponse);

        if (object.HasValue())
        {
            return LD::CreateResponse(LD::Type<V>{},V{*object},LD::Forward<Args>(args)...);
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
