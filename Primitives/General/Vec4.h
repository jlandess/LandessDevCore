//
//  Vec4.h
//  DataStructures
//
//  Created by James Landess on 11/11/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef DataStructures_Vec4_h
#define DataStructures_Vec4_h

#include "StaticArray.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Algorithms/Exponential.h"
namespace PDP
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
            inline tVec4(const StaticArray<T, 4> & a);

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
                const T magnitude = PDP::Sqrt(x*x + y*y + z*z + w*w);

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
                const T magnitude = PDP::Sqrt(x*x + y*y + z*z + w*w);

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
                const T magnitude = PDP::Sqrt(x*x + y*y + z*z + w*w);
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

            inline PDP::Detail::tVec2<T> XX() const
            {
                return PDP::Detail::tVec2<T>((*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec2<T> XY() const
            {
                return PDP::Detail::tVec2<T>((*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec2<T> XZ() const
            {
                return PDP::Detail::tVec2<T>((*this)[0],(*this)[2]);
            }

            inline PDP::Detail::tVec2<T> YY() const
            {
                return PDP::Detail::tVec2<T>((*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec2<T> YX() const
            {
                return PDP::Detail::tVec2<T>((*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec2<T> YZ() const
            {
                return PDP::Detail::tVec2<T>((*this)[1],(*this)[2]);
            }

            inline PDP::Detail::tVec2<T> ZX() const
            {
                return PDP::Detail::tVec2<T>((*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec2<T> ZY() const
            {
                return PDP::Detail::tVec2<T>((*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec2<T> ZZ() const
            {
                return PDP::Detail::tVec2<T>((*this)[2],(*this)[2]);
            }

            inline PDP::Detail::tVec3<T> XXX() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> XXY() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> XXZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> XXW() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> XYX() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> XYY() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> XYZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> XYW() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> XZX() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> XZY() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> XZZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> XZW() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> XWX() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> XWY() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> XWZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> XWW() const
            {
                return PDP::Detail::tVec3<T>((*this)[0],(*this)[3],(*this)[3]);
            }

            inline PDP::Detail::tVec3<T> YXX() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> YXY() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> YXZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> YXW() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> YYX() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> YYY() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> YYZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> YYW() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> YZX() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> YZY() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> YZZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> YZW() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> YWX() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> YWY() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> YWZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> YWW() const
            {
                return PDP::Detail::tVec3<T>((*this)[1],(*this)[3],(*this)[3]);
            }



            inline PDP::Detail::tVec3<T> ZXX() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> ZXY() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> ZXZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> ZXW() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> ZYX() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> ZYY() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> ZYZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> ZYW() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> ZZX() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> ZZY() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> ZZZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> ZZW() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> ZWX() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> ZWY() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> ZWZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> ZWW() const
            {
                return PDP::Detail::tVec3<T>((*this)[2],(*this)[3],(*this)[3]);
            }

            inline PDP::Detail::tVec3<T> WXX() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> WXY() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> WXZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> WXW() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> WYX() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> WYY() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> WYZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> WYW() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> WZX() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> WZY() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> WZZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> WZW() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec3<T> WWX() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec3<T> WWY() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec3<T> WWZ() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec3<T> WWW() const
            {
                return PDP::Detail::tVec3<T>((*this)[3],(*this)[3],(*this)[3]);
            }


            inline PDP::Detail::tVec4<T> XXXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XXXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XXXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XXXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XXYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XXYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XXYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XXYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XXZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XXZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XXZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XXZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XXWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XXWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XXWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XXWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XYXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XYXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XYXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XYXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XYYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XYYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XYYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XYYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XYZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XYZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XYZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XYZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XYWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XYWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XYWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XYWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XZXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XZXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XZXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XZXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XZYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XZYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XZYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XZYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XZZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XZZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XZZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XZZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XZWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XZWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XZWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XZWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XWXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XWXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XWXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XWXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XWYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XWYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XWYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XWYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XWZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XWZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XWZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XWZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> XWWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> XWWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> XWWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> XWWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[0],(*this)[3],(*this)[3],(*this)[3]);
            }

            inline PDP::Detail::tVec4<T> YXXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YXXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YXXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YXXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YXYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YXYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YXYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YXYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YXZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YXZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YXZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YXZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YXWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YXWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YXWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YXWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YYXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YYXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YYXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YYXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YYYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YYYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YYYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YYYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YYZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YYZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YYZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YYZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YYWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YYWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YYWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YYWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YZXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YZXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YZXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YZXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YZYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YZYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YZYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YZYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YZZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YZZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YZZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YZZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YZWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YZWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YZWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YZWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YWXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YWXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YWXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YWXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YWYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YWYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YWYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YWYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YWZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YWZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YWZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YWZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> YWWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> YWWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> YWWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> YWWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[1],(*this)[3],(*this)[3],(*this)[3]);
            }

            inline PDP::Detail::tVec4<T> ZXXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZXXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZXXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZXXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZXYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZXYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZXYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZXYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZXZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZXZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZXZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZXZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZXWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZXWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZXWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZXWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZYXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZYXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZYXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZYXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZYYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZYYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZYYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZYYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZYZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZYZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZYZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZYZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZYWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZYWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZYWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZYWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZZXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZZXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZZXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZZXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZZYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZZYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZZYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZZYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZZZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZZZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZZZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZZZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZZWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZZWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZZWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZZWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZWXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZWXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZWXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZWXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZWYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZWYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZWYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZWYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZWZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZWZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZWZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZWZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> ZWWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> ZWWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> ZWWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> ZWWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[2],(*this)[3],(*this)[3],(*this)[3]);
            }


            inline PDP::Detail::tVec4<T> WXXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WXXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WXXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WXXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WXYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WXYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WXYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WXYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WXZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WXZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WXZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WXZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WXWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WXWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WXWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WXWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WYXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WYXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[0],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WYXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WYXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WYYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WYYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WYYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WYYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WYZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WYZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WYZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WYZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WYWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WYWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WYWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WYWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[1],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WZXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WZXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WZXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WZXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WZYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WZYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WZYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WZYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WZZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WZZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WZZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WZZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WZWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WZWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WZWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WZWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[2],(*this)[3],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WWXX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WWXY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WWXZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WWXW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[0],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WWYX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WWYY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WWYZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WWYW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[1],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WWZX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WWZY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WWZZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WWZW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[2],(*this)[3]);
            }
            inline PDP::Detail::tVec4<T> WWWX() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[0]);
            }
            inline PDP::Detail::tVec4<T> WWWY() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[1]);
            }
            inline PDP::Detail::tVec4<T> WWWZ() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[2]);
            }
            inline PDP::Detail::tVec4<T> WWWW() const
            {
                return PDP::Detail::tVec4<T>((*this)[3],(*this)[3],(*this)[3],(*this)[3]);
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
#endif
