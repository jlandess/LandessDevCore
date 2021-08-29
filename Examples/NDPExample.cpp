//
// Created by phoenixflower on 8/25/21.
//
#include "Network/NDP/NDP.hpp"
#include "Network/NDP/ndp.h"
#include <net/if.h>
namespace LD
{
    namespace Example
    {
        void NDPExample()
        {
            LD::NDP ndp1;
            //ndp * ndpSocket = nullptr;
            //ndp_open(&ndpSocket);
            //run_main_loop(ndpSocket);
            ;
            //run_cmd_monitor(ndpSocket,NDP_MSG_ALL,if_nametoindex("ens22"));
            //ndp_close(ndpSocket);
            ndp1();
        }
    }
}