//
//  Mat3x3.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat3x3_h
#define DataStructures_Mat3x3_h

#include "StaticArray.hpp"
#include "Vec3.hpp"
#include "Mat2x3.hpp"
#include "Mat3x2.hpp"


namespace PDP
{
    namespace Detail
    {
        template<typename T>
        class tMat3x3
        {
        private:
            tVec3<T> Rows[3];
        public:



            inline tMat3x3();
            inline tMat3x3(const typename TypeAid<T, 9>::CoreType & a);
            inline tMat3x3(const typename TypeAid<T, 9>::CoreType & a0, const typename TypeAid<T, 9>::CoreType & b0, const typename TypeAid<T, 9>::CoreType & c0,
                           const typename TypeAid<T, 9>::CoreType & a1, const typename TypeAid<T, 9>::CoreType & b1, const typename TypeAid<T, 9>::CoreType & c1,
                           const typename TypeAid<T, 9>::CoreType & a2, const typename TypeAid<T, 9>::CoreType & b2, const typename TypeAid<T, 9>::CoreType & c2);
            inline tMat3x3(const tVec3<T> & a, const tVec3<T> & b, const tVec3<T> & c);



            inline tVec3<T> & operator [](const PDP::UInteger & index);
            inline const tVec3<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat3x3 & operator = (const typename TypeAid<T, 9>::CoreType & a);
        };


        template<typename T>
        tMat3x3<T>::tMat3x3()
        {
            this->Rows[0] = this->Rows[1] = this->Rows[2] = 0;
        }

        template<typename T>
        tMat3x3<T>::tMat3x3(const typename TypeAid<T, 9>::CoreType & a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,0,a);
        }

        template<typename T>
        tMat3x3<T>::tMat3x3(const typename TypeAid<T, 9>::CoreType & a0, const typename TypeAid<T, 9>::CoreType & b0, const typename TypeAid<T, 9>::CoreType & c0,
                            const typename TypeAid<T, 9>::CoreType & a1, const typename TypeAid<T, 9>::CoreType & b1, const typename TypeAid<T, 9>::CoreType & c1,
                            const typename TypeAid<T, 9>::CoreType & a2, const typename TypeAid<T, 9>::CoreType & b2, const typename TypeAid<T, 9>::CoreType & c2)
        {
            this->Rows[0] = tVec3<T>(a0,b0,c0);
            this->Rows[1] = tVec3<T>(a1,b1,c1);
            this->Rows[2] = tVec3<T>(a2,b2,c2);

        }

