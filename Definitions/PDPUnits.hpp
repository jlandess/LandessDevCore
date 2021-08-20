//
//  Units.h
//  DataStructures
//
//  Created by James Landess on 11/16/16.
//  Copyright (c) 2016 James Landess. All rights reserved.
//

#ifndef __DataStructures__Units__
#define __DataStructures__Units__

//#include "Common.hpp"
#include "TypeTraits/IsKindOf.hpp"
#include "TypeTraits/Detection.hpp"
#include "Algorithms/Move.hpp"
namespace PDP
{
#define CreateUnitsClassImplementation(name) private: T CurrentValue; public: inline name():CurrentValue(LD::Move(T())){} inline name(const T & value):CurrentValue(value){} inline name & operator = (const T & value){this->CurrentValue = value; return(*this);}  inline const T & Get##name() const {return this->CurrentValue;} inline void Set##name(const T & value){this->CurrentValue = value;} inline void SetValue(const T & value){this->CurrentValue = value;} virtual const T & GetValue() const {return this->CurrentValue;}
    
    
    
    template<typename T>
    class Unit
    {
    public:
        
        virtual const T & GetValue() const = 0;
    };
    
    
    template<typename T>
    class ConstantUnit: public PDP::Unit<T>
    {
    private:
        T ConstantValue;
    public:
        
        virtual const T & GetValue() const
        {
            return this->ConstantValue;
        }
    };
    
    
    
    
    
    template<PDP::UInteger Numerator, PDP::UInteger Denominator, template<typename> class UnitType, typename FloatingPointType ,class>
    class RadicandUnit
    {
    private:
        
    public:
        
        
        
        
        
        
    };
    
    template<PDP::UInteger Power,template<typename> class UnitType, typename FloatingPointType>
    class ExponentialUnit
    {
    private:
        
        inline ExponentialUnit();
        
        
        template<PDP::UInteger SecondaryPower>
        ExponentialUnit<Power+SecondaryPower, UnitType, FloatingPointType> operator * (const ExponentialUnit<SecondaryPower, UnitType, FloatingPointType> & exponentialUnit) const;
        
        template<PDP::UInteger SecondaryPower>
        ExponentialUnit<Power+SecondaryPower, UnitType, FloatingPointType> operator / (const ExponentialUnit<SecondaryPower, UnitType, FloatingPointType> & exponentialUnit) const;
        
    };
    
    template<PDP::UInteger Numerator, PDP::UInteger Denominator, template<typename> class UnitType, typename FloatingPointType>
    class RadicandUnit<Numerator, Denominator, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>>
    {
    private:
        
        UnitType<FloatingPointType> Radicand;
        
    public:
        inline RadicandUnit();
        
        template<PDP::UInteger Numerator1,PDP::UInteger Denominator1>
        RadicandUnit<Numerator*Numerator1, Denominator*Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>>
        operator * (const RadicandUnit<Numerator1, Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>> & a);
        
        
        
        
        template<PDP::UInteger Numerator1,PDP::UInteger Denominator1>
        RadicandUnit<Numerator+Numerator1, Denominator*Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>>
        operator *= (const RadicandUnit<Numerator1, Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>> & a);
        
        
        template<PDP::UInteger Numerator1,PDP::UInteger Denominator1>
        RadicandUnit<Numerator*Numerator1, Denominator*Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>>
        operator / (const RadicandUnit<Numerator1, Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>> & a);
        
        
        template<PDP::UInteger Numerator1,PDP::UInteger Denominator1>
        RadicandUnit<Numerator*Numerator1, Denominator*Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>>
        operator /= (const RadicandUnit<Numerator1, Denominator1, UnitType, FloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<FloatingPointType>, UnitType<FloatingPointType>>::value>> & a);

        
    };
    
    
    template<template<typename> class PrimaryUnitType, typename PrimaryFloatingPointType ,template<typename> class SecondaryUnitType, typename SecondaryFloatingPointType ,class>
    class RadicalUnit
    {

        
    };
    
    
    
    
    template<template<typename> class IndexUnitType, typename PrimaryFloatingPointType ,template<typename> class RadicanUnitType, typename SecondaryFloatingPointType>
    class RadicalUnit<IndexUnitType, PrimaryFloatingPointType, RadicanUnitType, SecondaryFloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<PrimaryFloatingPointType>, IndexUnitType<PrimaryFloatingPointType>>::value && LD::Detail::IsBaseOf<PDP::Unit<SecondaryFloatingPointType>, RadicanUnitType<SecondaryFloatingPointType>>::value>>
    {
    private:
        IndexUnitType<PrimaryFloatingPointType> Index;
        RadicanUnitType<SecondaryFloatingPointType> Radicand;
        
