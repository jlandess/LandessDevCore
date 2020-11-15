//
// Created by phoenixflower on 11/1/20.
//

#ifndef LANDESSDEVCORE_TEXT_HPP
#define LANDESSDEVCORE_TEXT_HPP
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/Span.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
namespace LD
{
    namespace SPA
    {
        /**
         *
         * @tparam T
         * @brief Text can only be represented by objects which themselves can be represented by a LD::ImmutableString or any container for a contiguous string of characters
         */
        template<typename T, class = void>
        class Text;
        template<typename T>
        class Text<T,LD::Enable_If_T<LD::Either<
                LD::CT::CanBeAnImmutableString(LD::Type<T>{}),
                LD::CT::SpannableWithConstraint(LD::Type<T>{},LD::Type<char>{})
        >>>
        {
        private:
            T mValue;
        public:

            Text() = default;

            constexpr Text(T && value) noexcept:mValue{LD::Forward<T>(value)}
            {

            }
            T & Element() noexcept
            {
                return this->mValue;
            }
            const T & Element() const noexcept
            {
                return this->mValue;
            }
        };

        template <typename T> Text(T && value) -> Text<T>;
    }

    namespace SPA
    {
        template<typename T>
        constexpr bool IsText(LD::Type<LD::SPA::Text<T>> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsText(LD::Type<T> ) noexcept
        {
            return false;
        }
    }
}
template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::Text<T>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("element");
public:
    static constexpr auto ClassName = ctll::fixed_string{"Text"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<T & (LD::SPA::Text<T>::*)(),&LD::SPA::Text<T>::Element>(),LD::CT::SelectOverload<const T & (LD::SPA::Text<T>::*)() const,&LD::SPA::Text<T>::Element>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_TEXT_HPP
