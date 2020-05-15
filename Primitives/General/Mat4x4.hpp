//
//  Mat4x4.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat4x4_h
#define DataStructures_Mat4x4_h

#include "StaticArray.hpp"
#include "Vec4.hpp"
#include "Mat2x4.hpp"
#include "Mat3x4.hpp"
#include "Mat4x3.hpp"


namespace PDP
{
    namespace Detail
    {
        template<typename T>
        class tMat4x4
        {
        private:
            tVec4<T> Rows[4];
        public:
            inline tMat4x4();
            inline tMat4x4(const typename TypeAid<T, 16>::CoreType & a);

            inline tMat4x4(const typename TypeAid<T, 16>::CoreType & a0, const typename TypeAid<T, 16>::CoreType & b0, const typename TypeAid<T, 16>::CoreType & c0,const typename TypeAid<T, 16>::CoreType & d0,
                           const typename TypeAid<T, 16>::CoreType & a1, const typename TypeAid<T, 16>::CoreType & b1, const typename TypeAid<T, 16>::CoreType & c1,const typename TypeAid<T, 16>::CoreType & d1,
                           const typename TypeAid<T, 16>::CoreType & a2, const typename TypeAid<T, 16>::CoreType & b2, const typename TypeAid<T, 16>::CoreType & c2,const typename TypeAid<T, 16>::CoreType & d2,
                           const typename TypeAid<T, 16>::CoreType & a3, const typename TypeAid<T, 16>::CoreType & b3, const typename TypeAid<T, 16>::CoreType & c3,const typename TypeAid<T, 16>::CoreType & d3);

            inline tMat4x4(const tVec4<T> &a, const tVec4<T> & b, const tVec4<T> & c, const tVec4<T> & d);

            inline tMat4x4(const tMat2x2<T> & a);

            inline tMat4x4(const tMat3x3<T> & a);



            inline tVec4<T> & operator [] (const PDP::UInteger & index);
            inline const tVec4<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat4x4 & operator = (const typename TypeAid<T, 16>::CoreType & a);
            inline tMat4x4 & operator = (const tMat2x2<T> & object);
            inline tMat4x4 & operator = (const tMat3x3<T> & object);
        };

        template<typename T>
        tMat4x4<T>::tMat4x4()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = this->Rows[3] = 0;
        }

        template<typename T>
        tMat4x4<T>::tMat4x4(const typename TypeAid<T, 16>::CoreType & a)
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = this->Rows[3] = a;
        }

        template<typename T>
        tMat4x4<T>::tMat4x4(const typename TypeAid<T, 16>::CoreType & a0, const typename TypeAid<T, 16>::CoreType & b0, const typename TypeAid<T, 16>::CoreType & c0,const typename TypeAid<T, 16>::CoreType & d0,
                            const typename TypeAid<T, 16>::CoreType & a1, const typename TypeAid<T, 16>::CoreType & b1, const typename TypeAid<T, 16>::CoreType & c1,const typename TypeAid<T, 16>::CoreType & d1,
                            const typename TypeAid<T, 16>::CoreType & a2, const typename TypeAid<T, 16>::CoreType & b2, const typename TypeAid<T, 16>::CoreType & c2,const typename TypeAid<T, 16>::CoreType & d2,
                            const typename TypeAid<T, 16>::CoreType & a3, const typename TypeAid<T, 16>::CoreType & b3, const typename TypeAid<T, 16>::CoreType & c3,const typename TypeAid<T, 16>::CoreType & d3)
        {

            this->Rows[0] = tVec4<T>(a0,b0,c0,d0);
            this->Rows[1] = tVec4<T>(a1,b1,c1,d1);
            this->Rows[2] = tVec4<T>(a2,b2,c2,d2);
            this->Rows[3] = tVec4<T>(a3,b3,c3,d3);


        }

        template<typename T>
        tMat4x4<T>::tMat4x4(const tVec4<T> &a, const tVec4<T> & b, const tVec4<T> & c, const tVec4<T> & d)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
            this->Rows[3] = d;
        }



        template<typename T>
        tVec4<T> & tMat4x4<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec4<T> & tMat4x4<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }

        template<typename T>
        tMat4x4<T> & tMat4x4<T>::operator=(const typename TypeAid<T, 16>::CoreType &a)
        {
            this->Rows[0] = tVec4<T>(a,0,0,0);
            this->Rows[1] = tVec4<T>(0,a,0,0);
            this->Rows[2] = tVec4<T>(0,0,a,0);
            this->Rows[3] = tVec4<T>(0,0,0,a);
            return (*this);
        }








        template <typename U, typename T>
        inline tMat4x4<T>& operator+= (tMat4x4<T> & a,const typename TypeAid<U, 16>::CoreType &s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            a[3] += s;

            return a;
        }

        template <typename U, typename T>
        inline tMat4x4<T>& operator+= (tMat4x4<U> & a, const tMat4x4<T> & b )
        {
            a[0] += b[0];
            a[1] += b[1];
            a[2] += b[2];
            a[3] += b[3];
            return a;
        }

        template <typename U, typename T>
        inline tMat4x4<T> & operator-= (tMat4x4<T> & a,const typename TypeAid<U, 16>::CoreType &s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            a[3] -= s;
            return a;
        }

        template <typename U, typename T>
        inline tMat4x4<T> & operator-= (tMat4x4<U> & a, const tMat4x4<T> & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            a[3] -= m[3];
            return a;
        }

        template <typename U, typename T>
        inline tMat4x4<T> & operator*= (tMat4x4<T> & a,const typename TypeAid<U, 16>::CoreType &s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            a[3] *= s;
            return a;
        }

        template <typename U, typename T>
        inline tMat4x4<T> & operator*= (tMat4x4<U> & a, const tMat4x4<T> & m)
        {
            return (a = a * m);
        }



        template <typename U, typename T>
        inline tMat4x4<T> & operator /= (tMat4x4<T> & a,const typename TypeAid<U, 16>::CoreType &s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            a[3] /= s;
            return a;
        }


        template <typename T>
        struct compute_inversetMat4x4
        {
            inline static tMat4x4<T> call( const tMat4x4<T> & m)
            {
                const T & Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
                const T & Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
                const T & Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

                const T & Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
                const T & Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
                const T & Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

                const T & Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
                const T & Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
                const T & Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

                const T & Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
                const T & Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
                const T & Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

                const T & Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
                const T & Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
                const T & Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

                const T & Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
                const T & Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
                const T & Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

                const tVec4<T> Fac0(Coef00, Coef00, Coef02, Coef03);
                const tVec4<T> Fac1(Coef04, Coef04, Coef06, Coef07);
                const tVec4<T> Fac2(Coef08, Coef08, Coef10, Coef11);
                const tVec4<T> Fac3(Coef12, Coef12, Coef14, Coef15);
                const tVec4<T> Fac4(Coef16, Coef16, Coef18, Coef19);
                const tVec4<T> Fac5(Coef20, Coef20, Coef22, Coef23);

                const tVec4<T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
                const tVec4<T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
                const tVec4<T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
                const tVec4<T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

                const tVec4<T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
                const tVec4<T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
                const tVec4<T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
                const tVec4<T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

                const tVec4<T> SignA(+1, -1, +1, -1);
                const tVec4<T> SignB(-1, +1, -1, +1);
                const tMat4x4<T>  Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

                const tVec4<T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

                const tVec4<T> Dot0(m[0] * Row0);
                const T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

                const T OneOverDeterminant = static_cast<T>(1) / Dot1;

                return Inverse * OneOverDeterminant;
            }
        };



        template <typename U, typename T>
        inline tMat4x4<T> & operator/= (tMat4x4<U> & a, const tMat4x4<T> & m)
        {

            return (a = a* compute_inversetMat4x4<T>::call(m));
        }

        template <typename T>
        inline tMat4x4<T> & operator++ (tMat4x4<T> & m)
        {
            m[0];
            m[1];
            m[2];
            m[3];
            return m;
        }

        template <typename T>
        inline tMat4x4<T> & operator-- (tMat4x4<T> & m)
        {
            m[0];
            m[1];
            m[2];
            m[3];
            return m;
        }

        template <typename T>
        inline tMat4x4<T> & operator++(tMat4x4<T> & m,int)
        {
            ++m;
            return m;
        }

        template <typename T>
        inline tMat4x4<T> & operator--(tMat4x4<T> & m, int)
        {
            --m;
            return m;
        }



        // Binary operators
        template <typename T, typename U>
        inline tMat4x4<T> operator+
                (
                        tMat4x4<T> const & m,
                        const typename TypeAid<U, 16>::CoreType &s
                )
        {
            return tMat4x4<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s,
                    m[3] + s);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator+
                (
                        const typename TypeAid<T, 16>::CoreType &s,
                        tMat4x4<U> const & m
                )
        {
            return tMat4x4<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s,
                    m[3] + s);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator+
                (
                        tMat4x4<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            return tMat4x4<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2],
                    m1[3] + m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator-
                (
                        tMat4x4<T> const & m,
                        const typename TypeAid<U, 16>::CoreType &s
                )
        {
            return tMat4x4<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s,
                    m[3] - s);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator-
                (
                        const typename TypeAid<T, 16>::CoreType &s,
                        tMat4x4<U> const & m
                )
        {
            return tMat4x4<T>(
                    s - m[0],
                    s - m[1],
                    s - m[2],
                    s - m[3]);
        }

        template <typename T>
        inline tMat4x4<T> operator-
                (
                        tMat4x4<T> const &m1,
                        tMat4x4<T> const &m2
                )
        {
            return tMat4x4<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2],
                    m1[3] - m2[3]);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator*
                (
                        tMat4x4<T> const & m,
                        const typename TypeAid<U, 16>::CoreType &s
                )
        {
            return tMat4x4<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator*
                (
                        const typename TypeAid<U, 16>::CoreType &s,
                        tMat4x4<T> const & m
                )
        {
            return tMat4x4<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s,
                    m[3] * s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        tMat4x4<T> const & m,
                        tVec4<U>  const & v
                )
        {


            tVec4<T> const Mov0(v[0]);
            tVec4<T> const Mov1(v[1]);
            tVec4<T> const Mul0 = m[0] * Mov0;
            tVec4<T> const Mul1 = m[1] * Mov1;
            tVec4<T> const Add0 = Mul0 + Mul1;
            tVec4<T> const Mov2(v[2]);
            tVec4<T> const Mov3(v[3]);
            tVec4<T> const Mul2 = m[2] * Mov2;
            tVec4<T> const Mul3 = m[3] * Mov3;
            tVec4<T> const Add1 = Mul2 + Mul3;
            tVec4<T> const Add2 = Add0 + Add1;
            return Add2;


        }

        template <typename T, typename  U>
        inline tVec4<T> operator*
                (
                        tVec4<T> const & v,
                        tMat4x4<T> const & m
                )
        {
            return tMat4x4<T>(
                    m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
                    m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
                    m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
                    m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]);
        }


        template <typename T, typename U>
        inline tMat2x4<T> operator*
                (
                        tMat4x4<T> const & m1,
                        tMat2x4<U> const & m2
                )
        {
            return tMat2x4<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
                    m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
                    m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3]);
        }


        template <typename T, typename  U>
        inline tMat4x4<T> operator*(tMat4x4<T> const & m1, tMat4x4<U> const & m2)
        {


            tMat4x4<T> t; // write to temp
            for (PDP::UShort i=0; i < 4; i++)
            {
                for (PDP::UShort j=0; j < 4; j++)
                {
                    t[i][j] = m1[i][0]*m2[0][j] + m1[i][1]*m2[1][j] + m1[i][2]*m2[2][j] + m1[i][3]*m2[3][j];
                }

            }



            return t;

        }
        template <typename T, typename U>
        inline tMat3x4<T> operator*
                (
                        tMat4x4<T> const & m1,
                        tMat3x4<U> const & m2
                )
        {
            return tMat3x4<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
                    m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
                    m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
                    m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
                    m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3]);
        }


        template <typename T, typename U>
        inline tMat4x4<T> operator*
                (
                        tVec4<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            tMat4x4<T> const SrcA0 = m1[0];
            tVec4<T> const SrcA1 = m1[1];
            tVec4<T> const SrcA2 = m1[2];
            tVec4<T> const SrcA3 = m1[3];

            tVec4<T> const SrcB0 = m2[0];
            tVec4<T> const SrcB1 = m2[1];
            tVec4<T> const SrcB2 = m2[2];
            tVec4<T> const SrcB3 = m2[3];

            tVec4<T> Result(0);
            Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
            Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
            Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
            Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
            return Result;
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator/
                (
                        tMat4x4<T> const & m,
                        typename TypeAid<U, 16>::CoreType  const & s
                )
        {
            return tMat4x4<T>(
                    m[0] / s,
                    m[1] / s,
                    m[2] / s,
                    m[3] / s);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator/
                (
                        typename TypeAid<T, 16>::CoreType const & s,
                        tMat4x4<U> const & m
                )
        {
            return tMat4x4<T>(
                    s / m[0],
                    s / m[1],
                    s / m[2],
                    s / m[3]);
        }

        template <typename T, typename U>
        inline tVec4<T>  operator/
                (
                        tMat4x4<T> const & m,
                        tVec4<U> const & v
                )
        {

            return compute_inversetMat4x4<T>::call(m)*v;
        }

        template <typename T, typename U>
        inline tVec4<T> operator/
                (
                        tVec4<T> const & v,
                        tMat4x4<U> const & m
                )
        {

            return v * compute_inversetMat4x4<U>::call(m);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator/
                (
                        tMat4x4<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            tMat4x4<T> m1_copy(m1);
            return m1_copy /= m2;
        }

        // Unary constant operators
        template <typename T>
        inline tMat4x4<T> const operator-
                (
                        tMat4x4<T> const & m
                )
        {
            return tMat4x4<T>(
                    -m[0],
                    -m[1],
                    -m[2],
                    -m[3]);
        }

        template <typename T>
        inline tMat4x4<T> const operator++
                (
                        tMat4x4<T> const & m,
                        int
                )
        {
            return tMat4x4<T>(
                    m[0] + static_cast<T>(1),
                    m[1] + static_cast<T>(1),
                    m[2] + static_cast<T>(1),
                    m[3] + static_cast<T>(1));
        }

        template <typename T>
        inline tMat4x4<T> const operator--
                (
                        tMat4x4<T> const & m,
                        int
                )
        {
            return tMat4x4<T>(
                    m[0] - static_cast<T>(1),
                    m[1] - static_cast<T>(1),
                    m[2] - static_cast<T>(1),
                    m[3] - static_cast<T>(1));
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat4x4<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat4x4<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]);
        }




        template <typename T, typename U>
        inline tMat4x3<T> operator*
                (
                        tMat4x3<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            return tMat4x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
                    m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3],
                    m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3]);
        }


        template <typename T, typename U>
        inline tMat4x4<T> operator*
                (
                        tMat3x4<T> const & m1,
                        tMat4x3<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA02 = m1[0][2];
            const T & SrcA03 = m1[0][3];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA12 = m1[1][2];
            const T & SrcA13 = m1[1][3];
            const T & SrcA20 = m1[2][0];
            const T & SrcA21 = m1[2][1];
            const T & SrcA22 = m1[2][2];
            const T & SrcA23 = m1[2][3];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB02 = m2[0][2];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB12 = m2[1][2];
            const T & SrcB20 = m2[2][0];
            const T & SrcB21 = m2[2][1];
            const T & SrcB22 = m2[2][2];
            const T & SrcB30 = m2[3][0];
            const T & SrcB31 = m2[3][1];
            const T & SrcB32 = m2[3][2];

            tMat4x4<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
            Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
            Result[0][3] = SrcA03 * SrcB00 + SrcA13 * SrcB01 + SrcA23 * SrcB02;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
            Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
            Result[1][3] = SrcA03 * SrcB10 + SrcA13 * SrcB11 + SrcA23 * SrcB12;
            Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
            Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
            Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
            Result[2][3] = SrcA03 * SrcB20 + SrcA13 * SrcB21 + SrcA23 * SrcB22;
            Result[3][0] = SrcA00 * SrcB30 + SrcA10 * SrcB31 + SrcA20 * SrcB32;
            Result[3][1] = SrcA01 * SrcB30 + SrcA11 * SrcB31 + SrcA21 * SrcB32;
            Result[3][2] = SrcA02 * SrcB30 + SrcA12 * SrcB31 + SrcA22 * SrcB32;
            Result[3][3] = SrcA03 * SrcB30 + SrcA13 * SrcB31 + SrcA23 * SrcB32;
            return Result;
        }



        template <typename T, typename U>
        inline tMat4x2<T> operator*
                (
                        tMat4x2<T> const & m1,
                        tMat4x4<U> const & m2
                )
        {
            return tMat4x2<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
                    m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3],
                    m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3]);
        }

        template <typename T, typename U>
        inline tMat4x4<T> operator*
                (
                        tMat2x4<T> const & m1,
                        tMat4x2<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA02 = m1[0][2];
            const T & SrcA03 = m1[0][3];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA12 = m1[1][2];
            const T & SrcA13 = m1[1][3];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB20 = m2[2][0];
            const T & SrcB21 = m2[2][1];
            const T & SrcB30 = m2[3][0];
            const T & SrcB31 = m2[3][1];

            tMat4x4<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01;
            Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01;
            Result[0][3] = SrcA03 * SrcB00 + SrcA13 * SrcB01;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11;
            Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11;
            Result[1][3] = SrcA03 * SrcB10 + SrcA13 * SrcB11;
            Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21;
            Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21;
            Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21;
            Result[2][3] = SrcA03 * SrcB20 + SrcA13 * SrcB21;
            Result[3][0] = SrcA00 * SrcB30 + SrcA10 * SrcB31;
            Result[3][1] = SrcA01 * SrcB30 + SrcA11 * SrcB31;
            Result[3][2] = SrcA02 * SrcB30 + SrcA12 * SrcB31;
            Result[3][3] = SrcA03 * SrcB30 + SrcA13 * SrcB31;
            return Result;
        }


    }
    typedef Detail::tMat4x4<PDP::UInteger> UMat4x4;
    typedef Detail::tMat4x4<PDP::Integer> IMat4x4;
    typedef Detail::tMat4x4<float> Mat4x4;
    typedef Detail::tMat4x4<double> DMat4x4;
    typedef Detail::tMat4x4<unsigned short> USMat4x4;
    typedef Detail::tMat4x4<short> SMat4x4;
    //typedef Detail::tMat4x4<PDP::Half> HMat4x4;

}
#endif
