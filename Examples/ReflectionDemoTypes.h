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
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
namespace LD
{
    class CPUPackageMetric
    {
    private:
        LD::UInteger mUser;
        LD::UInteger mNice;
        LD::UInteger mSystem;
        LD::UInteger mIdle;
        LD::UInteger mIOWait;
        LD::UInteger mIRQ;
        LD::UInteger mSoftIRQ;
        LD::UInteger mSteal;
        LD::UInteger mGuest;
        LD::UInteger mGuestNice;
    public:
        constexpr const LD::UInteger & User() const noexcept
        {
            return this->mUser;
        }
        constexpr LD::UInteger & User() noexcept
        {
            return this->mUser;
        }

        constexpr const LD::UInteger & Nice() const noexcept
        {
            return this->mNice;
        }

        constexpr LD::UInteger & Nice()  noexcept
        {
            return this->mNice;
        }

        constexpr const LD::UInteger & System() const noexcept
        {
            return this->mSystem;
        }

        constexpr LD::UInteger & System()  noexcept
        {
            return this->mSystem;
        }

        constexpr const LD::UInteger & Idle() const noexcept
        {
            return this->mIdle;
        }
        constexpr LD::UInteger & Idle() noexcept
        {
            return this->mIdle;
        }

        constexpr const LD::UInteger & IOWait() const noexcept
        {
            return this->mIOWait;
        }
        constexpr LD::UInteger & IOWait() noexcept
        {
            return this->mIOWait;
        }

        constexpr const LD::UInteger & IRQ() const noexcept
        {
            return this->mIRQ;
        }
        constexpr LD::UInteger & IRQ() noexcept
        {
            return this->mIRQ;
        }

        constexpr const LD::UInteger & SoftIRQ() const noexcept
        {
            return this->mSoftIRQ;
        }

        constexpr LD::UInteger & SoftIRQ() noexcept
        {
            return this->mSoftIRQ;
        }

        constexpr const LD::UInteger & Steal() const noexcept
        {
            return this->mSteal;
        }

        constexpr LD::UInteger & Steal() noexcept
        {
            return this->mSteal;
        }

        constexpr const LD::UInteger & Guest() const noexcept
        {
            return this->mGuest;
        }

        constexpr LD::UInteger & Guest() noexcept
        {
            return this->mGuest;
        }

        constexpr const LD::UInteger & GuestNice() const noexcept
        {
            return this->mGuestNice;
        }

        constexpr LD::UInteger & GuestNice() noexcept
        {
            return this->mGuestNice;
        }

    };

    template<LD::UInteger CoreNumber>
    class CPUCoreMetric
    {
    private:
        LD::UInteger mUser;
        LD::UInteger mNice;
        LD::UInteger mSystem;
        LD::UInteger mIdle;
        LD::UInteger mIOWait;
        LD::UInteger mIRQ;
        LD::UInteger mSoftIRQ;
        LD::UInteger mSteal;
        LD::UInteger mGuest;
        LD::UInteger mGuestNice;
    public:
        constexpr const LD::UInteger & User() const noexcept
        {
            return this->mUser;
        }
        constexpr LD::UInteger & User() noexcept
        {
            return this->mUser;
        }

        constexpr LD::UInteger Core() const noexcept
        {
            return CoreNumber;
        }

        constexpr const LD::UInteger & Nice() const noexcept
        {
            return this->mNice;
        }

        constexpr LD::UInteger & Nice()  noexcept
        {
            return this->mNice;
        }

        constexpr const LD::UInteger & System() const noexcept
        {
            return this->mSystem;
        }

        constexpr LD::UInteger & System()  noexcept
        {
            return this->mSystem;
        }

        constexpr const LD::UInteger & Idle() const noexcept
        {
            return this->mIdle;
        }
        constexpr LD::UInteger & Idle() noexcept
        {
            return this->mIdle;
        }

        constexpr const LD::UInteger & IOWait() const noexcept
        {
            return this->mIOWait;
        }
        constexpr LD::UInteger & IOWait() noexcept
        {
            return this->mIOWait;
        }

        constexpr const LD::UInteger & IRQ() const noexcept
        {
            return this->mIRQ;
        }
        constexpr LD::UInteger & IRQ() noexcept
        {
            return this->mIRQ;
        }

