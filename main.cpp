#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Chrono/Timer.h"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/span.hpp"
#include "Primitives/General/Span.hpp"
using PrimitiveVariant = LD::Variant<
        LD::ElementReference<float>,
        LD::ElementReference<double>,
        LD::ElementReference<long>,
        LD::ElementReference<double>,
        LD::ElementReference<bool>,
        LD::ElementReference<char>,
        LD::ElementReference<unsigned char>,
        LD::ElementReference<short>,
        LD::ElementReference<int>,
        LD::ElementReference<long>,
        LD::ElementReference<long long>,
        LD::ElementReference<long long int>,
        LD::ElementReference<unsigned short>,
        LD::ElementReference<unsigned int>,
        LD::ElementReference<unsigned long>,
        LD::ElementReference<unsigned long long>,
        LD::ElementReference<unsigned long long int>,
        LD::StringView,LD::Span<bool>,LD::Span<char>,LD::Span<short>,LD::Span<int>,
        LD::Span<long>,LD::Span<long long>,LD::Span<long long int>,LD::Span<unsigned short>,
        LD::Span<unsigned int>,LD::Span<unsigned long>,LD::Span<unsigned long long>,
        LD::Span<unsigned long long int>
>;

class AccessReadOnly
{

};

class AccessWriteOnly
{

};

class AccessReadWrite
{

};

using ArrayVariant = LD::Variant<LD::StringView>;
template<typename T, typename Key ,typename F, typename H, typename Q ,typename ... Args>
LD::Enable_If_T<
LD::Require<
LD::IsReflectable<T>,
LD::IsTypeString<Key>,
LD::Either<LD::IsSame<Q,AccessReadOnly>,LD::IsSame<Q,AccessWriteOnly>,LD::IsSame<Q,AccessReadWrite>>,
LD::ConvertiblyCallable<H,void(const LD::ContextualVariant<PrimitiveVariant(LD::StringView,Args...)> &)>::Value()
>
,void> NoSQLTraversal(const Key & key,T && object, F && onClassName, H && onMember , const Q & accessMode ,Args && ... arguements)
{

    using Type = LD::Decay_T<T>;
    using ClassName = decltype(Type::GetClassNameTypeString());
    using Period = LD::TypeString<'.'>;
    //using Period = LD::TypeString<'.'>;
    constexpr LD::UInteger numberOfMembers = Type::NumberOfMembers;

    onClassName(LD::StringView {Key::data(),Key::size()},LD::StringView{ClassName::data(),ClassName::size()});
    //todo optimize with access mode for different access patterns
    LD::ContextualVariant<LD::Variant<AccessReadWrite,AccessWriteOnly,AccessReadOnly>()> context;
    LD::For<1,numberOfMembers>([](auto I, T && object,F && onClass ,H && onMember,Args && ... arguements)
    {
        using MemberType = LD::Decay_T<decltype(LD::Get<I>(*object))>;
        using MemberName = decltype(LD::Declval<T>()[LD::CompileTimeIndex<I>{}]);
        using MemberKey = LD::tycat<Key,Period ,MemberName>;
        auto & reference = object[MemberName{}];
        if constexpr (LD::IsPrimitive<MemberType> && LD::Negate<LD::IsPointer<MemberType>>)
        {

            LD::ElementReference<MemberType> memberReference = LD::ElementReference<MemberType>{reference};
            onMember(LD::MakeContext(LD::ElementReference<MemberType>{memberReference},
                    LD::StringView{MemberKey::data(),MemberKey::size()},
                    LD::Forward<Args>(arguements)...));

        }else if constexpr (LD::Detail::Spannable<T>::value)
        {
            LD::Span<MemberType> currentSpan{reference};
            //todo maybe use iterators for ranging instead? offer optional iterators for insertions
            onMember(LD::MakeContext(LD::Span<MemberType>{currentSpan},
                                     LD::StringView{MemberKey::data(),MemberKey::size()},
                                     LD::Forward<Args>(arguements)...));


        }else if constexpr(LD::IsReflectable<MemberType>)
        {

            return NoSQLTraversal(MemberKey{},reference,LD::Forward<T>(onClass),LD::Forward<H>(onMember),LD::Forward<Args>(arguements)...);
        }
        return true;
    },LD::Forward<T>(object),LD::Forward<F>(onClassName),LD::Forward<H>(onMember),LD::Forward<Args>(arguements)...);
}

int main()
{

    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();
    LD::Example::ReflectionExample();
    currentTimer.Stop();

    std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}