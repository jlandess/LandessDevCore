//
// Created by phoenixflower on 9/22/21.
//

#ifndef LANDESSDEVCORE_QUALITYOFSERVICEQUEUE_HPP
#define LANDESSDEVCORE_QUALITYOFSERVICEQUEUE_HPP
namespace LD
{
    namespace Alg
    {
        template<typename QueueType, typename Policy, typename TimerType,class = void>
        class BasicQoS;
        template<typename QueueType, typename Policy, typename TimerType>
        class BasicQoS<QueueType,Policy,TimerType,LD::Enable_If_T<LD::Require<true>>>
        {

        };
    }
}
#endif //LANDESSDEVCORE_QUALITYOFSERVICEQUEUE_HPP