        constexpr const LD::UInteger & SoftIRQ() const noexcept
        {
            return this->mSoftIRQ;
        }

        constexpr LD::UInteger & SoftIRQ() noexcept
        {
            return this->mSoftIRQ;
        }

        constexpr const LD::UInteger & Steal() const noexcept
        {
            return this->mSteal;
        }

        constexpr LD::UInteger & Steal() noexcept
        {
            return this->mSteal;
        }

        constexpr const LD::UInteger & Guest() const noexcept
        {
            return this->mGuest;
        }

        constexpr LD::UInteger & Guest() noexcept
        {
            return this->mGuest;
        }

        constexpr const LD::UInteger & GuestNice() const noexcept
        {
            return this->mGuestNice;
        }

        constexpr LD::UInteger & GuestNice() noexcept
        {
            return this->mGuestNice;
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


    class ContextSwitchMetric
    {
    private:
        LD::UInteger mSwitches;
    public:
        inline ContextSwitchMetric(const LD::UInteger & metric = 0) noexcept :mSwitches{metric}{}

        inline constexpr const LD::UInteger & Switches() const noexcept {return this->mSwitches;}
        inline constexpr  LD::UInteger & Switches()  noexcept {return this->mSwitches;}
    };

    class InterruptServiceMetric
    {
    private:
        LD::UInteger mInterrupts;
    public:
        inline constexpr const LD::UInteger & Interrupts() const noexcept {return this->mInterrupts;}
        inline constexpr  LD::UInteger & Interrupts()  noexcept {return this->mInterrupts;}
    };

    class BootUpTimeMetric
    {
    private:
        LD::UInteger mTime;
    public:
        inline constexpr const LD::UInteger & BootUpTime() const noexcept {return this->mTime;}
        inline constexpr  LD::UInteger & BootUpTime()  noexcept {return this->mTime;}
    };

    class ProcessesMetric
    {
    private:
        LD::UInteger mAmount;
    public:
        inline constexpr const LD::UInteger & Amount() const noexcept {return this->mAmount;}
        inline constexpr  LD::UInteger & Amount()  noexcept {return this->mAmount;}
    };

    class ProcessesRunningMetric
    {
    private:
        LD::UInteger mAmount;
    public:
        inline constexpr const LD::UInteger & Amount() const noexcept {return this->mAmount;}
        inline constexpr  LD::UInteger & Amount()  noexcept {return this->mAmount;}
    };


    class ProcessesBlockedMetric
    {
    private:
        LD::UInteger mAmount;
    public:
        inline constexpr const LD::UInteger & Amount() const noexcept {return this->mAmount;}
        inline constexpr  LD::UInteger & Amount()  noexcept {return this->mAmount;}
    };

    class SoftIRQMetric
    {
    private:
        LD::UInteger mInterrupts;
    public:
        inline constexpr const LD::UInteger & Interrupts() const noexcept {return this->mInterrupts;}
        inline constexpr  LD::UInteger & Interrupts()  noexcept {return this->mInterrupts;}
    };


    class Square
    {
    private:
        //LD::Float  & mLength;
        LD::Float mLength;
    public:
        inline Square() noexcept:mLength{0}//noexcept :mLength((*this)["Length"_ts])
        {

        }


        inline Square(const LD::Float & length) noexcept:mLength{length}
        {

        }

        LD::Float GetArea() const
        {
            return this->mLength*this->mLength;
        }


        const LD::Float &  Length() const noexcept
        {
            return this->mLength;
        }

         LD::Float & Length() noexcept
        {
            return this->mLength;
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

template<>
class LD::CT::TypeDescriptor<LD::CPUPackageMetric>
{
private:
    static constexpr auto UserName = ctll::basic_fixed_string("User");
    static constexpr auto NiceName = ctll::basic_fixed_string("Nice");
    static constexpr auto SystemName = ctll::basic_fixed_string("System");
    static constexpr auto IdleName = ctll::basic_fixed_string("Idle");
    static constexpr auto IOWaitName = ctll::basic_fixed_string("IOWait");
    static constexpr auto IRQName = ctll::basic_fixed_string("IRQ");
    static constexpr auto SoftIRQName = ctll::basic_fixed_string("SoftIRQ");
    static constexpr auto StealName = ctll::basic_fixed_string("Steal");
    static constexpr auto GuestName = ctll::basic_fixed_string("Guest");
    static constexpr auto GuestNiceName = ctll::basic_fixed_string("GuestNice");
public:
    static constexpr auto ClassName = ctll::basic_fixed_string("cpu");
    using MemberList = LD::CT::TypeList<
            //LD::CT::Member<LengthName,T>,
            //LD::CT::MemberDescriptor<XName,xMember>,
            LD::CT::EncapsulatedMemberDescriptor<UserName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::User>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::User>()>,
            LD::CT::EncapsulatedMemberDescriptor<NiceName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::Nice>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::Nice>()>,
            LD::CT::EncapsulatedMemberDescriptor<SystemName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::System>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::System>()>,
            LD::CT::EncapsulatedMemberDescriptor<IdleName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::Idle>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::Idle>()>,
            LD::CT::EncapsulatedMemberDescriptor<IOWaitName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::IOWait>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::IOWait>()>,
            LD::CT::EncapsulatedMemberDescriptor<IRQName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::IRQ>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::IRQ>()>,
            LD::CT::EncapsulatedMemberDescriptor<SoftIRQName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::SoftIRQ>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::SoftIRQ>()>,
            LD::CT::EncapsulatedMemberDescriptor<StealName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::Steal>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::Steal>()>,
            LD::CT::EncapsulatedMemberDescriptor<GuestName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::Guest>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::Guest>()>,
            LD::CT::EncapsulatedMemberDescriptor<GuestNiceName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUPackageMetric::*)(),&LD::CPUPackageMetric::GuestNice>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUPackageMetric::*)() const,&LD::CPUPackageMetric::GuestNice>()>


    >;
    static constexpr MemberList Members{  };
};


template<LD::UInteger Core>
class LD::CT::TypeDescriptor<LD::CPUCoreMetric<Core>>
{
private:
    static constexpr auto UserName = ctll::basic_fixed_string("User");
    static constexpr auto NiceName = ctll::basic_fixed_string("Nice");
    static constexpr auto SystemName = ctll::basic_fixed_string("System");
    static constexpr auto IdleName = ctll::basic_fixed_string("Idle");
    static constexpr auto IOWaitName = ctll::basic_fixed_string("IOWait");
    static constexpr auto IRQName = ctll::basic_fixed_string("IRQ");
    static constexpr auto SoftIRQName = ctll::basic_fixed_string("SoftIRQ");
    static constexpr auto StealName = ctll::basic_fixed_string("Steal");
    static constexpr auto GuestName = ctll::basic_fixed_string("Guest");
    static constexpr auto GuestNiceName = ctll::basic_fixed_string("GuestNice");
public:
    //auto abc = ctll::fixed_string{LD::ImmutableString{"cpu"}+LD::ImmutableString{LD::ToTypeString<Core>{}}};
    //static constexpr auto ClassName =  ctll::fixed_string{LD::ImmutableString{"cpu"}+LD::ImmutableString{LD::ToTypeString<Core>{}}};

    static constexpr auto ClassName = ctll::fixed_string{LD::TypeString<'c','p','u'>{}+LD::ToTypeString<Core>{}};
    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<UserName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::User>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::User>()>,
            LD::CT::EncapsulatedMemberDescriptor<NiceName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::Nice>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::Nice>()>,
            LD::CT::EncapsulatedMemberDescriptor<SystemName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::System>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::System>()>,
            LD::CT::EncapsulatedMemberDescriptor<IdleName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::Idle>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::Idle>()>,
            LD::CT::EncapsulatedMemberDescriptor<IOWaitName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::IOWait>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::IOWait>()>,
            LD::CT::EncapsulatedMemberDescriptor<IRQName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::IRQ>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::IRQ>()>,
            LD::CT::EncapsulatedMemberDescriptor<SoftIRQName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::SoftIRQ>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::SoftIRQ>()>,
            LD::CT::EncapsulatedMemberDescriptor<StealName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::Steal>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::Steal>()>,
            LD::CT::EncapsulatedMemberDescriptor<GuestName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::Guest>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::Guest>()>,
            LD::CT::EncapsulatedMemberDescriptor<GuestNiceName,LD::CT::SelectOverload<LD::UInteger & (LD::CPUCoreMetric<Core>::*)(),&LD::CPUCoreMetric<Core>::GuestNice>(),LD::CT::SelectOverload<const LD::UInteger & (LD::CPUCoreMetric<Core>::*)() const,&LD::CPUCoreMetric<Core>::GuestNice>()>
    >;

    static constexpr MemberList Members{  };
};

template<>
struct LD::CT::TypeDescriptor<LD::ContextSwitchMetric>
{
private:
    static constexpr auto SwitchName = ctll::basic_fixed_string("Switch");
public:
    static constexpr auto ClassName = ctll::fixed_string{"ctxt"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<SwitchName,LD::CT::SelectOverload<LD::UInteger & (LD::ContextSwitchMetric::*)(),&LD::ContextSwitchMetric::Switches>(),LD::CT::SelectOverload<const LD::UInteger & (LD::ContextSwitchMetric::*)() const,&LD::ContextSwitchMetric::Switches>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::InterruptServiceMetric>
{
private:
    static constexpr auto InterruptName = ctll::basic_fixed_string("Interrupt");
public:
    static constexpr auto ClassName = ctll::fixed_string{"intr"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<InterruptName,LD::CT::SelectOverload<LD::UInteger & (LD::InterruptServiceMetric::*)(),&LD::InterruptServiceMetric::Interrupts>(),LD::CT::SelectOverload<const LD::UInteger & (LD::InterruptServiceMetric::*)() const,&LD::InterruptServiceMetric::Interrupts>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::BootUpTimeMetric>
{
private:
    static constexpr auto TimeName = ctll::basic_fixed_string("Time");
public:
    static constexpr auto ClassName = ctll::fixed_string{"btime"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TimeName,LD::CT::SelectOverload<LD::UInteger & (LD::BootUpTimeMetric::*)(),&LD::BootUpTimeMetric::BootUpTime>(),LD::CT::SelectOverload<const LD::UInteger & (LD::BootUpTimeMetric::*)() const,&LD::BootUpTimeMetric::BootUpTime>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::ProcessesMetric>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("Amount");
public:
    static constexpr auto ClassName = ctll::fixed_string{"processes"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::UInteger & (LD::ProcessesMetric::*)(),&LD::ProcessesMetric::Amount>(),LD::CT::SelectOverload<const LD::UInteger & (LD::ProcessesMetric::*)() const,&LD::ProcessesMetric::Amount>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::ProcessesRunningMetric>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("Amount");
public:
    static constexpr auto ClassName = ctll::fixed_string{"procs_running"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::UInteger & (LD::ProcessesRunningMetric::*)(),&LD::ProcessesRunningMetric::Amount>(),LD::CT::SelectOverload<const LD::UInteger & (LD::ProcessesRunningMetric::*)() const,&LD::ProcessesRunningMetric::Amount>()>
    >;


    static constexpr MemberList Members{  };

};
//procs_blocked

template<>
struct LD::CT::TypeDescriptor<LD::ProcessesBlockedMetric>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("Amount");
public:
    static constexpr auto ClassName = ctll::fixed_string{"procs_blocked"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::UInteger & (LD::ProcessesBlockedMetric::*)(),&LD::ProcessesBlockedMetric::Amount>(),LD::CT::SelectOverload<const LD::UInteger & (LD::ProcessesBlockedMetric::*)() const,&LD::ProcessesBlockedMetric::Amount>()>
    >;


    static constexpr MemberList Members{  };

};
//softirq
template<>
struct LD::CT::TypeDescriptor<LD::SoftIRQMetric>
{
private:
    static constexpr auto InterruptName = ctll::basic_fixed_string("Interrupts");
public:
    static constexpr auto ClassName = ctll::fixed_string{"softirq"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<InterruptName,LD::CT::SelectOverload<LD::UInteger & (LD::SoftIRQMetric::*)(),&LD::SoftIRQMetric::Interrupts>(),LD::CT::SelectOverload<const LD::UInteger & (LD::SoftIRQMetric::*)() const,&LD::SoftIRQMetric::Interrupts>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::Square>
{
private:
    static constexpr auto LengthName = ctll::basic_fixed_string("Length");
public:
    static constexpr auto ClassName = ctll::fixed_string{"Square"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<LengthName,LD::CT::SelectOverload<LD::Float & (LD::Square::*)(),&LD::Square::Length>(),LD::CT::SelectOverload<const LD::Float & (LD::Square::*)() const,&LD::Square::Length>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_REFLECTIONDEMOTYPES_H
