//
// Created by James Landess on 1/20/20.
//

#ifndef LANDESSDEVCORE_TERMBOXIMMUTABLELABEL_H
#define LANDESSDEVCORE_TERMBOXIMMUTABLELABEL_H

#include "Definitions/Common.h"
#include "Algorithms/CompileTimeControlFlow.h"
#include "Primitives/General/Vec2.h"
#include "Primitives/General/Immutable.h"
namespace LD
{
    namespace TB
    {
        class BasicImmutableLabelWarrant
        {

        };




        template <class T>
        using ImmutableLabelWarrantCheck = decltype(LD::Declval<T>().GetImmutableLabelWarrant());


        template<typename T>
        constexpr bool IsImmutableLabel= LD::Exists<LD::TB::ImmutableLabelWarrantCheck,T>;


        template<typename T,LD::UInteger LabeSize>
        class BasicImmutableLabel
        {
        private:
            LD::ImmutableString<LabeSize> mString;
            PDP::Detail::tVec2<LD::UInteger> mLocation;
        public:

            inline explicit BasicImmutableLabel() noexcept :mLocation{0,0} {}



            inline const LD::ImmutableString<LabeSize> & GetImmutableString() const noexcept {return this->mString;}
            inline const PDP::Detail::tVec2<LD::UInteger> & GetLocation() const noexcept {return this->mLocation;}
            inline void SetLocation(const PDP::Detail::tVec2<LD::UInteger> & location) noexcept {this->mLocation = location;}


            template<LD::UInteger size, typename = typename LD::Enable_If_T<size <= LabeSize>>
            inline explicit BasicImmutableLabel(const LD::ImmutableString<size> & string) noexcept
            {

                LD::For<LabeSize>([&](auto I)
                {
                    mString[I] = '\n';
                    return true;
                });

                LD::For<LabeSize>([&](auto I)
                {
                    this->mString[I] = string[I];
                    return true;
                });
            }


            constexpr BasicImmutableLabel(const char (&lit)[LabeSize + 1]): BasicImmutableLabel()
            {
                (*this) = lit;
            }

            template<char ... Characters,typename = typename LD::Enable_If_T<sizeof...(Characters) <= LabeSize>>
            inline explicit BasicImmutableLabel(const LD::TypeString<Characters...> & typeString)
            {
                LD::For<LabeSize>([&](auto I)
                {
                    mString[I] = '\n';
                    return true;
                });

                LD::For<LabeSize>([&](auto I)
                {
                    this->mString[I] = LD::TypeString<Characters...>::data()[I];
                    return true;
                });
            }


            constexpr BasicImmutableLabel & operator = (const char (&lit)[LabeSize + 1])
            {
                LD::For<0,LabeSize>([](auto I, LD::ImmutableString<LabeSize> & instance, const char * buffer)
                {
                    instance[I] = buffer[I];
                    return true;

                },this->mString,lit);

                return (*this);
            }


            constexpr inline BasicImmutableLabelWarrant GetImmutableLabelWarrant() const
            {
                return BasicImmutableLabelWarrant{};
            }

        };
    }
}
#endif //LANDESSDEVCORE_TERMBOXIMMUTABLELABEL_H
