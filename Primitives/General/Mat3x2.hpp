//
//  Mat3x2.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat3x2_h
#define DataStructures_Mat3x2_h

#include "StaticArray.hpp"
#include "Vec2.hpp"
#include "Mat2x3.hpp"
#include "Mat2x2.hpp"
#include "TypeTraits/StaticallySized.h"


namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tMat3x2
        {
        private:
            tVec2<T> Rows[3];
        public:
            inline tMat3x2();
            inline tMat3x2(const typename TypeAid<T, 6>::CoreType & a);
            inline tMat3x2(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,
                           const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,
                           const typename TypeAid<T, 6>::CoreType & a2, const typename TypeAid<T, 6>::CoreType & b2);


            inline tMat3x2(const tVec2<T> & a, const tVec2<T> & b, const tVec2<T> & c);

            inline tMat3x2(const typename TypeAid<T, 6>::SinglePrecision & a);
            inline tMat3x2(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,
                           const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,
                           const typename TypeAid<T, 6>::SinglePrecision & a2, const typename TypeAid<T, 6>::SinglePrecision & b2);



            inline tMat3x2 & operator = (const typename TypeAid<T, 6>::SinglePrecision & a );

            inline tMat3x2 & operator = (const typename TypeAid<T, 6>::CoreType & a );



            inline tVec2<T> & operator [](const PDP::UInteger & index);
            inline const tVec2<T> & operator [](const PDP::UInteger & index) const;


        };


        template<typename T>
        tMat3x2<T>::tMat3x2()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] =  0;
        }


        template<typename T>
        tMat3x2<T>::tMat3x2(const typename TypeAid<T, 6>::CoreType & a)
        {
            this->Rows[0] = tVec3<T>(a,0);
            this->Rows[1] = tVec3<T>(0,0);
            this->Rows[2] = tVec3<T>(0,a);
        }


        template<typename T>
        tMat3x2<T>::tMat3x2(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,
                            const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,
                            const typename TypeAid<T, 6>::CoreType & a2, const typename TypeAid<T, 6>::CoreType & b2)
        {
            this->Rows[0][0] = a0;
            this->Rows[0][1] = b0;

            this->Rows[1][0] = a1;
            this->Rows[1][1] = b1;

            this->Rows[2][0] = a2;
            this->Rows[2][1] = b2;
        }

        template<typename T>
        tMat3x2<T>::tMat3x2(const tVec2<T> & a, const tVec2<T> & b, const tVec2<T> &c )
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
        }

        template<typename T>
        tMat3x2<T>::tMat3x2(const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            this->Rows[0] = tVec3<T>(a,0);
            this->Rows[1] = tVec3<T>(0,a);
            this->Rows[2] = tVec3<T>(0,0);
        }

        template<typename T>
        tMat3x2<T>::tMat3x2(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,
                            const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,
                            const typename TypeAid<T, 6>::SinglePrecision & a2, const typename TypeAid<T, 6>::SinglePrecision & b2)
        {

            this->Rows[0] = tVec2<T>(a0,b0);
            this->Rows[1] = tVec2<T>(a1,b1);
            this->Rows[2] = tVec2<T>(a2,b2);

        }
        template<typename T>
        tMat3x2<T> & tMat3x2<T>::operator = (const typename TypeAid<T, 6>::SinglePrecision & a )
        {
            this->Rows[0] = tVec3<T>(a,0);
            this->Rows[1] = tVec3<T>(0,0);
            this->Rows[2] = tVec3<T>(0,a);
            return (*this);
        }
        template<typename T>
        tMat3x2<T> & tMat3x2<T>::operator = (const typename TypeAid<T, 6>::CoreType & a )
        {
            this->Rows[0] = tVec3<T>(a,0);
            this->Rows[1] = tVec3<T>(0,0);
            this->Rows[2] = tVec3<T>(0,a);

            return (*this);
        }



        template <typename T, typename U>
        inline tMat3x2<T> & operator+= (tMat3x2<T> & a, const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator+= (tMat3x2<T> & a,tMat3x2<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            a[2] += m[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator-= (tMat3x2<T> & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator-= (tMat3x2<T> & a,tMat3x2<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator*= (tMat3x2<T> & a,const typename TypeAid<T, 6>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator/= (tMat3x2<T> & a,const typename TypeAid<T, 6>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator++ (tMat3x2<T> & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> & operator-- (tMat3x2<T> & a)
        {
            --a[0];
            --a[1];
            --a[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator++(tMat3x2<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator--(tMat3x2<T> & a,int)
        {
            --a;
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat3x2<T> operator+
                (
                        tMat3x2<T> const & m,
                        typename TypeAid<T, 6>::CoreType const & s
                )
        {
            return tMat3x2<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator+
                (
                        tMat3x2<T> const & m1,
                        tMat3x2<U> const & m2
                )
        {
            return tMat3x2<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator-
                (
                        tMat3x2<T> const & m,
                        typename TypeAid<T, 6>::CoreType const & s
                )
        {
            return tMat3x2<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator-
                (
                        tMat3x2<T> const & m1,
                        tMat3x2<U> const & m2
                )
        {
            return tMat3x2<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator*
                (
                        tMat3x2<T> const & m,
                        typename TypeAid<T, 6>::CoreType const & s
                )
        {
            return tMat3x2<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator*
                (
                        typename TypeAid<T, 6>::CoreType const & s,
                        tMat3x2<U> const & m
                )
        {
            return tMat3x2<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tMat3x2<T> const & m,
                        tVec3<U> const & v)
        {
            return tVec2<T>(
                    m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2],
                    m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        tVec2<T> const & v,
                        tMat3x2<U> const & m)
        {
            return tVec3<T>(
                    v[0] * m[0][0] + v[1] * m[0][1],
                    v[0] * m[1][0] + v[1] * m[1][1],
                    v[0] * m[2][0] + v[1] * m[2][1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator*
                (
                        tMat3x2<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA20 = m1[2][0];
            const T & SrcA21 = m1[2][1];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB02 = m2[0][2];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB12 = m2[1][2];

            tMat2x2<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
            return Result;
        }






        template <typename T, typename U>
        inline tMat3x2<T> operator/
                (
                        tMat3x2<T> const & m,
                        typename TypeAid<U, 6>::CoreType const & s
                )
        {
            return tMat3x2<T>(
                    m[0] / s,
                    m[1] / s,
                    m[2] / s);
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator/
                (
                        typename TypeAid<T, 6>::CoreType const & s,
                        tMat3x2<U> const & m
                )
        {
            return tMat3x2<T>(
                    s / m[0],
                    s / m[1],
                    s / m[2]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat3x2<T> const operator-
                (
                        tMat3x2<T> const & m
                )
        {
            return tMat3x2<T>(
                    -m[0],
                    -m[1],
                    -m[2]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat3x2<T> const & m1,
                        tMat3x2<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat3x2<T> const & m1,
                        tMat3x2<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
        }


        template <typename T, typename U>
        inline tMat3x2<T> operator*
                (
                        tMat2x2<T> const & m1,
                        tMat3x2<U> const & m2
                )
        {
            return tMat3x2<U>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1]);
        }
    }
    typedef Detail::tMat3x2<PDP::UInteger> UMat3x2;
    typedef Detail::tMat3x2<PDP::Integer> IMat3x2;
    typedef Detail::tMat3x2<float> Mat3x2;
    typedef Detail::tMat3x2<double> DMat3x2;
    typedef Detail::tMat3x2<unsigned short> USMat2x4;
    typedef Detail::tMat3x2<short> SMat2x4;
    //typedef Detail::tMat2x4<PDP::Half> HMat2x4;

}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tMat3x2<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