    public:
    };
    
    
    
    /**
     @tparam Numerator - The numerator of the power related to the radicand
     @tparam Denominator - The denominator of the power related to the radicand
     @tparam PrimaryFloatingPointType - The precision of the PDP::ConstantUnit
     @tparam RadicandUnitType - The unit that resides within the radicand
     @tparam SecondaryFloatingPointType - The precision of the RadicandUnit type
     @param constant - The constant you're going to add to the radicand
     @param radicand - The radicand that's going to be added to the constant
     @return A Radical that represents the multiplication of the constant and the radicand
     */
    template<PDP::UInteger Numerator, PDP::UInteger Denominator,typename PrimaryFloatingPointType ,template<typename> class RadicanUnitType, typename SecondaryFloatingPointType>
    RadicalUnit<PDP::ConstantUnit, PrimaryFloatingPointType, RadicanUnitType, SecondaryFloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<SecondaryFloatingPointType>, RadicanUnitType<SecondaryFloatingPointType>>::value>>
    operator * (const PDP::ConstantUnit<PrimaryFloatingPointType> & constant, const PDP::RadicandUnit<Numerator, Denominator, RadicanUnitType, SecondaryFloatingPointType, typename LD::Detail::IsBaseOf<PDP::Unit<SecondaryFloatingPointType>, RadicanUnitType<SecondaryFloatingPointType>>::value> & radicand)
    {
        return RadicalUnit<PDP::ConstantUnit, PrimaryFloatingPointType, RadicanUnitType, SecondaryFloatingPointType, LD::Enable_If_T<LD::Detail::IsBaseOf<PDP::Unit<SecondaryFloatingPointType>, RadicanUnitType<SecondaryFloatingPointType>>::value>>();
    }
    
    
    
    template<typename T>
    class Distance: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Area: public Unit<T>
    {
        
    };
    
    
    template<typename T>
    class Volume: public Unit<T>
    {
        
    };
    
    template<typename T>
    class LinearVelocity: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Meter: public Distance<T>
    {
        
        CreateUnitsClassImplementation(Meter)
    };
    
    
    template<typename T>
    class Mile: public Distance<T>
    {
        CreateUnitsClassImplementation(Mile)
    };
    
    template<typename T>
    class Time: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Second: public Time<T>
    {
        CreateUnitsClassImplementation(Second)
        
        
    };
    
    
    
    
    template<typename T>
    class SecondSquared: public Unit<T>
    {
        CreateUnitsClassImplementation(SecondSquared)
        
        template<typename _T = T, typename = LD::Enable_If_T<!LD::Detail::IsSame<_T, long double>::value>>
        SecondSquared(long double abc)
        {
            (*this) = abc;
        }
        
        template<typename _T = T, typename = LD::Enable_If_T<!LD::Detail::IsSame<_T, long double>::value>>
        SecondSquared(const SecondSquared<long double> & abc)
        {
            (*this) = abc;
        }
        
        template<typename _T = T>
        inline LD::Enable_If_T<!LD::Detail::IsSame<_T, long double>::value,SecondSquared<_T>&> operator = (long double abc)
        {
            this->CurrentValue = abc;
            return (*this);
        }
        
