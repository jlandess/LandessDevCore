//
// Created by phoenixflower on 2/25/21.
//

#ifndef LANDESSDEVCORE_BASE64ENCODING_H
#define LANDESSDEVCORE_BASE64ENCODING_H


namespace LD
{
    static inline bool IsBase64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
}



namespace LD
{
    template<LD::UInteger Size>
    LD::ImmutableString<4*(Size/3)+1> Base64Encode(LD::ImmutableString<Size> input) noexcept
    {


        const static const char * base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";
        unsigned char const* bytes_to_encode = (unsigned char *)input.Data();
        LD::ImmutableString<4*(Size/3)+1> ret;
        LD::BackInserter inserter{ret};
        LD::UInteger in_len = input.GetSize();
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    inserter = base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                inserter = base64_chars[char_array_4[j]];

            while((i++ < 3))
                inserter =  '=';

        }
        inserter = '\0';
        return ret;
    }
}

/*
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}
 */

namespace LD
{
    template<LD::UInteger Size>
    LD::ImmutableString<((Size-1)*3)/4> Base64Decode(LD::ImmutableString<Size> input) noexcept
    {
        const static const char * base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";
        auto InternalBase64DecodeFind = [](const char * input, unsigned char b)
        {
            for(LD::UInteger n = 0;n<26*2 + 10+2;++n)
            {
                if(input[n] == b)
                {
                    return n;
                }
            }
            return LD::UInteger {0};
        };
        LD::ImmutableString<((Size-1)*3)/4> ret;
        LD::BackInserter inserter{ret};

        int in_len = input.GetSize();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];

        unsigned char * encoded_string = (unsigned char*)input.Data();
        while (in_len-- && ( encoded_string[in_] != '=') && LD::IsBase64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_]; in_++;
            if (i ==4) {
                for (i = 0; i <4; i++)
                    char_array_4[i] = InternalBase64DecodeFind(base64_chars,char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    inserter = char_array_3[i];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = InternalBase64DecodeFind(base64_chars,char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) inserter = char_array_3[j];
        }
        return ret;
    }
}
/*
std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}
 */


template<LD::UInteger BitCount>
LD::StaticArray<char,BitCount*3> str_to_hex(LD::StringView hexView, LD::CompileTimeIndex<BitCount>) {
    std::string ssir = "DNQwSinfOUSSWd+U04r23A==";
    static const char *hex = "0123456789ABCDEF";
    //std::string result;
    LD::StaticArray<char,BitCount*3> immutableResult;
    LD::BackInserter<LD::StaticArray<char,BitCount*3>> backInserter{immutableResult};
    for (auto i = hexView.begin(), end = hexView.end(); i != end; ++i) {
        if (i != hexView.begin())
            backInserter = ':';
        backInserter = hex[*i >> 4];
        backInserter = hex[*i & 0xf];;
    }
    //return result;
    return immutableResult;
}
/**
 *     std::cout << str_to_hex(LD::StringView{"0123456789ABCDEF"},LD::CompileTimeIndex<1024>{}).GetData() << std::endl;
    std::cout << base64_encode(reinterpret_cast<const unsigned char *>("the wheels"), strlen("the wheels")) << std::endl;
 */
#endif //LANDESSDEVCORE_BASE64ENCODING_H
