//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_BRIDGENAME_HPP
#define LANDESSDEVCORE_BRIDGENAME_HPP
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace PVE
    {
        template<typename T>
        class BridgeName
        {
        private:
            T mName;
        public:
            BridgeName() noexcept
            {

            }
            BridgeName(T && name) noexcept:mName{LD::Forward<T>(name)}
            {

            }
            auto Value() const
            {
                return this->mName;
            }
        };
        template<typename StringType,typename ... A> BridgeName(StringType &&) -> BridgeName<StringType>;
    }


    template<typename T>
    inline auto ToImmutableString(LD::PVE::BridgeName<T> name) noexcept
    {
        return LD::ImmutableString{"name="}+name.Value();
    }
}
#endif //LANDESSDEVCORE_BRIDGENAME_HPP
