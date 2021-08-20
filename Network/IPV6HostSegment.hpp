//
// Created by phoenixflower on 2/13/21.
//

#ifndef LANDESSDEVCORE_IPV6HOSTSEGMENT_HPP
#define LANDESSDEVCORE_IPV6HOSTSEGMENT_HPP
#include "ipv6.h"
#include "Primitives/General/Immutable.hpp"
#include "MacAddress.hpp"
#include "Port.hpp"
namespace LD
{
    class IPV6HostSegment
    {
    private:
        LD::ImmutableArray<LD::HexElement,8> mSegments;
        LD::Hexademical<16> mOctets;
        LD::Port mPort;

    public:
        IPV6HostSegment() noexcept
        {

        }
        IPV6HostSegment(LD::MacAddress hostMacAddress) noexcept
        {
            //ae:c0:9f:a3:61:a1
            //mac_to_ipv6_link_local("ae:c0:9f:a3:61:a1");
            LD::Hexademical<16> hex1;
            hex1[0] = hostMacAddress[0];
            hex1[1] = hostMacAddress[1];

            hex1[2] = hostMacAddress[2];
            hex1[3] = hostMacAddress[3];

            hex1[4] = hostMacAddress[4];
            hex1[5] = hostMacAddress[5];

            hex1[6] = 'f';
            hex1[7] = 'f';

            hex1[8] = 'f';
            hex1[9] = 'e';

            hex1[10] = hostMacAddress[6];
            hex1[11] = hostMacAddress[7];


            hex1[12] = hostMacAddress[8];
            hex1[13] = hostMacAddress[9];

            hex1[14] = hostMacAddress[10];
            hex1[15] = hostMacAddress[11];

            LD::BitSet<64> hostBitSet = LD::ToBitset(hex1);
            hostBitSet[6] = !hostBitSet[6];
            LD::Hexademical host{hostBitSet};


            this->mOctets = host;

            LD::Hexademical<2> firstHostOctetPair{{host[0],host[1]}};
            LD::Hexademical<2> secondHostOctetPair{{host[2],host[3]}};
            LD::Hexademical<2> thirdHostOctetPair{{host[4],host[5]}};
            LD::Hexademical<2> fourthHostOctetPair{{host[6],host[7]}};
            LD::Hexademical<2> fifthHostOctetPair{{host[8],host[9]}};
            LD::Hexademical<2> sixthHostOctetPair{{host[10],host[11]}};
            LD::Hexademical<2> seventhHostOctetPair{{host[12],host[13]}};
            LD::Hexademical<2> eightHostOctetPair{{host[14],host[15]}};




            this->mSegments[0] = LD::ToBase10(firstHostOctetPair);
            this->mSegments[1] = LD::ToBase10(secondHostOctetPair);
            this->mSegments[2] = LD::ToBase10(thirdHostOctetPair);
            this->mSegments[3] = LD::ToBase10(fourthHostOctetPair);
            this->mSegments[4] = LD::ToBase10(fifthHostOctetPair);
            this->mSegments[5] = LD::ToBase10(sixthHostOctetPair);
            this->mSegments[6] = LD::ToBase10(seventhHostOctetPair);
            this->mSegments[7] = LD::ToBase10(eightHostOctetPair);
        }
        IPV6HostSegment(LD::MacAddress hostMacAddress, LD::Port port) noexcept
        {
            LD::Hexademical<16> hex1;
            hex1[0] = hostMacAddress[0];
            hex1[1] = hostMacAddress[1];

            hex1[2] = hostMacAddress[2];
            hex1[3] = hostMacAddress[3];

            hex1[4] = hostMacAddress[4];
            hex1[5] = hostMacAddress[5];

            hex1[6] = 'f';
            hex1[7] = 'f';

            hex1[8] = 'f';
            hex1[9] = 'e';

            hex1[10] = hostMacAddress[6];
            hex1[11] = hostMacAddress[7];


            hex1[12] = hostMacAddress[8];
            hex1[13] = hostMacAddress[9];

            hex1[14] = hostMacAddress[10];
            hex1[15] = hostMacAddress[11];

            LD::BitSet<64> hostBitSet = LD::ToBitset(hex1);
            hostBitSet[6] = !hostBitSet[6];
            LD::Hexademical host{hostBitSet};


            LD::Hexademical<2> firstHostOctetPair{{host[0],host[1]}};
            LD::Hexademical<2> secondHostOctetPair{{host[2],host[3]}};
            LD::Hexademical<2> thirdHostOctetPair{{host[4],host[5]}};
            LD::Hexademical<2> fourthHostOctetPair{{host[6],host[7]}};
            LD::Hexademical<2> fifthHostOctetPair{{host[8],host[9]}};
            LD::Hexademical<2> sixthHostOctetPair{{host[10],host[11]}};
            LD::Hexademical<2> seventhHostOctetPair{{host[12],host[13]}};
            LD::Hexademical<2> eightHostOctetPair{{host[14],host[15]}};


            this->mSegments[0] = LD::ToBase10(firstHostOctetPair);
            this->mSegments[1] = LD::ToBase10(secondHostOctetPair);
            this->mSegments[2] = LD::ToBase10(thirdHostOctetPair);
            this->mSegments[3] = LD::ToBase10(fourthHostOctetPair);
            this->mSegments[4] = LD::ToBase10(fifthHostOctetPair);
            this->mSegments[5] = LD::ToBase10(sixthHostOctetPair);
            this->mSegments[6] = LD::ToBase10(seventhHostOctetPair);
            this->mSegments[7] = LD::ToBase10(eightHostOctetPair);
            this->mPort = port;
        }

