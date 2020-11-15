//
// Created by phoenixflower on 11/2/20.
//

#ifndef LANDESSDEVCORE_JSTAG_H
#define LANDESSDEVCORE_JSTAG_H
#include "Text.hpp"
#include "Reflection/Reflection.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename CodeStringType>
        class Javascript
        {
        private:
            LD::SPA::Text<CodeStringType> mSourceCode;
        public:
            Javascript() = default;

            constexpr Javascript(CodeStringType && code) noexcept:mSourceCode{code}
            {

            }

            LD::SPA::Text<CodeStringType> & Code() noexcept
            {
                return this->mSourceCode;
            }

            const LD::SPA::Text<CodeStringType> & Code() const noexcept
            {
                return this->mSourceCode;
            }
        };
    }
}

template<typename JSTring>
struct LD::CT::TypeDescriptor<LD::SPA::Javascript<JSTring>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("elements");
public:
    static constexpr auto ClassName = ctll::fixed_string{"script"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::SPA::Text<JSTring> & (LD::SPA::Text<JSTring>::*)(),&LD::SPA::Text<JSTring>::Code>(),LD::CT::SelectOverload<const LD::SPA::Text<JSTring> & (LD::SPA::Text<JSTring>::*)() const,&LD::SPA::Javascript<JSTring>::Elements>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_JSTAG_H
