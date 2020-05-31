//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_ISCONVERTIBLE_HPP
#define LANDESSDEVCORE_ISCONVERTIBLE_HPP
namespace LD
{

    namespace Detail
    {
        struct IsConvertibleImpl
        {
            template <typename _Type, typename _OtherType>
            static void convert(_OtherType);
            template <typename _Type, typename _OtherType>
            static auto test(_Type&&, _OtherType&&) -> decltype(convert<_Type, _OtherType>(LD::Declval<_Type>()), LD::TrueType());
            static auto test(...) -> decltype(LD::FalseType());


            typedef char small_;

            struct big_ {
                char dummy[2];
            };

            template <class U> static small_ tester(void (U::*)());
            template <class U> static big_ tester(...);
            //static const bool value = sizeof(tester<T>(0)) == sizeof(small_);


            template <typename From, typename To>
            struct ConvertHelper {
                static small_ Test(To);
                static big_ Test(...);
                static From Create();
            };
        };



        template <typename _Type, typename _OtherType>
        struct IsConvertible : public decltype(Detail::IsConvertibleImpl::test(LD::Declval<_Type>(), LD::Declval<_OtherType>()))
        {};
    }
}
#endif //LANDESSDEVCORE_ISCONVERTIBLE_HPP
