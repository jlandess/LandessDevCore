//
//  SoftwareQuaternion.h
//  DataStructures
//
//  Created by James Landess on 8/15/18.
//  Copyright © 2018 James Landess. All rights reserved.
//

#ifndef SoftwareQuaternion_h
#define SoftwareQuaternion_h

#include "Definitions/Common.hpp"

//#include <Algorithms/SSE_Trig.h>
#include "Algorithms/Periodic.hpp"

#include "Algorithms/Exponential.hpp"

namespace LD
{
    template<typename T, class = void>
    class Quaternion
    {

    };

    template<typename T>
    class Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>
    {
    private:
        T x;
        T y;
        T z;
        T w;
    public:
        Quaternion();
        Quaternion(const T & x, const T & y, const T & z, const T & w);



        void SetX(const T & x);
        void SetY(const T & y);
        void SetZ(const T & z);
        void SetW(const T & w);



        const T & X() const;
        const T & Y() const;
        const T & Z() const;
        const T & W() const;



        const T Magnitude() const;



        void Normalize();



        void Conjugate();



        void Multiply(const Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>> & q);
        void Multiply(const T & v_x, const T & v_y, const T & v_z);

        void Print();
    };

    template<typename T>
    Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Quaternion()
    {

    }


    template<typename T>
    Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Quaternion(const T & x0, const T & y0, const T & z0, const T & w0)
    {
        this->x = x0;
        this->y = y0;
        this->z = z0;
        this->w = w0;
    }

    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::SetX(const T & x0)
    {
        this->x = x0;
    }

    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::SetY(const T & y0)
    {
        this->y = y0;
    }


    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::SetZ(const T & z0)
    {
        this->z = z0;
    }

    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::SetW(const T & w0)
    {
        this->w = w0;
    }

    template<typename T>
    const T & Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::X() const
    {
        return x;
    }

    template<typename T>
    const T & Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Y() const
    {
        return y;
    }

    template<typename T>
    const T & Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Z() const
    {
        return z;
    }

    template<typename T>
    const T & Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::W() const
    {
        return w;
    }

    template<typename T>
    const T Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Magnitude() const
    {
        return (T) PDP::Sqrt(
                x * x + y * y + z * z
                + w * w);
    }


    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Normalize()
    {
        T mag = Magnitude();
        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }

    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Conjugate()
    {
        T x_prime = -x;
        T y_prime = -y;
        T z_prime = -z;
        T w_prime = w;
        x = x_prime;
        y = y_prime;
        z = z_prime;
        w = w_prime;
    }

    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Multiply(const Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>> & q)
    {
        T w_prime = w * q.w - x * q.x - y * q.y - z * q.z;
        T x_prime = x * q.w + w * q.x + y * q.z - z * q.y;
        T y_prime = y * q.w + w * q.y + z * q.x - x * q.z;
        T z_prime = z * q.w + w * q.z + x * q.y - y * q.x;


        x = x_prime;
        y = y_prime;
        z = z_prime;
        w = w_prime;

    }


    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Multiply(const T & v_x, const T & v_y, const T & v_z)
    {

        T x_prime = w * v_x + y * v_z - z * v_y;
        T y_prime = w * v_y + z * v_x - x * v_z;
        T z_prime = w * v_z + x * v_y - y * v_x;
        T w_prime = -x * v_x - y * v_y - z * v_z;


        x = x_prime;
        y = y_prime;
        z = z_prime;
        w = w_prime;
    }


    template<typename T>
    void Quaternion<T,PDP::Enable_If_T<PDP::IsPrimitiveType<T>::value>>::Print()
    {
        //printf("quaternion: %f %f %f %f\n", x, y, z, w);
    }
}
#endif /* SoftwareQuaternion_h */
