//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_LIMIT_HPP
#define LANDESSDEVCORE_LIMIT_HPP

#include <climits>
#include <cfloat>
namespace LD
{
    template<typename T>
    class Limit;

    template<>
    class Limit<short>
    {
    public:


        static inline constexpr short GetMin() noexcept
        {
            return SHRT_MIN;
        }


        static inline constexpr short GetMax() noexcept
        {
            return SHRT_MAX;
        }
    };

    template<>
    class Limit<unsigned short>
    {
    public:


        static inline constexpr unsigned short GetMin() noexcept
        {
            return 0;
        }


        static inline constexpr unsigned short GetMax() noexcept
        {
            return USHRT_MAX;
        }
    };



    template<>
    class Limit<int>
    {
    public:


        static inline constexpr int GetMin() noexcept
        {
            return INT_MIN;
        }


        static inline constexpr int GetMax() noexcept
        {
            return INT_MAX;
        }
    };


    template<>
    class Limit<unsigned int>
    {
    public:


        static inline constexpr unsigned int GetMin() noexcept
        {
            return 0;
        }


        static inline constexpr unsigned int GetMax() noexcept
        {
            return UINT_MAX;
        }
    };


    template<>
    class Limit<long>
    {
    public:


        static inline constexpr long GetMin() noexcept
        {
            return LONG_MIN;
        }


        static inline constexpr long GetMax() noexcept
        {
            return LONG_MAX;
        }
    };




    template<>
    class Limit<unsigned long>
    {
    public:


        static inline constexpr unsigned long GetMin() noexcept
        {
            return 0;
        }


        static inline constexpr unsigned long GetMax() noexcept
        {
            return ULONG_MAX;
        }
    };


    template<>
    class Limit<long long int>
    {
    public:


        static inline constexpr long long int GetMin() noexcept
        {
            return LLONG_MIN;
        }


        static inline constexpr long long int GetMax() noexcept
        {
            return  LLONG_MAX;
        }
    };


    template<>
    class Limit<unsigned long long int>
    {
    public:


        static inline constexpr unsigned long long int GetMin() noexcept
        {
            return 0;
        }


        static inline constexpr unsigned long long int GetMax() noexcept
        {
            return  ULLONG_MAX;
        }
    };


    template<>
    class Limit<float>
    {
    public:


        static inline constexpr float GetMin() noexcept
        {
            return 1.17549e-38;
        }


        static inline constexpr float GetMax() noexcept
        {
            return 3.40282e+38;
        }
    };


    template<>
    class Limit<double>
    {
    public:


        static inline constexpr double GetMin() noexcept
        {
            return DBL_MIN;
        }


        static inline constexpr double GetMax() noexcept
        {
            return DBL_MAX;
        }
    };

    template<>
    class Limit<long double>
    {
    public:


        static inline constexpr long double GetMin() noexcept
        {

            return LDBL_MIN;
        }


        static inline constexpr long double GetMax() noexcept
        {

            return LDBL_MAX;
        }
    };
    namespace Detail
    {

    }
}
#endif //LANDESSDEVCORE_LIMIT_HPP
