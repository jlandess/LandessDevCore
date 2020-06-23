//
//  Per.h
//  DataStructures
//
//  Created by James Landess on 11/28/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __DataStructures__Per__
#define __DataStructures__Per__

#include "Definitions/Float.hpp"
#include "Definitions/Integer.hpp"
#include "vdt/vdtmath.h"

namespace LD
{
    inline LD::Float Sin(const LD::Float & x)
    {
        return vdt::fast_sin(x);
    }
    inline LD::Float Cos(const LD::Float & x)
    {
        return vdt::fast_cos(x);
    }
    inline LD::Float Tan(const LD::Float & x)
    {
        return vdt::fast_tan(x);
    }
    inline LD::Float Csc(const LD::Float & x)
    {
        return 1.0/vdt::fast_sin(x);
    }
    inline LD::Float Sec(const LD::Float & x)
    {
        return 1.0/vdt::fast_cos(x);
    }
    inline LD::Float CoTan(const LD::Float & x)
    {
        return 1.0/vdt::fast_tan(x);
    }
    inline LD::Float ASin(const LD::Float & x)
    {

        return vdt::fast_asin(x);
    }
    inline LD::Float ACos(const LD::Float & x)
    {
        return vdt::fast_cos(x);
    }
    inline LD::Float ATan(const LD::Float & x)
    {
        return vdt::fast_atan(x);
    }
    inline LD::Float ASec(const LD::Float & x)
    {
        return LD::Float(1.0)/vdt::fast_acos(x);
    }
    inline LD::Float ACsc(const LD::Float & x)
    {
        return LD::Float(1.0)/vdt::fast_asin(x);
    }
    inline LD::Float ACoTan(const LD::Float & x)
    {
        return LD::Float(1.0)/vdt::fast_atan(x);
    }

    //return (PDP::Exp(floatingPointNumber,precision) - PDP::Exp(-floatingPointNumber,precision))/PDP::Float(2.0);
    //return (PDP::Exp(floatingPointNumber,precision) + PDP::Exp(-floatingPointNumber,precision))/PDP::Float(2.0);
    inline LD::Float Sinh(const LD::Float & floatingPointNumber)
    {
        return (vdt::fast_exp(floatingPointNumber) - vdt::fast_exp(-floatingPointNumber))*0.5;
    }
    inline LD::Float Cosh(const LD::Float & floatingPointNumber)
    {
        return (vdt::fast_exp(floatingPointNumber) + vdt::fast_exp(-floatingPointNumber))*0.5;
    }
    inline LD::Float Tanh(const LD::Float & floatingPointNumber)
    {
        LD::Float a = (vdt::fast_exp(floatingPointNumber) - vdt::fast_exp(-floatingPointNumber))*0.5;
        LD::Float b = (vdt::fast_exp(floatingPointNumber) + vdt::fast_exp(-floatingPointNumber))*0.5;

        return a/b;
    }
    //return PDP::Ln(floatingPointNumber + PDP::Sqrt(1 + floatingPointNumber*floatingPointNumber,precision),precision);
    //return PDP::Ln(floatingPointNumber + PDP::Sqrt(floatingPointNumber*floatingPointNumber -1 ,precision),precision);
    inline LD::Float ASinh(const LD::Float & floatingPointNumber)
    {

        return vdt::fast_log(floatingPointNumber + vdt::fast_approx_isqrt(1 + floatingPointNumber*floatingPointNumber));
    }
    inline LD::Float ACosh(const LD::Float & floatingPointNumber)
    {
        return vdt::fast_log(floatingPointNumber + vdt::fast_approx_isqrt(floatingPointNumber*floatingPointNumber -1));
    }

    //return PDP::Float(0.5)*PDP::Ln((1+floatingPointNumber)/(1-floatingPointNumber),precision);
    inline LD::Float ATanh(const LD::Float & floatingPointNumber)
    {
        return LD::Float(0.5)*vdt::fast_log(1+floatingPointNumber)/(1-floatingPointNumber);
    }
    inline LD::Float ATan2(const LD::Float & y, const LD::Float & x)
    {
        if (x == 0 && y == 0)
        {
            return 0;
        }else if (x < 0 && y == 0)
        {
            return 3.14;
        }else if (x <= 0 && y != 0)
        {

            return LD::Float(2.0)*LD::ATan((vdt::fast_approx_isqrt(x*x + y*y) - x)/y);
        }
        return LD::Float(2.0)*LD::ATan(y/(vdt::fast_approx_isqrt(x*x + y*y) + x));
    }
    inline LD::Float SquareWave(const LD::Float & period, const LD::Float & phaseShift ,const LD::Float & t)
    {
        return LD::Sin(t / period * PDP::Float(2.0) * PDP::Float(3.14) *PDP::Float(2.0) + phaseShift)>= PDP::Float(0.0)  ? PDP::Float(1.0) :PDP::Float(-1.0);
    }
    inline PDP::Float TriangleWave(const LD::Float & period, const LD::Float & phaseShift ,const LD::Float & t)
    {
        LD::Float a = LD::Float(0.5) * (t+phaseShift)/period  + LD::Float(1)/LD::Float(4);
        LD::Float fraction = a - (LD::UInteger)a;
        return LD::Float(1.0) - LD::Float(4.0) * (LD::Float(0.5) - fraction);
    }
    inline LD::Float SawToothWave(const LD::Float & period, const LD::Float & phaseShift ,const LD::Float & t)
    {
        //return a -  (PDP::UInteger)a;
        LD::Float a = t/period + phaseShift;


        return  a -  (LD::UInteger)a;
    }






}
#endif /* defined(__DataStructures__Per__) */