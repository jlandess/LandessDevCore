//
// Created by phoenixflower on 10/27/20.
//

#ifndef LANDESSDEVCORE_HTML_HPP
#define LANDESSDEVCORE_HTML_HPP
#include "Definitions/Common.hpp"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename ... Components>
        class HTML
        {
        private:
            LD::Tuple<Components...> mComponents;
        public:
            HTML(Components && ... arguments) noexcept:mComponents{LD::Forward<Components>(arguments)...}
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
        template <typename ... Components> HTML(Components && ... components) -> HTML<Components...>;
    }

}

template<typename ... B>
struct LD::CT::TypeDescriptor<LD::SPA::HTML<B...>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("elements");
public:
    static constexpr auto ClassName = ctll::fixed_string{"html"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::Tuple<B...> & (LD::SPA::HTML<B...>::*)(),&LD::SPA::HTML<B...>::Elements>(),LD::CT::SelectOverload<const LD::Tuple<B...> & (LD::SPA::HTML<B...>::*)() const,&LD::SPA::HTML<B...>::Elements>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_HTML_HPP
