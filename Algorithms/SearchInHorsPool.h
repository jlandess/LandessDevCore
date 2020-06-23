//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_SEARCHINHORSPOOL_H
#define LANDESSDEVCORE_SEARCHINHORSPOOL_H
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    template<LD::UInteger Size>
    static constexpr LD::ImmutableArray<LD::UInteger,Size> CreateOccTable(const LD::ImmutableString<Size> & needle) noexcept
    {
        LD::ImmutableArray<unsigned char,Size> occ = {UCHAR_MAX+1};
        const LD::UInteger needle_length = needle.GetSize();
        //occtable_type occ(UCHAR_MAX+1, needle_length); // initialize a table of UCHAR_MAX+1 elements to value needle_length

        //Populate it with the analysis of the needle
       // But ignoring the last letter
        if(needle_length >= 1)
        {
            const size_t needle_length_minus_1 = needle_length-1;
            for(size_t a=0; a<needle_length_minus_1; ++a)
                occ[needle[a]] = needle_length_minus_1 - a;
        }
        return occ;
    }


    template<LD::UInteger Size>
    static constexpr LD::UInteger SearchInHorspool(const unsigned char* haystack, size_t haystack_length,
                            const LD::ImmutableArray<LD::UInteger,Size> & occ,
                            const LD::ImmutableString<Size> & needle) noexcept
    {
        const LD::UInteger needle_length = needle.GetSize();
        if(needle_length > haystack_length) return haystack_length;
        if(needle_length == 1)
        {

            const unsigned char* result = (const unsigned char*)memchr(haystack, *needle, haystack_length);
            return result ? size_t(result-haystack) : haystack_length;
        }

        const size_t needle_length_minus_1 = needle_length-1;

        const unsigned char last_needle_char = needle[needle_length_minus_1];

        size_t haystack_position=0;
        while(haystack_position <= haystack_length-needle_length)
        {
            const unsigned char occ_char = haystack[haystack_position + needle_length_minus_1];

            // The author modified this part. Original algorithm matches needle right-to-left.
            // This code calls memcmp() (usually matches left-to-right) after matching the last
            // character, thereby incorporating some ideas from
            // "Tuning the Boyer-Moore-Horspool String Searching Algorithm"
            // by Timo Raita, 1992.
            if(last_needle_char == occ_char
               && memcmp(needle, haystack+haystack_position, needle_length_minus_1) == 0)
            {
                return haystack_position;
            }

            haystack_position += occ[occ_char];
        }
        return haystack_length;
    }
}
#endif //LANDESSDEVCORE_SEARCHINHORSPOOL_H
