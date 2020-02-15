#ifndef MAPBOX_UTIL_VARIANT_VISITOR_HPP
#define MAPBOX_UTIL_VARIANT_VISITOR_HPP

#include "Definitions/Common.h"
#include "mapboxvariant.h"
namespace mapbox {
    namespace util {

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
            visitor(T&& fn, Ts&&... fns)
                    : Fn(LD::Forward<T>(fn))
                    , visitor<Fns...>(LD::Forward<Ts>(fns)...) {}
        };

        template <typename... Fns>
        visitor<typename std::decay<Fns>::type...> make_visitor(Fns&&... fns)
        {
            return visitor<typename LD::Detail::Decay<Fns>::type...>
                    (LD::Forward<Fns>(fns)...);
        }




    } // namespace util
} // namespace mapbox

namespace LD
{
    template<typename ...T, typename ... Fns>
    auto Match(const mapbox::util::variant<T...> & variant,Fns && ... fns)
    {
        return mapbox::util::apply_visitor(
                mapbox::util::make_visitor(fns...),
                variant
        );
    }

}

#endif