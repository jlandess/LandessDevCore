//
// Created by phoenixflower on 3/23/21.
//

#ifndef LANDESSDEVCORE_CTREIMMUTABLEEXTENSION_HPP
#define LANDESSDEVCORE_CTREIMMUTABLEEXTENSION_HPP
#include "Primitives/General/ctre.hpp"
#include "Unicode/UTF8.hpp"
#include "Core/RequestResponse.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    template<LD::UInteger Size>
    LD::ImmutableString<Size*4> ToImmutableString(const ctll::fixed_string<Size> & string) noexcept
    {
        auto distance = LD::UTF8::DistanceOfOctets(LD::UTF8::Begin(string),LD::UTF8::End(string),string);
        auto onCodePoint = [](uint32_t size,const ctll::fixed_string<Size> & string) noexcept
        {
            LD::StaticArray<char,Size*4> buffer;
            LD::BackInserter backInserter{buffer};
            bool hasAnErrorOccured = false;
            for(LD::UInteger n = 0;n<size && !hasAnErrorOccured;++n)
            {
                auto octetResponse =  LD::UTF8::AppendOctet(string[n],backInserter);

                auto onOctet = [](auto octetIterator) noexcept
                {
                    return false;
                };

                auto onOctetError = [](LD::TransactionError) noexcept
                {
                    return true;
                };
                hasAnErrorOccured = LD::InvokeVisitation(LD::Overload{onOctet,onOctetError},octetResponse);
            }
            if (hasAnErrorOccured)
            {
                return  LD::ImmutableString<Size*4>{};
            }
            return LD::ImmutableString{buffer};
        };

        auto onError = [](LD::TransactionError,const ctll::fixed_string<Size> & string) noexcept
        {
            return LD::ImmutableString<Size*4>{};
        };

        return LD::InvokeVisitation(LD::Overload{onCodePoint,onError},distance);
    }
}
#endif //LANDESSDEVCORE_CTREIMMUTABLEEXTENSION_HPP
