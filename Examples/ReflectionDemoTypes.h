//
// Created by phoenixflower on 5/16/20.
//

#ifndef LANDESSDEVCORE_REFLECTIONDEMOTYPES_H
#define LANDESSDEVCORE_REFLECTIONDEMOTYPES_H
#include "Reflection/reflectable.hpp"
#include "Definitions/Float.hpp"
#include "Functor/FunctionView.h"
#include "Primitives/General/StaticArray.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    class CPUPackageMetric: public LD::Reflectable<
            decltype("cpu"_ts)(
                    decltype("User"_ts),LD::UInteger,
                    decltype("Nice"_ts),LD::UInteger,
                    decltype("System"_ts),LD::UInteger,
                    decltype("Idle"_ts),LD::UInteger,
                    decltype("IOWait"_ts),LD::UInteger,
                    decltype("Irq"_ts),LD::UInteger,
                    decltype("SoftIrq"_ts),LD::UInteger,
                    decltype("Steal"_ts),LD::UInteger,
                    decltype("Guest"_ts),LD::UInteger,
                    decltype("GuestNice"_ts),LD::UInteger
            )>
    {
    private:

    public:
        constexpr const LD::UInteger & User() const noexcept
        {
            return (*this)["User"_ts];
        }
        constexpr LD::UInteger & User() noexcept
        {
            return (*this)["User"_ts];
        }

        constexpr const LD::UInteger & Nice() const noexcept
        {
            return (*this)["Nice"_ts];
        }

        constexpr LD::UInteger & Nice()  noexcept
        {
            return (*this)["Nice"_ts];
        }

        constexpr const LD::UInteger & System() const noexcept
        {
            return (*this)["System"_ts];
        }

        constexpr LD::UInteger & System()  noexcept
        {
            return (*this)["System"_ts];
        }

        constexpr const LD::UInteger & Idle() const noexcept
        {
            return (*this)["Idle"_ts];
        }
        constexpr LD::UInteger & Idle() noexcept
        {
            return (*this)["Idle"_ts];
        }

        constexpr const LD::UInteger & IOWait() const noexcept
        {
            return (*this)["IOWait"_ts];
        }
        constexpr LD::UInteger & IOWait() noexcept
        {
            return (*this)["IOWait"_ts];
        }

        constexpr const LD::UInteger & IRQ() const noexcept
        {
            return (*this)["Irq"_ts];
        }
        constexpr LD::UInteger & IRQ() noexcept
        {
            return (*this)["Irq"_ts];
        }

        constexpr const LD::UInteger & SoftIRQ() const noexcept
        {
            return (*this)["SoftIrq"_ts];
        }

        constexpr LD::UInteger & SoftIRQ() noexcept
        {
            return (*this)["SoftIrq"_ts];
        }

        constexpr const LD::UInteger & Steal() const noexcept
        {
            return (*this)["Steal"_ts];
        }

        constexpr LD::UInteger & Steal() noexcept
        {
            return (*this)["Steal"_ts];
        }

        constexpr const LD::UInteger & Guest() const noexcept
        {
            return (*this)["Guest"_ts];
        }

        constexpr LD::UInteger & Guest() noexcept
        {
            return (*this)["Guest"_ts];
        }

        constexpr const LD::UInteger & GuestNice() const noexcept
        {
            return (*this)["GuestNice"_ts];
        }

        constexpr LD::UInteger & GuestNice() noexcept
        {
            return (*this)["GuestNice"_ts];
        }

    };
    template<LD::UInteger CoreNumber>
    class CPUCoreMetric: public LD::Reflectable<
            decltype("cpu"_ts+LD::ToTypeString<CoreNumber>{})(
                    decltype("User"_ts),LD::UInteger,
                    decltype("Nice"_ts),LD::UInteger,
                    decltype("System"_ts),LD::UInteger,
                    decltype("Idle"_ts),LD::UInteger,
                    decltype("IOWait"_ts),LD::UInteger,
                    decltype("Irq"_ts),LD::UInteger,
                    decltype("SoftIrq"_ts),LD::UInteger,
                    decltype("Steal"_ts),LD::UInteger,
                    decltype("Guest"_ts),LD::UInteger,
                    decltype("GuestNice"_ts),LD::UInteger
                    )>
    {
    private:

    public:

        constexpr static const LD::UInteger Core = CoreNumber;

        constexpr const LD::UInteger & User() const noexcept
        {
            return (*this)["User"_ts];
        }
        constexpr LD::UInteger & User() noexcept
        {
            return (*this)["User"_ts];
        }

        constexpr const LD::UInteger & Nice() const noexcept
        {
            return (*this)["Nice"_ts];
        }

        constexpr LD::UInteger & Nice()  noexcept
        {
            return (*this)["Nice"_ts];
        }

        constexpr const LD::UInteger & System() const noexcept
        {
            return (*this)["System"_ts];
        }

        constexpr LD::UInteger & System()  noexcept
        {
            return (*this)["System"_ts];
        }

        constexpr const LD::UInteger & Idle() const noexcept
        {
            return (*this)["Idle"_ts];
        }
        constexpr LD::UInteger & Idle() noexcept
        {
            return (*this)["Idle"_ts];
        }

        constexpr const LD::UInteger & IOWait() const noexcept
        {
            return (*this)["IOWait"_ts];
        }
        constexpr LD::UInteger & IOWait() noexcept
        {
            return (*this)["IOWait"_ts];
        }

        constexpr const LD::UInteger & IRQ() const noexcept
        {
            return (*this)["Irq"_ts];
        }
        constexpr LD::UInteger & IRQ() noexcept
        {
            return (*this)["Irq"_ts];
        }

        constexpr const LD::UInteger & SoftIRQ() const noexcept
        {
            return (*this)["SoftIrq"_ts];
        }

        constexpr LD::UInteger & SoftIRQ() noexcept
        {
            return (*this)["SoftIrq"_ts];
        }

        constexpr const LD::UInteger & Steal() const noexcept
        {
            return (*this)["Steal"_ts];
        }

        constexpr LD::UInteger & Steal() noexcept
        {
            return (*this)["Steal"_ts];
        }

        constexpr const LD::UInteger & Guest() const noexcept
        {
            return (*this)["Guest"_ts];
        }

        constexpr LD::UInteger & Guest() noexcept
        {
            return (*this)["Guest"_ts];
        }

        constexpr const LD::UInteger & GuestNice() const noexcept
        {
            return (*this)["GuestNice"_ts];
        }

        constexpr LD::UInteger & GuestNice() noexcept
        {
            return (*this)["GuestNice"_ts];
        }

    };

    namespace Detail
    {
        template<typename T>
        struct IsCPUCore: public LD::Detail::IntegralConstant<bool,false>
        {

        };

        template<LD::UInteger Core>
        struct IsCPUCore<LD::CPUCoreMetric<Core>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }

    template<typename T>
    constexpr bool IsCPUCore = LD::Detail::IsCPUCore<T>::value;


    class ContextSwitchMetric: public LD::Reflectable<
            decltype("ctxt"_ts)(
                    decltype("Switches"_ts),LD::UInteger
            )>
    {

    };
    class InterruptServiceMetric: public LD::Reflectable<
            decltype("intr"_ts)(
                    decltype("Switches"_ts),LD::UInteger
            )>
    {

    };

    class BootUpTimeMetric: public LD::Reflectable<
            decltype("btime"_ts)(
                    decltype("Time"_ts),LD::UInteger
            )>
    {

    };

    class ProcessesMetric: public LD::Reflectable<
            decltype("processes"_ts)(
                    decltype("Number"_ts),LD::UInteger
            )>
    {

    };

    class ProcessesRunningMetric: public LD::Reflectable<
            decltype("procs_running"_ts)(
                    decltype("Number"_ts),LD::UInteger
            )>
    {

    };

    class ProcessesBlockedMetric: public LD::Reflectable<
            decltype("procs_blocked"_ts)(
                    decltype("Number"_ts),LD::UInteger
            )>
    {

    };

    class SoftIRQMetric: public LD::Reflectable<
            decltype("softirq"_ts)(
                    decltype("Interrupts"_ts),LD::UInteger
            )>
    {

    };


    class Square: public LD::Reflectable<
            decltype("Square"_ts)(
                    decltype("Length"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    private:
        //LD::Float  & mLength;
    public:
        inline Square() //noexcept :mLength((*this)["Length"_ts])
        {
            (*this)["Length"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Square::GetArea);
        }


        inline Square(const LD::Float & length)
        {
            (*this)["Length"_ts] = length;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Square::GetArea);
        }

        LD::Float GetArea() const
        {
            return (*this)["Length"_ts] * (*this)["Length"_ts];
        }


        const LD::Float &  Length() const noexcept
        {
            return (*this)["Length"_ts];
        }

         LD::Float & Length() noexcept
        {
            return (*this)["Length"_ts];
        }



    };

    class ArrayTest: public LD::Reflectable<
            decltype("Square"_ts)(
                    decltype("Length"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>,
                    decltype("array"_ts),LD::StaticArray<int,55>)>
    {
    public:
        inline ArrayTest()
        {
            (*this)["Length"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&ArrayTest::GetArea);
        }

        inline ArrayTest(const LD::Float & length)
        {
            (*this)["Length"_ts] = length;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&ArrayTest::GetArea);
        }
        LD::Float GetArea() const
        {
            return (*this)["Length"_ts] * (*this)["Length"_ts];
        }
    };

    class Triangle: public LD::Reflectable<
            decltype("Triangle"_ts)(
                    decltype("Base"_ts),      LD::Float,
                    decltype("Height"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Triangle()
        {
            (*this)["Base"_ts] = 0;
            (*this)["Height"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Triangle::GetArea);
        }
        inline Triangle(const LD::Float & base, const LD::Float & height)
        {
            (*this)["Base"_ts] = base;
            (*this)["Height"_ts] = height;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Triangle::GetArea);
        }
        LD::Float GetArea() const
        {
            return 0.5 * (*this)["Base"_ts] * (*this)["Height"_ts];
        }

        LD::Float  & Base() noexcept
        {
            return (*this)["Base"_ts];
        }

        const LD::Float  & Base() const noexcept
        {
            return (*this)["Base"_ts];
        }

        LD::Float  & Height() noexcept
        {
            return (*this)["Height"_ts];
        }

        const LD::Float  & Height() const noexcept
        {
            return (*this)["Height"_ts];
        }

    };
    class Circle: public LD::Reflectable<
            decltype("Circle"_ts)(
                    decltype("Radius"_ts),      LD::Float,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    public:
        inline Circle()
        {
            (*this)["Radius"_ts] = 0;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Circle::GetArea);
        }
        inline Circle(const LD::Float & radius)
        {
            (*this)["Radius"_ts] = radius;
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Circle::GetArea);
        }
        LD::Float GetArea() const
        {
            return 3.14 * (*this)["Radius"_ts] * (*this)["Radius"_ts];
        }
    };

    class Pyramid: public LD::Reflectable<
            decltype("Pyramid"_ts)(
                    decltype("Base"_ts),      LD::Square,
                    decltype("Side"_ts),      LD::Triangle,
                    decltype("area"_ts),LD::FunctionView<LD::Float ()>)>
    {
    private:
        LD::Square & mBase;
        LD::Triangle & mSide;
    public:
        inline Pyramid() noexcept :mBase((*this)["Base"_ts]),mSide((*this)["Side"_ts])
        {
            (*this)["Base"_ts] = LD::Square{};
            (*this)["Side"_ts] =  LD::Triangle{};
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Pyramid::GetArea);
        }
        inline Pyramid(const LD::Square & width, const LD::Triangle & height) noexcept : Pyramid()
        {
            (*this)["Base"_ts] = LD::Square{width};
            (*this)["Side"_ts] =  LD::Triangle{height};
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Pyramid::GetArea);
        }

        inline Pyramid(const Pyramid & pyramid) noexcept :Pyramid()
        {
            (*this) = pyramid;
        }

        inline Pyramid & operator = (const Pyramid & pyramid)
        {
            (*this)["Base"_ts] = pyramid["Base"_ts];
            (*this)["Side"_ts] = pyramid["Side"_ts];
            (*this)["area"_ts] = LD::FunctionView<LD::Float()>(this,&Pyramid::GetArea);
            return (*this);
        }

        LD::Square & Base() noexcept
        {
            return this->mBase;
        }

        LD::Triangle & Side() noexcept
        {
            return  this->mSide;
        }

        const LD::Square & Base() const noexcept
        {
            return this->mBase;
        }

        const LD::Triangle & Side() const noexcept
        {
            return  this->mSide;
        }
        LD::Float GetArea() const
        {
            return 5;
        }
    };
}
#endif //LANDESSDEVCORE_REFLECTIONDEMOTYPES_H
