//
//  Vec4.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Vec4_h
#define DataStructures_Vec4_h

#include "StaticArray.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename T>
        class tVec4
        {
        private:
            T InternalBuffer[4];
        public:
            inline tVec4();
            inline tVec4(const tVec4 & vector);
            inline tVec4(const tVec3<T> & vector);
            inline tVec4(const tVec2<T> & vector);

            inline tVec4(const T & a);
            inline tVec4(const T & a, const T & b, const T & c, const T & d);
            inline tVec4(const LD::StaticArray<T, 4> & a);

            inline tVec4 & operator = (const tVec4 & vector);
            inline tVec4 & operator = (const tVec3<T> & vector);
            inline tVec4 & operator = (const tVec2<T> & vector);
            inline tVec4 & operator = (const T & a );
            inline tVec4 & operator = (const StaticArray<T, 4> & a );

            inline tVec4 & Normalize()
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T & w = (*this)[3];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z + w*w);

                (*this)[0]/=magnitude;
                (*this)[1]/=magnitude;
                (*this)[2]/=magnitude;
                (*this)[3]/=magnitude;
                return (*this);

            }

            inline T & operator[](const PDP::UInteger & index)
            {
                return InternalBuffer[index];
            }

            inline const T & operator[](const PDP::UInteger & index) const
            {
                return InternalBuffer[index];
            }


            inline const tVec4 Normalized() const
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T & w = (*this)[3];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z + w*w);

                tVec4 temp(*this);

                temp[0]/=magnitude;
                temp[1]/=magnitude;
                temp[2]/=magnitude;
                temp[3]/=magnitude;
                return temp;
            }


            inline  const T Magnitude() const
            {
                const T & x = (*this)[0];
                const T & y = (*this)[1];
                const T & z = (*this)[2];
                const T & w = (*this)[3];
                const T magnitude = LD::Sqrt(x*x + y*y + z*z + w*w);
                return magnitude;
            }

            inline T & X(){return (*this)[0];}
            inline const T & X() const {return (*this)[0];}

            inline T & Y() {return (*this)[1];}
            inline const T & Y() const{return (*this)[1];}

            inline T & Z() {return (*this)[2];}
            inline const T & Z() const {return (*this)[2];}

            inline T & W() {return (*this)[3];}
            inline const T & W() const {return (*this)[3];}

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
            inline LD::Detail::tVec3<T> XXW() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[3]);
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
            inline LD::Detail::tVec3<T> XYW() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[3]);
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
            inline LD::Detail::tVec3<T> XZW() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> XWX() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> XWY() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> XWZ() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> XWW() const
            {
                return LD::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[3]);
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
            inline LD::Detail::tVec3<T> YXW() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[3]);
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
            inline LD::Detail::tVec3<T> YYW() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[3]);
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
            inline LD::Detail::tVec3<T> YZW() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> YWX() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> YWY() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> YWZ() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> YWW() const
            {
                return LD::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[3]);
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
            inline LD::Detail::tVec3<T> ZXW() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[3]);
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
            inline LD::Detail::tVec3<T> ZYW() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[3]);
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
            inline LD::Detail::tVec3<T> ZZW() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> ZWX() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> ZWY() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> ZWZ() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> ZWW() const
            {
                return LD::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[3]);
            }

            inline LD::Detail::tVec3<T> WXX() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> WXY() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> WXZ() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> WXW() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> WYX() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> WYY() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> WYZ() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> WYW() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> WZX() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> WZY() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> WZZ() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> WZW() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec3<T> WWX() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec3<T> WWY() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec3<T> WWZ() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec3<T> WWW() const
            {
                return LD::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[3]);
            }


            inline LD::Detail::tVec4<T> XXXX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XXXY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XXXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XXXW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XXYX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XXYY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XXYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XXYW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XXZX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XXZY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XXZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XXZW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XXWX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XXWY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XXWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XXWW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XYXX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XYXY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XYXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XYXW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XYYX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XYYY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XYYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XYYW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XYZX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XYZY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XYZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XYZW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XYWX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XYWY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XYWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XYWW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XZXX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XZXY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XZXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XZXW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XZYX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XZYY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XZYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XZYW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XZZX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XZZY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XZZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XZZW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XZWX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XZWY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XZWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XZWW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XWXX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XWXY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XWXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XWXW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XWYX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XWYY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XWYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XWYW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XWZX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XWZY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XWZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XWZW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> XWWX() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> XWWY() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> XWWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> XWWW() const
            {
                return LD::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[3]);
            }

            inline LD::Detail::tVec4<T> YXXX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YXXY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YXXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YXXW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YXYX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YXYY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YXYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YXYW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YXZX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YXZY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YXZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YXZW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YXWX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YXWY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YXWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YXWW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YYXX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YYXY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YYXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YYXW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YYYX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YYYY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YYYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YYYW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YYZX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YYZY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YYZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YYZW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YYWX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YYWY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YYWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YYWW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YZXX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YZXY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YZXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YZXW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YZYX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YZYY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YZYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YZYW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YZZX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YZZY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YZZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YZZW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YZWX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YZWY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YZWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YZWW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YWXX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YWXY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YWXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YWXW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YWYX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YWYY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YWYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YWYW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YWZX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YWZY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YWZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YWZW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> YWWX() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> YWWY() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> YWWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> YWWW() const
            {
                return LD::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[3]);
            }

            inline LD::Detail::tVec4<T> ZXXX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZXXY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZXXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZXXW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZXYX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZXYY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZXYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZXYW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZXZX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZXZY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZXZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZXZW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZXWX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZXWY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZXWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZXWW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZYXX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZYXY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZYXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZYXW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZYYX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZYYY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZYYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZYYW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZYZX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZYZY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZYZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZYZW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZYWX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZYWY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZYWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZYWW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZZXX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZZXY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZZXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZZXW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZZYX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZZYY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZZYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZZYW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZZZX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZZZY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZZZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZZZW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZZWX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZZWY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZZWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZZWW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZWXX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZWXY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZWXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZWXW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZWYX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZWYY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZWYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZWYW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZWZX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZWZY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZWZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZWZW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> ZWWX() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> ZWWY() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> ZWWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> ZWWW() const
            {
                return LD::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[3]);
            }


            inline LD::Detail::tVec4<T> WXXX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WXXY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WXXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WXXW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WXYX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WXYY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WXYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WXYW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WXZX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WXZY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WXZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WXZW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WXWX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WXWY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WXWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WXWW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WYXX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WYXY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WYXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WYXW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WYYX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WYYY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WYYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WYYW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WYZX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WYZY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WYZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WYZW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WYWX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WYWY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WYWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WYWW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WZXX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WZXY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WZXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WZXW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WZYX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WZYY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WZYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WZYW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WZZX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WZZY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WZZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WZZW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WZWX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WZWY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WZWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WZWW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WWXX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WWXY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WWXZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WWXW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WWYX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WWYY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WWYZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDLDP::Detail::tVec4<T> WWYW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WWZX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WWZY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WWZZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WWZW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline LD::Detail::tVec4<T> WWWX() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline LD::Detail::tVec4<T> WWWY() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline LD::Detail::tVec4<T> WWWZ() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline LD::Detail::tVec4<T> WWWW() const
            {
                return LD::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[3]);
            }
        };


        template<typename T>
        tVec4<T>::tVec4()
        {

        }

        template<typename T>
        tVec4<T>::tVec4(const tVec4 & vector)
        {
            (*this) = vector;
        }

        template<typename T>
        tVec4<T>::tVec4(const tVec3<T> & vector)
        {
            (*this) = vector;
        }

        template<typename T>
        tVec4<T>::tVec4(const tVec2<T> & vector)
        {
            (*this) = vector;
        }

        template<typename T>
        tVec4<T>::tVec4(const T & a)
        {
            (*this)[0] = (*this)[1] = (*this)[2] = (*this)[3] = a;
        }

        template<typename T>
        tVec4<T>::tVec4(const T & a,const T & b,const T & c,const T & d)
        {
            (*this)[0] = a;
            (*this)[1] = b;
            (*this)[2] = c;
            (*this)[3] = d;
        }


        template<typename T>
        tVec4<T>::tVec4(const StaticArray<T, 4> & a )
        {
            (*this)[0] = a[0];
            (*this)[1] = a[1];
            (*this)[2] = a[2];
            (*this)[3] = a[3];
        }


        template<typename T>
        tVec4<T> & tVec4<T>::operator=(const tVec2<T> &vector)
        {
            (*this)[0] = vector[0];
            (*this)[1] = vector[1];
            (*this)[2] = 0;
            (*this)[3] = 0;
            return (*this);
        }

        template<typename T>
        tVec4<T> & tVec4<T>::operator=(const tVec3<T> &vector)
        {
            (*this)[0] = vector[0];
            (*this)[1] = vector[1];
            (*this)[2] = vector[2];
            (*this)[3] = 0;
            return (*this);
        }

        template<typename T>
        tVec4<T> & tVec4<T>::operator=(const tVec4<T> &vector)
        {
            (*this)[0] = vector[0];
            (*this)[1] = vector[1];
            (*this)[2] = vector[2];
            (*this)[3] = vector[3];
            return (*this);
        }

        template<typename T>
        tVec4<T> & tVec4<T>::operator=(const T & a)
        {
            (*this)[0] = (*this)[1] = (*this)[2]= (*this)[3] = a;
            return (*this);
        }

        template<typename T>
        tVec4<T> & tVec4<T>::operator=(const StaticArray<T, 4> & a)
        {
            (*this)[0]  = a[0];
            (*this)[1] = a[1];
            (*this)[2] = a[2];
            (*this)[3] = a[3];
            return (*this);
        }



        template <typename T, typename U>
        inline tVec4<T>  & operator+=
                (
                        tVec4<T>  & a, const U & s
                )
        {
            a[0] += s;
            a[1] += s;
            a[2] += s;
            a[3] += s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator+=
                (
                        tVec4<T>  & a,tVec4<U> const & v
                )
        {
            a[0] += v[0];
            a[1] += v[1];
            a[2] += v[2];
            a[3] += v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator-=
                (
                        tVec4<T> & a, const U & s
                )
        {
            a[0] -= s;
            a[1] -= s;
            a[2] -=s;
            a[3] -= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator-=
                (
                        tVec4<T>  & a,tVec4<U> const & v
                )
        {
            a[0] -= v[0];
            a[1] -= v[1];
            a[2] -= v[2];
            a[3] -= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator*=
                (
                        tVec4<T>  & a,const U & s
                )
        {
            a[0] *= s;
            a[1] *= s;
            a[2] *= s;
            a[3] *= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator*=
                (
                        tVec4<T>  & a,tVec4<U> const & v
                )
        {
            a[0] *= v[0];
            a[1] *= v[1];
            a[2] *= v[2];
            a[3] *= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator/=
                (
                        tVec4<T>  & a,const U & s
                )
        {
            a[0] /= s;
            a[1] /= s;
            a[2] /= s;
            a[2] /= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T>  & operator/=
                (
                        tVec4<T>  & a,tVec4<U> const & v
                )
        {
            a[0] /= v[0];
            a[1] /= v[1];
            a[2] /= v[2];
            a[3] /= v[3];
            return a;
        }

        //////////////////////////////////////
        // Increment and decrement operators

        template <typename T>
        inline tVec4<T>  & operator++(tVec4<T>  & a)
        {
            ++a[0];
            ++a[1];
            ++a[2];
            ++a[3];
            return a;
        }

        template <typename T>
        inline tVec4<T>  & operator--(tVec4<T>  & a)
        {
            --a[0];
            --a[1];
            --a[2];
            --a[3];
            return a;
        }

        template <typename T>
        inline tVec4<T>  & operator++(tVec4<T>  & a,int)
        {
            a[0]++;
            a[1]++;
            a[2]++;
            a[3]++;
            return a;
        }

        template <typename T>
        inline tVec4<T>  & operator--(tVec4<T>  & a,int)
        {
            a[0]--;
            a[1]--;
            a[2]--;
            a[3]--;
            return a;
        }

        //////////////////////////////////////
        // Boolean operators

        template <typename T, typename U>
        inline bool operator==
                (
                        tVec4<T>  const & v1,
                        tVec4<U> const & v2
                )
        {
            return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2] && v1[3] == v2[3]);
        }

        template <typename T, typename U>
        inline bool operator!=
                (
                        tVec4<T>  const & v1,
                        tVec4<U> const & v2
                )
        {
            return (v1[0] != v2[0]) || (v1[1] != v2[1] || v1[2] != v2[2]) || (v1[3] == v2[3]);
        }

        //////////////////////////////////////
        // Unary bit operators

        template <typename T, typename U>
        inline tVec4<T> & operator%= (tVec4<T> & a, const U & s)
        {
            a[0]%=s;
            a[1]%=s;
            a[2]%=s;
            a[3]%=s;
            return s;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator%= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] %= v[0];
            a[1] %= v[1];
            a[2] %= v[2];
            a[3] %= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator&= (tVec4<T> & a, const U & s)
        {
            a[0] &= a;
            a[1] &= a;
            a[2] &= a;
            a[3] &= a;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator&= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] &= v[0];
            a[1] &= v[1];
            a[2] &= v[2];
            a[3] &= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator|= (tVec4<T> & a,const U & s)
        {
            a[0] |= s;
            a[1] |= s;
            a[2] |= s;
            a[3] |= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator|= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] |= v[0];
            a[1] |= v[1];
            a[2] |= v[2];
            a[3] |= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator^= (tVec4<T> & a,const U & s)
        {
            a[0] ^= s;
            a[1] ^= s;
            a[2] ^= s;
            a[3] ^= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator^= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] ^= v[0];
            a[1] ^= v[1];
            a[2] ^= v[2];
            a[3] ^= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator<<= (tVec4<T> & a, const U & s)
        {
            a[0] <<= s;
            a[1] <<= s;
            a[2] <<= s;
            a[3] <<= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator<<= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] <<= v[0];
            a[1] <<= v[1];
            a[2] <<= v[2];
            a[3] <<= v[3];
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator>>= (tVec4<T> & a, const U & s)
        {
            a[0] >>= s;
            a[1] >>= s;
            a[2] >>= s;
            a[3] >>= s;
            return a;
        }

        template <typename T, typename U>
        inline tVec4<T> & operator>>= (tVec4<T> & a,tVec4<U> const & v)
        {
            a[0] >>= v[0];
            a[1] >>= v[1];
            a[2] >>= a[2];
            a[3] >>= a[3];
            return a;
        }

        //////////////////////////////////////
        // Binary arithmetic operators

        template <typename T, typename U>
        inline tVec4<T> operator+
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] + s,
                    v[1] + s,
                    v[2] + s,
                    v[3] + s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator+
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s + v[0],
                    s + v[1],
                    s + v[2],
                    s + v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator+
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] + v2[0],
                    v1[1] + v2[1],
                    v1[2] + v2[2],
                    v1[3] + v2[3]);
        }

        //operator-
        template <typename T, typename U>
        inline tVec4<T> operator-
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0]- s,
                    v[1] - s,
                    v[2] - s,
                    v[3] - s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator-
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s - v[0],
                    s - v[1],
                    s - v[2],
                    s - v[2]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator-
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0]- v2[0],
                    v1[1] - v2[1],
                    v1[2] - v2[2],
                    v1[3] - v2[3]);
        }

        //operator*
        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] * s,
                    v[1] * s,
                    v[2] * s,
                    v[3] * s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s * v[0],
                    s * v[1],
                    s * v[2],
                    s * v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator*
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] * v2[0],
                    v1[1] * v2[1],
                    v1[2] * v2[2],
                    v1[3] * v2[3]);
        }

        //operator/
        template <typename T, typename U>
        inline tVec4<T> operator/
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] / s,
                    v[1] / s,
                    v[2] / s,
                    v[3] / s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator/
                (
                        T const & s,
                        tVec4<U>const & v
                )
        {
            return tVec4<T>(
                    s / v[0],
                    s / v[1],
                    s / v[2],
                    s / v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator/
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] / v2[0],
                    v1[1] / v2[1],
                    v1[2] / v2[2],
                    v1[3] / v2[3]);
        }

        // Unary constant operators
        template <typename T>
        inline tVec4<T> operator-
                (
                        tVec4<T> const & v
                )
        {
            return tVec4<T>(
                    -v[0],
                    -v[1],
                    -v[2],
                    -v[3]);
        }

        //////////////////////////////////////
        // Binary bit operators

        template <typename T, typename U>
        inline tVec4<T> operator%
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] % s,
                    v[1] % s,
                    v[2] % s,
                    v[3] % s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator%
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s % v[0],
                    s % v[1],
                    s % v[2],
                    s % v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator%
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] % v2[0],
                    v1[1] % v2[1],
                    v1[2] % v2[2],
                    v1[3] % v2[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator&
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] & s,
                    v[1] & s,
                    v[2] & s,
                    v[3] & s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator&
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s & v[0],
                    s & v[1],
                    s & v[2],
                    s & v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator&
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] & v2[0],
                    v1[1] & v2[1],
                    v1[2] & v2[2],
                    v1[3] & v2[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator|
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] | s,
                    v[1] | s,
                    v[2] | s,
                    v[3] | s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator|
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s | v[0],
                    s | v[1],
                    s | v[2],
                    s | v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator|
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] | v2[0],
                    v1[1] | v2[1],
                    v1[2] | v2[2],
                    v1[3] | v2[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator^
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] ^ s,
                    v[1] ^ s,
                    v[2] ^ s,
                    v[3] ^ s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator^
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s ^ v[0],
                    s ^ v[1],
                    s ^ v[2],
                    s ^ v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator^
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec3<T>(
                    v1[0] ^ v2[0],
                    v1[1] ^ v2[1],
                    v1[2] ^ v2[2],
                    v1[3] ^ v2[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator<<
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] << s,
                    v[1] << s,
                    v[2] << s,
                    v[3] << s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator<<
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s << v[0],
                    s << v[1],
                    s << v[2],
                    s << v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator<<
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] << v2[0],
                    v1[1] << v2[1],
                    v1[2] << v2[2],
                    v1[3] << v2[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator>>
                (
                        tVec4<T> const & v,
                        U const & s
                )
        {
            return tVec4<T>(
                    v[0] >> s,
                    v[1] >> s,
                    v[2] >> s,
                    v[3] >> s);
        }

        template <typename T, typename U>
        inline tVec4<T> operator>>
                (
                        T const & s,
                        tVec4<U> const & v
                )
        {
            return tVec4<T>(
                    s >> v[0],
                    s >> v[1],
                    s >> v[2],
                    s >> v[3]);
        }

        template <typename T, typename U>
        inline tVec4<T> operator>>
                (
                        tVec4<T> const & v1,
                        tVec4<U> const & v2
                )
        {
            return tVec4<T>(
                    v1[0] >> v2[0],
                    v1[1] >> v2[1],
                    v1[2] >> v2[2],
                    v1[3] >> v2[3]);
        }

        template <typename T>
        inline tVec4<T> operator~
                (
                        tVec4<T> const & v
                )
        {
            return tVec4<T>(
                    ~v[0],
                    ~v[1],
                    ~v[2],
                    ~v[3]);
        }

    }
    typedef Detail::tVec4<PDP::UInteger> UVec4;
    typedef Detail::tVec4<PDP::Integer> IVec4;
    typedef Detail::tVec4<float> Vec4;
    typedef Detail::tVec4<double> DVec4;
    typedef Detail::tVec4<unsigned short> USVec4;
    typedef Detail::tVec4<short> SVec4;
    //typedef Detail::tVec4<PDP::Half> HVec4;


}

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct StaticallySized<LD::Detail::tVec4<T>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif
