//
//  Mat3x4.h
//  DataStructures
//
//  Created by James Landess on 11/13/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat3x4_h
#define DataStructures_Mat3x4_h

#include "StaticArray.h"
#include "Vec4.h"
#include "Mat3x3.h"
#include "Mat2x3.h"
#include "Mat2x4.h"
#include "Mat3x2.h"

namespace PDP
{
    namespace Detail
    {
        template<typename T>
        class tMat3x4
        {
        private:
            tVec4<T> Rows[3];
        public:


            inline tMat3x4();
            inline tMat3x4(const typename TypeAid<T, 6>::CoreType & a);
            inline tMat3x4(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,const typename TypeAid<T, 6>::CoreType & d0,
                           const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1,const typename TypeAid<T, 6>::CoreType & d1,
                           const typename TypeAid<T, 6>::CoreType & a2, const typename TypeAid<T, 6>::CoreType & b2,const typename TypeAid<T, 6>::CoreType & c2, const typename TypeAid<T, 6>::CoreType & d2);


            inline tMat3x4(const tVec4<T> & a, const tVec4<T> & b, const tVec4<T> & c);

            inline tMat3x4(const typename TypeAid<T, 6>::SinglePrecision & a);
            inline tMat3x4(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,const typename TypeAid<T, 6>::SinglePrecision & d0,
                           const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1, const typename TypeAid<T, 6>::SinglePrecision & d1,
                           const typename TypeAid<T, 6>::SinglePrecision & a2, const typename TypeAid<T, 6>::SinglePrecision & b2,const typename TypeAid<T, 6>::SinglePrecision & c2, const typename TypeAid<T, 6>::SinglePrecision & d2);



            inline tVec4<T> & operator [](const PDP::UInteger & index);


            inline const tVec4<T> & operator [](const PDP::UInteger & index) const;
        };



        template<typename T>
        tMat3x4<T>::tMat3x4()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = 0;
        }


