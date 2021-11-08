//
// Created by phoenixflower on 8/27/21.
//

#ifndef LANDESSDEVCORE_VISITATION_HPP
#define LANDESSDEVCORE_VISITATION_HPP
#include "Overload.hpp"
#include "TypeTraits/IsVoid.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "TypeTraits/RemoveReference.hpp"
#include "TypeTraits/AddRValueReference.hpp"
#include "TypeTraits/RemoveReference.hpp"
#include "TypeTraits/IsReference.hpp"
#include "TypeTraits/Conditional.hpp"
#include "TypeTraits/RemoveConst.hpp"
#include "VariantOperations.hpp"
#include "Primitives/General/Tuple.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "TypeTraits/Detection.hpp"
#include "Algorithms/Forward.hpp"
#include "Algorithms/Invoke.hpp"
namespace LD
{
    namespace detail
    {

        template <typename  Ts>
        Ts variant_access_(const Ts *);

        template <typename T>
        using variant_access = decltype(variant_access_(static_cast<LD::Detail::Decay_T<T>*>(nullptr)));

        template<typename T>
        constexpr bool is_variant_v = true;
        template<LD::UInteger I, LD::UInteger ... Is>
        constexpr
        LD::IndexSequence<I, Is...>
        prepend(LD::IndexSequence<Is...>) {
            return {};
        }

        constexpr
        LD::IndexSequence<>
        next_seq(LD::IndexSequence<>, LD::IndexSequence<>) {
            return {};
        }

        template <typename T, typename V>
        struct copy_referencenesss_
        {
            using type = T;
        };

        template <typename T, typename V>
        struct copy_referencenesss_<T,V&>
        {
            using type = T&;
        };

        template <typename T, typename V>
        struct copy_referencenesss_<T, V&&>
        {
            //using type = std::remove_reference_t<T>&&;
            using type = typename LD::Detail::RemoveReference<T>::type &&;
        };

        template <typename T, typename V>
        using copy_referenceness = typename copy_referencenesss_<T,V>::type;

        template <typename T, typename TSource>
        using as_if_forwarded = LD::Conditonal<
                !LD::Detail::IsReference<TSource>{},
                typename LD::Detail::AddRvalueReference<typename LD::Detail::RemoveReference<T>::type>::type ,
                copy_referenceness<T, TSource>
        >;

        template <typename TLike, typename T>
        constexpr
        decltype(auto)
        forward_like(T && x) noexcept
        {
            static_assert(!(std::is_rvalue_reference<decltype(x)>{} &&
                            std::is_lvalue_reference<TLike>{}));

            return static_cast<as_if_forwarded<T, TLike>>(x);
        }

        template<
                LD::UInteger I, LD::UInteger ... Is,
                LD::UInteger J, LD::UInteger ... Js
        >
        constexpr
        auto
        next_seq(LD::IndexSequence<I, Is...>, LD::IndexSequence<J, Js...>) {
            if constexpr (I + 1 == J) {
                return prepend<0>(next_seq(LD::IndexSequence<Is...>{},
                                           LD::IndexSequence<Js...>{}));
            } else {
                return LD::IndexSequence<I + 1, Is...>{};
            }
        }

        template<LD::UInteger ... I>
        static
        constexpr
        LD::UInteger
        sum(LD::IndexSequence<I...>) { return (I + ...); }

        template <typename T>
        using remove_cv_ref_t = typename LD::RemoveConst<typename LD::Detail::RemoveReference<T>::type>::type;


        template <LD::UInteger I, typename T>
        decltype(auto) get(T&& t) noexcept
        {
            if constexpr (is_variant_v<T>)
            {
                return LD::VariantOperations<LD::Detail::Decay_T<T>>::template Get<I>(LD::Forward<T>(t));
                //return std::get<I>(LD::Forward<T>(t));
            }
            else
            {
                static_assert(I == 0);
                return LD::Forward<T>(t);
            }
        }

        template <LD::UInteger I, typename T>
        auto get_if(T* t) noexcept
        {
            if constexpr (is_variant_v<T>)
            {
                //LD::CT::DebugTemplate<LD::VariantOperations<T>>{};

                return VariantOperations<T>::template GetIf<I>(t);
                //return mpark::get_if<I>(t);
                //return LD::VariantOperations<T>::GetIf<I>(t);
                //return std::get_if<I>(t);
            }
            else {
                static_assert(I == 0);
                return t;
            }
        }

        template <typename V>
        constexpr LD::UInteger variant_size() noexcept
        {
            if constexpr (is_variant_v<V>)
            {

                return LD::VariantOperations<LD::Detail::Decay_T<V>>::Size();
                //return std::variant_size_v<variant_access<V>>;
            }
            else
            {
                return 1;
            }
        }

        template <typename V>
        constexpr LD::UInteger index(const V& v) noexcept
        {
            if constexpr (is_variant_v<V>)
            {
                return LD::VariantOperations<LD::Detail::Decay_T<V>>::Index(v);
                //return LD::VariantOperations<LD::Detail::Decay_T<V>>::Index(v);
                //return v.index();
            }
            else
            {
                return 0;
            }
        }
        template<
                LD::UInteger ... Is,
                LD::UInteger ... Ms,
                typename F,
                typename ... Vs
        >
        inline
        constexpr
        auto
        Visit(
                LD::IndexSequence<Is...> i,
                LD::IndexSequence<Ms...> m,
                F &&f,
                Vs &&... vs) noexcept
        {
            constexpr auto n = next_seq(i, m);
            if constexpr (sum(n) == 0) {
                return f(get<Is>(LD::Forward<Vs>(vs))...);
            } else {
                if (LD::Tuple(detail::index(vs)...) == LD::Tuple(Is...))
                {
                    return f(forward_like<Vs>(*VariantOperations<LD::Detail::Decay_T<Vs>>::template GetIf<Is>(&vs))...);
                    //return f(forward_like<Vs>(*get_if<Is>(&vs))...);
                }
                return LD::detail::Visit(n, m, LD::Forward<F>(f), LD::Forward<Vs>(vs)...);
            }
        }

        template<typename>
        inline constexpr LD::UInteger zero = 0;
    }
    template <typename F, typename ... Vs>
    inline auto Visit(F&& f, Vs&& ... vs) noexcept
    {

        if constexpr (((detail::variant_size<Vs>() == 1) && ...))
        {
            return f(detail::forward_like<Vs>(*detail::get_if<0>(&vs))...);
        } else {
            return LD::detail::Visit(
                    LD::IndexSequence<detail::zero<Vs>...>{},
                    LD::IndexSequence<detail::variant_size<Vs>()...>{},
                    LD::Forward<F>(f),
                    LD::Forward<Vs>(vs)...);
        }
    }

    template<typename F,typename  ... A, typename  B, typename =
    LD::Enable_If_T<
            LD::Require<
                    LD::CT::IsTuple(LD::Type<B>{})
            >>>
    auto VisitWithContext(F && functor,const B & context,A && ... variant) noexcept
    {
        auto visitation = [&](auto  && ... abc) noexcept
        {
            return LD::VariadicInvoke(LD::Forward<F>(functor),context,LD::Forward<decltype(abc)>(abc)...);
        };
        return LD::Visit(visitation,LD::Forward<A>(variant)...);
    }

}
#endif //LANDESSDEVCORE_VISITATION_HPP
