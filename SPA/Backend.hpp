//
// Created by phoenixflower on 10/26/20.
//

#ifndef LANDESSDEVCORE_BACKEND_HPP
#define LANDESSDEVCORE_BACKEND_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
namespace LD
{
    namespace SPW
    {
        class JQuery
        {

        };

        class Axios
        {

        };

        class React
        {

        };

        template<class B,LD::UInteger Major, LD::UInteger Minor, LD::UInteger Patch>
        class Backend;

        template<>
        struct Backend<JQuery,3,5,1>
        {
            static constexpr LD::StringView cdn = "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.5.1/jquery.min.js\" integrity=\"sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0=\" crossorigin=\"anonymous\"></script>";
        };

        template<>
        struct Backend<Axios,0,21,0>
        {
            static constexpr LD::StringView cdn = "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/axios/0.21.0/axios.min.js\" integrity=\"sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0=\" crossorigin=\"anonymous\"></script>";
            static constexpr LD::StringView require = "const axios = require('axios');";
        };


        template<>
        struct Backend<React,17,0,1>
        {
            static constexpr LD::StringView cdn = "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/react/17.0.1/umd/react.production.min.js\" integrity=\"sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0=\" crossorigin=\"anonymous\"></script>";
        };


        namespace Detail
        {
            template<typename T>
            struct IsBackend
            {
                static constexpr bool value = false;
            };

            template<class B, LD::UInteger Major, LD::UInteger Minor, LD::UInteger Patch>
            struct IsBackend<Backend<B,Major,Minor,Patch>>
            {
                static constexpr bool value =  true;
            };
        }

        template<typename T>
        constexpr bool IsBackend = LD::SPW::Detail::IsBackend<T>::value;
    }
}
#endif //LANDESSDEVCORE_BACKEND_HPP
