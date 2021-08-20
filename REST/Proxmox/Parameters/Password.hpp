//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_PASSWORD_HPP
#define LANDESSDEVCORE_PASSWORD_HPP
#include "Definitions/Common.hpp"
namespace LD
{
    namespace PVE
    {
        template<typename StringType>
        class Password
        {
        private:
            StringType mPassword;
        public:
            Password() noexcept
            {

            }
            Password(StringType && password) noexcept:mPassword{LD::Forward<StringType>(password)}
            {

            }
            auto Key() const noexcept
            {
                return LD::ImmutableString{"password"};
            }
            auto Value() const noexcept
            {
                return this->mPassword;
            }
        };

        template<typename StringType> Password(StringType && volumeLocation) -> Password<StringType>;
    }
}
#endif //LANDESSDEVCORE_PASSWORD_HPP
