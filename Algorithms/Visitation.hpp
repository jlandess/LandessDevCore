//
// Created by phoenixflower on 8/27/21.
//

#ifndef LANDESSDEVCORE_VISITATION_HPP
#define LANDESSDEVCORE_VISITATION_HPP
#include "Overload.hpp"
namespace LD
{
    template <size_t N, typename R, typename Variant, typename Visitor>
    [[nodiscard]] constexpr R visit_nt(Variant &&var, Visitor &&vis) {
        if constexpr (N == 0) {
            if (N == var.index()) {
                // If this check isnt there the compiler will generate
                // exception code, this stops that
                return std::forward<Visitor>(vis)(
                        std::get<N>(std::forward<Variant>(var)));
            }
        } else {
            if (var.index() == N) {
                return std::forward<Visitor>(vis)(
                        std::get<N>(std::forward<Variant>(var)));
            }
            return visit_nt<N - 1, R>(std::forward<Variant>(var),
                                      std::forward<Visitor>(vis));
        }
        while (true) {
        }  // unreachable but compilers complain
    }

    template <class... Args, typename Visitor, typename... Visitors>
    [[nodiscard]] constexpr decltype(auto) visit_nt(
            std::variant<Args...> const &var, Visitor &&vis, Visitors &&... visitors) {
    auto ol =
            overload{std::forward<Visitor>(vis), std::forward<Visitors>(visitors)...};
    using result_t = decltype(std::invoke(std::move(ol), std::get<0>(var)));

    static_assert(sizeof...(Args) > 0);
    return visit_nt<sizeof...(Args) - 1, result_t>(var, std::move(ol));
}

template <class... Args, typename Visitor, typename... Visitors>
[[nodiscard]] constexpr decltype(auto) visit_nt(std::variant<Args...> &var,
                                                Visitor &&vis,
                                                Visitors &&... visitors) {
    auto ol =
            overload(std::forward<Visitor>(vis), std::forward<Visitors>(visitors)...);
    using result_t = decltype(std::invoke(std::move(ol), std::get<0>(var)));

    static_assert(sizeof...(Args) > 0);
    return visit_nt<sizeof...(Args) - 1, result_t>(var, std::move(ol));
}

template <class... Args, typename Visitor, typename... Visitors>
[[nodiscard]] constexpr decltype(auto) visit_nt(std::variant<Args...> &&var,
                                                Visitor &&vis,
                                                Visitors &&... visitors) {
    auto ol =
            overload{std::forward<Visitor>(vis), std::forward<Visitors>(visitors)...};
    using result_t =
    decltype(std::invoke(std::move(ol), std::move(std::get<0>(var))));

    static_assert(sizeof...(Args) > 0);
    return visit_nt<sizeof...(Args) - 1, result_t>(std::move(var), std::move(ol));
}

//template <typename Value, typename... Visitors>
//inline constexpr bool is_visitable_v = (std::is_invocable_v<Visitors, Value> or
//...);
//}
#endif //LANDESSDEVCORE_VISITATION_HPP