        template<typename T>
        tMat3x3<T>::tMat3x3(const tVec3<T> & a, const tVec3<T> & b, const tVec3<T> & c)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
            this->Rows[2] = c;
        }




        template<typename T>
        tVec3<T> & tMat3x3<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec3<T> & tMat3x3<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }

        template<typename T>
        tMat3x3<T> & tMat3x3<T>::operator=(const typename TypeAid<T, 9>::CoreType &a)
        {
            this->Rows[0] = tVec3<T>(a,0,0);
            this->Rows[1] = tVec3<T>(0,a,0);
            this->Rows[2] = tVec3<T>(0,0,a);
            return (*this);
        }







        template <typename T,typename U>
        inline tMat3x3<T> & operator+= (tMat3x3<T> & a,const typename TypeAid<T, 9>::CoreType & s)
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x3<T> & operator+= (tMat3x3<T> & a ,tMat3x3<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            a[2] += m[2];
            return a;
        }

        template <typename T,typename U>
        inline tMat3x3<T> & operator-= (tMat3x3<T> & a,const typename TypeAid<T, 9>::CoreType & s)
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x3<T> & operator-= (tMat3x3<T>  const & a, const tMat3x3<U> &m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            a[2] -= m[2];
            return a;
        }

        template <typename T, typename U>
        inline tMat3x3<T> & operator*= (tMat3x3<T> & a,const typename TypeAid<T, 9>::CoreType & s)
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat3x3<T> & operator*= (tMat3x3<T> & a,tMat3x3<U> const & m)
        {
            return (a = a * m);
        }

        template <typename T, typename U>
        inline tMat3x3<T> & operator/= (tMat3x3<T> & a,const typename TypeAid<T, 9>::CoreType & s)
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            return a;
        }


        template <typename T>
        struct compute_inversetMat3x3
        {
            inline static tMat3x3<T> call(tMat3x3<T> const & m)
            {
                const T OneOverDeterminant = static_cast<T>(1) / (
                        + m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
                        - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
                        + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

                tMat3x3<T> Inverse(T(0));
                Inverse[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
                Inverse[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
                Inverse[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
                Inverse[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
                Inverse[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
                Inverse[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
                Inverse[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
                Inverse[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
                Inverse[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

                return Inverse;
            }
        };

        template <typename T>
        inline tMat3x3<T> compute_inverse_mat3(tMat3x3<T> const & m)
        {
            const T & S00 = m[0][0];
            const T & S01 = m[0][1];
            const T & S02 = m[0][2];

            const T & S10 = m[1][0];
            const T & S11 = m[1][1];
            const T & S12 = m[1][2];

            const T & S20 = m[2][0];
            const T & S21 = m[2][1];
            const T & S22 = m[2][2];

            tMat3x3<T> Inverse(
                    S11 * S22 - S21 * S12,
                    S12 * S20 - S22 * S10,
                    S10 * S21 - S20 * S11,
                    S02 * S21 - S01 * S22,
                    S00 * S22 - S02 * S20,
                    S01 * S20 - S00 * S21,
                    S12 * S01 - S11 * S02,
                    S10 * S02 - S12 * S00,
                    S11 * S00 - S10 * S01);

            const T Determinant =
                    + S00 * (S11 * S22 - S21 * S12)
                    - S10 * (S01 * S22 - S21 * S02)
                    + S20 * (S01 * S12 - S11 * S02);

            Inverse /= Determinant;
            return Inverse;
        }


        template <typename T, typename U>
        inline tMat3x3<T> & operator/= (tMat3x3<T> & a,tMat3x3<U> const & m)
        {
            return (a = a * compute_inversetMat3x3<T>::call(m));
        }

        template <typename T>
        inline tMat3x3<T> & operator++ (tMat3x3<T> & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }

        template <typename T>
        inline tMat3x3<T> & operator--(tMat3x3<T> & a)
        {
            --a[0];
            --a[1];
            --a[2];
            return a;
        }

        template <typename T>
        inline tMat3x3<T> &  operator++(tMat3x3<T> & a,int)
        {

            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }

        template <typename T>
        inline tMat3x3<T> &  operator--(tMat3x3<T> & a,int)
        {

            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }



        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat3x3<T> operator+
                (
                        tMat3x3<T> const & m,
                        typename TypeAid<U, 9>::CoreType const & s
                )
        {
            return tMat3x3<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator+
                (
                        typename TypeAid<T, 9>::CoreType const & s,
                        tMat3x3<U> const & m
                )
        {
            return tMat3x3<T>(
                    m[0] + s,
                    m[1] + s,
                    m[2] + s);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator+
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return tMat3x3<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1],
                    m1[2] + m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator-
                (
                        tMat3x3<T> const & m,
                        typename TypeAid<U, 9>::CoreType const & s
                )
        {
            return tMat3x3<T>(
                    m[0] - s,
                    m[1] - s,
                    m[2] - s);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator-
                (
                        typename TypeAid<T, 9>::CoreType const & s,
                        tMat3x3<U> const & m
                )
        {
            return tMat3x3<T>(
                    s - m[0],
                    s - m[1],
                    s - m[2]);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator-
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return tMat3x3<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1],
                    m1[2] - m2[2]);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator*
                (
                        tMat3x3<T> const & m,
                        typename TypeAid<U, 9>::CoreType const & s
                )
        {
            return tMat3x3<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator*
                (
                        typename TypeAid<T, 9>::CoreType const & s,
                        tMat3x3<U> const & m
                )
        {
            return tMat3x3<T>(
                    m[0] * s,
                    m[1] * s,
                    m[2] * s);
        }

        template <typename T, typename U>
        inline  tVec3<T> operator*
                (
                        tMat3x3<T> const & m,
                        tVec3<U> const & v
                )
        {
            return  tVec3<T>(
                    m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
                    m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
                    m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
        }

        template <typename T, typename U>
        inline  tVec3<T> operator*
                (
                        tVec3<T> const & v,
                        tMat3x3<U> const & m
                )
        {
            return tVec3<T>(
                    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator*
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            T const & SrcA00 = m1[0][0];
            T const & SrcA01 = m1[0][1];
            T const & SrcA02 = m1[0][2];
            T const & SrcA10 = m1[1][0];
            T const & SrcA11 = m1[1][1];
            T const & SrcA12 = m1[1][2];
            T const & SrcA20 = m1[2][0];
            T const & SrcA21 = m1[2][1];
            T const & SrcA22 = m1[2][2];

            T const & SrcB00 = m2[0][0];
            T const & SrcB01 = m2[0][1];
            T const & SrcB02 = m2[0][2];
            T const & SrcB10 = m2[1][0];
            T const & SrcB11 = m2[1][1];
            T const & SrcB12 = m2[1][2];
            T const & SrcB20 = m2[2][0];
            T const & SrcB21 = m2[2][1];
            T const & SrcB22 = m2[2][2];

            tMat3x3<T> Result(T(0),T(0),T(0),T(0),T(0),T(0),T(0),T(0),T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
            Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
            Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
            Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
            Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
            Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
            return Result;
        }

        template <typename T, typename U>
        inline tMat2x3<T> operator*
                (
                        tMat3x3<T> const & m1,
                        tMat2x3<U> const & m2
                )
        {
            return tMat2x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2]);
        }



        template <typename T, typename U>
        inline tMat3x3<T> operator/
                (
                        tMat3x3<T> const & m,
                        typename TypeAid<U, 9>::CoreType const & s
                )
        {
            return tMat3x3<T>(
                    m[0] / s,
                    m[1] / s,
                    m[2] / s);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator/
                (
                        typename TypeAid<T, 9>::CoreType const & s,
                        tMat3x3<U> const & m
                )
        {
            return tMat3x3<T>(
                    s / m[0],
                    s / m[1],
                    s / m[2]);
        }

        template <typename T, typename U>
        inline  tVec3<T> operator/
                (
                        tMat3x3<T> const & m,
                        tVec3<T> const & v
                )
        {
            return compute_inversetMat3x3<T>::call(m)*v;
        }

        template <typename T, typename U>
        inline tVec3<T> operator/
                (
                        tVec3<T> const & v,
                        tMat3x3<U> const & m
                )
        {
            return v * compute_inversetMat3x3<T>::call(m);
        }

        template <typename T, typename U>
        inline tMat3x3<T> operator/
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return ( m1 * compute_inversetMat3x3<T>::call(m2));
        }

        // Unary constant operators
        template <typename T>
        inline tMat3x3<T> const operator-
                (
                        tMat3x3<T> const & m
                )
        {
            return tMat3x3<T>(
                    -m[0],
                    -m[1],
                    -m[2]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat3x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
        }



        template <typename T, typename U>
        inline tMat3x3<T> operator*
                (
                        tMat2x3<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            const T & SrcA00 = m1[0][0];
            const T & SrcA01 = m1[0][1];
            const T & SrcA02 = m1[0][2];
            const T & SrcA10 = m1[1][0];
            const T & SrcA11 = m1[1][1];
            const T & SrcA12 = m1[1][2];

            const T & SrcB00 = m2[0][0];
            const T & SrcB01 = m2[0][1];
            const T & SrcB10 = m2[1][0];
            const T & SrcB11 = m2[1][1];
            const T & SrcB20 = m2[2][0];
            const T & SrcB21 = m2[2][1];

            tMat3x3<T> Result(T(0));
            Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01;
            Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01;
            Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01;
            Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11;
            Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11;
            Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11;
            Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21;
            Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21;
            Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21;
            return Result;
        }

        template <typename T, typename U>
        inline tMat3x2<T> operator*
                (
                        tMat3x2<T> const & m1,
                        tMat3x3<U> const & m2
                )
        {
            return tMat3x3<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
                    m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
                    m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2]);
        }
    }
    typedef Detail::tMat3x3<PDP::UInteger> UMat3x3;
    typedef Detail::tMat3x3<PDP::Integer> IMat3x3;
    typedef Detail::tMat3x3<float> Mat3x3;
    typedef Detail::tMat3x3<double> DMat3x3;
    typedef Detail::tMat3x3<unsigned short> USMat3x3;
    typedef Detail::tMat3x3<short> SMat3x3;
    //typedef Detail::tMat3x3<PDP::Half> HMat3x3;

}
#endif
