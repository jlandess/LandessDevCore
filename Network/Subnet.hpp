//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_SUBNET_HPP
#define LANDESSDEVCORE_SUBNET_HPP
namespace LD
{
    class Subnet
    {
    private:
        unsigned char mCIDR;
    public:
        Subnet() noexcept:mCIDR{0}
        {

        }
        Subnet(unsigned char cidr) noexcept:mCIDR{cidr}
        {

        }

        unsigned char Value() const noexcept
        {
            return this->mCIDR;
        }

    };
}

#endif //LANDESSDEVCORE_SUBNET_HPP
