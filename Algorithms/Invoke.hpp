#ifndef LANDESSDEVCORE_INVOKE_HPP
#define LANDESSDEVCORE_INVOKE_HPP
#include "Definitions/Integer.hpp"
#include "TypeTraits/IntegerSequence.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
//#include "Definitions/Common.hpp"
#include "TypeTraits/IsFunction.hpp"
#include "SumTypes/MPark/variant.hpp"
namespace LD
{
    namespace Detail
    {
        template<typename F, typename ... Args,LD::UInteger ...Indices >
        auto InvokeFold(LD::IndexSequence<Indices...> sequence, F && function, const LD::Tuple<Args...> & tuple)
        {
            return function(LD::Get<Indices>(tuple)...);
        }
        template<typename F, typename H,typename ... Args,LD::UInteger ...Indices >
        auto InvokeFold1(LD::IndexSequence<Indices...> sequence, F && function, H && a,const LD::Tuple<Args...> & tuple)
        {
            return function(LD::Forward<H>(a),LD::Get<Indices>(tuple)...);
        }

        template<typename F, typename ... Args,LD::UInteger ...Indices >
        auto VariadicFold(LD::IndexSequence<Indices...> sequence, F && function,const LD::Tuple<Args...> & tuple)
        {
            return function(LD::Get<Indices>(tuple)...);
        }
    }

    //return Delegate(PDP::Get<S>(Arguements) ...);
    template<typename F, typename ... Args>
    auto Invoke(F && function, const LD::Tuple<Args...> & tuple)
    {
        return LD::Detail::InvokeFold(LD::MakeIndexSequence_T<sizeof...(Args)>{}, LD::Forward<F>(function), tuple);
    }
    template<typename F, typename H, typename ... Args>
    auto Invoke1(F && function, H && a,const LD::Tuple<Args...> & tuple)
    {
        return LD::Detail::InvokeFold1(LD::MakeIndexSequence_T<sizeof...(Args)>{}, LD::Forward<F>(function), LD::Forward<H>(a),tuple);
    }

    template<typename F, typename ... H, typename TupleType, typename =
            LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsTuple(LD::Type<TupleType>{})
                            >>>
    auto VariadicInvoke(F && function, const TupleType & tuple, H && ... a) noexcept
    {
        auto combinedTuple = LD::ConcatenateTuple(LD::MakeTuple(LD::Forward<H>(a)...),tuple);
        constexpr LD::UInteger CombinedTupleSize = LD::CT::TupleSize(LD::Type<decltype(combinedTuple)>{});
        return LD::Detail::VariadicFold(LD::MakeIndexSequence_T<CombinedTupleSize>{}, LD::Forward<F>(function), combinedTuple);
    }
    template<typename H, typename ... Args>
    auto InvokeVisitation(H && a,const LD::Tuple<Args...> & tuple)
    {
        auto visitation = [](auto && ... abc)
        {
            return mpark::visit(LD::Forward<decltype(abc)>(abc)...);
        };
        return LD::Invoke1(visitation,LD::Forward<H>(a),tuple);
    }
    template<typename D, typename H, typename ... Args>
    auto Invoke2(D && , H && a, const LD::Tuple<Args...> & tuple)
    {

    }
}
/**
 * namespace Detail
{
    template <class F, class... Args>
    inline auto INVOKE(F&& f, Args&&... args) ->
    decltype(LD::Forward<F>(f)(LD::Forward<Args>(args)...))
    {
        return LD::Forward<F>(f)(LD::Forward<Args>(args)...);
    }

    template <class Base, class T, class Derived>
    inline auto INVOKE(T Base::*pmd, Derived&& ref) ->
    decltype(LD::Forward<Derived>(ref).*pmd)
    {
        return LD::Forward<Derived>(ref).*pmd;
    }

    template <class PMD, class Pointer>
    inline auto INVOKE(PMD&& pmd, Pointer&& ptr) ->
    decltype((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMD>(pmd))
    {
        return (*LD::Forward<Pointer>(ptr)).*LD::Forward<PMD>(pmd);
    }

    template <class Base, class T, class Derived, class... Args>
    inline auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args) ->
    decltype((LD::Forward<Derived>(ref).*pmf)(LD::Forward<Args>(args)...))
    {
        return (LD::Forward<Derived>(ref).*pmf)(LD::Forward<Args>(args)...);
    }

    template <class PMF, class Pointer, class... Args>
    inline auto INVOKE(PMF&& pmf, Pointer&& ptr, Args&&... args) ->
    decltype(((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMF>(pmf))(LD::Forward<Args>(args)...))
    {
        return ((*LD::Forward<Pointer>(ptr)).*LD::Forward<PMF>(pmf))(LD::Forward<Args>(args)...);
    }

}
 */
#endif