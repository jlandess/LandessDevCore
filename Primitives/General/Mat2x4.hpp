//
//  Mat2x4.h
//  DataStructures
//
//  Created by James Landess on 11/13/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat2x4_h
#define DataStructures_Mat2x4_h

#include "StaticArray.hpp"
#include "Mat2x2.hpp"
#include "Vec4.hpp"
#include "TypeTraits/StaticallySized.h"


namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tMat2x4
        {
        private:
            tVec4<T> Rows[2];

        public:
            inline tMat2x4();
            inline tMat2x4(const typename TypeAid<T, 6>::CoreType & a);
            inline tMat2x4(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,const typename TypeAid<T, 6>::CoreType & d0,
                           const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1,const typename TypeAid<T, 6>::CoreType & d1);


            inline tMat2x4(const tVec4<T> & a, const tVec4<T> & b);

            inline tMat2x4(const typename TypeAid<T, 6>::SinglePrecision & a);
            inline tMat2x4(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,const typename TypeAid<T, 6>::SinglePrecision & d0,
                           const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1,const typename TypeAid<T, 6>::SinglePrecision & d1);

            inline tVec4<T> & operator [] (const PDP::UInteger & index);
            inline const tVec4<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat2x4 & operator = (const typename TypeAid<T, 6>::CoreType & a);
            inline tMat2x4 & operator = (const typename TypeAid<T, 6>::SinglePrecision & a);
        };


        template<typename T>
        tMat2x4<T>::tMat2x4()
        {
            this->Rows[0] = this->Rows[1] = 0;
        }


        template<typename T>
        tMat2x4<T>::tMat2x4(const typename TypeAid<T, 6>::CoreType & a)
        {
            this->Rows[0] = tVec4<T>(a,0,0,0);
            this->Rows[1] = tVec4<T>(0,0,0,a);
        }

        template<typename T>
        tMat2x4<T>::tMat2x4(const typename TypeAid<T, 6>::CoreType & a0, const typename TypeAid<T, 6>::CoreType & b0,const typename TypeAid<T, 6>::CoreType & c0,const typename TypeAid<T, 6>::CoreType & d0,
                            const typename TypeAid<T, 6>::CoreType & a1, const typename TypeAid<T, 6>::CoreType & b1,const typename TypeAid<T, 6>::CoreType & c1,const typename TypeAid<T, 6>::CoreType & d1)
        {
            this->Rows[0] = tVec4<T>(a0,b0,c0,d0);
            this->Rows[1] = tVec4<T>(a1,b1,c1,d1);

        }
        template<typename T>
        tMat2x4<T>::tMat2x4(const tVec4<T> & a, const tVec4<T> & b)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
        }
        template<typename T>
        tMat2x4<T>::tMat2x4(const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            this->Rows[0] = tVec4<T>(a,0,0,0);
            this->Rows[1] = tVec4<T>(0,a,0,0);
        }

        template<typename T>
        tMat2x4<T>::tMat2x4(const typename TypeAid<T, 6>::SinglePrecision & a0, const typename TypeAid<T, 6>::SinglePrecision & b0,const typename TypeAid<T, 6>::SinglePrecision & c0,const typename TypeAid<T, 6>::SinglePrecision & d0,
                            const typename TypeAid<T, 6>::SinglePrecision & a1, const typename TypeAid<T, 6>::SinglePrecision & b1,const typename TypeAid<T, 6>::SinglePrecision & c1,const typename TypeAid<T, 6>::SinglePrecision & d1)
        {
            this->Rows[0] = tVec4<T>(a0,b0,c0,d0);
            this->Rows[1] = tVec4<T>(a1,b1,c1,d1);
        }
        template<typename T>
        inline tVec4<T> & tMat2x4<T>::operator [] (const PDP::UInteger & index)
        {
            return this->Rows[0];
        }


        template<typename T>
        inline const tVec4<T> & tMat2x4<T>::operator [] (const PDP::UInteger & index) const
        {
            return this->Rows[1];
        }

        template<typename T>
        inline tMat2x4<T> & tMat2x4<T>::operator = (const typename TypeAid<T, 6>::CoreType & a)
        {
            this->Rows[0] = tVec4<T>(a,0,0,0);
            this->Rows[1] = tVec4<T>(0,0,0,a);

            return (*this);
        }

        template<typename T>
        inline tMat2x4<T> & tMat2x4<T>::operator = (const typename TypeAid<T, 6>::SinglePrecision & a)
        {
            this->Rows[0] = tVec4<T>(a,0,0,0);
            this->Rows[1] = tVec4<T>(0,0,0,a);

            return (*this);
        }





        template <typename T, typename U>
        inline tMat2x4<T> & operator+= (tMat2x4<T> & a ,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator+= (tMat2x4<T> & a,tMat2x4<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator-= (tMat2x4<T> & a ,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator-= (tMat2x4<T> & a,tMat2x4<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator*= (tMat2x4<T> & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator/= (tMat2x4<T> & a,const typename TypeAid<U, 6>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T>& operator++(tMat2x4<T> &a)
        {
            ++a[0];
            ++a[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator--(tMat2x4<T> & a)
        {
            --a[0];
            --a[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator++(tMat2x4<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x4<T> & operator--(tMat2x4<T> & a,int)
        {
            --a;
            return a;
        }

        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat2x4<T> operator+
                (
                        tMat2x4<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat2x4<T>(
                    m[0] + s,
                    m[1] + s);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator+
                (
                        tMat2x4<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return tMat2x4<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator-
                (
                        tMat2x4<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat2x4<T>(
                    m[0] - s,
                    m[1] - s);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator-
                (
                        tMat2x4<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return tMat2x4<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator*
                (
                        tMat2x4<T> const & m,
                        T const & s
                )
        {
            return tMat2x4<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator*
                (
                        typename TypeAid<U, 8>::CoreType const & s,
                        tMat2x4<U> const & m
                )
        {
            return tMat2x4<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline  tVec2<T> operator*
                (
                        tMat2x4<T> const & m,
                        tVec2<U> const & v
                )
        {
            return tVec4<T>(
                    m[0][0] * v[0] + m[1][0] * v[1],
                    m[0][1] * v[0] + m[1][1] * v[1],
                    m[0][2] * v[0] + m[1][2] * v[1],
                    m[0][3] * v[0] + m[1][3] * v[1]);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tVec4<T> const & v,
                        tMat2x4<U> const & m
                )
        {
            return tVec2<T>(
                    v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m[0][2] + v[3] * m[0][3],
                    v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m[1][2] + v[3] * m[1][3]);
        }



        template <typename T, typename U>
        inline tMat2x4<T> operator*
                (
                        tMat2x4<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return tMat2x4<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1],
                    m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1],
                    m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1]);
        }


        template <typename T, typename U>
        inline tMat2x4<T> operator/
                (
                        tMat2x4<T> const & m,
                        typename TypeAid<U, 8>::CoreType const & s
                )
        {
            return tMat2x4<T>(
                    m[0] / s,
                    m[1] / s);
        }

        template <typename T, typename U>
        inline tMat2x4<T> operator/
                (
                        typename TypeAid<T, 8>::CoreType const & s,
                        tMat2x4<U> const & m
                )
        {
            return tMat2x4<T>(
                    s / m[0],
                    s / m[1]);
        }

        // Unary constant operators
        template <typename T>
        inline tMat2x4<T> const operator-
                (
                        tMat2x4<T> const & m
                )
        {
            return tMat2x4<T>(
                    -m[0],
                    -m[1]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat2x4<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat2x4<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]);
        }

    }

    typedef Detail::tMat2x4<PDP::UInteger> UMat2x4;
    typedef Detail::tMat2x4<PDP::Integer> IMat2x4;
    typedef Detail::tMat2x4<float> Mat2x4;
    typedef Detail::tMat2x4<double> DMat2x4;
    typedef Detail::tMat2x4<unsigned short> USMat2x4;
    typedef Detail::tMat2x4<short> SMat2x4;
    //typedef Detail::tMat2x4<PDP::Half> HMat2x4;
}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tMat2x4<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