        template<typename _T = T>
        inline LD::Enable_If_T<!LD::Detail::IsSame<_T, long double>::value,SecondSquared<_T>&> operator = (const SecondSquared<long double> & abc)
        {
            this->CurrentValue = abc.GetValue();
            return (*this);
        }
    };
    
    template<typename T>
    class Frequency: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Hertz: public Frequency<T>
    {
        CreateUnitsClassImplementation(Hertz)
    };
    
    template<typename T>
    class Ampere: public Unit<T>
    {
        CreateUnitsClassImplementation(Ampere)
    };
    template<typename T>
    class Volt: public Unit<T>
    {
        CreateUnitsClassImplementation(Volt)
    };
    
    template<typename T>
    class Coulomb: public Unit<T>
    {
        CreateUnitsClassImplementation(Coulomb)
    };
    
    
    template<typename T>
    class Ohm: public Unit<T>
    {
        CreateUnitsClassImplementation(Ohm)
    };
    
    template<typename T>
    class Temperature: public Unit<T>
    {
        
    };
    template<typename T>
    class Celsius: public Temperature<T>
    {
        CreateUnitsClassImplementation(Celsius)
    };
    
    template<typename T>
    class Kelvin: public Temperature<T>
    {
        CreateUnitsClassImplementation(Kelvin)
    };
    
    template<typename T>
    class Fahrenheit: public Temperature<T>
    {
        CreateUnitsClassImplementation(Fahrenheit)
    };
    
    
    template<typename T>
    class Energy: public Unit<T>
    {
        
    };
    template<typename T>
    class Joule: public Energy<T>
    {
        CreateUnitsClassImplementation(Joule)
    };
    
    template<typename T>
    class Work : public Energy<T>
    {
        CreateUnitsClassImplementation(Work)
    };
    
    template<typename T>
    class Watt: public Energy<T>
    {
        CreateUnitsClassImplementation(Watt)
    };
    
    template<typename T>
    class Mole: public Unit<T>
    {
        CreateUnitsClassImplementation(Mole)
    };
    
    template<typename T>
    class Candela: public Unit<T>
    {
        CreateUnitsClassImplementation(Candela)
    };
    
    template<typename T>
    class MetersPerSecond: public LinearVelocity<T>
    {
        CreateUnitsClassImplementation(MetersPerSecond)
    };
    
    template<typename T>
    class MetersPerSecondSquared: public Unit<T>
    {
        CreateUnitsClassImplementation(MetersPerSecondSquared)
    };
    
    template<typename T>
    class Angle: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Radian: public Angle<T>
    {
        CreateUnitsClassImplementation(Radian)
    };
    
    template<typename T>
    class Degree: public Angle<T>
    {
        CreateUnitsClassImplementation(Degree)
    };
    
    template<typename T>
    class SquareMeter: public Unit<T>
    {
        CreateUnitsClassImplementation(SquareMeter)
    };
    
    template<typename T>
    class CubicMeter: public Unit<T>
    {
        CreateUnitsClassImplementation(CubicMeter)
    };
    
    template<typename T>
    class Force: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Newton: public Force<T>
    {
        CreateUnitsClassImplementation(Newton)
    };
    
    
    template<typename T>
    class AngularVelocity: public Unit<T>
    {
        
    };
    
    template<typename T>
    class AngularAcceleration: public Unit<T>
    {
        
    };
    
    template<typename T>
    class RadiansPerSecond: public AngularVelocity<T>
    {
        CreateUnitsClassImplementation(RadiansPerSecond)
    };
    
    template<typename T>
    class RadiansPerSecondSquared: public AngularAcceleration<T>
    {
        CreateUnitsClassImplementation(RadiansPerSecondSquared)
    };
    
    template<typename T>
    class Mass: public Unit<T>
    {
        
    };
    
    template<typename T>
    class Kilogram: public Mass<T>
    {
        CreateUnitsClassImplementation(Kilogram)
    };
    
