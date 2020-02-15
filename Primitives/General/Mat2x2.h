//
//  Mat2x2.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Mat2x2_h
#define DataStructures_Mat2x2_h

#include "StaticArray.h"
#include "Vec2.h"


namespace PDP
{
    namespace Detail
    {
        template<typename T>
        class tMat2x2
        {
        private:
            tVec2<T> Rows[2];
        public:
            inline tMat2x2();
            inline tMat2x2(const typename TypeAid<T, 4>::CoreType & a);
            inline tMat2x2(const typename TypeAid<T, 4>::CoreType & a0, const typename TypeAid<T, 4>::CoreType & b0,
                           const typename TypeAid<T, 4>::CoreType & a1, const typename TypeAid<T, 4>::CoreType & b1);
            inline tMat2x2(const tVec2<T> & a, const tVec2<T> & b);


            inline tVec2<T> & operator [] (const PDP::UInteger & index);
            inline const tVec2<T> & operator [] (const PDP::UInteger & index) const;

            inline tMat2x2 & operator = (const typename TypeAid<T, 4>::CoreType & a);
        };

        template<typename T>
        tMat2x2<T>::tMat2x2()
        {
            this->Rows[0] = this->Rows[1] = 0;
        }

        template<typename T>
        tMat2x2<T>::tMat2x2(const typename TypeAid<T, 4>::CoreType & a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,a);

        }

        template<typename T>
        tMat2x2<T>::tMat2x2(const typename TypeAid<T, 4>::CoreType & a0, const typename TypeAid<T, 4>::CoreType & b0,
                            const typename TypeAid<T, 4>::CoreType & a1, const typename TypeAid<T, 4>::CoreType & b1)
        {
            this->Rows[0][0] = a0;
            this->Rows[0][1] = b0;
            this->Rows[1][0] = a1;
            this->Rows[1][1] = b1;
        }


        template<typename T>
        tMat2x2<T>::tMat2x2(const tVec2<T> &a ,const tVec2<T> & b)
        {
            this->Rows[0] = a;
            this->Rows[1] = b;
        }


        template<typename T>
        tVec2<T> & tMat2x2<T>::operator[](const PDP::UInteger &index)
        {
            return this->Rows[index];
        }

        template<typename T>
        const tVec2<T> & tMat2x2<T>::operator[](const PDP::UInteger &index) const
        {
            return this->Rows[index];
        }

        template<typename T>
        tMat2x2<T> & tMat2x2<T>::operator=(const typename TypeAid<T, 4>::CoreType &a)
        {
            this->Rows[0] = tVec2<T>(a,0);
            this->Rows[1] = tVec2<T>(0,a);

            return (*this);
        }



