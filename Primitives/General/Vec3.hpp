//
//  Vec3.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Vec3_h
#define DataStructures_Vec3_h

#include "StaticArray.hpp"
#include "Vec2.hpp"
#include "Quaternion.hpp"
#include "TypeTraits/IsClassType.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tVec3
        {
        private:
            T InternalBuffer[3];
        public:
            inline tVec3();
            inline tVec3(const tVec3 & vector);
            //inline tVec3(const tVec2<T> & vector);
            inline tVec3(const LD::StaticArray<T, 3> & a);

            inline tVec3(const T & a);
            inline tVec3(const T & a, const T & b, const T & c);

            inline tVec3 & operator = (const tVec3 & vector);
            //inline tVec3 & operator = (const tVec2<T> & vector);
            inline tVec3 & operator = (const T & a);
            inline tVec3 & operator = (const LD::StaticArray<T, 3> & a);

            inline tVec3 & Normalize()
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z);

                (*this)[0]/=magnitude;
                (*this)[1]/=magnitude;
                (*this)[2]/=magnitude;
                return (*this);

            }


            inline tVec3 & Cross(const tVec3 & v)
            {
                tVec3 result;


                result.X() = this->Y() * v.Z() - this->Z() * v.Y();
                result.Y() = this->Z() * v.X() - this->X() * v.Z();
                result.Z() = this->X() * v.Y() - this->Y() * v.X();
                return result;
            }

            inline const tVec3 Normalized() const
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z);

                tVec3 temp(*this);

                temp[0]/=magnitude;
                temp[1]/=magnitude;
                temp[2]/=magnitude;
                return temp;
            }


            inline  const T Magnitude() const
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z);
                return magnitude;
            }


            tVec3 & Rotate(const T & radians, const tVec3 & axis)
            {
                T sin_half = PDP::Sin(radians / 2);
                T cos_half = PDP::Cos(radians / 2);
                T r_x = axis.X() * sin_half;
                T r_y = axis.Y() * sin_half;
                T r_z = axis.Z() * sin_half;
                T r_w = cos_half;

                LD::Quaternion<T,LD::EnableIf<LD::IsPrimitive<T>>> rotation(r_x,r_y,r_w);

                //quaternion rotation(r_x, r_y, r_z, r_w);
                LD::Quaternion<T,LD::EnableIf<LD::IsPrimitive<T>>> c = rotation;
                c.Conjugate();


                LD::Quaternion<T,LD::EnableIf<LD::IsPrimitive<T>>> w = rotation;


                w.Multiply(this->X(), this->Y(), this->Z());
                w.Multiply(c);


                this->X() = w.X();
                this->Y() = w.Y();
                this->Z() = w.Z();
            }

            inline T & operator[](const PDP::UInteger & index)
            {
                return InternalBuffer[index];
            }

            inline const T & operator[](const PDP::UInteger & index) const
            {
                return InternalBuffer[index];
            }

            inline T & X(){return (*this)[0];}
            inline const T & X() const{return (*this)[0];}

            inline T & Y(){return (*this)[1];}
            inline const T & Y() const {return (*this)[1];}

            inline T & Z() {return (*this)[2];}
            inline const T & Z() const {return (*this)[2];}

            inline LD::Detail::tVec2<T> XX() const
            {
                return LD::Detail::tVec2<T>((*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec2<T> XY() const
            {
                return LD::Detail::tVec2<T>((*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec2<T> XZ() const
            {
                return LD::Detail::tVec2<T>((*this)[0],(*this)[2]);
            }

            inline LD::Detail::tVec2<T> YY() const
            {
                return LD::Detail::tVec2<T>((*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec2<T> YX() const
            {
                return LD::Detail::tVec2<T>((*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec2<T> YZ() const
            {
                return LD::Detail::tVec2<T>((*this)[1],(*this)[2]);
            }

            inline LD::Detail::tVec2<T> ZX() const
            {
                return LD::Detail::tVec2<T>((*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec2<T> ZY() const
            {
                return LD::Detail::tVec2<T>((*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec2<T> ZZ() const
            {
                return LD::Detail::tVec2<T>((*this)[2],(*this)[2]);
            }

            inline LD::Detail::tVec3<T> XXX() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> XXY() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> XXZ() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> XYX() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> XYY() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> XYZ() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> XZX() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> XZY() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> XZZ() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[2]);
            }


            inline LD::Detail::tVec3<T> YXX() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> YXY() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> YXZ() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> YYX() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> YYY() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> YYZ() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> YZX() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> YZY() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> YZZ() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[2]);
            }



            inline LD::Detail::tVec3<T> ZXX() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> ZXY() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> ZXZ() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> ZYX() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> ZYY() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> ZYZ() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> ZZX() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> ZZY() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> ZZZ() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[2]);
            }




        };

        template<typename T>
        tVec3<T>::tVec3()
        {
            (*this)[0] = (*this)[1] = (*this)[2] = 0;
        }

        template<typename T>
        tVec3<T>::tVec3(const tVec3 & vector)
        {
            (*this) = vector;
        }
        /*
         template<typename T>
         tVec3<T>::tVec3(const tVec2<T> & vector)
         {
         (*this) = vector;
         }
         */
        template<typename T>
        tVec3<T>::tVec3(const StaticArray<T, 3> & a)
        {
            (*this)[0] = a[0];
            (*this)[1] = a[1];
            (*this)[2] = a[2];
        }

        template<typename T>
        tVec3<T>::tVec3(const T & a)
        {
            (*this)[0] = (*this)[1] = (*this)[2] = a;
        }


        template<typename T>
        tVec3<T>::tVec3(const T & a, const T & b, const T & c)
        {
            (*this)[0] = a;
            (*this)[1] = b;
            (*this)[2] = c;
        }

        /*
         template<typename T>
         tVec3<T> & tVec3<T>::operator=(const tVec2<T> &vector)
         {
         (*this)[0] = vector[0];
         (*this)[1] = vector[1];
         (*this)[2] = 0;
         return (*this);
         }
         */

        template<typename T>
        tVec3<T> & tVec3<T>::operator=(const tVec3<T> &vector)
        {
            (*this)[0] = vector[0];
            (*this)[1] = vector[1];
            (*this)[2] = vector[2];
            return (*this);
        }

        template<typename T>
        tVec3<T> & tVec3<T>::operator=(const T & a)
        {
            (*this)[0] = (*this)[1] = (*this)[2] = a;
            return (*this);
        }

        template<typename T>
        tVec3<T> & tVec3<T>::operator=(const StaticArray<T, 3> & a)
        {
            (*this)[0] = a[0];
            (*this)[1] = a[1];
            (*this)[2] = a[2];
        }



        template <typename T, typename U>
        inline tVec3<T>  & operator+=
                (
                        tVec3<T>  & a, const U & s
                )
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator+=
                (
                        tVec3<T>  & a,tVec3<U> const & v
                )
        {
            a[0] += v[0];
            a[1] += v[1];
            a[2] += v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator-=
                (
                        tVec3<T> & a, const U & s
                )
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator-=
                (
                        tVec3<T>  & a,tVec3<U> const & v
                )
        {
            a[0] -= v[0];
            a[1] -= v[1];
            a[2] -= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator*=
                (
                        tVec3<T>  & a,const U & s
                )
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator*=
                (
                        tVec3<T>  & a,tVec3<U> const & v
                )
        {
            a[0] *= v[0];
            a[1] *= v[1];
            a[2] *= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator/=
                (
                        tVec3<T>  & a,const U & s
                )
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T>  & operator/=
                (
                        tVec3<T>  & a,tVec3<U> const & v
                )
        {
            a[0] /= v[0];
            a[1] /= v[1];
            a[2] /= v[2];
            return a;
        }

        //////////////////////////////////////
        // Increment and decrement operators

        template <typename T>
        inline tVec3<T>  & operator++(tVec3<T>  & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            return a;
        }

        template <typename T>
        inline tVec3<T>  & operator--(tVec3<T>  & a)
        {
            --a[0];
            --a[1];
            --a[2];
            return a;
        }

        template <typename T>
        inline tVec3<T>  & operator++(tVec3<T>  & a,int)
        {
            a[0]++;
            a[1]++;
            a[2]++;
            return a;
        }

        template <typename T>
        inline tVec3<T>  & operator--(tVec3<T>  & a,int)
        {
            a[0]--;
            a[1]--;
            a[2]--;
            return a;
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tVec3<T>  const & v1,
                        tVec3<U> const & v2
                )
        {
            return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tVec3<T>  const & v1,
                        tVec3<U> const & v2
                )
        {
            return (v1[0] != v2[0]) || (v1[1] != v2[1] || v1[2] != v2[2]);
        }

        //////////////////////////////////////
        // Unary bit operators

        template <typename T, typename U>
        inline tVec3<T> & operator%= (tVec3<T> & a, const U & s)
        {
            a[0]%=s;
            a[1]%=s;
            a[2]%=s;
            return s;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator%= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] %= v[0];
            a[1] %= v[1];
            a[2] %= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator&= (tVec3<T> & a, const U & s)
        {
            a[0] &= a;
            a[1] &= a;
            a[2] &= a;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator&= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] &= v[0];
            a[1] &= v[1];
            a[2] &= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator|= (tVec3<T> & a,const U & s)
        {
            a[0] |= s;
            a[1] |= s;
            a[2] |= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator|= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] |= v[0];
            a[1] |= v[1];
            a[2] |= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator^= (tVec3<T> & a,const U & s)
        {
            a[0] ^= s;
            a[1] ^= s;
            a[2] ^= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator^= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] ^= v[0];
            a[1] ^= v[1];
            a[2] ^= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator<<= (tVec3<T> & a, const U & s)
        {
            a[0] <<= s;
            a[1] <<= s;
            a[2] <<= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator<<= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] <<= v[0];
            a[1] <<= v[1];
            a[2] <<= v[2];
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator>>= (tVec3<T> & a, const U & s)
        {
            a[0] >>= s;
            a[1] >>= s;
            a[2] >>= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec3<T> & operator>>= (tVec3<T> & a,tVec3<U> const & v)
        {
            a[0] >>= v[0];
            a[1] >>= v[1];
            a[2] >>= a[2];
            return a;
        }

        //////////////////////////////////////
        // Binary arithmetic operators

        template <typename T, typename U>
        inline tVec3<T> operator+
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] + s,
                    v[1] + s,
                    v[2] + s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator+
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s + v[0],
                    s + v[1],
                    s + v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator+
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] + v2[0],
                    v1[1] + v2[1],
                    v1[2] + v2[2]);
        }

        //operator-
        template <typename T, typename U>
        inline tVec3<T> operator-
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0]- s,
                    v[1] - s,
                    v[2] - s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator-
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s - v[0],
                    s - v[1],
                    s - v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator-
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0]- v2[0],
                    v1[1] - v2[1],
                    v1[2] - v2[2]);
        }

        //operator*
        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] * s,
                    v[1] * s,
                    v[2] * s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s * v[0],
                    s * v[1],
                    s * v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator*
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] * v2[0],
                    v1[1] * v2[1],
                    v1[2] * v2[2]);
        }

        //operator/
        template <typename T, typename U>
        inline tVec3<T> operator/
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] / s,
                    v[1] / s,
                    v[2] / s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator/
                (
                        T const & s,
                        tVec3<U>const & v
                )
        {
            return tVec3<T>(
                    s / v[0],
                    s / v[1],
                    s / v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator/
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] / v2[0],
                    v1[1] / v2[1],
                    v1[2] / v2[2]);
        }

        // Unary constant operators
        template <typename T>
        inline tVec3<T> operator-
                (
                        tVec3<T> const & v
                )
        {
            return tVec3<T>(
                    -v[0],
                    -v[1],
                    -v[2]);
        }

        //////////////////////////////////////
        // Binary bit operators

        template <typename T, typename U>
        inline tVec3<T> operator%
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] % s,
                    v[1] % s,
                    v[2] % s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator%
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s % v[0],
                    s % v[1],
                    s % v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator%
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] % v2[0],
                    v1[1] % v2[1],
                    v1[2] % v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator&
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] & s,
                    v[1] & s,
                    v[2] & s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator&
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s & v[0],
                    s & v[1],
                    s & v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator&
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] & v2[0],
                    v1[1] & v2[1],
                    v1[2] & v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator|
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] | s,
                    v[1] | s,
                    v[2] | s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator|
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s | v[0],
                    s | v[1],
                    s | v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator|
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] | v2[0],
                    v1[1] | v2[1],
                    v1[2] | v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator^
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] ^ s,
                    v[1] ^ s,
                    v[2] ^ s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator^
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s ^ v[0],
                    s ^ v[1],
                    s ^ v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator^
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] ^ v2[0],
                    v1[1] ^ v2[1],
                    v1[2] ^ v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator<<
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] << s,
                    v[1] << s,
                    v[2] << s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator<<
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s << v[0],
                    s << v[1],
                    s << v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator<<
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] << v2[0],
                    v1[1] << v2[1],
                    v1[2] << v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator>>
                (
                        tVec3<T> const & v,
                        U const & s
                )
        {
            return tVec3<T>(
                    v[0] >> s,
                    v[1] >> s,
                    v[2] >> s);
        }

        template <typename T, typename U>
        inline tVec3<T> operator>>
                (
                        T const & s,
                        tVec3<U> const & v
                )
        {
            return tVec3<T>(
                    s >> v[0],
                    s >> v[1],
                    s >> v[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator>>
                (
                        tVec3<T> const & v1,
                        tVec3<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] >> v2[0],
                    v1[1] >> v2[1],
                    v1[2] >> v2[2]);
        }

        template <typename T, typename U>
        inline tVec3<T> operator~
                (
                        tVec3<T> const & v
                )
        {
            return tVec3<T>(
                    ~v[0],
                    ~v[1],
                    ~v[2]);
        }
    }
    typedef Detail::tVec3<PDP::UInteger> UVec3;
    typedef Detail::tVec3<PDP::Integer> IVec3;
    typedef Detail::tVec3<float> Vec3;
    typedef Detail::tVec3<double> DVec3;
    typedef Detail::tVec3<short> SVec3;
    typedef Detail::tVec3<unsigned short> USVec3;
    //typedef Detail::tVec3<PDP::Half> HVec3;
}


namespace LD
{
    template<typename T>
    using BasicVec3D = LD::Detail::tVec3<T>;
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tVec3<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