    template<typename T>
    class Real: public Unit<T>
    {
        CreateUnitsClassImplementation(Real)
    };
    
    template<typename T>
    class Imaginary: public Unit<T>
    {
        CreateUnitsClassImplementation(Imaginary)
    };
    
    
    
    
    
    template<typename T>
    struct DerivativeOfUnit
    {
        
    };
    
    
    template<typename T>
    struct IntegralOfUnit
    {
        
    };
    
    //beginning of second comparison operands
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator < (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() < b.GetValue();
    }
    
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator > (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() > b.GetValue();
    }
    
    
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator == (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() == b.GetValue();
    }
    
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator != (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() != b.GetValue();
    }
    
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator <= (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() <= b.GetValue();
    }
    
    template<typename T,typename U>
    LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value,const bool> operator >= (const Second<T> & a,const Second<U> & b)
    {
        return a.GetValue() >= b.GetValue();
    }
    
    
    
    
    //end of second comparison operands
    
    
    //beginning of periodic functions
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Sin(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Sin(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Cos(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Cos(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Tan(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Tan(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> CsC(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::CsC(currentUnit.GetValue()));
    }
    
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Sec(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Sec(currentUnit.GetValue()));
    }
    
    
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> CoTan(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::CoTan(currentUnit.GetValue()));
    }
    
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ASin(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ASin(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ACos(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ACos(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ATan(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ATan(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ACsC(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ACsC(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ASec(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ASec(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ACoTan(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::CoTan(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Sinh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Sinh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Cosh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Cosh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Tanh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Tanh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ASinh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ASinh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ACosh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ASinh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ATanh(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::ASinh(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> ATan2(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1)
    {
        return UnitType<TypeOfUnit>(PDP::ATan2(currentUnit.GetValue(),currentUnit1.GetValue()));
    }
    
    //end of periodic functions
    
    
    /**
     
     
     
     inline PDP::UInteger Ceil(const PDP::Float & x,const PDP::Precision & precision = PDP::eHIgh)
     {
     if(x<0)return (PDP::UInteger)x;
     return ((int)x)+1;
     }
     //extern PDP::Float Floor(const PDP::Float & a,const PDP::Precision & precision = PDP::eHIgh);
     inline PDP::Float Radians(const PDP::Float & degrees)
     {
     return degrees*57.2958;
     }
     inline PDP::Float Degrees(const PDP::Float & radians)
     {
     return radians/57.2958;
     }
     inline PDP::Float Fmod(const PDP::Float & a, const PDP::Float & b)
     {
     return (PDP::Integer)((((a/b)-((PDP::Integer)(a/b)))*b)+PDP::Float(0.5));
     }
     inline PDP::Float Fraction(const PDP::Float & a)
     {
     return a -  (PDP::UInteger)a;
     }

     
     */
    //start of power functions
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Exp(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Exp(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Ln(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Ln(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Log(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Log(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Log10(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Log10(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Log2(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Log2(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> GCD(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::GCD(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> LCM(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::LCM(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Min(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1)
    {
        return UnitType<TypeOfUnit>(PDP::Min(currentUnit.GetValue(),currentUnit1.GetValue()));
    }
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Min(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1,const UnitType<TypeOfUnit> & currentUnit2)
    {
        return UnitType<TypeOfUnit>(PDP::Min(currentUnit.GetValue(),currentUnit1.GetValue(),currentUnit2.GetValue()));
    }
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Min(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1,const UnitType<TypeOfUnit> & currentUnit2,const UnitType<TypeOfUnit> & currentUnit3)
    {
        return UnitType<TypeOfUnit>(PDP::Min(currentUnit.GetValue(),currentUnit1.GetValue(),currentUnit2.GetValue(),currentUnit3.GetValue()));
    }
    
    
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Max(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1)
    {
        return UnitType<TypeOfUnit>(PDP::Max(currentUnit.GetValue(),currentUnit1.GetValue()));
    }
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Max(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1,const UnitType<TypeOfUnit> & currentUnit2)
    {
        return UnitType<TypeOfUnit>(PDP::Max(currentUnit.GetValue(),currentUnit1.GetValue(),currentUnit2.GetValue()));
    }
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Max(const UnitType<TypeOfUnit> & currentUnit,const UnitType<TypeOfUnit> & currentUnit1,const UnitType<TypeOfUnit> & currentUnit2,const UnitType<TypeOfUnit> & currentUnit3)
    {
        return UnitType<TypeOfUnit>(PDP::Max(currentUnit.GetValue(),currentUnit1.GetValue(),currentUnit2.GetValue(),currentUnit3.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Round(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Round(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Ceil(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Ceil(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Floor(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Floor(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Fmod(const UnitType<TypeOfUnit> & a,const UnitType<TypeOfUnit> & b)
    {
        return UnitType<TypeOfUnit>(PDP::Fmod(a.GetValue(),b.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Fraction(const UnitType<TypeOfUnit> & currentUnit)
    {
        return UnitType<TypeOfUnit>(PDP::Fraction(currentUnit.GetValue()));
    }
    
    template<template<typename> class UnitType, typename TypeOfUnit>
    LD::Enable_If_T<LD::Detail::IsKindOf<PDP::Unit<TypeOfUnit>, UnitType<TypeOfUnit>>::value,UnitType<TypeOfUnit>> Clamp(const UnitType<TypeOfUnit> & a,const UnitType<TypeOfUnit> & lower, const UnitType<TypeOfUnit> & upper)
    {
        return UnitType<TypeOfUnit>(PDP::Clamp(a.GetValue(),lower.GetValue(),upper.GetValue()));
    }
    //end of power functions
}


//beginning of second implementation

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Second<T>> operator + (const PDP::Second<T> & a, const PDP::Second<U> & b)
{
    return PDP::Second<T>(a.GetSecond()+b.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Second<T>> operator - (const PDP::Second<T> & a, const PDP::Second<U> & b)
{
    return PDP::Second<T>(a.GetSecond()+b.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value && !LD::Detail::IsSame<U,PDP::Second<U>>::value,const PDP::Second<T>> operator * (const PDP::Second<T> & a, const U & b)
{
    return PDP::Second<T>(a.GetSecond()*b);
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value && !LD::Detail::IsSame<U,PDP::Second<U>>::value,const PDP::Second<T>> operator * (const U & b,const PDP::Second<T> & a)
{
    return PDP::Second<T>(a.GetSecond()*b);
}


template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::SecondSquared<T>> operator * (const PDP::Second<T> & a, const PDP::Second<U> & b)
{
    return PDP::SecondSquared<T>(a.GetSecond()*b.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const T>  operator / (const PDP::Second<T> & a, const PDP::Second<U> & b)
{
    return a.GetValue()/b.GetValue();
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U, T>::value && !LD::Detail::IsSame<U,PDP::Second<U>>::value,const PDP::Second<T>> operator / (const PDP::Second<T> & a, const U & b)
{
    return PDP::Second<T>(a.GetSecond()/b);
}



template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::Second<T>&> operator += (PDP::Second<T> & a, const PDP::Second<U> & b)
{
    a = PDP::Second<T>(a.GetValue()+b.GetValue());
    return a;
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::Second<T>&>  operator -= (PDP::Second<T> & a, const PDP::Second<U> & b)
{
    a = PDP::Second<T>(a.GetValue()-b.GetValue());
    return a;
}





//end of second implementation


//beginning of meter implementation

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Meter<T>>  operator + (const PDP::Meter<T> & a, const PDP::Meter<U> & b)
{
    return PDP::Meter<T>(a.GetMeter() + b.GetMeter());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Meter<T>> operator - (const PDP::Meter<T> & a, const PDP::Meter<U> & b)
{
    return PDP::Meter<T>(a.GetMeter() - b.GetMeter());
}


template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Meter<T>> operator * (const PDP::MetersPerSecond<T> & a, const PDP::Second<U> & b)
{
    return PDP::Meter<T>(a.GetMetersPerSecond() * b.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value, const PDP::SquareMeter<T>> operator * (const PDP::Meter<T> & a, const PDP::Meter<U> & b)
{
    return PDP::SquareMeter<T>(a.GetValue() * b.GetValue());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value && !LD::Detail::IsSame<U, PDP::Second<U>>::value,const PDP::Meter<T>> operator * (const PDP::MetersPerSecond<T> & a, const U & b)
{
    return PDP::Meter<T>(a.GetMetersPerSecond() * b);
}



template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value && !LD::Detail::IsSame<U, PDP::Second<U>>::value,const PDP::Meter<T>> operator * ( const U & b,const PDP::MetersPerSecond<T> & a)
{
    return PDP::Meter<T>(a.GetMetersPerSecond() * b);
}





template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Meter<T>> operator * (const PDP::Second<T> & a, const PDP::MetersPerSecond<U> & b)
{
    return PDP::Meter<T>(a.GetMetersPerSecond() * b.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value && !LD::Detail::IsSame<U, PDP::Second<U>>::value,const PDP::Meter<T>> operator / (const PDP::MetersPerSecond<T> & a, const U & b)
{
    return PDP::Meter<T>(a.GetMetersPerSecond() / b);
}



template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::Meter<T>&>  operator += (PDP::Meter<T> & a, const PDP::Meter<U> & b)
{
    a =  PDP::Meter<T>(a.GetMeter() + b.GetMeter());
    return a;
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::Meter<T>&> operator -= (PDP::Meter<T> & a, const PDP::Meter<U> & b)
{
    a =  PDP::Meter<T>(a.GetMeter() - b.GetMeter());
    return a;
}
//end of meter implementation


//beginning of meterspersecond implementation

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::MetersPerSecond<T>> operator + (const PDP::MetersPerSecond<T> & meter, const PDP::MetersPerSecond<U> & second)
{
    return PDP::MetersPerSecond<T>(meter.GetMeter()+second.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::MetersPerSecond<T>> operator - (const PDP::MetersPerSecond<T> & meter, const PDP::MetersPerSecond<U> & second)
{
    return PDP::MetersPerSecond<T>(meter.GetMeter()-second.GetSecond());
}


template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::MetersPerSecond<T>&> operator += (const PDP::MetersPerSecond<T> & meter, const PDP::MetersPerSecond<U> & second)
{
    meter =  PDP::MetersPerSecond<T>(meter.GetMeter()+second.GetSecond());
    return meter;
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,PDP::MetersPerSecond<T>&> operator -= (const PDP::MetersPerSecond<T> & meter, const PDP::MetersPerSecond<U> & second)
{
    meter =  PDP::MetersPerSecond<T>(meter.GetMeter()-second.GetSecond());
    return meter;
}



template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::MetersPerSecond<T>> operator / (const PDP::Meter<T> & meter, const PDP::Second<U> & second)
{
    return PDP::MetersPerSecond<T>(meter.GetMeter()/second.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::MetersPerSecondSquared<T>> operator / (const PDP::MetersPerSecond<T> & velocity, const PDP::Second<U> & second)
{
    return PDP::MetersPerSecondSquared<T>(velocity.MetersPerSecond()/second.GetSecond());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Work<T>> operator * (const PDP::Newton<T> & force, const PDP::Meter<U> & distance)
{
    return PDP::Work<T>(force.GetValue()*distance.GetValue());
}

template<typename T, typename U>
inline LD::Enable_If_T<LD::Detail::IsConvertible<U,T>::value,const PDP::Work<T>> operator * (const PDP::Meter<U> & distance,const PDP::Newton<T> & force)
{
    return PDP::Work<T>(force.GetValue()*distance.GetValue());
}

//end of meters per second implementation

//beginning of Newton implementation
template<typename T, typename U>
inline PDP::Newton<T> operator *(const PDP::MetersPerSecondSquared<T> & acceleration, const PDP::Kilogram<U> & mass)
{
    return PDP::Newton<T>(acceleration.GetMetersPerSecondSquare() * mass.GetKilogram());
}


//beginning of second literals
/*
inline PDP::Second<long double>  operator "" _ds(long double a)
{
    return PDP::Second<long double>(a*(1E-1));
}

inline PDP::Second<long double>  operator "" _cs(long double a)
{
    return PDP::Second<long double>(a*(1E-2));
}

inline PDP::Second<long double>  operator "" _ms(long double a)
{
    return PDP::Second<long double>(a*(1E-6));
}

inline PDP::Second<long double>  operator "" _us(long double a)
{
    return PDP::Second<long double>(a*(1E-9));
}


inline PDP::Second<long double>  operator "" _ns(long double a)
{
    return PDP::Second<long double>(a*(1E-12));
}

inline PDP::Second<long double>   operator "" _ps(long double a)
{
    return PDP::Second<long double>(a*(1E-15));
}

inline PDP::Second<long double>  operator "" _fs(long double a)
{
    return PDP::Second<long double>(a*(1E-18));
}

inline PDP::Second<long double>   operator "" _as(long double a)
{
    return PDP::Second<long double>(a*(1E-18));
}

inline PDP::Second<long double>   operator "" _zs(long double a)
{
    return PDP::Second<long double>(a*(1E-21));
}

inline PDP::Second<long double>  operator "" _ys(long double a)
{
    return PDP::Second<long double>(a*(1E-24));
}


inline PDP::Second<long double> operator "" _s(long double a)
{
    return PDP::Second<long double>(a);
}
 */



inline PDP::Second<long double>  operator "" _das(long double a)
{
    return PDP::Second<long double>(a*(1E1));
}

inline PDP::Second<long double>  operator "" _hs(long double a)
{
    return PDP::Second<long double>(a*(1E2));
}


inline PDP::Second<long double>  operator "" _ks(long double a)
{
    return PDP::Second<long double>(a*(1E3));
}

inline PDP::Second<long double>  operator "" _Ms(long double a)
{
    return PDP::Second<long double>(a*(1E6));
}

inline PDP::Second<long double>  operator "" _Gs(long double a)
{
    return PDP::Second<long double>(a*(1E9));
}

inline PDP::Second<long double>  operator "" _Ts(long double a)
{
    return PDP::Second<long double>(a*(1E12));
}


inline PDP::Second<long double>  operator "" _Ps(long double a)
{
    return PDP::Second<long double>(a*(1E15));
}

inline PDP::Second<long double>  operator "" _Es(long double a)
{
    return PDP::Second<long double>(a*(1E18));
}

inline PDP::Second<long double>  operator "" _Zs(long double a)
{
    return PDP::Second<long double>(a*(1E21));
}


inline PDP::Second<long double>  operator "" _Ys(long double a)
{
    return PDP::Second<long double>(a*(1E24));
}
















inline PDP::Second<unsigned long long int>  operator "" _ds(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-1));
}

inline PDP::Second<unsigned long long int>  operator "" _cs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-2));
}

inline PDP::Second<unsigned long long int>  operator "" _ms(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-6));
}

inline PDP::Second <unsigned long long int>  operator "" _us(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-9));
}


inline PDP::SecondSquared<unsigned long long int>  operator "" _ns(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-12));
}

inline PDP::Second<unsigned long long int>   operator "" _ps(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-15));
}

inline PDP::Second<unsigned long long int>  operator "" _fs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-18));
}

inline PDP::Second<unsigned long long int>   operator "" _as(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-18));
}

inline PDP::Second<unsigned long long int>   operator "" _zs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-21));
}

inline PDP::Second<unsigned long long int>  operator "" _ys(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E-24));
}


inline PDP::Second<unsigned long long int> operator "" _s(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a);
}



inline PDP::Second<unsigned long long int>  operator "" _das(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E1));
}

inline PDP::Second<unsigned long long int>  operator "" _hs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E2));
}


inline PDP::Second<unsigned long long int>  operator "" _ks(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E3));
}

inline PDP::Second<unsigned long long int>  operator "" _Ms(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E6));
}

inline PDP::Second<unsigned long long int>  operator "" _Gs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E9));
}

inline PDP::Second<unsigned long long int>  operator "" _Ts(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E12));
}


inline PDP::Second<unsigned long long int>  operator "" _Ps(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E15));
}

inline PDP::Second<unsigned long long int>  operator "" _Es(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E18));
}

inline PDP::Second<unsigned long long int>  operator "" _Zs(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E21));
}


inline PDP::Second<unsigned long long int>  operator "" _Ys(unsigned long long int a)
{
    return PDP::Second<unsigned long long int>(a*(1E24));
}
//end of second literals




//beginning of second squared literals

inline PDP::SecondSquared<long double>  operator "" _ds2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-1));
}

inline PDP::SecondSquared<long double>  operator "" _cs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-2));
}

inline PDP::SecondSquared<long double>  operator "" _ms2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-6));
}

inline PDP::SecondSquared<long double>  operator "" _us2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-9));
}


inline PDP::SecondSquared<long double>  operator "" _ns2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-12));
}

inline PDP::SecondSquared<long double>  operator "" _ps2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-15));
}

inline PDP::SecondSquared<long double>  operator "" _fs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-18));
}

