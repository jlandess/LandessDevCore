//
// Created by phoenixflower on 2/25/21.
//

#ifndef LANDESSDEVCORE_NAMEDTUPLE_H
#define LANDESSDEVCORE_NAMEDTUPLE_H
#include "TypeTraits/Detection.hpp"
namespace LD
{
    template<const auto & Name, typename T, class = void>
    class NamedType;


    template<const auto & N, typename T>
    class NamedType<N,T,LD::Enable_If_T<
            LD::Require<
            true
            >>>
    {
    private:
        T mObject;
    public:
        NamedType() = default;
        NamedType(T && object) noexcept:mObject{LD::Forward<T>(object)} {}
        static constexpr auto ManagedName = N;

        const T & Object() const noexcept
        {
            return this->mObject;
        }

        T & Object() noexcept
        {
            return this->mObject;
        }

        constexpr auto & Name() const noexcept
        {
            return ManagedName;
        }
    };

    namespace CT
    {
        template<typename T>
        constexpr bool IsNamedType(LD::Type<T>) noexcept
        {
            return false;
        }
        template<const auto & Name, typename T>
        constexpr bool IsNamedType(LD::Type<LD::NamedType<Name,T>>) noexcept
        {
            return true;
        }


        template<typename T>
        constexpr bool IsNamedTuple(LD::Type<T> ) noexcept
        {
            return false;
        }

        template<typename ... T>
        constexpr bool IsNamedTuple(LD::Type<LD::Tuple<T...>> ) noexcept
        {
            return (LD::CT::IsNamedType(LD::CT::RemoveQualifiers(LD::Type<T>{})) && ...);
        }
    }

}
#endif //LANDESSDEVCORE_NAMEDTUPLE_H
