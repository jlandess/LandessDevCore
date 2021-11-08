//
// Created by phoenixflower on 2/24/21.
//

#ifndef LANDESSDEVCORE_HASHIVAULT_HPP
#define LANDESSDEVCORE_HASHIVAULT_HPP
#include "TypeTraits/Type.h"
#include "IO/json.hpp"
#include "Core/RequestResponse.hpp"
#include "REST/CPR/cpr.h"
#include "Primitives/General/Immutable.hpp"
#include <simdjson/simdjson.h>
#include "Algorithms/FromJSON.hpp"
#include "Chrono/DateTime.h"
#include "Algorithms/ToJSON.hpp"
//#include "IO/rapidjson/rapidjson.h"
namespace LD
{

    class VaultResponseMetaData
    {
    private:
        LD::DateTime mCreatedTime;
        LD::DateTime mDeletedTime;
        bool mDestroyed;
        LD::UInteger mVersion;
    public:
    };

    template<typename T>
    class VaultData
    {
    private:
        T mData;
        VaultResponseMetaData mMetaData;
    public:
    };
    template<typename T>
    class VaultResponse
    {
    private:
        LD::ImmutableString<64> mID;
        LD::ImmutableString<64> mLeaseID;
        bool mRenewable;
        LD::Second<LD::UInteger> mLeaseDuration;
        VaultData<T> mData;
    };
    template<typename URLType,typename TokenType>
    class HashiVault;


    template<typename URLType,typename TokenType>
    class HashiVault
    {
    private:
        TokenType mToken;
        URLType mURL;
    public:
        HashiVault(URLType && url,TokenType && token) noexcept:mURL{LD::Forward<URLType>(url)},mToken{LD::Forward<TokenType>(token)}
        {

        }

        template<typename T, typename ProjectType, typename SecretType,typename ... A>
        LD::RequestResponse<LD::VaultResponse<T>(A...)> QueryVaultResponse(LD::Type<T>, ProjectType && project,SecretType && secret ,LD::UInteger version,A && ... args);
        template<typename T, typename ProjectType, typename SecretType,typename ... A>
        LD::RequestResponse<T(A...)> Query(LD::Type<T>, ProjectType && project,SecretType && secret ,A && ... args)
        {
            auto vaultFetchURL = this->mURL+LD::ImmutableString{"/v1/"}+project+LD::ImmutableString{"/data/"}+secret;
            cpr::Response fetchSecretResponse =  cpr::Get(cpr::Url{vaultFetchURL.Data()},cpr::Header{{"X-Vault-Token",this->mToken.Data()}});


            if (fetchSecretResponse.status_code == 200)
            {
                //std::cout << fetchSecretResponse.text << std::endl;
                simdjson::dom::parser parser;
                simdjson::dom::element parsedResponse = parser.parse(fetchSecretResponse.text);

                std::cout << parsedResponse << std::endl;
                auto response = LD::FromJSON(LD::Type<T>{},parsedResponse["data"]["data"],LD::Forward<A>(args)...);

                auto onObject = [](auto Object, auto && ... arguments) noexcept
                {
                    return LD::CreateResponse(LD::Type<T>{},T{Object},LD::Forward<A>(arguments)...);
                };

                auto onError = [](const LD::TransactionError & error, auto && ... arguments) noexcept
                {
                    return LD::CreateResponse(LD::Type<T>{},LD::TransactionError{},LD::Forward<A>(arguments)...);
                };
                return LD::InvokeVisitation(LD::Overload{onObject,onError},response);
            }
            return LD::CreateResponse(LD::Type<T>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }

        template<typename T, typename ProjectType, typename SecretType,typename ... A>
        LD::RequestResponse<bool(A...)> Post(T && object, ProjectType && project,SecretType && secret ,A && ... args)
        {

            nlohmann::json  document;
            LD::ToJson(document["data"],LD::Forward<T>(object));
            //document["data"] = nlohmann::json::object();
            //document["data"]["name"] = "piplup";

           // std::cout << document.dump(2) << std::endl;

            auto vaultFetchURL = this->mURL+LD::ImmutableString{"/v1/"}+project+LD::ImmutableString{"/data/"}+secret;
            cpr::Response fetchSecretResponse =  cpr::Post(cpr::Url{vaultFetchURL.Data()},cpr::Header{{"X-Vault-Token",this->mToken.Data()}},cpr::Body{object.dump().c_str()});
            //std::cout << fetchSecretResponse.text << std::endl;

            if (fetchSecretResponse.status_code == 200)
            {
                return LD::CreateResponse(LD::Type<bool>{},bool{true},LD::Forward<A>(args)...);
            }
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<A>(args)...);
        }
    };

    template<typename URLType, typename TokenType> HashiVault(URLType &&, TokenType &&) ->HashiVault<URLType,TokenType>;
}
#endif //LANDESSDEVCORE_HASHIVAULT_HPP