inline PDP::SecondSquared<long double>  operator "" _as2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-18));
}

inline PDP::SecondSquared<long double>  operator "" _zs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-21));
}

inline PDP::SecondSquared<long double>  operator "" _ys2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E-24));
}

inline PDP::SecondSquared<long double>  operator "" _s2(long double a)
{
    return PDP::SecondSquared<long double>(a);
}

inline PDP::SecondSquared<long double>  operator "" _das2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E1));
}

inline PDP::SecondSquared<long double>  operator "" _hs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E2));
}


inline PDP::SecondSquared<long double>  operator "" _ks2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E3));
}

inline PDP::SecondSquared<long double>  operator "" _Ms2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E6));
}

inline PDP::SecondSquared<long double>  operator "" _Gs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E9));
}

inline PDP::SecondSquared<long double>  operator "" _Ts2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E12));
}


inline PDP::SecondSquared<long double>  operator "" _Ps2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E15));
}

inline PDP::SecondSquared<long double>  operator "" _Es2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E18));
}

inline PDP::SecondSquared<long double>  operator "" _Zs2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E21));
}


inline PDP::SecondSquared<long double>  operator "" _Ys2(long double a)
{
    return PDP::SecondSquared<long double>(a*(1E24));
}















inline PDP::SecondSquared<unsigned long long int>  operator "" _ds2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-1));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _cs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-2));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _ms2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-6));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _us2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-9));
}


inline PDP::SecondSquared<unsigned long long int>  operator "" _ns2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-12));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _ps2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-15));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _fs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-18));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _as2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-18));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _zs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-21));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _ys2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E-24));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _s2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a);
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _das2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E1));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _hs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E2));
}


inline PDP::SecondSquared<unsigned long long int>  operator "" _ks2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E3));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _Ms2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E6));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _Gs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E9));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _Ts2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E12));
}


inline PDP::SecondSquared<unsigned long long int>  operator "" _Ps2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E15));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _Es2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E18));
}

inline PDP::SecondSquared<unsigned long long int>  operator "" _Zs2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E21));
}


inline PDP::SecondSquared<unsigned long long int>  operator "" _Ys2(unsigned long long int a)
{
    return PDP::SecondSquared<unsigned long long int>(a*(1E24));
}
//end of second squared literals

#endif /* defined(__DataStructures__Units__) */
