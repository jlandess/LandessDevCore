//
// Created by phoenixflower on 10/21/19.
//

#ifndef LANDESSDEVDATASTRUCTURES_RAPIDJSONEXTENSION_H
#define LANDESSDEVDATASTRUCTURES_RAPIDJSONEXTENSION_H

/*
#include <Definitions/Common.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <Primitives/General/Pair.h>
#include <Primitives/General/string_view.hpp>
#include <Memory/ElementReference.h>
template<typename T, typename DataStore>
constexpr LD::Enable_If_T<LD::Require<LD::IsFloatingPoint<T>>,rapidjson::PrettyWriter<DataStore>> & operator << (
        rapidjson::PrettyWriter<DataStore> & writer,
const PDP::DataStructures::Pair<nonstd::basic_string_view<char>,T> & object)
{
    const nonstd::basic_string_view<char> & key = object.GetFirst();
    const T & value = object.GetSecond();

    writer.Key(key.data());

    if constexpr(LD::IsFloatingPoint<T>)

    {
        writer.Double(value);
    }else if constexpr(LD::IsUnsignedInteger<T>)
    {
        writer.Uint(value);
    }else if constexpr(LD::IsInteger<T>)

    {
        writer.Int64(value);
    }else if constexpr(LD::IsCharacter<T>)

    {
        writer.Int(value);
    }else if constexpr(LD::IsSame<bool,T>)

    {
        writer.Bool(value);
    }else if constexpr(LD::IsSame<nonstd::basic_string_view<char>,T>)
    {
        writer.String(value.data());
    }
    return writer;
}
template<typename T, typename Encoding, typename Allocator>
LD::Enable_If_T <LD::Require<LD::IsPrimitive<T>>,const rapidjson::GenericValue<Encoding,Allocator >  & >operator >> (const rapidjson::GenericValue<Encoding,Allocator>  & currentDocument,PDP::DataStructures::Pair<nonstd::basic_string_view<char>,PDP::ElementReference<T>> & kvp)
{

    const auto & key = kvp.GetFirst();
    LD::ElementReference<T> & value = kvp.GetSecond();
    if constexpr(LD::IsFloatingPoint<T>)
    {
        *value = currentDocument[key.data()].GetDouble();
    }else if constexpr(LD::IsUnsignedInteger<T>)
    {
        *value = currentDocument[key.data()].GetUint64();
    }else if constexpr(LD::IsInteger<T>)
    {
        *value = currentDocument[key.data()].GetInt64();
    }else if constexpr(LD::IsCharacter<T>)
    {
        const char * keyedValue = currentDocument[key.data()].GetString();
        *value = keyedValue[0];
    }else if constexpr(LD::IsSame<bool,T>)
    {
        *value = currentDocument[key.data()].GetBool();
    }
    return currentDocument;
}

#endif //LANDESSDEVDATASTRUCTURES_RAPIDJSONEXTENSION_H
*/