//
// Created by phoenixflower on 10/17/20.
//

#ifndef LANDESSDEVCORE_INPUT_HPP
#define LANDESSDEVCORE_INPUT_HPP
#include "TypeTraits/TypeList.hpp"
namespace LD
{
    namespace SPA
    {
        struct ButtonInput
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct CheckBox
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct ColorWheel
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Date
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct DateTimeLocal
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Email
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct File
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Hidden
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Image
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Month
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Number
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Password
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Radio
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Range
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Reset
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Submit
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Telephone
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Text
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct URL
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };

        struct Week
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };



        template<typename InputType>
        struct Input
        {
            using AcceptedAttributes = LD::CT::TypeList<>;
        };
    }
}
#endif //LANDESSDEVCORE_INPUT_HPP
