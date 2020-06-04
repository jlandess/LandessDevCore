//
//  Mat4x3.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat4x3_h
#define DataStructures_Mat4x3_h

#include "StaticArray.hpp"
#include "Vec3.hpp"
#include "Mat2x4.hpp"
#include "Mat2x3.hpp"
#include "Mat3x3.hpp"
#include "Mat3x4.hpp"
#include "Mat4x2.hpp"
#include "TypeTraits/StaticallySized.h"


namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tMat4x3: public TypeAid<T, 12>::Type
        {
        private:
            tVec3<T> Rows[4];
        public:
            inline tMat4x3();
            inline tMat4x3(const typename TypeAid<T, 12>::CoreType & a);
            inline tMat4x3(const typename TypeAid<T, 12>::CoreType & a0, const typename TypeAid<T, 12>::CoreType & b0, const typename TypeAid<T, 12>::CoreType & c0,
                           const typename TypeAid<T, 12>::CoreType & a1, const typename TypeAid<T, 12>::CoreType & b1, const typename TypeAid<T, 12>::CoreType & c1,
                           const typename TypeAid<T, 12>::CoreType & a2, const typename TypeAid<T, 12>::CoreType & b2, const typename TypeAid<T, 12>::CoreType & c2,
                           const typename TypeAid<T, 12>::CoreType & a3, const typename TypeAid<T, 12>::CoreType & b3, const typename TypeAid<T, 12>::CoreType & c3);

            inline tMat4x3(const tVec3<T> &a, const tVec3<T> & b, const tVec3<T> & c, const tVec3<T> & d);

            inline tMat4x3(const typename TypeAid<T, 12>::SinglePrecision & a);
            inline tMat4x3(const typename TypeAid<T, 12>::SinglePrecision & a0, const typename TypeAid<T, 12>::SinglePrecision & b0, const typename TypeAid<T, 12>::SinglePrecision & c0,
                           const typename TypeAid<T, 12>::SinglePrecision & a1, const typename TypeAid<T, 12>::SinglePrecision & b1, const typename TypeAid<T, 12>::SinglePrecision & c1,
                           const typename TypeAid<T, 12>::SinglePrecision & a2, const typename TypeAid<T, 12>::SinglePrecision & b2, const typename TypeAid<T, 12>::SinglePrecision & c2,
                           const typename TypeAid<T, 12>::SinglePrecision & a3, const typename TypeAid<T, 12>::SinglePrecision & b3, const typename TypeAid<T, 12>::SinglePrecision & c3);

            inline tVec3<T> & operator [] (const PDP::UInteger & index);
            inline const tVec3<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat4x3 & operator = (const typename TypeAid<T, 12>::CoreType & a );
            inline tMat4x3 & operator = (const typename TypeAid<T, 12>::SinglePrecision & a );

        };

        template<typename T>
        tMat4x3<T>::tMat4x3()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = this->Rows[3] = 0;
        }

        template<typename T>
        tMat4x3<T>::tMat4x3(const typename TypeAid<T, 12>::CoreType & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,a,0);
            this->Rows[3] = tVec3<T>(0,0,a);

        }

        template<typename T>
        tMat4x3<T>::tMat4x3(const typename TypeAid<T, 12>::CoreType & a0, const typename TypeAid<T, 12>::CoreType & b0, const typename TypeAid<T, 12>::CoreType & c0,
                            const typename TypeAid<T, 12>::CoreType & a1, const typename TypeAid<T, 12>::CoreType & b1, const typename TypeAid<T, 12>::CoreType & c1,
                            const typename TypeAid<T, 12>::CoreType & a2, const typename TypeAid<T, 12>::CoreType & b2, const typename TypeAid<T, 12>::CoreType & c2,
                            const typename TypeAid<T, 12>::CoreType & a3, const typename TypeAid<T, 12>::CoreType & b3, const typename TypeAid<T, 12>::CoreType & c3)
        {
            this->Rows[0] = tVec3<T>(a0,b0,c0);
            this->Rows[1] = tVec3<T>(a1,b1,c1);
            this->Rows[2] = tVec3<T>(a2,b2,c2);
            this->Rows[3] = tVec3<T>(a3,b2,c3);
        }

        template<typename T>
        tMat4x3<T>::tMat4x3(const tVec3<T> &a, const tVec3<T> & b, const tVec3<T> & c, const tVec3<T> & d)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
            this->Rows[3] = d;
        }

        template<typename T>
        tMat4x3<T>::tMat4x3(const typename TypeAid<T, 12>::SinglePrecision & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,0,a);
            this->Rows[3] = tVec3<T>(0,0,0);
        }

        template<typename T>
        tMat4x3<T>::tMat4x3(const typename TypeAid<T, 12>::SinglePrecision & a0, const typename TypeAid<T, 12>::SinglePrecision & b0, const typename TypeAid<T, 12>::SinglePrecision & c0,
                            const typename TypeAid<T, 12>::SinglePrecision & a1, const typename TypeAid<T, 12>::SinglePrecision & b1, const typename TypeAid<T, 12>::SinglePrecision & c1,
                            const typename TypeAid<T, 12>::SinglePrecision & a2, const typename TypeAid<T, 12>::SinglePrecision & b2, const typename TypeAid<T, 12>::SinglePrecision & c2,
                            const typename TypeAid<T, 12>::SinglePrecision & a3, const typename TypeAid<T, 12>::SinglePrecision & b3, const typename TypeAid<T, 12>::SinglePrecision & c3)
        {
            this->Rows[0] = tVec3<T>(a0,b0,c0);
            this->Rows[1] = tVec3<T>(a1,b1,c1);
            this->Rows[2] = tVec3<T>(a2,b2,c2);
            this->Rows[3] = tVec3<T>(a3,b2,c3);
        }

        template<typename T>
        tVec3<T> & tMat4x3<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec3<T> & tMat4x3<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }

        template<typename T>
        tMat4x3<T> & tMat4x3<T>::operator=(const typename TypeAid<T, 12>::CoreType &a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,0,0);
            this->Rows[2] = tVec3<T>(0,a,0);
            this->Rows[3] = tVec3<T>(0,0,a);
            return (*this);
        }

        template<typename T>
        tMat4x3<T> & tMat4x3<T>::operator=(const typename TypeAid<T, 12>::SinglePrecision &a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,0,0);
            this->Rows[2] = tVec3<T>(0,a,0);
            this->Rows[3] = tVec3<T>(0,0,a);
            return (*this);
        }





        template<typename T, typename U>
        inline tMat4x3<T> & operator+= (tMat4x3<T> & a,const typename TypeAid<U, 12>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            a[3] += s;
            return a;
        }

        template<typename T, typename U>
        inline tMat4x3<T> & operator+= (tMat4x3<T> & a,tMat4x3<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            a[2] += m[2];
            a[3] += m[3];
            return a;
        }

        template<typename T, typename U>
        inline tMat4x3<T> & operator-= (tMat4x3<T> & a,const typename TypeAid<U, 12>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            a[3] -= s;
            return a;
        }

        template<typename T, typename U>
        inline tMat4x3<T>& operator-= (tMat4x3<T> & a,tMat4x3<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            a[3] -= m[3];
            return a;
        }

        template<typename T, typename U>
        inline tMat4x3<T> & operator*= (tMat4x3<T> & a ,const typename TypeAid<U, 12>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            a[3] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat4x3<T> & operator/= (tMat4x3<T> & a, const typename TypeAid<U, 12>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            a[3] /= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat4x3<T> & operator++ (tMat4x3<T> & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            ++a[3];
            return a;
        }

        template <typename T, typename U>
        inline tMat4x3<T> & operator-- (tMat4x3<T> & a)
        {
            --a[0];
            --a[1];
            --a[2];
            --a[3];
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat4x3<T> operator+ (
                tMat4x3<T> const & m,
                typename TypeAid<T, 12>::CoreType const & s)
        {
            return tMat4x3<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s,
                    m[3] + s);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator+ (
                tMat4x3<T> const & m1,
                tMat4x3<U> const & m2)
        {
            return tMat4x3<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2],
                    m1[3] + m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator- (
                tMat4x3<T> const & m,
                typename TypeAid<U, 12>::CoreType const & s)
        {
            return tMat4x3<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s,
                    m[3] - s);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator- (
                tMat4x3<T> const & m1,
                tMat4x3<U> const & m2)
        {
            return tMat4x3<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2],
                    m1[3] - m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator* (
                tMat4x3<T> const & m,
                typename TypeAid<U, 12>::CoreType const & s)
        {
            return tMat4x3<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator* (
                typename TypeAid<T, 12>::CoreType const & s,
                tMat4x3<U> const & m)
        {
            return tMat4x3<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator*
                (
                        tMat4x3<T> const & m,
                        tVec4<U> const & v)
        {
            return tVec3<T>(
                    m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
                    m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
                    m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        tVec3<T> const & v,
                        tMat4x3<U> const & m)
        {
            return tVec4<T>(
                    v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
                    v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2],
                    v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2],
                    v[0] * m[3][0] + v[1] * m[3][1] + v[2] * m[3][2]);
        }

        template <typename T, typename U>
        tMat2x3<T> operator*
                (
                        tMat4x3<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return tMat2x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3]);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator*
                (
                        tMat4x3<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA02 = m1[0][2];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA12 = m1[1][2];
            const T & SrcA20 = m1[2][0];
            const T & SrcA21 = m1[2][1];
            const T & SrcA22 = m1[2][2];
            const T & SrcA30 = m1[3][0];
            const T & SrcA31 = m1[3][1];
            const T & SrcA32 = m1[3][2];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB02 = m2[0][2];
            const T & SrcB03 = m2[0][3];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB12 = m2[1][2];
            const T & SrcB13 = m2[1][3];
            const T & SrcB20 = m2[2][0];
            const T & SrcB21 = m2[2][1];
            const T & SrcB22 = m2[2][2];
            const T & SrcB23 = m2[2][3];

            tMat3x3<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02 + SrcA30 * SrcB03;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02 + SrcA31 * SrcB03;
            Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02 + SrcA32 * SrcB03;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12 + SrcA30 * SrcB13;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12 + SrcA31 * SrcB13;
            Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12 + SrcA32 * SrcB13;
            Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22 + SrcA30 * SrcB23;
            Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22 + SrcA31 * SrcB23;
            Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22 + SrcA32 * SrcB23;
            return Result;
        }


        template <typename T, typename U>
        inline tMat4x3<T> operator/
                (
                        tMat4x3<T> const & m,
                        typename TypeAid<U, 12>::CoreType const & s
                )
        {
            return tMat4x3<T>(
                    m[0] / s,
                    m[1] / s,
                    m[2] / s,
                    m[3] / s);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator/
                (
                        typename TypeAid<T, 12>::CoreType const & s,
                        tMat4x3<U> const & m
                )
        {
            return tMat4x3<T>(
                    s / m[0],
                    s / m[1],
                    s / m[2],
                    s / m[3]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat4x3<T> const operator-
                (
                        tMat4x3<T>  & m
                )
        {

            return tMat4x3<T>(
                    -m[0],
                    -m[1],
                    -m[2],
                    -m[3]);
        }

        template <typename T>
        inline tMat4x3<T> const operator++
                (
                        tMat4x3<T>  & m,
                        int
                )
        {
            ++m[0];
            ++m[1];
            ++m[2];
            ++m[3];
            return m;
        }

        template <typename T>
        inline tMat4x3<T> const operator--
                (
                        tMat4x3<T>  & m,
                        int
                )
        {
            m[0]-=1;;
            m[1]-=1;
            m[2]-=1;
            m[3]-=1;

            return m;
        }

        template <typename T>
        inline tMat4x3<T> operator++(tMat4x3<T> & a,int)
        {
            a[0]+=1;
            a[1]+=1;
            a[2]+=1;
            a[3]+=1;

            return a;
        }

        template <typename T>
        inline tMat4x3<T> & operator--(tMat4x3<T> & a,int)
        {
            a[0]-=1;
            a[1]-=1;
            a[2]-=1;
            a[3]-=1;
            return a;
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat4x3<T> const & m1,
                        tMat4x3<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat4x3<T> const & m1,
                        tMat4x3<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]);
        }




        template <typename T, typename U>
        inline tMat4x2<T> operator*
                (
                        tMat3x2<T> const & m1,
                        tMat4x3<U> const & m2
                )
        {
            return tMat4x2<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2]);
        }



        template <typename T, typename U>
        inline tMat4x3<T> operator*
                (
                        tMat2x3<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            return tMat4x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1],
                    m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1],
                    m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1]);
        }

        template <typename T, typename U>
        inline tMat4x3<T> operator*
                (
                        tMat3x3<T> const & m1,
                        tMat4x3<U> const & m2
                )
        {
            return tMat4x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
                    m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2],
                    m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2]);
        }


    }
    typedef Detail::tMat4x3<PDP::UInteger> UMat4x3;
    typedef Detail::tMat4x3<PDP::Integer> IMat4x3;
    typedef Detail::tMat4x3<float> Mat4x3;
    typedef Detail::tMat4x3<double> DMat4x3;
    typedef Detail::tMat4x3<unsigned short> USMat4x3;
    typedef Detail::tMat4x3<short> SMat4x3;
    //typedef Detail::tMat4x3<PDP::Half> HMat4x3;
}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tMat4x3<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
