//
// Created by James Landess on 2/6/20.
//

#ifndef LANDESSDEVCORE_DETECTION_HPP
#define LANDESSDEVCORE_DETECTION_HPP

#include "IsSame.hpp"
#include "IsVoid.hpp"
namespace Detail
{
    struct nonesuch {
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };
    template <class Default, class AlwaysVoid,
            template<class...> class Op, class... Args>
    struct Detector {
        using value = LD::FalseType;
        using type = Default;
    };

    template <class Default, template<class...> class Op, class... Args>
    struct Detector<Default, LD::Void_T<Op<Args...>>, Op, Args...> {
        // Note that std::void_t is a C++17 feature
        using value = LD::TrueType;
        using type = Op<Args...>;
    };
}
#endif //LANDESSDEVCORE_DETECTION_HPP
