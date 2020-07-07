#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Examples/DelimeterSeperatedFileExample.h"
#include "Chrono/Timer.h"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Algorithms/FuzzySearch.h"
#include "IO/RowBackingStore.h"
#include "IO/BasicDelimeterSeperatedFile.h"
#include "Async/Thread.h"
#include <unistd.h>
#include "Primitives/General/Range.h"
#include "Algorithms/FullAdder.hpp"
#include "Algorithms/ComplimentGenerator.h"
#include "Primitives/General/Hexademical.hpp"
#include "Primitives/General/ByteBuffer.hpp"
#include "Algorithms/refl.hpp"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/FunctionalReflection.hpp"






template<typename T>
void MapToNamedTuple()
{

}

template<typename T>
class Square
{
private:
    static constexpr auto stuffings = "abc"_ts;
};


struct Point
{
    float x;
    float y;

    const float & X() const noexcept
    {
        return this->x;
    }

    float & X() noexcept
    {
        return this->x;
    }

    void SetX(const float & x) noexcept
    {
        this->x = x;
    }
};

namespace LD
{
    namespace CT
    {
        template<typename TFunctionSignature, TFunctionSignature tBoundFunction>
        constexpr auto SelectOverload() noexcept
        {
            return tBoundFunction;
        }
    }

}

template<>
class LD::CT::TypeDescriptor<Point>//: public LD::CT::Reflectable<LD::CT::ClassName<SquareClassName>(LD::CT::Member<stuffings,T>)>
{
private:
    static constexpr auto XName = "x"_ts;
    static constexpr auto YName = "y"_ts;
public:
    static constexpr auto ClassName = "Point"_ts;
    using MemberList = LD::CT::TypeList<
            //LD::CT::Member<LengthName,T>,
            //LD::CT::MemberDescriptor<XName,xMember>,
            LD::CT::EncapsulatedMemberDescriptor<XName,LD::CT::SelectOverload<float & (Point::*)(),&Point::X>(),LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>()>,
            LD::CT::MemberDescriptor<YName,&Point::y>
            >;
    static constexpr MemberList Members{  };
};

template<typename T, typename F>
struct MapMembersToTuple
{

};

template<typename ... Members, typename Object>
struct MapMembersToTuple<LD::CT::TypeList<Members...>,Object>
{
    //using type = LD::CT::TypeList<LD::Detail::Decay_T<decltype(LD::Declval<Members>()(LD::Declval<Object>()))>...>;
    using type = LD::CT::TypeList<LD::Detail::Decay_T<LD::CT::GetMemberType<Members,Object>>...>;
};
template<typename T, typename V = LD::Detail::Decay_T<T>,
        typename Members = typename MapMembersToTuple<typename LD::CT::TypeDescriptor<V>::MemberList,T>::type>
constexpr LD::Enable_If_T<
        LD::Require<
                LD::CT::Detail::IsReflectable<V>::value
>,
LD::CT::RebindList<Members,LD::Tuple>> MapToTuple(T && object) noexcept
{

    using MemberTuple = LD::CT::RebindList<Members,LD::Tuple>;
    MemberTuple ret;
    auto traits = LD::CT::Reflect(LD::Forward<T>(object)).Members;
    LD::For<Members::size()>([](
            auto I,
            T && object,
            auto && traits,
            auto && tuple)
    {
        auto memberInfo = LD::Get<I>(traits);

        LD::Get<I>(LD::Forward<MemberTuple>(tuple)) =  memberInfo(LD::Forward<T>(object));

        return true;
    },object,LD::Forward<decltype(traits)>(traits),LD::Forward<MemberTuple>(ret));
    using stuff = typename MapMembersToTuple<typename LD::CT::TypeDescriptor<V>::MemberList,T>::type;
    //LD::CT::DebugTemplate<stuff>{};
    typename LD::CT::TypeDescriptor<V>::MemberList;
    //constexpr LD::UInteger ListSize = List::size();
    //LD::Rebind<typename T::ValueTypeList,LD::Tuple>;

    return ret;
}





/* Metadata */
REFL_AUTO(
        type(Point),
        field(x, /* attributes */),
        field(y)
)
template <typename Member>
struct instance_field_invoker
{
    template <typename T>
    static constexpr auto invoke(T&& target) -> decltype(target.*(Member::pointer))
    {
        return target.*(Member::pointer);
    }

    template <typename T, typename U, typename M = Member, std::enable_if_t<M::is_writable, int> = 0>
    static constexpr auto invoke(T&& target, U&& value) -> decltype(target.*(Member::pointer) = std::forward<U>(value))
    {
        return target.*(Member::pointer) = std::forward<U>(value);
    }
};