        template <typename T, typename U>
        inline tMat2x2<T>& operator+= (tMat2x2<T> & a,const tVec2<T> & s)
        {
            a[0] += s;
            a[1] += s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator+= (tMat2x2<T> & a,tMat2x2<U> const & m)
        {
            a[0] += m[0];
            a[1] += m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator-= (tMat2x2<T> & a, const tVec2<U> &s)
        {
            a[0] -= s;
            a[1] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator-= (tMat2x2<T> & a, tMat2x2<U> const & m)
        {
            a[0] -= m[0];
            a[1] -= m[1];
            return a;
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator*= (tMat2x2<T> & a,const tVec2<U> & s)
        {
            a[0] *= s;
            a[1] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator*= (tMat2x2<T> & a,tMat2x2<U> const & m)
        {
            return (a = a*m);
        }

        template <typename T, typename U>
        inline tMat2x2<T> & operator/= (tMat2x2<T> & a,const tVec2<U> & s)
        {
            a[0] /= s;
            a[1] /= s;
            return a;
        }
        template <typename T>
        struct compute_inverseMat2x2
        {
            inline static tMat2x2<T> call(tMat2x2<T>  const & m)
            {
                const T OneOverDeterminant = static_cast<T>(1) / (
                        + m[0][0] * m[1][1]
                        - m[1][0] * m[0][1]);

                tMat2x2<T>  Inverse(
                        + m[1][1] * OneOverDeterminant,
                        - m[0][1] * OneOverDeterminant,
                        - m[1][0] * OneOverDeterminant,
                        + m[0][0] * OneOverDeterminant);

                return Inverse;
            }
        };


        template <typename T, typename U>
        inline tMat2x2<T> & operator/= (tMat2x2<T> & a,tMat2x2<U> const & m)
        {

            return (a = a * compute_inverseMat2x2<T>::call(m));
        }





        template <typename T>
        inline tMat2x2<T>& operator++(tMat2x2<T> & a)
        {
            ++a[0];
            ++a[1];
            return a;
        }

        template <typename T>
        inline tMat2x2<T> & operator--(tMat2x2<T> & a)
        {
            --a[0];
            --a[1];
            return a;
        }

        template <typename T>
        inline tMat2x2<T> & operator++(tMat2x2<T> & a,int)
        {
            ++a;
            return a;
        }

        template <typename T>
        inline tMat2x2<T> operator--(tMat2x2<T>& a,int)
        {
            --a;
            return a;
        }



        //////////////////////////////////////////////////////////////
        // Binary operators

        template <typename T, typename U>
        inline tMat2x2<T> operator+
                (
                        tMat2x2<T> const & m,
                        tVec2<U> const & s
                )
        {
            return tMat2x2<T>(
                    m[0] + s,
                    m[1] + s);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator+
                (
                        tVec2<T> const & s,
                        tMat2x2<U> const & m
                )
        {
            return tMat2x2<T>(
                    m[0] + s,
                    m[1] + s);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator+
                (
                        tMat2x2<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return tMat2x2<T>(
                    m1[0] + m2[0],
                    m1[1] + m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator-
                (
                        tMat2x2<T> const & m,
                        tVec2<U> const & s
                )
        {
            return tMat2x2<T>(
                    m[0] - s,
                    m[1] - s);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator-
                (
                        tVec2<T> const & s,
                        tMat2x2<U> const & m
                )
        {
            return tMat2x2<T>(
                    s - m[0],
                    s - m[1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator-
                (
                        tMat2x2<T>const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return tMat2x2<T>(
                    m1[0] - m2[0],
                    m1[1] - m2[1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator*
                (
                        tMat2x2<T>const & m,
                        const typename TypeAid<U, 4>::CoreType & s
                )
        {
            return tMat2x2<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator*
                (
                        const typename TypeAid<T, 4>::CoreType & s,
                        tMat2x2<U> const & m
                )
        {
            return tMat2x2<T>(
                    m[0] * s,
                    m[1] * s);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tMat2x2<T> const & m,
                        tVec2<U> const & v
                )
        {
            return tVec2<T>(
                    m[0][0] * v.x + m[1][0] * v.y,
                    m[0][1] * v.x + m[1][1] * v.y);
        }

        template <typename T, typename U>
        inline tVec2<T> operator*
                (
                        tVec2<T> const & v,
                        tMat2x2<U> const & m
                )
        {
            return tVec2<T>(
                    v.x * m[0][0] + v.y * m[0][1],
                    v.x * m[1][0] + v.y * m[1][1]);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator*
                (
                        tMat2x2<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return tMat2x2<T>(
                    m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                    m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                    m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                    m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1]);
        }





        template <typename T, typename U>
        inline tMat2x2<T> operator/
                (
                        tMat2x2<T> const & m,
                        typename TypeAid<U, 4>::CoreType const & s
                )
        {
            return tMat2x2<T>(
                    m[0] / s,
                    m[1] / s);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator/
                (
                        typename TypeAid<T, 4>::CoreType const & s,
                        tMat2x2<U> const & m
                )
        {
            return tMat2x2<T>(
                    s / m[0],
                    s / m[1]);
        }

        template <typename T, typename U>
        inline tVec2<T> operator/
                (
                        tMat2x2<T> const & m,
                        const tVec2<U> & v
                )
        {

            return compute_inverseMat2x2<T>::call(m)*v;
        }

        template <typename T, typename U>
        inline tVec2<T> operator/
                (
                        tVec2<T> const & v,
                        tMat2x2<U> const & m
                )
        {
            return v * compute_inverseMat2x2<U>::call(m);
        }

        template <typename T, typename U>
        inline tMat2x2<T> operator/
                (
                        tMat2x2<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            tMat2x2<T> m1_copy(m1);
            return m1_copy /= m2;
        }

        // Unary constant operators
        template <typename T>
        inline tMat2x2<T> const operator-
                (
                        tMat2x2<T> const & m
                )
        {
            return tMat2x2<T>(
                    -m[0],
                    -m[1]);
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tMat2x2<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return (m1[0] == m2[0]) && (m1[1] == m2[1]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tMat2x2<T> const & m1,
                        tMat2x2<U> const & m2
                )
        {
            return (m1[0] != m2[0]) || (m1[1] != m2[1]);
        }
    }
    typedef Detail::tMat2x2<PDP::UInteger> UMat2x2;
    typedef Detail::tMat2x2<PDP::Integer> IMat2x2;
    typedef Detail::tMat2x2<float> Mat2x2;
    typedef Detail::tMat2x2<double> DMat2x2;
    typedef Detail::tMat2x2<unsigned short> USMat2x2;
    typedef Detail::tMat2x2<short> SMat2x2;
    //typedef Detail::tMat2x2<PDP::Half> HMat2x2;
}
#endif
