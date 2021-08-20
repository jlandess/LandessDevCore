//
// Created by phoenixflower on 2/8/21.
//

#ifndef LANDESSDEVCORE_CREATEINSTANCERESPONSE_HPP
#define LANDESSDEVCORE_CREATEINSTANCERESPONSE_HPP
#include "Primitives/General/Hexademical.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Definitions/Integer.hpp"
namespace LD
{
    namespace PVE
    {
        enum CreationMethod{VZCreate};
        class CreateInstanceResponse
        {
        private:
            LD::ImmutableString<25> mHostname;
            LD::Hexademical<12> mHex1;
            LD::Hexademical<12> mHex2;
            LD::Hexademical<12> mHex3;
            LD::PVE::CreationMethod mMethod;
            LD::UInteger mID;
            LD::ImmutableString<25> mUserName;
            LD::ImmutableString<25> mBase;
        public:
            constexpr CreateInstanceResponse() noexcept: mHostname{LD::ImmutableString<25>{}},mHex1(),mHex2(),mHex3(),mMethod{LD::PVE::VZCreate},mID{0},mUserName(),mBase()
            {

            }


            constexpr CreateInstanceResponse(
                    LD::ImmutableString<25> hostName,
                    LD::Hexademical<12> hex1,
                    LD::Hexademical<12> hex2,
                    LD::Hexademical<12> hex3,
                    LD::PVE::CreationMethod method,
                    LD::UInteger vmID,
                    LD::ImmutableString<25> userName,
                    LD::ImmutableString<25> base) noexcept:
                    mHostname{hostName},
                    mHex1{hex1},
                    mHex2{hex2},
                    mHex3{hex3},
                    mMethod{method},
                    mID{vmID},
                    mUserName{userName},
                    mBase{base}
            {

            }


            LD::ImmutableString<25> HostName() const noexcept
            {
                return this->mHostname;
            }

            LD::PVE::CreationMethod Method() const noexcept
            {
                return this->mMethod;
            }

            LD::UInteger ID() const noexcept
            {
                return this->mID;
            }

            LD::ImmutableString<25> UserName() const noexcept
            {
                return this->mUserName;
            }

            LD::ImmutableString<25> Base() const noexcept
            {
                return this->mBase;
            }

        };
    }
}
#endif //LANDESSDEVCORE_CREATEINSTANCERESPONSE_HPP