template<const  auto  Rawr,auto Bunnies>
class Mooo
{

};
int main()
{

    static constexpr auto Rawr = LD::CT::GetSignatureReturn(LD::CT::FunctionSignature<decltype(LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>())>{});


    LD::CT::IsSignatureConst(LD::CT::FunctionSignature<decltype(LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>())>{});
    LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>();
    //LD::CT::FunctionSignature<decltype(LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>())>::
    Mooo<&Point::SetX,LD::CT::SelectOverload<float & (Point::*)(),&Point::X>()>{};
    //"[a-z]"_ctre.match(std::string_view{});
    //Mooo<ctre::fixed_string{"1234"},&Point::SetX> {};
    static_assert(LD::CT::Detail::HasReflectableTraits<LD::CT::TypeDescriptor<Point>>::value,"");
    using MemberListType = LD::Detail::Decay_T<decltype(LD::Declval<LD::CT::TypeDescriptor<Point>>().Members)>;
    static_assert(LD::CT::IsTypeList<MemberListType>);
    static_assert(LD::CT::Detail::IsReflectable<Point>::value);
    LD::CT::Detail::IsReflectable<Point>{};
    //static constexpr auto xSetter = &Point::SetX;
    static constexpr  float & (Point::*xSetter)() = &Point::X;
    //static constexpr auto xGetter = &Point::X;
    static constexpr float & (Point::*xGetter)()  = &Point::X;
    //LD::CT::DebugTemplate<decltype(LD::Declval<LD::CT::TypeDescriptor<int>>())>{};
    static constexpr auto pointer(&Point::x);
    //LD::CT::DebugTemplate<decltype(pointer)>{};
    //LD::CT::Member<stuffz,int>;
    static constexpr std::size_t (std::string::*mooo)() const = &std::string::length;

    static constexpr auto str = ctll::fixed_string{ "h.*" };
    static constexpr auto stuffings = "abc"_ts;
    LD::CT::Member<pointer,int> abc;
    static_assert(LD::CT::IsMember<LD::CT::Member<stuffings,int>>,"");
    ctre::match<str>("");
    //Test<"abc"_ts> abc;
    LD::BitSet<8> five1{0,0,0,0,0,1,0,1};
    LD::BitSet<8> three1{0,0,0,0,0,0,1,1};

    Point pt{5,7};


    pt.X() = 97;

    LD::Tuple<float,float> point =  MapToTuple(pt);

    std::cout << "Point : " << LD::Get<0>(point) << "," << LD::Get<1>(point) << std::endl;

    constexpr auto traits = LD::CT::Reflect(pt).Members;

    LD::Get<1>(traits)(pt);
    using Traits = decltype(traits);
    static constexpr  LD::UInteger traitsSize = Traits::size();
    LD::For<traitsSize>([](auto I, const Point & pt, auto && traits)
    {

        auto memberInfo = LD::Get<I>(traits);

        //memberInfo(pt).Name;
        std::cout << memberInfo.Name.data() << " : "  << memberInfo(pt) << std::endl;

        return true;
    },pt,traits);

    LD::CT::MemberDescriptor<stuffings,pointer> s;
    LD::CT::EncapsulatedMemberDescriptor<stuffings,xSetter,xGetter> t;


    //pt.(xGetter)();
    t(pt) = 72;
    std::cout << (pt.*xGetter)() << std::endl;
    //s(pt) = 92;
    //pt.*(pointer) = 9;


    //pt.*(Point::x);



    refl::is_reflectable(int{});
    auto rawr = refl::reflect(pt).members;
    //LD::CT::DebugTemplate<decltype(rawr)>{};
     auto values = map_to_tuple(refl::reflect(pt).members, [&](auto member) {
        // refl::descriptor::is_readable (found by Koenig lookup)


        is_writable(member);
         //LD::CT::DebugTemplate<decltype(member)>{};
         //is_readable(member);
        if constexpr (is_readable(member))
        {
            //member.type;
            std::cout << member.name.str() << std::endl;
            return member(pt); // invoke the member
        }
    });



     refl::is_reflectable(int{});
     LD::For<2>([](auto I, auto && tuple)
     {
         std::cout << std::get<I>(tuple) << std::endl;
         return true;
     },values);

    LD::FullAdder<8> adder;

    auto setResult = adder(five1,three1);

    LD::Hexademical<2> address{LD::BitSet<8>{1,1,1,0,1,1,1,1}};
    LD::Hexademical<16> number{LD::GenerateBitSet(92392423216,LD::CT::Range<0,64>{})};

    LD::ImmutableByteBuffer<8> numberAsBytes{LD::GenerateBitSet(92392423216,LD::CT::Range<0,64>{})};
    for(LD::UInteger n = 0;n<8;++n)
    {
        std::cout  << LD::UInteger (numberAsBytes[n]) << ",";
    }
    std::cout << "\n";
    for(LD::UInteger n = 0;n<16;++n)
    {
        std::cout << number[n];
    }
    std::cout << "\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << address[i];
    }
    std::cout << "\n";
    std::cout << "Test 5" << std::endl;
    for(LD::UInteger n = 0;n<8;++n)
    {
        std::cout << setResult[n];
    }

    auto bits = LD::GenerateBitSet(-8,LD::CT::Range<0,11>{});

    std::cout << std::endl;
    for(LD::UInteger n = 0;n<11;++n)
    {
        std::cout << bits[n];
    }
    std::cout << std::endl;

    auto doubleAsBits = LD::GenerateBitSet(0.043);
    for(LD::UInteger n = 0;n<18;++n)
    {
        std::cout << doubleAsBits[n];
    }
    /*
     */
    //std::printf("x = %.2lf = %.2lf * 2^%d\n", x, fraction, e);
    /*
    LD::RowBackingStore backingStore{"/proc/stat"};
    LD::SpaceSpeerateValueFile<LD::RowBackingStore> file{backingStore};
    //using ObjectPack = LD::CT::TypeList<LD::CPUPackageMetric,LD::CPUCoreMetric<0>,LD::CPUCoreMetric<1>,LD::CPUCoreMetric<2>>;
    using ObjectPack = LD::GenerateSystemRepresentation<16>;
    using ObjectPackVariant = LD::Rebind<ObjectPack,LD::Variant>;
    LD::UInteger previous_idle_time=0, previous_total_time=0;
    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> it{previous_idle_time,previous_total_time};// = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{},previous_idle_time,previous_total_time);
    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> end = file.End(ObjectPack{},previous_idle_time,previous_total_time);
    std::cout << LD::CPUCoreMetric<0>::GetClassName() << std::endl;
    for (LD::UInteger i = 0; i < 10; ++i)
    {

        for (auto it = file.Begin(ObjectPack{},previous_idle_time,previous_total_time); it != end; ++it)
        {


            //called when we get a LD::CPUPackageMetric Object
            auto onCPUPackageMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::CPUPackageMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {

                std::cout << "found package " << std::endl;
                LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(context));
                LD::UInteger & previous_idle_time = LD::Get(LD::Get<2>(context));
                LD::UInteger & previous_total_time = LD::Get(LD::Get<3>(context));
                LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

                //std::cout << "Previous Idle Time = " << previous_idle_time << std::endl;
                //std::cout << "Previous Total Time = " << previous_idle_time << std::endl;
                const LD::UInteger idle_time = metric.Idle();
                //std::cout << "Idle Time : " << idle_time << std::endl;
                const double idle_time_delta = idle_time - previous_idle_time;
                const double total_time_delta = total_time - previous_total_time;
                const double utilization = (idle_time_delta!=total_time_delta)*(100.0 * (1.0 - idle_time_delta / total_time_delta));
                std::cout << "Utilizaiton : " <<  utilization << std::endl;
                //std::cout << "Sum : " << total_time << std::endl;
                LD::Float idlePercentage = (metric.Idle()*100)/total_time;
                //std::cout << "Usage: " << ((sum-metric.Idle())*100.0)/sum << std::endl;

                previous_idle_time = idle_time;
                previous_total_time = total_time;
            };

            //called when we find an object that matches LD::CPUCoreMetric<LD::UInteger>
            auto onCPUCoreMetric = [](auto  && stuff) noexcept -> LD::Enable_If_T<LD::Require<LD::IsCPUCore<LD::GetType<decltype(LD::GetQueryType(stuff))>>>,void>
            {
                using CoreType = decltype(LD::GetQueryType(stuff));
                using Underlyingcore = LD::GetType<CoreType >;
                constexpr static const LD::UInteger Core = Underlyingcore::Core;
                std::cout << "found core " << Core <<  std::endl;


                if constexpr(LD::IsCPUCore<Type>)
                {
                    using CoreType = decltype(LD::GetQueryType(stuff));
                    using Underlyingcore = typename CoreType::type;
                    constexpr static const LD::UInteger Core = Underlyingcore::Core;
                    std::cout << "found core " << Core <<  std::endl;
                }


            };

            //called when we get a LD::CPUPackageMetric Object
            auto onContextSwitchMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ContextSwitchMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onInterruptServiceMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::InterruptServiceMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onBootUpTimeMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::BootUpTimeMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessRunningMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesRunningMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessBlockedMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesBlockedMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            //LD::SoftIRQMetric

            auto onSoftIRQMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::SoftIRQMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            //called if we get a Database Error, such as getting a row with no associated objectyghtytynonneaew WQQq
            auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::UInteger&,LD::UInteger&> & context) noexcept
            {

                //std::cout << "found core 0 " << std::endl;
            };


            LD::Match(*it,
                    onCPUPackageMetric,
                    onDatabaseError,
                    onCPUCoreMetric,
                    onContextSwitchMetric,
                    onInterruptServiceMetric,
                    onBootUpTimeMetric,
                    onProcessMetric,
                    onProcessRunningMetric,
                    onProcessBlockedMetric,
                    onSoftIRQMetric);
           //sleep(1);
        }
        sleep(1);
    }
     */
    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();
    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();
    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}