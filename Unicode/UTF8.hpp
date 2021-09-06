//
// Created by phoenixflower on 7/11/20.
//

#ifndef LANDESSDEVCORE_UTF8_HPP
#define LANDESSDEVCORE_UTF8_HPP
#include <stdint.h>
#include "utf8/core.h"
#include "IO/FetchRequest.h"
#include "Primitives/General/ctre.hpp"
#include "Core/RequestResponse.hpp"
#include "Primitives/General/StringView.hpp"
namespace LD
{

    namespace UTF8
    {
        template<typename octet_iterator, typename ... Args>
        LD::RequestResponse<octet_iterator(Args...)> AppendOctet(uint32_t cp, octet_iterator result, Args && ... arguments) noexcept
        {
            //todo add error handling for when the iterator is out of bounds
            if (!utf8::internal::is_code_point_valid(cp))
                return LD::CreateResponse(LD::Type<octet_iterator>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);

            if (cp < 0x80)                        // one octet
                *(result++) = static_cast<uint8_t>(cp);
            else if (cp < 0x800) {                // two octets
                *(result++) = static_cast<uint8_t>((cp >> 6)            | 0xc0);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            else if (cp < 0x10000) {              // three octets
                *(result++) = static_cast<uint8_t>((cp >> 12)           | 0xe0);
                *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            else {                                // four octets
                *(result++) = static_cast<uint8_t>((cp >> 18)           | 0xf0);
                *(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f)  | 0x80);
                *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            //return result;
            return LD::CreateResponse(LD::Type<octet_iterator>{},octet_iterator{result},LD::Forward<Args>(arguments)...);
        }
        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<octet_iterator(Args...)> Append(uint32_t cp, octet_iterator  result, Args && ... arguments) noexcept
        {
            //todo add error handling for when the iterator is out of bounds
            if (!utf8::internal::is_code_point_valid(cp))
                return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);

            if (cp < 0x80)                        // one octet
                *(result++) = static_cast<uint8_t>(cp);
            else if (cp < 0x800) {                // two octets
                *(result++) = static_cast<uint8_t>((cp >> 6)            | 0xc0);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            else if (cp < 0x10000) {              // three octets
                *(result++) = static_cast<uint8_t>((cp >> 12)           | 0xe0);
                *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            else {                                // four octets
                *(result++) = static_cast<uint8_t>((cp >> 18)           | 0xf0);
                *(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f)  | 0x80);
                *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
                *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            }
            //return result;
            return LD::MakeContext(LD::TransactionResult{},octet_iterator{result},LD::Forward<Args>(arguments)...);
        }



        template <typename octet_iterator, typename output_iterator, typename ... Args>
        LD::QueryResult<output_iterator(Args...)> ReplaceInvalid(
                octet_iterator start,
                octet_iterator end,
                output_iterator out,
                uint32_t replacement, Args && ... arguments) noexcept
        {

            bool shouldContinue = true;
            while (start != end && shouldContinue)
            {
                octet_iterator sequence_start = start;
                utf8::internal::utf_error err_code = utf8::internal::validate_next(start, end);
                //LD::QueryResult<octet_iterator(Args...)>
                switch (err_code)
                {
                    case utf8::internal::UTF8_OK :
                        for (octet_iterator it = sequence_start; it != start; ++it)
                            *out++ = *it;
                        break;
                    case utf8::internal::NOT_ENOUGH_ROOM:
                    {
                        //throw not_enough_room();
                        return LD::QueryResult<output_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                    }
                    case utf8::internal::INVALID_LEAD:
                    {
                        //todo incorporate append from the previous declaration
                        //out = utf8::append (replacement, out);
                        ++start;
                        break;
                    }
                    case utf8::internal::INCOMPLETE_SEQUENCE:
                    {
                        [[fallthrough]];
                    }
                    case utf8::internal::OVERLONG_SEQUENCE:
                    {
                        [[fallthrough]];
                    }
                    case utf8::internal::INVALID_CODE_POINT:
                    {
                        LD::QueryResult<octet_iterator(octet_iterator&,octet_iterator&,octet_iterator&)> appendResult = LD::UTF8::Append(replacement,out,start,end,out);

                        auto onAppend = [](const LD::Context<LD::TransactionResult,octet_iterator,octet_iterator&,octet_iterator&,octet_iterator&> & context) noexcept
                        {
                            octet_iterator & start = LD::Get(LD::Get<2>(context));
                            octet_iterator & end = LD::Get(LD::Get<3>(context));
                            LD::Get(LD::Get<4>(context)) = LD::Get(LD::Get<1>(context));
                            ++start;
                            // just one replacement mark for the sequence
                            while (start != end && utf8::internal::is_trail(*start))
                                ++start;
                            return true;
                        };

                        auto onError = [](const LD::Context<LD::TransactionError,octet_iterator&,octet_iterator&,octet_iterator&> & context) noexcept
                        {
                            return false;
                        };
                        //todo incorporate append from the previous declaration
                        //out = utf8::append (replacement, out);
                        shouldContinue = LD::Match(appendResult,onAppend,onError);
                        break;
                    }
                }
            }
            LD::QueryResult<output_iterator(Args...)> res[2];
            res[0] = LD::QueryResult<output_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            res[1] = LD::QueryResult<output_iterator(Args...)>{LD::TransactionResult{},output_iterator{out},LD::Forward<Args>(arguments)...};
            return res[shouldContinue];
        }

        /* todo - implement this functionality when replace_invalid has been completed
         * template <typename octet_iterator, typename output_iterator>
    inline output_iterator replace_invalid(octet_iterator start, octet_iterator end, output_iterator out)
    {
        static const uint32_t replacement_marker = utf8::internal::mask16(0xfffd);
        return utf8::replace_invalid(start, end, out, replacement_marker);
    }
         */



        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<uint32_t(Args...)> Next(octet_iterator& it, octet_iterator end, Args && ... arguments) noexcept
        {
            uint32_t cp = 0;
            utf8::internal::utf_error err_code = utf8::internal::validate_next(it, end, cp);
            switch (err_code) {
                case utf8::internal::UTF8_OK :
                    break;
                case utf8::internal::NOT_ENOUGH_ROOM :
                    return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};//throw not_enough_room();
                case utf8::internal::INVALID_LEAD :
                    [[fallthrough]];
                case utf8::internal::INCOMPLETE_SEQUENCE :
                    [[fallthrough]];
                case utf8::internal::OVERLONG_SEQUENCE :
                    return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //throw invalid_utf8(*it);
                case utf8::internal::INVALID_CODE_POINT :
                    return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //return {};
                    //return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};//throw invalid_code_point(cp);
            }
            return LD::MakeContext(LD::TransactionResult{},uint32_t {cp},LD::Forward<Args>(arguments)...);
            //return cp;
        }

        template<typename octet_iterator, typename ... Args>
        LD::RequestResponse<uint32_t(Args...)> NextOctet(octet_iterator & it, octet_iterator end, Args && ... arguments) noexcept
        {
            uint32_t cp = 0;
            utf8::internal::utf_error err_code = utf8::internal::validate_next(it, end, cp);
            switch (err_code) {
                case utf8::internal::UTF8_OK :
                    break;
                case utf8::internal::NOT_ENOUGH_ROOM :
                    return LD::CreateResponse(LD::Type<uint32_t >{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};//throw not_enough_room();
                case utf8::internal::INVALID_LEAD :
                    [[fallthrough]];
                case utf8::internal::INCOMPLETE_SEQUENCE :
                    [[fallthrough]];
                case utf8::internal::OVERLONG_SEQUENCE :
                    return LD::CreateResponse(LD::Type<uint32_t >{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //throw invalid_utf8(*it);
                case utf8::internal::INVALID_CODE_POINT :
                    return LD::CreateResponse(LD::Type<uint32_t>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
                    //return {};
                    //return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};//throw invalid_code_point(cp);
            }
            return LD::CreateResponse(LD::Type<uint32_t>{},uint32_t {cp},LD::Forward<Args>(arguments)...);
        }


        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<uint32_t(Args...)> PeekNext(octet_iterator it, octet_iterator end, Args && ... arguments) noexcept
        {
            return LD::UTF8::Next(it,end,LD::Forward<Args>(arguments)...);
        }

        template<typename octet_iterator, typename ... Args>
        LD::RequestResponse<uint32_t(Args...)> PeekNextOctet(octet_iterator it, octet_iterator end, Args && ... arguments) noexcept
        {
            return LD::UTF8::NextOctet(it,end,LD::Forward<Args>(arguments)...);
        }

        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<uint32_t(Args...)> Prior(octet_iterator& it, octet_iterator start, Args && ... arguments) noexcept
        {
            // can't do much if it == start
            if (it == start)
            {
                return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                //throw not_enough_room();
            }


            octet_iterator end = it;
            // Go back until we hit either a lead octet or start
            while (utf8::internal::is_trail(*(--it)))
            {
                if (it == start)
                {
                    //throw invalid_utf8(*it); // error - no lead byte in the sequence
                    return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                }
            }
            return LD::UTF8::PeekNext(it, end,LD::Forward<Args>(arguments)...);
        }
        template <typename octet_iterator, typename ... Args>
        LD::RequestResponse<uint32_t(Args...)> PriorOctet(octet_iterator it, octet_iterator start, Args && ... arguments) noexcept
        {
            if (it == start)
            {
                return LD::CreateResponse(LD::Type<uint32_t>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
            }
        }

        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<uint32_t(Args...)> Previous(octet_iterator& it, octet_iterator pass_start, Args && ... arguments) noexcept
        {
            octet_iterator end = it;
            while (utf8::internal::is_trail(*(--it)))
            {
                if (it == pass_start)
                {
                    //throw invalid_utf8(*it); // error - no lead byte in the sequence
                    return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                }

            }

            octet_iterator temp = it;
            return LD::UTF8::Next(temp,end,LD::Forward<Args>(arguments)...);
        }

        template<typename octet_iterator, typename ... Args>
        LD::RequestResponse<uint32_t(Args...)> PreviousOctet(octet_iterator it, octet_iterator pass_start, Args && ... arguments) noexcept
        {
            octet_iterator end = it;
            while (utf8::internal::is_trail(*(--it)))
            {
                if (it == pass_start)
                {
                    //throw invalid_utf8(*it); // error - no lead byte in the sequence
                    //return LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                    return LD::CreateResponse(LD::Type<uint32_t>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
                }

            }
            octet_iterator temp = it;
            return LD::UTF8::NextOctet(temp,end,LD::Forward<Args>(arguments)...);
        }


        template <typename octet_iterator, typename distance_type, typename ... Args>
        LD::QueryResult<uint32_t(Args...)> Advance (octet_iterator& it, distance_type n, octet_iterator end, Args && ... arguments) noexcept
        {
            LD::QueryResult<uint32_t(Args...)> resultant = LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            for (distance_type i = 0; i < n; ++i)
            {
                resultant = LD::UTF8::Next(it, end,LD::Forward<Args>(arguments)...);
            }
            return resultant;
        }
        template<typename octet_iterator, typename distance_type, typename ... Args>
        LD::RequestResponse<uint32_t (Args...)> AdvanceOctet(octet_iterator it, distance_type n, octet_iterator end, Args && ... arguments) noexcept
        {
            //LD::RequestResponse<uint32_t(Args...)> resultant = LD::QueryResult<uint32_t(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            LD::RequestResponse<uint32_t (Args...)> resultant = LD::CreateResponse(LD::Type<uint32_t>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
            for (distance_type i = 0; i < n; ++i)
            {
                resultant = LD::UTF8::NextOctet(it, end,LD::Forward<Args>(arguments)...);
            }
            return resultant;
        }

        template <typename octet_iterator, typename ... Args>
        LD::QueryResult<LD::UInteger (Args...)> Distance (octet_iterator first, octet_iterator last, Args && ... arguments) noexcept
        {
            LD::UInteger dist = {};
            LD::QueryResult<LD::UInteger(Args...)> result = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            bool shouldContinue = true;
            for (dist = 0; first < last && shouldContinue; ++dist)
            {
                LD::QueryResult<uint32_t()> it = LD::UTF8::Next(first,last);

                auto onFoundCharacter = [](const LD::Context<LD::TransactionResult,uint32_t> & context) noexcept
                {
                    return true;
                };

                auto onTransactionError = [](const LD::Context<LD::TransactionError> & context) noexcept
                {
                    return false;
                };

                shouldContinue = LD::Match(it,onFoundCharacter,onTransactionError);
                //LD::UTF8:::Next(first, last,LD::Forward<Args>(arguements)...);
            }
            LD::QueryResult<LD::UInteger(Args...)> possibilities[2];
            possibilities[0] = LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguments)...);
            possibilities[1] = LD::MakeContext(LD::TransactionResult{},LD::UInteger{dist},LD::Forward<Args>(arguments)...);;
            return possibilities[shouldContinue];
        }

        template<typename octet_iterator, typename ... Args>
        LD::RequestResponse<LD::UInteger (Args...)> DistanceOfOctets(octet_iterator first, octet_iterator last, Args && ... arguments) noexcept
        {
            LD::UInteger dist = {};
            LD::RequestResponse<LD::UInteger (Args...)> result = LD::CreateResponse(LD::Type<LD::UInteger>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
            bool shouldContinue = true;
            for(dist = 0;first<last && shouldContinue;++dist)
            {

                LD::RequestResponse<uint32_t ()> it = LD::UTF8::NextOctet(first,last);
                auto onFoundCharacter = [](uint32_t character) noexcept
                {
                    return true;
                };

                auto onTransactionError = [](LD::TransactionError error) noexcept
                {
                    return false;
                };
                shouldContinue = LD::InvokeVisitation(LD::Overload{onFoundCharacter,onTransactionError},it);
            }
            LD::RequestResponse<LD::UInteger (Args...)> possibilities[2];
            possibilities[0] = LD::CreateResponse(LD::Type<LD::UInteger>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
            possibilities[1] = LD::CreateResponse(LD::Type<LD::UInteger>{},dist,LD::Forward<Args>(arguments)...);
            return possibilities[shouldContinue];
        }

        template <typename u16bit_iterator, typename octet_iterator, typename ... Args>
        LD::QueryResult<octet_iterator(Args...)> UTF16To8 (u16bit_iterator start, u16bit_iterator end, octet_iterator result, Args && ... arguments) noexcept
        {
            LD::QueryResult<octet_iterator(Args...)> res = LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            bool shouldContinue = true;
            while (start != end && shouldContinue)
            {
                uint32_t cp = utf8::internal::mask16(*start++);
                // Take care of surrogate pairs first
                if (utf8::internal::is_lead_surrogate(cp))
                {
                    if (start != end)
                    {
                        uint32_t trail_surrogate = utf8::internal::mask16(*start++);
                        if (utf8::internal::is_trail_surrogate(trail_surrogate))
                            cp = (cp << 10) + trail_surrogate + utf8::internal::SURROGATE_OFFSET;
                        else
                        {
                            res =  LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                            shouldContinue = false;
                            //throw invalid_utf16(static_cast<uint16_t>(trail_surrogate));
                        }
                    }
                    else
                    {
                        res =  LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                        shouldContinue = false;
                        //throw invalid_utf16(static_cast<uint16_t>(cp));
                    }
                }
                // Lone trail surrogate
                else if (utf8::internal::is_trail_surrogate(cp))
                {
                    res =  LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                    LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
                    //throw invalid_utf16(static_cast<uint16_t>(cp));
                }else
                {
                    res = LD::UTF8::Append(cp,result,LD::Forward<Args>(arguments)...);
                }
                auto onTransaction = [](const LD::Context<LD::TransactionResult,octet_iterator> & ) noexcept
                {
                    return true;
                };

                auto onError = [](const LD::Context<LD::TransactionError> & ) noexcept
                {
                    return false;
                };

                shouldContinue = LD::Match(res,onTransaction,onError);
            }
            return res;
        }


        template <typename u16bit_iterator, typename octet_iterator, typename ... Args>
        LD::QueryResult<u16bit_iterator(Args...)> UTF8To16 (octet_iterator start, octet_iterator end, u16bit_iterator result, Args && ... arguments) noexcept
        {
            bool shouldContinue = true;
            while (start != end && shouldContinue)
            {
                //uint32_t cp = utf8::next(start, end);
                LD::QueryResult<uint32_t(u16bit_iterator&)> codePoint = LD::UTF8::Next(start,end,result);
                auto onCodePoint = [](const LD::Context<LD::TransactionResult,uint32_t,u16bit_iterator&> & context) noexcept
                {
                    uint32_t & cp = LD::Get(LD::Get<1>(context));
                    u16bit_iterator & result = LD::Get(LD::Get<2>(context));

                    if (cp > 0xffff)
                    {
                        //make a surrogate pair
                        *result++ = static_cast<uint16_t>((cp >> 10)   + utf8::internal::LEAD_OFFSET);
                        *result++ = static_cast<uint16_t>((cp & 0x3ff) + utf8::internal::TRAIL_SURROGATE_MIN);
                    }
                    else
                        *result++ = static_cast<uint16_t>(cp);
                    return true;
                };

                auto onError = [](const LD::Context<LD::TransactionError> & ) noexcept
                {
                    return false;
                };
                shouldContinue = LD::Match(codePoint,onCodePoint,onError);
            }
            LD::QueryResult<u16bit_iterator(Args...)> res[2];
            res[0] = LD::QueryResult<u16bit_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            res[1] = LD::QueryResult<u16bit_iterator(Args...)>{LD::TransactionResult{},result,LD::Forward<Args>(arguments)...};
            return res[shouldContinue];
        }

        template <typename octet_iterator, typename u32bit_iterator, typename ... Args>
        LD::QueryResult<octet_iterator(Args...)> UTF32To8 (u32bit_iterator start, u32bit_iterator end, octet_iterator result, Args && ... arguments) noexcept
        {
            bool shouldContinue = true;
            while (start != end && shouldContinue)
            {
                //LD::QueryResult<octet_iterator(Args...)>
                LD::QueryResult<octet_iterator(Args...)> it = LD::UTF8::Append((*start++),result);

                auto onAppend = [](const LD::Context<LD::TransactionResult,octet_iterator> & ) noexcept
                {
                    return true;
                };

                auto onError = [](const LD::Context<LD::TransactionError> & ) noexcept
                {
                    return false;
                };

                shouldContinue = LD::Match(it,onAppend,onError);
                //result = utf8::append(*(start++), result);
            }


            LD::QueryResult<octet_iterator(Args...)> res[2];
            res[0] = LD::QueryResult<octet_iterator(Args...)>{LD::TransactionError{},LD::Forward<Args>(arguments)...};
            res[1] = LD::QueryResult<octet_iterator(Args...)>{LD::TransactionResult{},octet_iterator{result},LD::Forward<Args>(arguments)...};
            return res[shouldContinue];
        }

        template <typename octet_iterator, typename u32bit_iterator, typename ... Args>
        LD::QueryResult<u32bit_iterator(Args...)> UTF8To32 (octet_iterator start, octet_iterator end, u32bit_iterator result, Args && ... arguments) noexcept
        {
            bool shouldContinue = true;
            while (start != end && shouldContinue)
            {
                //LD::QueryResult<uint32_t(Args...)>
                LD::QueryResult<uint32_t(u32bit_iterator&)> codePoint = LD::UTF8::Next(start,end,result);

                auto onCodePoint = [](const LD::Context<LD::TransactionResult,uint32_t,u32bit_iterator&> & context) noexcept
                {
                    u32bit_iterator & result = LD::Get(LD::Get<2>(context));
                    const uint32_t codePoint = LD::Get(LD::Get<1>(context));
                    (*result++) = codePoint;
                    return true;
                };

                auto onError = [](const LD::Context<LD::TransactionError,u32bit_iterator&> & context) noexcept
                {
                    return false;
                };

                shouldContinue = LD::Match(codePoint,onCodePoint,onError);
                //(*result++) = utf8::next(start, end);
            }


            return result;
        }

    }
}

namespace LD
{
    namespace UTF8
    {
        template<LD::UInteger N>
        constexpr LD::UInteger NumberOfBytes(const ctll::fixed_string<N> & lit) noexcept
        {
            LD::UInteger size = 0;
            //we don't want to count the null terminating character
            constexpr LD::UInteger Size = (N-1);
            for(LD::UInteger n = 0;n<Size;++n)
            {
                const wchar_t & cp = lit[n];
                if (cp < 0x80)
                {
                    size+=1;
                }
                else if (cp < 0x800)
                {                // two octets
                    size+=2;
                }
                else if (cp < 0x10000)
                {              // three octets
                    size+=3;
                }
                else
                {
                    size+=4;
                }
            }
            return size;
        }

        template<LD::UInteger N>
        constexpr LD::UInteger NumberOfBytes(const LD::ImmutableString<N> & lit) noexcept
        {
            LD::UInteger size = 0;
            //we don't want to count the null terminating character
            constexpr LD::UInteger Size = (N-1);
            for(LD::UInteger n = 0;n<Size;++n)
            {
                const char & cp = lit[n];
                if (cp < 0x80)
                {
                    size+=1;
                }
                else if (cp < 0x800)
                {                // two octets
                    size+=2;
                }
                else if (cp < 0x10000)
                {              // three octets
                    size+=3;
                }
                else
                {
                    size+=4;
                }
            }
            return size;
        }

        template<char... C>
        constexpr LD::UInteger NumberOfBytes(const LD::TypeString<C...> & lit) noexcept
        {
            constexpr LD::UInteger N = sizeof...(C);
            LD::UInteger size = 0;
            //we don't want to count the null terminating character
            constexpr LD::UInteger Size = (N);
            for(LD::UInteger n = 0;n<Size;++n)
            {
                const char & cp = lit[n];
                if (cp < 0x80)
                {
                    size+=1;
                }
                else if (cp < 0x800)
                {                // two octets
                    size+=2;
                }
                else if (cp < 0x10000)
                {              // three octets
                    size+=3;
                }
                else
                {
                    size+=4;
                }
            }
            return size;
        }

        template<LD::UInteger N>
        inline auto Begin(const LD::StaticArray<char,N> & str) noexcept
        {
            return (char*)str.GetData();
        }
        template<LD::UInteger N>
        inline auto End(const LD::StaticArray<char,N> & str) noexcept
        {
            return (char*)str.GetData()+str.GetSize();
        }
        template<LD::UInteger N>
        inline auto Begin(const ctll::fixed_string<N> & str) noexcept
        {
            return (char*)str.begin();
        }

        template<LD::UInteger N>
        inline auto End(const ctll::fixed_string<N> & str) noexcept
        {
            return (char*)str.content+str.size();
        }

        template<LD::UInteger N>
        inline auto Begin(const LD::ImmutableString<N> & str) noexcept
        {
            return (char*)str.Begin();
        }

        template<LD::UInteger N>
        inline auto End(const LD::ImmutableString<N> & str) noexcept
        {
            return (char*)str.Data()+str.GetSize();
        }


        template<char ... C>
        inline auto Begin(LD::TypeString<C...> ) noexcept
        {
            return LD::TypeString<C...>::cbegin();
        }


        template<char ... C>
        inline auto End(LD::TypeString<C...> ) noexcept
        {
            return LD::TypeString<C...>::cend();
        }

        inline auto Begin(LD::StringView  view) noexcept
        {
            return view.begin();
        }

        inline auto End(LD::StringView  view) noexcept
        {
            return view.end();
        }

    }

}
#endif //LANDESSDEVCORE_UTF8_HPP
