//
// Created by phoenixflower on 5/14/20.
//

#ifndef LANDESSDEVCORE_STRINGTOPRIMITIVE_HPP
#define LANDESSDEVCORE_STRINGTOPRIMITIVE_HPP
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
namespace LD
{
    float atof(const char *s) noexcept
    {
        int f, m, sign, d=1;
        f = m = 0;

        sign = (s[0] == '-') ? -1 : 1;
        if (s[0] == '-' || s[0] == '+') s++;

        for (; *s != '.' && *s; s++) {
            f = (*s-'0') + f*10;
        }
        if (*s == '.')
            for (++s; *s; s++) {
                m = (*s-'0') + m*10;
                d *= 10;
            }
        return sign*(f + (float)m/d);
    }

    double atod(const char *s) noexcept
    {
        long f, m, sign, d=1;
        f = m = 0;
        sign = (s[0] == '-') ? -1 : 1;

        if (s[0] == '-' || s[0] == '+') s++;

        for (; *s != '.' && *s; s++)
        {
            f = (*s-'0') + f*10;
        }
        if (*s == '.')
        {
            for (++s; *s; s++)
            {
                m = (*s-'0') + m*10;
                d *= 10;
            }
        }
        return sign*(f + (double )m/d);
    }

    // A simple atoi() function
    int atoi (const char *s) noexcept
    {
        long r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;  // no conversion could be performed
        while (*s && isspace (*s))
            ++s;// get past initial spaces
        if (*s == '\0')
            return 0;  // no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;  //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (INT_MAX - d) / 10)
            {
                //out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? INT_MAX : INT_MIN;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (int) r : (int) (-r);
    }

    unsigned  int atoui (const char *s) noexcept
    {
        unsigned long r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;  // no conversion could be performed
        while (*s && isspace (*s))
            ++s;// get past initial spaces
        if (*s == '\0')
            return 0;  // no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;  //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (UINT_MAX - d) / 10)
            {
                //out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? UINT_MAX : 0;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (unsigned int) r : (unsigned int) (r);
    }

    long atol (const char *s) noexcept
    {
        long r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;   //no conversion could be performed
        while (*s && isspace (*s))
            ++s;  //get past initial spaces
        if (*s == '\0')
            return 0;   //no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;   //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (LONG_MAX - d) / 10)
            {
                 //out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? LONG_MAX : LONG_MIN;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (long) r : (long) (-r);
    }

    unsigned long atoul (const char *s) noexcept
    {
        long r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;   //no conversion could be performed
        while (*s && isspace (*s))
            ++s;  //get past initial spaces
        if (*s == '\0')
            return 0;   //no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;   //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (LONG_MAX - d) / 10)
            {
                //out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? LONG_MAX : LONG_MIN;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (long) r : (long) (r);
    }

    long long int atoll (const char *s) noexcept
    {
        long long int r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;   //no conversion could be performed
        while (*s && isspace (*s))
            ++s;  //get past initial spaces
        if (*s == '\0')
            return 0;   //no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;   //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (LONG_LONG_MAX - d) / 10)
            {
                // out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? LONG_LONG_MAX : LONG_LONG_MIN;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (long long int) r : (long long int) (-r);
    }

    unsigned long long int atoull (const char *s)
    {
        unsigned long long int r = 0L, d;
        char sign = '+';

        if (s == 0)
            return 0;   //no conversion could be performed
        while (*s && isspace (*s))
            ++s;  //get past initial spaces
        if (*s == '\0')
            return 0;   //no conversion could be performed
        sign = (*s == '+' || *s == '-') ? *s++ : '+';
        if (isdigit (*s) == 0)
            return 0;   //no conversion could be performed
        while (isdigit (*s))
        {
            d = *s - '0';
            if (r > (ULONG_LONG_MAX - d) / 10)
            {
                // out of range for an integer
                errno = ERANGE;
                return (sign == '+') ? ULONG_LONG_MAX : 0;
            }
            else
                r = r * 10L + d;
        }
        return (sign == '+') ? (unsigned long long int) r : (unsigned long long int) (r);
    }


    template<typename T, class = void>
    class StringToPrimitive;

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,char>>>>
    {
    public:

        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {

                return {};
            }

            return {view[0]};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,int>>>>
    {
    public:

        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }

            return {atoi(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,long>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }

            return {atol(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,long long>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }


            return {atoll(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,float>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }


            return {LD::atof(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,double>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }
            return {LD::atod(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,unsigned int>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }
            return {LD::atoui(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,unsigned long>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }
            return {std::atol(view.data())};
            //return {LD::atoul(view.data())};
            //return {LD::atol(view.data())};
        }
    };

    template<typename T>
    class StringToPrimitive<T,LD::Enable_If_T<LD::Require<LD::IsSame<T,unsigned long long int>>>>
    {
    public:
        LD::Variant<LD::NullClass,T> operator()(const LD::StringView & view) noexcept
        {
            if (view.size() == 0 && view.data() == nullptr)
            {
                return {};
            }
            return {LD::atoull(view.data())};
        }
    };

}
#endif //LANDESSDEVCORE_STRINGTOPRIMITIVE_HPP