        template<typename T>
        tMat3x4<T>::tMat3x4(const typename TypeAid<T, 6>::CoreType & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,0,a);
            this->Rows[3] = tVec3<T>(0,0,0);
        }

        template<typename T>
        tMat3x4<T>::tMat3x4(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,const typename TypeAid<T, 6>::CoreType & d0,
                            const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1,const typename TypeAid<T, 6>::CoreType & d1,
                            const typename TypeAid<T, 6>::CoreType & a2, const typename TypeAid<T, 6>::CoreType & b2,const typename TypeAid<T, 6>::CoreType & c2, const typename TypeAid<T, 6>::CoreType & d2)
        {
            this->Rows[0] = tVec4<T>(a0,b0,c0,d0);
            this->Rows[1] = tVec4<T>(a1,b1,c1,d1);
            this->Rows[2] = tVec4<T>(a2,b2,c2,d2);
        }


        template<typename T>
        tMat3x4<T>::tMat3x4(const tVec4<T> & a, const tVec4<T> & b, const tVec4<T> & c)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
        }

        template<typename T>
        tMat3x4<T>::tMat3x4(const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,0,a);
            this->Rows[3] = tVec3<T>(0,0,0);
        }
        template<typename T>
        tMat3x4<T>::tMat3x4(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,const typename TypeAid<T, 6>::SinglePrecision & d0,
                            const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1, const typename TypeAid<T, 6>::SinglePrecision & d1,
                            const typename TypeAid<T, 6>::SinglePrecision & a2, const typename TypeAid<T, 6>::SinglePrecision & b2,const typename TypeAid<T, 6>::SinglePrecision & c2, const typename TypeAid<T, 6>::SinglePrecision & d2)
        {
            this->Rows[0] = tVec4<T>(a0,b0,c0,d0);
            this->Rows[1] = tVec4<T>(a1,b1,c1,d1);
            this->Rows[2] = tVec4<T>(a2,b2,c2,d2);
        }
        template<typename T>
        tVec4<T> & tMat3x4<T>::operator [](const PDP::UInteger & index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec4<T> & tMat3x4<T>::operator [](const PDP::UInteger & index) const
        {
            return this->Rows[index];
        }




        template <typename T, typename U>
        inline tMat3x4<T>& operator+= (tMat3x4<T> & a, const typename TypeAid<T, 12>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x4<T> & operator+= (tMat3x4<T> & a,tMat3x4<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            a[2] += m[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x4<T> & operator-= (tMat3x4<T> & a,const typename TypeAid<U, 12>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x4<T> & operator-= (tMat3x4<T> & a,tMat3x4<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x4<T> & operator*= (tMat3x4<T> & a,const typename TypeAid<U, 12>::CoreType &  s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x4<T> & operator/= (tMat3x4<T> & a,const typename TypeAid<T, 12>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            return a;
        }

        template <typename T>
        inline tMat3x4<T> & operator++ (tMat3x4<T> &a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }

        template <typename T>
        inline tMat3x4<T> & operator-- (tMat3x4<T> &a)
        {
            --a;
            --a;
            --a;
            return a;
        }

        template <typename T>
        inline tMat3x4<T> & operator++(tMat3x4<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T>
        inline tMat3x4<T> & operator--(tMat3x4<T> & a,int)
        {
            --a;
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat3x4<T> operator+
                (
                        tMat3x4<T> const & m,
                        typename TypeAid<T, 12>::CoreType  const & s
                )
        {
            return tMat3x4<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator+
                (
                        tMat3x4<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return tMat3x4<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x4<T>operator-
                (
                        tMat3x4<T> const & m,
                        typename TypeAid<T, 12>::CoreType const & s
                )
        {
            return tMat3x4<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator-
                (
                        tMat3x4<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return tMat3x4<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator*
                (
                        tMat3x4<T> const & m,
                        typename TypeAid<T, 12>::CoreType const & s
                )
        {
            return tMat3x4<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator*
                (
                        typename TypeAid<T, 12>::CoreType const & s,
                        tMat3x4<U> const & m
                )
        {
            return tMat3x4<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        tMat3x4<T> const & m,
                        tVec3<U> const & v
                )
        {
            return  tVec4<T>(
                    m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2],
                    m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2],
                    m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2],
                    m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        tVec4<T> const & v,
                        tMat3x4<U> const & m
                )
        {
            return tVec3<T>(
                    v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2] + v[3] * m[0][3],
                    v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2] + v[3] * m[1][3],
                    v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m[2][2] + v[3] * m[2][3]);
        }



        template <typename T, typename U>
        inline tMat2x4<T> operator*
                (
                        tMat3x4<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return tMat3x4<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
                    m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
                    m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2]);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator*
                (
                        tMat3x4<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return tMat3x4<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
                    m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
                    m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
                    m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2],
                    m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2]);
        }

        template <typename T, typename U>
        inline tMat3x4<T> operator/
                (
                        tMat3x4<T> const & m,
                        typename TypeAid<T, 12>::CoreType const & s
                )
        {
            return tMat3x4<T> (
                    m[0] / s,
                    m[1] / s,
                    m[2] / s);
        }

        template <typename T, typename U>
        inline tMat3x4<T>  operator/
                (
                        typename TypeAid<T, 12>::CoreType const & s,
                        tMat3x4<U>  const & m
                )
        {
            return tMat3x4<T> (
                    s / m[0],
                    s / m[1],
                    s / m[2]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat3x4<T>  const operator-
                (
                        tMat3x4<T> const & m
                )
        {
            return tMat3x4<T>(
                    -m[0],
                    -m[1],
                    -m[2]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat3x4<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat3x4<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
        }


        template <typename T, typename U>
        inline tMat3x4<T> operator*
                (
                        tMat2x4<T> const & m1,
        tMat3x2<U> const & m2
        )
    {
        return tMat3x4<T>(
                m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
                m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1],
                m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1],
                m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1],
                m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1],
                m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1],
                m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1],
                m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1]);
    }


}

typedef Detail::tMat3x4<PDP::UInteger> UMat3x4;
typedef Detail::tMat3x4<PDP::Integer> IMat3x4;
typedef Detail::tMat3x4<float> Mat3x4;
typedef Detail::tMat3x4<double> DMat3x4;
typedef Detail::tMat3x4<unsigned short> USMat3x4;
typedef Detail::tMat3x4<short> SMat3x4;
//typedef Detail::tMat3x4<PDP::Half> HMat3x4;
}
#endif
