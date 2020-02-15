//
//  Mat2x3.h
//  DataStructures
//
//  Created by James Landess on 11/13/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat2x3_h
#define DataStructures_Mat2x3_h

#include "StaticArray.h"
#include "Mat2x2.h"
#include "Vec3.h"
namespace PDP
{
    namespace Detail
    {
        template<typename T>
        class tMat2x3
        {
        private:
            tVec3<T> Rows[2];

        public:
            inline tMat2x3();
            inline tMat2x3(const typename TypeAid<T, 6>::CoreType & a);
            inline tMat2x3(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,
                           const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1);


            inline tMat2x3(const tVec3<T> & a, const tVec3<T> & b);



            inline tMat2x3(const typename TypeAid<T, 6>::SinglePrecision & a);
            inline tMat2x3(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,
                           const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1);

            inline tVec3<T> & operator [] (const PDP::UInteger & index);
            inline const tVec3<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat2x3 & operator = (const typename TypeAid<T, 6>::CoreType & a);
            inline tMat2x3 & operator = (const typename TypeAid<T, 6>::SinglePrecision & a);
        };



        template<typename T>
        tMat2x3<T>::tMat2x3()
        {
            this->Rows[0] = this->Rows[1] = 0;
        }

        template<typename T>
        tMat2x3<T>::tMat2x3(const typename TypeAid<T, 6>::CoreType & a)
        {
            (*this) = a;
        }


        template<typename T>
        tMat2x3<T>::tMat2x3(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,
                            const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1)
        {

        }


        template<typename T>
        tMat2x3<T>::tMat2x3(const tVec3<T> & a, const tVec3<T> & b)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
        }

        template<typename T>
        tMat2x3<T>::tMat2x3(const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            (*this) = a;
        }


        template<typename T>
        tMat2x3<T>::tMat2x3(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,
                            const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1)
        {
            this->Rows[0] = tVec3<T>(a0,b0,c0);
            this->Rows[1] = tVec3<T>(a1,b1,c1);
        }

        template<typename T>
        inline tVec3<T> & tMat2x3<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        inline const tVec3<T> & tMat2x3<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }


        template<typename T>
        inline tMat2x3<T> & tMat2x3<T>::operator = (const typename TypeAid<T, 6>::CoreType & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            return (*this);
        }
        template<typename T>
        inline tMat2x3<T> & tMat2x3<T>::operator = (const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            return (*this);
        }





        template <typename T, typename U>
        inline tMat2x3<T>  & operator+= (tMat2x3<T>  & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x3<T> & operator+= (tMat2x3<T> & a,tMat2x2<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x3<T> & operator-= (tMat2x3<T>  & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x3<T> & operator-= (tMat2x3<T> & a,tMat2x3<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x3<T> & operator*= (tMat2x3<T> & a, const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x3<T> & operator/= (tMat2x3<T> & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            return a;
        }

        template <typename T>
        inline tMat2x3<T> & operator++(tMat2x3<T> & a)
        {
            ++a[0];
            ++a[1];
            return a;
        }

        template <typename T>
        inline tMat2x3<T> & operator--(tMat2x3<T> & a)
        {
            --a[0];
            --a[1];
            return a;
        }

        template <typename T>
        inline tMat2x3<T> & operator++(tMat2x3<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T>
        inline tMat2x3<T>operator--(tMat2x3<T> & a,int)
        {
            --a;
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat2x3<T> operator+
                (
                        tMat2x3<T> const & m,
                        typename TypeAid<U, 6>::CoreType const & s
                )
        {
            return tMat2x3<T>(
                    m[0] + s,
                    m[1] + s);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator+
                (
                        tMat2x3<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return tMat2x3<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator-
                (
                        tMat2x3<T> const & m,
                        typename TypeAid<U, 6>::CoreType const & s
                )
        {
            return tMat2x3<T>(
                    m[0] - s,
                    m[1] - s);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator-
                (
                        tMat2x3<T> const & m1,
                        tMat2x3<T> const & m2
                )
        {
            return tMat2x3<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator*
                (
                        tMat2x3<T> const & m,
                        typename TypeAid<U, 6>::CoreType const & s
                )
        {
            return tMat2x3<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator*
                (
                        typename TypeAid<T, 6>::CoreType const & s,
                        tMat2x3<U> const & m
                )
        {
            return tMat2x3<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        tMat2x3<T> const & m,
                        tVec2<U> const & v)
        {
            return  tVec2<T>(
                    m[0][0] * v[0] + m[1][0] * v[1],
                    m[0][1] * v[0] + m[1][1] * v[1],
                    m[0][2] * v[0] + m[1][2] * v[1]);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tVec3<T> const & v,
                        tMat2x3<U> const & m)
        {
            return tVec2<T>(
                    v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2],
                    v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2]);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator*
                (
                        tMat2x3<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return tMat2x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1]);
        }




        template <typename T, typename U>
        inline tMat2x3<T> operator/
                (
                        tMat2x3<T> const & m,
                        typename TypeAid<U, 6>::CoreType const & s
                )
        {
            return tMat2x3<T>(
                    m[0] / s,
                    m[1] / s);
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator/
                (
                        typename TypeAid<T, 6>::CoreType const & s,
                        tMat2x3<U> const & m
                )
        {
            return tMat2x3<T>(
                    s / m[0],
                    s / m[1]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat2x3<T> const operator-
                (
                        tMat2x3<T> const & m
                )
        {
            return tMat2x3<T>(
                    -m[0],
                    -m[1]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat2x3<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat2x3<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]);
        }

    }

    typedef Detail::tMat2x3<PDP::UInteger> UMat2x3;
    typedef Detail::tMat2x3<PDP::Integer> IMat2x3;
    typedef Detail::tMat2x3<float> Mat2x3;
    typedef Detail::tMat2x3<double> DMat2x3;
    typedef Detail::tMat2x3<unsigned short> USMat2x3;
    typedef Detail::tMat2x3<short> SMat2x3;
    //typedef Detail::tMat2x3<PDP::Half> HMat2x3;
}
#endif