        IPV6HostSegment(LD::Port port, LD::MacAddress hostMacAddress) noexcept
        {
            LD::Hexademical<16> hex1;
            hex1[0] = hostMacAddress[0];
            hex1[1] = hostMacAddress[1];

            hex1[2] = hostMacAddress[2];
            hex1[3] = hostMacAddress[3];

            hex1[4] = hostMacAddress[4];
            hex1[5] = hostMacAddress[5];

            hex1[6] = 'f';
            hex1[7] = 'f';

            hex1[8] = 'f';
            hex1[9] = 'e';

            hex1[10] = hostMacAddress[6];
            hex1[11] = hostMacAddress[7];


            hex1[12] = hostMacAddress[8];
            hex1[13] = hostMacAddress[9];

            hex1[14] = hostMacAddress[10];
            hex1[15] = hostMacAddress[11];

            LD::BitSet<64> hostBitSet = LD::ToBitset(hex1);
            hostBitSet[6] = !hostBitSet[6];
            LD::Hexademical host{hostBitSet};


            LD::Hexademical<2> firstHostOctetPair{{host[0],host[1]}};
            LD::Hexademical<2> secondHostOctetPair{{host[2],host[3]}};
            LD::Hexademical<2> thirdHostOctetPair{{host[4],host[5]}};
            LD::Hexademical<2> fourthHostOctetPair{{host[6],host[7]}};
            LD::Hexademical<2> fifthHostOctetPair{{host[8],host[9]}};
            LD::Hexademical<2> sixthHostOctetPair{{host[10],host[11]}};
            LD::Hexademical<2> seventhHostOctetPair{{host[12],host[13]}};
            LD::Hexademical<2> eightHostOctetPair{{host[14],host[15]}};


            this->mSegments[0] = LD::ToBase10(firstHostOctetPair);
            this->mSegments[1] = LD::ToBase10(secondHostOctetPair);
            this->mSegments[2] = LD::ToBase10(thirdHostOctetPair);
            this->mSegments[3] = LD::ToBase10(fourthHostOctetPair);
            this->mSegments[4] = LD::ToBase10(fifthHostOctetPair);
            this->mSegments[5] = LD::ToBase10(sixthHostOctetPair);
            this->mSegments[6] = LD::ToBase10(seventhHostOctetPair);
            this->mSegments[7] = LD::ToBase10(eightHostOctetPair);
            this->mPort = port;
        }

        LD::ImmutableArray<LD::HexElement,8> Segments() const noexcept
        {
            return this->mSegments;
        }
        auto Octets() const noexcept
        {
            return this->mOctets;
        }
        LD::Port Port() const noexcept
        {
            return this->mPort;
        }
    };
}
#endif //LANDESSDEVCORE_IPV6HOSTSEGMENT_HPP
