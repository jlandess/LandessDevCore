//
//  Vec2.h
//  DataStructures
//
//  Created by James Landess on 11/28/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

//
//  Vec2.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Vec2_h
#define DataStructures_Vec2_h

#include "StaticArray.hpp"
#include "Definitions/Float.hpp"
#include "Definitions/Integer.hpp"
#include "TypeTraits/StaticallySized.h"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tVec2
        {
        private:
            T InternalBuffer[2];
        public:

            typedef T Type;

            inline constexpr tVec2() noexcept ;
            inline constexpr tVec2(const tVec2 & vector) noexcept ;
            inline constexpr tVec2(const T & a) noexcept ;
            inline constexpr tVec2(const T & a, const T & b) noexcept ;
            //inline tVec2(const typename TypeAid<T, 2>::SinglePrecision & a, const typename TypeAid<T, 2>::SinglePrecision & b);
            inline constexpr tVec2 & operator = (const tVec2 & vector) noexcept ;
            inline constexpr tVec2 & operator = (const T & a ) noexcept ;
            inline constexpr tVec2 & operator = (const LD::StaticArray<T, 2> & a ) noexcept ;

            inline constexpr const T & X()  const noexcept{return (*this)[0];}
            inline constexpr T & X() noexcept {return (*this)[0];}

            inline constexpr const T & Y()  const noexcept{return (*this)[1];}
            inline constexpr T & Y() noexcept {return (*this)[1];}

            inline constexpr LD::Detail::tVec2<T> XX()  const noexcept ;
            inline constexpr LD::Detail::tVec2<T> XY()  const noexcept ;
            inline constexpr LD::Detail::tVec2<T> YY() const noexcept ;
            inline constexpr LD::Detail::tVec2<T> YX() const noexcept ;

            inline constexpr tVec2 & Normalize() noexcept
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T magnitude = LD::Sqrt(x*x + y*y);

                (*this)[0]/=magnitude;
                (*this)[1]/=magnitude;

                return (*this);

            }

            inline constexpr T & operator[](const PDP::UInteger & index) noexcept
            {
                return InternalBuffer[index];
            }

            inline constexpr const T & operator[](const PDP::UInteger & index) const noexcept
            {
                return InternalBuffer[index];
            }


            inline constexpr const tVec2 Normalized() const noexcept
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T magnitude = LD::Sqrt(x*x + y*y);

                tVec2 temp(*this);

                temp[0]/=magnitude;
                temp[1]/=magnitude;

                return temp;
            }


            inline  constexpr const T Magnitude() const noexcept
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T magnitude = LD::Sqrt(x*x + y*y);
                return magnitude;
            }
        };

        template<typename T>
        constexpr  tVec2<T>::tVec2() noexcept
        {
            (*this)[0] = (*this)[1] = 0;
        }

        template<typename T>
        constexpr  tVec2<T>::tVec2(const tVec2 & vector) noexcept
        {
            (*this) = vector;
        }

        template<typename T>
        constexpr tVec2<T>::tVec2(const T & a) noexcept
        {
            (*this)[0] = (*this)[1] = a;
        }



        template<typename T>
        constexpr tVec2<T>::tVec2(const T & a,const T & b) noexcept
        {
            (*this)[0] = a;
            (*this)[1] = b;
        }



        template<typename T>
        constexpr LD::Detail::tVec2<T> tVec2<T>::XX() const noexcept
        {
            return LD::Detail::tVec2<T>((*this)[0],(*this)[0]);
        }


        template<typename T>
        constexpr LD::Detail::tVec2<T> tVec2<T>::XY() const noexcept
        {
            return LD::Detail::tVec2<T>((*this)[0],(*this)[1]);
        }


        template<typename T>
        constexpr LD::Detail::tVec2<T> tVec2<T>::YY() const noexcept
        {
            return LD::Detail::tVec2<T>((*this)[1],(*this)[1]);
        }

        template<typename T>
        constexpr LD::Detail::tVec2<T> tVec2<T>::YX() const noexcept
        {
            return LD::Detail::tVec2<T>((*this)[1],(*this)[0]);
        }

        template<typename T>
        constexpr tVec2<T> & tVec2<T>::operator=(const tVec2<T> &vector) noexcept
        {
            (*this)[0] = vector[0];
            (*this)[1] = vector[1];
            return (*this);
        }

        template<typename T>
        constexpr tVec2<T> & tVec2<T>::operator=(const T & a) noexcept
        {
            (*this)[0] = (*this)[1] = a;
            return (*this);
        }






        template <typename T, typename U>
        inline constexpr tVec2<T> & operator+=
                (
                        tVec2<T> & a, const U & s
                ) noexcept
        {
            a[0] += s;
            a[1] += s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator+=
                (
                        tVec2<T> & a,tVec2<U> const & v
                ) noexcept
        {
            a[0] += v[0];
            a[1] += v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator-=
                (
                        tVec2<T> & a, const U & s
                ) noexcept
        {
            a[0] -= s;
            a[1] -= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator-=
                (
                        tVec2<T> & a,tVec2<U> const & v
                ) noexcept
        {
            a[0] -= v[0];
            a[0] -= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator*=
                (
                        tVec2<T> & a,const U & s
                ) noexcept
        {
            a[0] *= s;
            a[1] *= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator*=
                (
                        tVec2<T> & a,tVec2<U> const & v
                ) noexcept
        {
            a[0] *= v[0];
            a[1] *= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator/=
                (
                        tVec2<T> & a,const U & s
                ) noexcept
        {
            a[0] /= s;
            a[1] /= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator/=
                (
                        tVec2<T> & a,tVec2<U> const & v
                ) noexcept
        {
            a[0] /= v[0];
            a[0] /= v[1];
            return a;
        }

        //////////////////////////////////////
        // Increment and decrement operators

        template <typename T>
        inline constexpr tVec2<T> & operator++(tVec2<T> & a) noexcept
        {
            ++a[0];
            ++a[1];
            return a;
        }

        template <typename T>
        inline constexpr tVec2<T> & operator--(tVec2<T> & a) noexcept
        {
            --a[0];
            --a[1];
            return a;
        }

        template <typename T>
        inline constexpr tVec2<T> & operator++(tVec2<T> & a,int) noexcept
        {
            a[0]++;
            a[1]++;
            return a;
        }

        template <typename T>
        inline constexpr tVec2<T> & operator--(tVec2<T> & a,int) noexcept
        {
            a[0]--;
            a[1]--;
            return a;
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline constexpr bool operator==
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return (v1[0] == v2[0]) && (v1[1] == v2[1]);
        }

        template <typename T, typename U>
        inline constexpr bool operator!=
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return (v1[0] != v2[0]) || (v1[1] != v2[1]);
        }

        //////////////////////////////////////
        // Unary bit operators

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator%= (tVec2<T> & a, const U & s) noexcept
        {
            a[0]%=s;
            a[1]%=s;
            return s;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator%= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] %= v[0];
            a[1] %= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator&= (tVec2<T> & a, const U & s) noexcept
        {
            a[0] &= a;
            a[1] &= a;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator&= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] &= v[0];
            a[1] &= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator|= (tVec2<T> & a,const U & s) noexcept
        {
            a[0] |= s;
            a[1] |= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator|= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] |= v[0];
            a[1] |= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator^= (tVec2<T> & a,const U & s) noexcept
        {
            a[0] ^= s;
            a[1] ^= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator^= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] ^= v[0];
            a[1] ^= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator<<= (tVec2<T> & a, const U & s) noexcept
        {
            a[0] <<= s;
            a[1] <<= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator<<= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] <<= v[0];
            a[1] <<= v[1];
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator>>= (tVec2<T> & a, const U & s) noexcept
        {
            a[0] >>= s;
            a[1] >>= s;
            return a;
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> & operator>>= (tVec2<T> & a,tVec2<U> const & v) noexcept
        {
            a[0] >>= v[0];
            a[1] >>= v[1];
            return a;
        }

        //////////////////////////////////////
        // Binary arithmetic operators

        template <typename T, typename U>
        inline constexpr tVec2<T> operator+
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] + s,
                    v[1] + s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator+
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s + v[0],
                    s + v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator+
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] + v2[0],
                    v1[1] + v2[1]);
        }

        //operator-
        template <typename T, typename U>
        inline constexpr tVec2<T> operator-
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0]- s,
                    v[1] - s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator-
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s - v[0],
                    s - v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator-
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0]- v2[0],
                    v1[1] - v2[1]);
        }

        //operator*
        template <typename T, typename U>
        inline constexpr tVec2<T> operator*
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] * s,
                    v[1] * s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator*
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s * v[0],
                    s * v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator*
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] * v2[0],
                    v1[1] * v2[1]);
        }

        //operator/
        template <typename T, typename U>
        inline constexpr tVec2<T> operator/
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] / s,
                    v[1] / s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator/
                (
                        T const & s,
                        tVec2<U>const & v
                ) noexcept
        {
            return tVec2<T>(
                    s / v[0],
                    s / v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator/
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] / v2[0],
                    v1[1] / v2[1]);
        }

        // Unary constant operators
        template <typename T>
        inline constexpr tVec2<T> operator-
                (
                        tVec2<T> const & v
                ) noexcept
        {
            return tVec2<T>(
                    -v[0],
                    -v[1]);
        }

        //////////////////////////////////////
        // Binary bit operators

        template <typename T, typename U>
        inline constexpr tVec2<T> operator%
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] % s,
                    v[1] % s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator%
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s % v[0],
                    s % v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator%
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] % v2[0],
                    v1[1] % v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator&
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] & s,
                    v[1] & s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator&
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s & v[0],
                    s & v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator&
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] & v2[0],
                    v1[1] & v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator|
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] | s,
                    v[1] | s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator|
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s | v[0],
                    s | v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator|
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] | v2[0],
                    v1[1] | v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator^
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] ^ s,
                    v[1] ^ s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator^
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s ^ v[0],
                    s ^ v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator^
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                )  noexcept
        {
            return tVec2<T>(
                    v1[0] ^ v2[0],
                    v1[1] ^ v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator<<
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] << s,
                    v[0] << s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator<<
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s << v[0],
                    s << v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator<<
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] << v2[0],
                    v1[1] << v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator>>
                (
                        tVec2<T> const & v,
                        U const & s
                ) noexcept
        {
            return tVec2<T>(
                    v[0] >> s,
                    v[1] >> s);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator>>
                (
                        T const & s,
                        tVec2<U> const & v
                ) noexcept
        {
            return tVec2<T>(
                    s >> v[0],
                    s >> v[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator>>
                (
                        tVec2<T> const & v1,
                        tVec2<U> const & v2
                ) noexcept
        {
            return tVec2<T>(
                    v1[0] >> v2[0],
                    v1[1] >> v2[1]);
        }

        template <typename T, typename U>
        inline constexpr tVec2<T> operator~
                (
                        tVec2<T> const & v
                ) noexcept
        {
            return tVec2<T>(
                    ~v[0],
                    ~v[1]);
        }

    }
    typedef Detail::tVec2<LD::UInteger> UVec2;
    typedef Detail::tVec2<LD::Integer> IVec2;
    typedef Detail::tVec2<float> Vec2;
    typedef Detail::tVec2<double> DVec2;
    typedef Detail::tVec2<unsigned short> USVec2;
    typedef Detail::tVec2<short> SVec2;
}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tVec2<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }

    template<typename T>
    using BasicVec2D = LD::Detail::tVec2<T>;
}
#endif

