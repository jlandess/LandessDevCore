//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_LIMIT_HPP
#define LANDESSDEVCORE_LIMIT_HPP

#include <climits>
namespace LD
{
    namespace Detail
    {
        template<typename T>
        class Limit;

        template<>
        class Limit<short>
        {
        public:


            static inline short GetMin()
            {
                return SHRT_MIN;
            }


            static inline short GetMax()
            {
                return SHRT_MAX;
            }
        };

        template<>
        class Limit<unsigned short>
        {
        public:


            static inline unsigned short GetMin()
            {
                return 0;
            }


            static inline unsigned short GetMax()
            {
                return USHRT_MAX;
            }
        };



        template<>
        class Limit<int>
        {
        public:


            static inline int GetMin()
            {
                return INT_MIN;
            }


            static inline int GetMax()
            {
                return INT_MAX;
            }
        };


        template<>
        class Limit<unsigned int>
        {
        public:


            static inline unsigned int GetMin()
            {
                return 0;
            }


            static inline unsigned int GetMax()
            {
                return UINT_MAX;
            }
        };


        template<>
        class Limit<long>
        {
        public:


            static inline long GetMin()
            {
                return LONG_MIN;
            }


            static inline long GetMax()
            {
                return LONG_MAX;
            }
        };




        template<>
        class Limit<unsigned long>
        {
        public:


            static inline unsigned long GetMin()
            {
                return 0;
            }


            static inline unsigned long GetMax()
            {
                return ULONG_MAX;
            }
        };


        template<>
        class Limit<long long int>
        {
        public:


            static inline long long int GetMin()
            {
                return LLONG_MIN;
            }


            static inline long long int GetMax()
            {
                return  LLONG_MAX;
            }
        };


        template<>
        class Limit<unsigned long long int>
        {
        public:


            static inline unsigned long long int GetMin()
            {
                return 0;
            }


            static inline unsigned long long int GetMax()
            {
                return  ULLONG_MAX;
            }
        };


        template<>
        class Limit<float>
        {
        public:


            static inline float GetMin()
            {
                return 1.17549e-38;
            }


            static inline float GetMax()
            {
                return 3.40282e+38;
            }
        };


        template<>
        class Limit<double>
        {
        public:


            static inline double GetMin()
            {
                return 2.22507e-308;
            }


            static inline double GetMax()
            {
                return 1.79769e+308;
            }
        };

        template<>
        class Limit<long double>
        {
        public:


            static inline long double GetMin()
            {

                return 4.9406564584124654E-324;
            }


            static inline long double GetMax()
            {
                return 1.7976931348623157E+308;
            }
        };
    }
}
#endif //LANDESSDEVCORE_LIMIT_HPP
