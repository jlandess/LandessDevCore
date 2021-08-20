//
// Created by phoenixflower on 7/23/21.
//

#ifndef LANDESSDEVCORE_TDD_HPP
#define LANDESSDEVCORE_TDD_HPP
#include "Primitives/General/Unit.hpp"
#include "Reflection/Reflection.hpp"
#include "TypeTraits/Type.h"
namespace LD
{
    namespace TDD
    {
        template<typename T, typename TestNameType>
        class Result
        {
        private:
            LD::Milisecond<LD::Float> mRuntime;
            TestNameType mName;
            bool mPassed;
        public:
            Result() = default;

            Result(LD::Milisecond<LD::Float> runtime, const TestNameType & name, bool passState) noexcept:mRuntime{runtime},mName{name},mPassed{passState}
            {
            }
            LD::Milisecond<LD::Float> & RunTime() noexcept
            {
                return this->mRuntime;
            }

            TestNameType & Name() noexcept
            {
                return this->mName;
            }

            const TestNameType & Name() const noexcept
            {
                return this->mName;
            }


            bool & Passed() noexcept{return this->mPassed;}
            const bool & Passed() const noexcept{return this->mPassed;}

            const LD::Milisecond <LD::Float> & RunTime() const noexcept {return this->mRuntime;}

            const T & ProvidedResult() const noexcept {return this->mResult;}

            T & ProvidedResult() noexcept {return this->mResult;}

            T & ExpectedResult() noexcept {return this->mExpectedResult;}

            const T & ExpectedResult() const noexcept {return this->mExpectedResult;}
        };

        namespace CT
        {
            template<typename T>
            constexpr bool IsResult(LD::Type<T> ) noexcept
            {
                return false;
            }

            template<typename T, typename TestNameType>
            constexpr bool IsResult(LD::Type<LD::TDD::Result<T,TestNameType>> ) noexcept
            {
                return true;
            }
        }
    }
}

template<typename T, typename TS>
struct LD::CT::TypeDescriptor<LD::TDD::Result<T,TS>>
{
private:
    static constexpr auto RunTimeTag = ctll::basic_fixed_string("Runtime");
    static constexpr auto PassedTag = ctll::basic_fixed_string("PassedState");
    static constexpr auto NameTag = ctll::basic_fixed_string("Name");
public:
    static constexpr auto ClassName = ctll::fixed_string{"Test"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<RunTimeTag,LD::CT::SelectOverload<LD::Milisecond <LD::Float> & (LD::TDD::Result<T,TS>::*)(),&LD::TDD::Result<T,TS>::RunTime>(),LD::CT::SelectOverload<const LD::Milisecond <LD::Float> & (LD::TDD::Result<T,TS>::*)() const,&LD::TDD::Result<T,TS>::RunTime>()>,
            LD::CT::EncapsulatedMemberDescriptor<PassedTag,LD::CT::SelectOverload<bool & (LD::TDD::Result<T,TS>::*)(),&LD::TDD::Result<T,TS>::Passed>(),LD::CT::SelectOverload<const bool & (LD::TDD::Result<T,TS>::*)() const,&LD::TDD::Result<T,TS>::Passed>()>,
            LD::CT::EncapsulatedMemberDescriptor<NameTag,LD::CT::SelectOverload<TS & (LD::TDD::Result<T,TS>::*)(),&LD::TDD::Result<T,TS>::Name>(),LD::CT::SelectOverload<const TS & (LD::TDD::Result<T,TS>::*)() const,&LD::TDD::Result<T,TS>::Name>()>
    >;


    static constexpr MemberList Members{  };

};

namespace LD
{
    namespace TDD
    {
        template<typename Timer,typename T,typename ... Args,LD::UInteger N,
                typename A = decltype(LD::Declval<T>()(LD::Declval<Args>()...))>
        inline auto Execute(
                LD::ImmutableString<N> testName,
                Timer & timer,
                T && object,
                A && expectedResponse,
                Args && ... args) noexcept -> LD::TDD::Result<A,LD::ImmutableString<N>>
        {

            timer.Start();
            bool result = (expectedResponse == object(LD::Forward<Args>(args)...));
            timer.Stop();
            auto time = timer.Time();
            return LD::TDD::Result<A,LD::ImmutableString<N>>{timer.Time(),testName,result};
        }
    }
}
#endif //LANDESSDEVCORE_TDD_HPP
