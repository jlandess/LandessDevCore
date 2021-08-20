//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_NETWORKBRIDGE_HPP
#define LANDESSDEVCORE_NETWORKBRIDGE_HPP
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General/Context.h"
#include "InterfaceIndex.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Algorithms/AddAll.hpp"
#include "NetworkBridgeForwardDeclaration.hpp"
namespace LD
{
    namespace PVE
    {
        //template<typename List,class = void>
        //class NetworkBridge;

        template<typename ... A>
        class NetworkBridge<LD::CT::TypeList<A...>,
                LD::Enable_If_T<
                        LD::Require<
                                true
                        >>>
        {
        private:
            LD::PVE::InterfaceIndex mIndex;
            LD::Context<A...> mOptions;

        public:
            NetworkBridge(LD::PVE::InterfaceIndex index,A && ... options) noexcept:mOptions{LD::MakeContext(LD::Forward<A>(options)...)}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"net"}+LD::ToImmutableString(LD::UInteger(this->mIndex.Index()));
            }
            auto Value() const noexcept
            {
                auto abc = [](auto  && ... components) noexcept
                {
                    return LD::AddAllWithDelimeter(LD::ToImmutableString(LD::Forward<decltype(components)>(components))...);
                };
                //LD::AddAll(LD::Tuple<int,float>{});
                //auto result = LD::Invoke(abc,this->mOptions);
                return LD::Invoke(abc,this->mOptions);;
            }
        };

        template<typename ... A> NetworkBridge( LD::PVE::InterfaceIndex,A && ...) -> NetworkBridge<LD::CT::TypeList<A...>>;
    }
}
#endif //LANDESSDEVCORE_NETWORKBRIDGE_HPP
