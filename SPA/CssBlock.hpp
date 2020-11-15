//
// Created by phoenixflower on 11/4/20.
//

#ifndef LANDESSDEVCORE_CSSBLOCK_HPP
#define LANDESSDEVCORE_CSSBLOCK_HPP
#include "Definitions/Common.hpp"
#include "Primitives/General/Context.h"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "Primitives/General/Span.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename ClassNameType, typename tList>
        class CSSBlock;

        template<typename ClassNameType, typename ... Args>
        class CSSBlock<ClassNameType,LD::CT::TypeList<Args...>>
        {
        private:
            LD::Context<Args...> mContext;
            ClassNameType mClassName;
        public:
            CSSBlock(ClassNameType && className, Args && ... arguments) noexcept:mClassName{LD::Forward<ClassNameType>(className)},mContext{LD::Forward<Args>(arguments)...}
            {

            }

            LD::Tuple<Args...> & Elements() noexcept
            {
                return this->mComponents;
            }
            const LD::Tuple<Args...> & Elements() const noexcept
            {
                return this->mComponents;
            }
            const LD::Span<ClassNameType> & ClassName() const noexcept
            {
                return LD::Span{this->mClassName};
            }
        };

        template <typename StringType,typename ... Components> CSSBlock(StringType &&,Components && ... components) -> CSSBlock<StringType,LD::CT::TypeList<Components...>>;
    }
}

namespace LD
{
    namespace SPA
    {
        template<typename T>
        constexpr bool IsCSSBlock(LD::Type<T>) noexcept
        {
            return false;
        }

        template<typename ClassNameType, typename ... Args>
        constexpr bool IsCSSBlock(LD::Type<LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<Args...>>>) noexcept
        {
            return true;
        }
    }
}

template<typename ClassNameType,typename ... B>
struct LD::CT::TypeDescriptor<LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<B...>>>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("elements");
public:
    static constexpr auto ClassName = ctll::fixed_string{"cssblock"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::Tuple<B...> & (LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<B...>>::*)(),&LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<B...>>::Elements>(),LD::CT::SelectOverload<const LD::Tuple<B...> & (LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<B...>>::*)() const,&LD::SPA::CSSBlock<ClassNameType,LD::CT::TypeList<B...>>::Elements>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_CSSBLOCK_HPP
