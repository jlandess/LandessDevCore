//
// Created by phoenixflower on 9/30/21.
//

#ifndef LANDESSDEVCORE_OBSERVER_HPP
#define LANDESSDEVCORE_OBSERVER_HPP
#include "TypeTraits/Detection.hpp"
#include "Primitives/General/Tuple.hpp"
#include "Algorithms/Invoke.hpp"
#include "Algorithms/Forward.hpp"
namespace LD
{
    template<typename T, class = void>
    class BasicObserver;


    template<typename T, typename ... A>
    class BasicObserver<T(A...),LD::Enable_If_T<LD::Require<true>>>
    {
    private:
        T mObservee;
        //LD::Tuple<A...> mContext;
    public:
         BasicObserver() noexcept
         {

         }

        BasicObserver(T && functor) noexcept:mObservee{LD::Forward<T>(functor)}
        {

        }


        template<typename ... B>
        auto operator()(B && ... args) noexcept
        {
            return (*this->mObservee)(LD::Forward<B>(args)...);
        }

    };

    template<typename T, typename ... A>
    class BasicObserver<T&(A...),LD::Enable_If_T<LD::Require<true>>>
    {
    private:
        LD::ElementReference<T> mObservee;
        //LD::Tuple<A...> mContext;
    public:
        BasicObserver() noexcept
        {

        }

        BasicObserver(T * functor) noexcept:mObservee{LD::Forward<T>(functor)}
        {

        }

        BasicObserver(T & functor) noexcept:mObservee{LD::Forward<T>(functor)}
        {

        }

        BasicObserver(LD::ElementReference<T> functor) noexcept:mObservee{LD::Forward<T>(functor)}
        {

        }


        template<typename ... B>
        auto operator()(B && ... args) noexcept
        {
            return this->mObservee(LD::Forward<B>(args)...);
        }

    };

    template<typename T, typename ... A>
    class BasicObserver<T*(A...),LD::Enable_If_T<LD::Require<true>>>
    {
    private:
        LD::ElementReference<T> mObservee;
        //LD::Tuple<A...> mContext;
    public:
        BasicObserver() noexcept
        {

        }

        BasicObserver(T * functor) noexcept:mObservee{LD::Forward<T>(functor)}
        {

        }


        auto operator()(A && ... args) noexcept
        {
            return this->mObservee(LD::Forward<A>(args)...);
        }

    };
}
#endif //LANDESSDEVCORE_OBSERVER_HPP
