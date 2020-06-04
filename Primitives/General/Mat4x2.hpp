//
//  Mat4x2.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat4x2_h
#define DataStructures_Mat4x2_h

#include "StaticArray.hpp"
#include "Vec2.hpp"
#include "Mat2x4.hpp"
#include "Mat3x4.hpp"
#include "Mat3x2.hpp"
#include "TypeTraits/StaticallySized.h"

namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tMat4x2
        {
        private:
            tVec2<T> Rows[4];
        public:
            inline tMat4x2();
            inline tMat4x2(const typename TypeAid<T, 8>::CoreType & a);
            inline tMat4x2(const typename TypeAid<T ,8>::CoreType & a0, const typename TypeAid<T, 8>::CoreType & b0,
                           const typename TypeAid<T, 8>::CoreType & a1, const typename TypeAid<T, 8>::CoreType & b1,
                           const typename TypeAid<T, 8>::CoreType & a2, const typename TypeAid<T, 8>::CoreType & b2,
                           const typename TypeAid<T, 8>::CoreType & a3, const typename TypeAid<T, 8>::CoreType & b3);

            inline tMat4x2(const tVec2<T> & a, const tVec2<T> & b, const tVec2<T> & c, const tVec2<T> & d);

            inline tMat4x2(const typename TypeAid<T, 8>::SinglePrecision & a);
            inline tMat4x2(const typename TypeAid<T ,8>::SinglePrecision & a0, const typename TypeAid<T, 8>::SinglePrecision & b0,
                           const typename TypeAid<T, 8>::SinglePrecision & a1, const typename TypeAid<T, 8>::SinglePrecision & b1,
                           const typename TypeAid<T, 8>::SinglePrecision & a2, const typename TypeAid<T, 8>::SinglePrecision & b2,
                           const typename TypeAid<T, 8>::SinglePrecision & a3, const typename TypeAid<T, 8>::SinglePrecision & b3);

            inline tVec2<T> & operator [](const PDP::UInteger & index);
            inline const tVec2<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat4x2 & operator = (const typename TypeAid<T, 8>::CoreType & a );
            inline tMat4x2 & operator = (const typename TypeAid<T, 8>::SinglePrecision & a );

        };

        template<typename T>
        tMat4x2<T>::tMat4x2()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = this->Rows[3] = 0;
        }

        template<typename T>
        tMat4x2<T>::tMat4x2(const typename TypeAid<T, 8>::CoreType & a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,a);
            this->Rows[2] = tVec2<T>(0,0);
            this->Rows[3] = tVec2<T>(0,0);
        }


        template<typename T>
        tMat4x2<T>::tMat4x2(const typename TypeAid<T ,8>::CoreType & a0, const typename TypeAid<T, 8>::CoreType & b0,
                            const typename TypeAid<T, 8>::CoreType & a1, const typename TypeAid<T, 8>::CoreType & b1,
                            const typename TypeAid<T, 8>::CoreType & a2, const typename TypeAid<T, 8>::CoreType & b2,
                            const typename TypeAid<T, 8>::CoreType & a3, const typename TypeAid<T, 8>::CoreType & b3)
        {
            this->Rows[0] = tVec2<T>(a0,b0);
            this->Rows[1] = tVec2<T>(a1,b1);
            this->Rows[2] = tVec2<T>(a2,b2);
            this->Rows[3] = tVec2<T>(a3,b3);
        }

        template<typename T>
        tMat4x2<T>::tMat4x2(const tVec2<T> & a, const tVec2<T> & b, const tVec2<T> & c, const tVec2<T> & d)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
            this->Rows[3] = d;
        }

        template<typename T>
        tMat4x2<T>::tMat4x2(const typename TypeAid<T, 8>::SinglePrecision & a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,0);
            this->Rows[2] = tVec2<T>(0,0);
            this->Rows[3] = tVec2<T>(0,a);
        }


        template<typename T>
        tMat4x2<T>::tMat4x2(const typename TypeAid<T ,8>::SinglePrecision & a0, const typename TypeAid<T, 8>::SinglePrecision & b0,
                            const typename TypeAid<T, 8>::SinglePrecision & a1, const typename TypeAid<T, 8>::SinglePrecision & b1,
                            const typename TypeAid<T, 8>::SinglePrecision & a2, const typename TypeAid<T, 8>::SinglePrecision & b2,
                            const typename TypeAid<T, 8>::SinglePrecision & a3, const typename TypeAid<T, 8>::SinglePrecision & b3)
        {
            this->Rows[0] = tVec2<T>(a0,b0);
            this->Rows[1] = tVec2<T>(a1,b1);
            this->Rows[2] = tVec2<T>(a2,b2);
            this->Rows[3] = tVec2<T>(a3,b3);
        }

        template<typename T>
        tVec2<T> & tMat4x2<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec2<T> & tMat4x2<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }

        template<typename T>
        tMat4x2<T> & tMat4x2<T>::operator=(const typename TypeAid<T, 8>::CoreType &a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,0);
            this->Rows[2] = tVec2<T>(0,0);
            this->Rows[3] = tVec2<T>(0,a);
            return (*this);
        }

        template<typename T>
        tMat4x2<T> & tMat4x2<T>::operator=(const typename TypeAid<T, 8>::SinglePrecision &a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,0);
            this->Rows[2] = tVec2<T>(0,0);
            this->Rows[3] = tVec2<T>(0,a);
            return (*this);
        }



        template <typename T, typename U>
        inline tMat4x2<T> & operator+= (tMat4x2<T> & a, const typename TypeAid<U, 8>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            a[3] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat4x2<T> & operator+= (tMat4x2<T> & a,const tMat4x2<U> & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            a[2] += m[2];
            a[3] += m[3];
            return a;
        }

        template<typename T, typename U>
        inline tMat4x2<T> & operator-= (tMat4x2<T> & a, const typename TypeAid<U, 8>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            a[3] -= s;
            return a;
        }

        template<typename T, typename U>
        inline tMat4x2<T> & operator-= (tMat4x2<T> & a,tMat4x2<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            a[3] -= m[3];
            return a;
        }

        template<typename T, typename U>
        inline tMat4x2<T> & operator*= (tMat4x2<T> & a,const typename TypeAid<U, 8>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            a[3] *= s;
            return a;
        }

        template<typename T, typename U>
        inline tMat4x2<T> & operator/= (tMat4x2<T> & a,const typename TypeAid<U, 8>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            a[3] /= s;
            return a;
        }

        template <typename T>
        inline tMat4x2<T> operator++ (tMat4x2<T> & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            ++a[3];
            return a;
        }

        template <typename T>
        inline tMat4x2<T> & operator-- (tMat4x2<T> & a)
        {
            --a[0];
            --a[1];
            --a[2];
            --a[3];
            return a;
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator++(tMat4x2<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T>
        inline tMat4x2<T> & operator--(tMat4x2<T> & a,int)
        {
            --a;
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat4x2<T> operator+
                (
                        tMat4x2<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat4x2<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s,
                    m[3] + s);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator+
                (
                        tMat4x2<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return tMat4x2<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2],
                    m1[3] + m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator-
                (
                        tMat4x2<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat4x2<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s,
                    m[3] - s);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator-
                (
                        tMat4x2<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return tMat4x2<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2],
                    m1[3] - m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator*
                (
                        tMat4x2<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat4x2<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator*
                (
                        typename TypeAid<T, 8>::CoreType const & s,
                        tMat4x2<U> const & m
                )
        {
            return tMat4x2<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tMat4x2<T> const & m,
                        tVec4<U> const & v
                )
        {
            return tVec2<T>(
                    m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
                    m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3]);
        }

        template <typename T, typename U>
        tVec3<T> operator*
                (
                        tVec2<T> const & v,
                        tMat4x2<U> const & m
                )
        {
            return tVec3<T>(
                    v[0] * m[0][0] + v[1] * m[0][1],
                    v[0] * m[1][0] + v[1] * m[1][1],
                    v[0] * m[2][0] + v[1] * m[2][1],
                    v[0] * m[3][0] + v[1] * m[3][1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator*
                (
                        tMat4x2<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA20 = m1[2][0];
            const T & SrcA21 = m1[2][1];
            const T & SrcA30 = m1[3][0];
            const T & SrcA31 = m1[3][1];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB02 = m2[0][2];
            const T & SrcB03 = m2[0][3];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB12 = m2[1][2];
            const T & SrcB13 = m2[1][3];

            tMat2x2<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02 + SrcA30 * SrcB03;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02 + SrcA31 * SrcB03;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12 + SrcA30 * SrcB13;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12 + SrcA31 * SrcB13;
            return Result;
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator*
                (
                        tMat4x2<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return tMat3x2<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3]);
        }



        template <typename T, typename U>
        inline tMat4x2<T> operator/
                (
                        tMat4x2<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat4x2<T>(
                    m[0] / s,
                    m[1] / s,
                    m[2] / s,
                    m[3] / s);
        }

        template <typename T, typename U>
        inline tMat4x2<T> operator/
                (
                        typename TypeAid<T, 8>::CoreType const & s,
                        tMat4x2<T> const & m
                )
        {
            return tMat4x2<T>(
                    s / m[0],
                    s / m[1],
                    s / m[2],
                    s / m[3]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat4x2<T> const operator-
                (
                        tMat4x2<T> const & m
                )
        {
            return tMat4x2<T>(
                    -m[0],
                    -m[1],
                    -m[2],
                    -m[3]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat4x2<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat4x2<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]);
        }




        template <typename T, typename U>
        inline tMat4x2<T> operator*
                (
                        tMat2x2<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return tMat4x2<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1]);
        }



    }
    typedef Detail::tMat4x2<PDP::UInteger> UMat4x2;
    typedef Detail::tMat4x2<PDP::Integer> IMat4x2;
    typedef Detail::tMat4x2<float> Mat4x2;
    typedef Detail::tMat4x2<double> DMat4x2;
    typedef Detail::tMat4x2<unsigned short> USMat4x2;
    typedef Detail::tMat4x2<short> SMat4x2;
    //typedef Detail::tMat4x2<PDP::Half> HMat4x2;
}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tMat4x2<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
