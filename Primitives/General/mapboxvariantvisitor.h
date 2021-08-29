#ifndef MAPBOX_UTIL_VARIANT_VISITOR_HPP
#define MAPBOX_UTIL_VARIANT_VISITOR_HPP

//#include "Definitions/Common.hpp"
#include "mapboxvariant.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Algorithms/Overload.hpp"

namespace LD
{
    template<typename T>
    constexpr bool IsVariant(T && ) noexcept
    {
        return false;
    }

    template<typename ... A>
    constexpr bool IsVariant(LD::Variant<A...>) noexcept
    {
        return true;
    }
}
namespace mapbox
{
    namespace util
    {

        template <typename... Fns>
        struct visitor;


        template <typename Fn>
        struct visitor<Fn> : Fn
        {
            using Fn::operator();

            template<typename T>
            visitor(T&& fn) : Fn(LD::Forward<T>(fn)) {}
        };

        template <typename Fn, typename... Fns>
        struct visitor<Fn, Fns...> : Fn, visitor<Fns...>
        {
            using Fn::operator();
            using visitor<Fns...>::operator();

            template<typename T, typename... Ts>
            visitor(T&& fn, Ts&&... fns): Fn(LD::Forward<T>(fn)), visitor<Fns...>(LD::Forward<Ts>(fns)...)
            {

            }
        };

        template <typename... Fns>
        visitor<typename std::decay<Fns>::type...> make_visitor(Fns&&... fns)
        {
            return visitor<typename LD::Detail::Decay<Fns>::type...>(LD::Forward<Fns>(fns)...);
        }







    } // namespace util
} // namespace mapbox

namespace LD
{
    template <typename... Fns>
    auto MakeVisitor(Fns&&... fns) noexcept
    {
        return mapbox::util::visitor<typename LD::Detail::Decay<Fns>::type...>(LD::Forward<Fns>(fns)...);
    }
    template<typename ...T, typename ... Fns>
    constexpr auto Match(const mapbox::util::variant<T...> & variant,Fns && ... fns)
    {
        return mapbox::util::apply_visitor(
                mapbox::util::make_visitor(fns...),
                variant
        );
    }

    template<typename ... V, typename Fn>
    auto MultiMatch(Fn && function,V && ... variants) noexcept
    {
        //using Abc = decltype(LD::Declval<Fn>()(LD::Declval<V>()...));
        //LD::CT::DebugTemplate<Abc>{};

        return mapbox::util::apply_visitor(LD::Forward<Fn>(function),LD::Forward<V>(variants)...);
    }

}

#endif