//
// Created by phoenixflower on 11/15/20.
//

#ifndef LANDESSDEVCORE_PHONENUMBER_HPP
#define LANDESSDEVCORE_PHONENUMBER_HPP
#include "Immutable.hpp"
#include "StringView.hpp"
namespace LD
{
    class PhoneNumber
    {
    private:
        LD::UInteger mCountryCode;
        LD::ImmutableString<10> mNumber;
    public:
        PhoneNumber(LD::StringView number) noexcept
        {

        }

        LD::UInteger CountryCode() const noexcept
        {
            return this->mCountryCode;
        }
        const LD::ImmutableString<10> & Number() const noexcept
        {
            return this->mNumber;
        }
    };



    auto ToImmutableString(const PhoneNumber & number) noexcept
    {
        return LD::ImmutableString{"+"}+LD::ToImmutableString(number.CountryCode())+number.Number();
    }
}
#endif //LANDESSDEVCORE_PHONENUMBER_HPP
