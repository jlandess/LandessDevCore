//
// Created by phoenixflower on 11/12/20.
//

#ifndef LANDESSDEVCORE_ISATTRIBUTE_HPP
#define LANDESSDEVCORE_ISATTRIBUTE_HPP
#include "GlobalAttributes.hpp"
#include "EventHandler.hpp"
namespace LD
{
    namespace SPA
    {
        constexpr bool IsAttribute(LD::Type<LD::SPA::AccessKey> ) noexcept
        {
            return true;
        }
        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::Language<T>> ) noexcept
        {
            return true;
        }

        constexpr bool IsAttribute(LD::Type<LD::SPA::AutoCapitalize> ) noexcept
        {
            return true;
        }
        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::ClassName<T>> ) noexcept
        {
            return true;
        }
        constexpr bool IsAttribute(LD::Type<LD::SPA::ContentEditable> ) noexcept
        {
            return true;
        }
        constexpr bool IsAttribute(LD::Type<LD::SPA::Direction> ) noexcept
        {
            return true;
        }
        constexpr bool IsAttribute(LD::Type<LD::SPA::Draggable> ) noexcept
        {
            return true;
        }
        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::ID<T>> ) noexcept
        {
            return true;
        }

        constexpr bool IsAttribute(LD::Type<LD::SPA::InputMode> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::ItemID<T>> ) noexcept
        {
            return true;
        }

        constexpr bool IsAttribute(LD::Type<LD::SPA::SpellCheck> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::Style<T>> ) noexcept
        {
            return true;
        }

        constexpr bool IsAttribute(LD::Type<LD::SPA::TabIndex> ) noexcept
        {
            return true;
        }

        template<typename T>
        constexpr bool IsAttribute(LD::Type<LD::SPA::Title<T>> ) noexcept
        {
            return true;
        }

        constexpr bool IsAttribute(LD::Type<LD::SPA::Translate> ) noexcept
        {
            return true;
        }
        template<typename T, typename Expression>
        constexpr bool IsAttribute(LD::Type<LD::SPA::EventHandler<T,Expression>> ) noexcept
        {
            return true;
        }
    }
}
#endif //LANDESSDEVCORE_ISATTRIBUTE_HPP
