//
// Created by phoenixflower on 10/27/20.
//

#ifndef LANDESSDEVCORE_DIV_HPP
#define LANDESSDEVCORE_DIV_HPP
#include "Definitions/Common.hpp"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/Detection.hpp"
#include "Text.hpp"
#include "Attributes.hpp"
#include "TypeTraits/TypeList.hpp"
#include "SPA/IsAttribute.hpp"
namespace LD
{
    namespace SPA
    {
        namespace Detail
        {
            template<typename T>
            using IsXMLValue = LD::Detail::IntegralConstant<bool, LD::SPA::IsText(LD::CT::RemoveQualifiers(LD::Type<T>{}))>;
            template<typename T>
            using IsXMLAttribute = LD::Detail::IntegralConstant<bool, LD::SPA::IsAttribute(LD::CT::RemoveQualifiers(LD::Type<T>{}))>;

            template<typename ... B>
            constexpr LD::UInteger GetNumerOfAttributes(LD::CT::TypeList<B...>) noexcept
            {
                using Auo1 = LD::CT::TypeList<B...>;
                LD::UInteger result = 0;
                LD::For<Auo1 ::size()>([](auto I,LD::UInteger & result)
                {
                    result += LD::SPA::IsAttribute(LD::CT::RemoveQualifiers(LD::Type<LD::CT::TypeAtIndex<I,Auo1>>{}));
                    return true;

                },result);
                return result;
            }
        }
        //using TL = LD::CT::Filter<TypeListDemo ,IsXMLValue>

        /**
         *
         * @tparam Components
         * @brief A div can be instansiated if it only has text and attributes or if it has attributes and child nodes, it cannot contain text and child nodes
         */
        template<typename Components, class = void>
        class Div;
        template<typename ... Components>
        class Div<LD::CT::TypeList<Components...>,LD::Enable_If_T<
                LD::Either<
                        (LD::CT::Filter<LD::CT::TypeList<Components...> ,LD::SPA::Detail::IsXMLValue>::size() == 0),
                        LD::Require<
                                (LD::CT::Filter<LD::CT::TypeList<Components...> ,LD::SPA::Detail::IsXMLValue>::size() == 1),
                                (LD::SPA::Detail::GetNumerOfAttributes(LD::CT::TypeList<Components...>{}) == sizeof...(Components)-1)
                                >
                        >>>
        {
        private:
            LD::Tuple<Components...> mComponents;
            template<typename T>
            using InternalAttributionCheck = LD::Detail::IntegralConstant<bool,LD::SPA::IsAttribute(LD::CT::RemoveQualifiers(LD::Type<T>{}))>;
        public:
            Div(Components && ... arguments) noexcept:mComponents{LD::Forward<Components>(arguments)...}
            {
            }
            LD::Tuple<Components...> & Elements() noexcept
            {
                return this->mComponents;
            }
            const LD::Tuple<Components...> & Elements() const noexcept
            {
                return this->mComponents;
            }
        };
        template <typename ... Components> Div(Components && ... components) -> Div<LD::CT::TypeList<Components...>>;
    }
}
template<typename ... B>
struct LD::CT::TypeDescriptor<LD::SPA::Div<LD::CT::TypeList<B...>>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("elements");
public:
    static constexpr auto ClassName = ctll::fixed_string{"div"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::Tuple<B...> & (LD::SPA::Div<LD::CT::TypeList<B...>>::*)(),&LD::SPA::Div<LD::CT::TypeList<B...>>::Elements>(),LD::CT::SelectOverload<const LD::Tuple<B...> & (LD::SPA::Div<LD::CT::TypeList<B...>>::*)() const,&LD::SPA::Div<LD::CT::TypeList<B...>>::Elements>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_DIV_HPP
