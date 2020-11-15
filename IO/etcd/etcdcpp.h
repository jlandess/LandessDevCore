
#ifndef LIBETCDCPP_cxx_
#define LIBETCDCPP_cxx_
#undef SIMDJSON_EXCEPTIONS
#include <vector>
#include <string.h>
#include <simdjson/simdjson.h>
#include "IO/rapidjson/document.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Algorithms/StringToPrimitive.hpp"
#include "IO/FetchRequest.h"
#include "Unicode/UTF8.hpp"
#include <memory>
#include <curl/curl.h>
namespace etcdcpp
{


    class etcd_host
    {
    public:
        etcd_host(std::string host, short port): host(host), port(port) {};
        const std::string & get_host() const { return host; }
        unsigned short get_port() const { return port; }
    private:
        std::string host;
        unsigned short port;
    };



    class etcd_session
   {


   private:

       std::vector<etcd_host> server_list;

   public:

       etcd_session(std::vector<etcd_host> server_list);


       etcd_session(etcd_host host);


       /* curl uses a callback to read urls. It passes the result buffer reference as an argument */
       static int writer(char *data, size_t size, size_t nmemb, std::string *buffer) noexcept
       {

           int result = 0;

           if(buffer != NULL)
           {
               buffer -> append(data, size * nmemb);
               result = size * nmemb;
           }

           return result;

       }

    /**
    * User must free returned Document *
    */

    std::unique_ptr<rapidjson::Document> get(std::string key);

    std::unique_ptr<simdjson::dom::element> get1(std::string key);

    template<typename F, typename ... Args,
            typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,Args...>>())),
            typename Ret = LD::QueryResult<FunctorRet(LD::StringView,Args...)>>
    LD::Enable_If_T<
    LD::Require<
    LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
    >
    ,
    Ret> Fetch(const LD::StringView & key, F && functor, Args && ... arguments) const noexcept
    {
        CURLcode res;
        std::unique_ptr<CURL,void(*)(CURL*)> curl{curl_easy_init(),curl_easy_cleanup};
        LD::StaticArray<char,1024> urlBuffer;
        LD::BackInserter<LD::StaticArray<char,1024>> urlBufferBackInserter{urlBuffer};
        LD::StringView  protocol{"http://"};
        for(LD::UInteger n= 0;n<7;++n)
        {
            urlBufferBackInserter = protocol[n];
        }
        auto url_builder = [](LD::BackInserter<LD::StaticArray<char,1024>> & inserter,const etcdcpp::etcd_host &host, LD::StringView key) noexcept -> LD::UInteger
        {
            for(LD::UInteger n = 0;n<host.get_host().size();++n)
            {
                inserter = host.get_host()[n];
            }
            inserter = ':';
            auto stringifiedPort = LD::ToImmutableString(host.get_port());
            auto stringifiedPortSize = stringifiedPort.GetSize();
            for(LD::UInteger n = 0;n<stringifiedPortSize;++n)
            {
                inserter = stringifiedPort[n];
            }
            LD::StringView api= "/v2/keys";
            for(LD::UInteger n = 0;n<api.size();++n)
            {
                inserter = api[n];
            }
            inserter = '/';
            for(LD::UInteger n = 0;n<key.size();++n)
            {
                bool isperiod = (key[n] == '.');
                inserter = (isperiod)*'/' + (!isperiod)*key[n];
            }
            inserter = '\0';
            return host.get_host().size() + stringifiedPortSize + + api.size() + key.size() + 2;
        };

        if (curl)
        {
            bool shouldContinue = true;
            std::string result;
            for (LD::UInteger n = 0;n<server_list.size() && shouldContinue;++n)
            {
                const auto & server = server_list[n];
                LD::UInteger numberOfBytesWritten = url_builder(urlBufferBackInserter,server,key);
                curl_easy_setopt(curl.get(), CURLOPT_URL, urlBuffer.GetData());
                //curl_easy_setopt(curl.get(), CURLOPT_POST,1);
                //curl_easy_setopt(curl.get(), CURLOPT_COPYPOSTFIELDS, valueBuffer.GetData());
                curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &etcd_session::writer);
                curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &result);

                res = curl_easy_perform(curl.get());

                if (res == CURLE_OK)
                {

                    shouldContinue = false;
                    simdjson::dom::parser parser;
                    simdjson::simdjson_result<simdjson::dom::element> element =  parser.parse(LD::StringView{result.c_str()});
                    //todo do error checking
                    if (element.error())
                    {
                        return LD::MakeContext(LD::TransactionError{},LD::StringView{},LD::Forward<Args>(arguments)...);
                    }
                    auto actionType = element["action"];

                    if (actionType.is_string())
                    {
                        auto node = element["node"];
                        auto keyedNode = node["key"];
                        auto versions = node["nodes"];
                        LD::StringView buffer;
                        keyedNode.get(buffer);
                        //std::cout << buffer << std::endl;
                        if (versions.is_array())
                        {
                            LD::StringView memes;

                            LD::UInteger mostRecentlyModified = 0;
                            LD::UInteger mostRecentlyModifiedIndex = 0;
                            LD::UInteger index = 0;
                            for(const auto & version: versions)
                            {
                                //LD::StringToPrimitive<LD::UInteger > converter{};
                                const auto & modifiedIndex = version["modifiedIndex"];
                                //LD::StringView fetchedValue;
                                LD::UInteger modifiedIndexValue;
                                modifiedIndex.get(modifiedIndexValue);
                                //LD::UInteger ret = LD::Match(converter(fetchedValue),[](auto){ return LD::UInteger {};},[](const LD::UInteger & obj){return obj;});
                                //std::cout << modifiedIndexValue << std::endl;
                                if (modifiedIndexValue > mostRecentlyModified)
                                {
                                    mostRecentlyModified =modifiedIndexValue;
                                    mostRecentlyModifiedIndex = index;
                                }
                                ++index;
                            }
                            //std::cout << "recently modified idnex: " << mostRecentlyModifiedIndex << std::endl;
                            auto it = versions.begin();
                            for(LD::UInteger n = 0 ;n<mostRecentlyModifiedIndex;++n,++it);

                            //std::cout << (*it)["modifiedIndex"] << std::endl;
                            LD::StringView valueView;
                            (*it)["value"].get(valueView);
                            //std::cout << valueView<< std::endl;
                            FunctorRet value = functor(LD::MakeContext(LD::StringView{key},LD::StringView{valueView},LD::Forward<Args>(arguments)...));
                            return LD::MakeContext(LD::TransactionResult{},FunctorRet{value},LD::StringView{key},LD::Forward<Args>(arguments)...);
                        }
                    }
                    //always pop the information related to the server_URL and request info
                }
                for(LD::UInteger n = 0;n<numberOfBytesWritten;++n)
                {
                    urlBuffer.PopBack();
                }
            }
        }
        return LD::MakeContext(LD::TransactionError{},LD::StringView {},LD::Forward<Args>(arguments)...);
    }
    template<typename ... Args,typename Ret = LD::QueryResult<bool(Args...)>>
    Ret Store(const LD::StringView & key, const LD::StringView & data, Args && ... arguements) const noexcept
    {
        CURLcode res;
        std::unique_ptr<CURL,void(*)(CURL*)> curl{curl_easy_init(),curl_easy_cleanup};
        LD::StaticArray<char,1024> urlBuffer;
        LD::StaticArray<char,1024> valueBuffer;
        LD::BackInserter<LD::StaticArray<char,1024>> valueBufferBackInserter{valueBuffer};
        LD::BackInserter<LD::StaticArray<char,1024>> urlBufferBackInserter{urlBuffer};
        LD::StringView  protocol{"http://"};
        for(LD::UInteger n= 0;n<7;++n)
        {
            urlBufferBackInserter = protocol[n];
        }


        auto url_builder = [](LD::BackInserter<LD::StaticArray<char,1024>> & inserter,const etcdcpp::etcd_host &host, LD::StringView key) noexcept -> LD::UInteger
        {
            for(LD::UInteger n = 0;n<host.get_host().size();++n)
            {
                inserter = host.get_host()[n];
            }
            inserter = ':';
            auto stringifiedPort = LD::ToImmutableString(host.get_port());
            auto stringifiedPortSize = stringifiedPort.GetSize();
            for(LD::UInteger n = 0;n<stringifiedPortSize;++n)
            {
                inserter = stringifiedPort[n];
            }
            LD::StringView api= "/v2/keys";
            for(LD::UInteger n = 0;n<api.size();++n)
            {
                inserter = api[n];
            }
            inserter = '/';
            for(LD::UInteger n = 0;n<key.size();++n)
            {
                bool isperiod = (key[n] == '.');
                inserter = (isperiod)*'/' + (!isperiod)*key[n];
            }
            inserter = '\0';


            return host.get_host().size() + stringifiedPortSize + + api.size() + key.size() + 2;
        };
        LD::StringView valueTag = "value=";
        for(LD::UInteger n=0;n<valueTag.size();++n)
        {
            valueBufferBackInserter=valueTag[n];
        }
        for(LD::UInteger n = 0;n<data.size();++n)
        {
            valueBufferBackInserter = data[n];
        }
        valueBufferBackInserter = '\0';
        //std::cout << "value : " << valueBuffer.GetData() << std::endl;
        std::string result;
        if (curl)
        {
            bool shouldContinue = true;
            for (LD::UInteger n = 0;n<server_list.size() && shouldContinue;++n)
            {
                const auto & server = server_list[n];
                //std::string url = build_url(server, key);
                //build the URL and return how many bytes were written in regards to creating the URL
                LD::UInteger numberOfBytesWritten = url_builder(urlBufferBackInserter,server,key);
                //std::cout << urlBuffer.GetData() << std::endl;
                //todo insert stuff into value buffer
                //std::string buffer = std::string{"value="} + data.data();
                curl_easy_setopt(curl.get(), CURLOPT_URL, urlBuffer.GetData());
                curl_easy_setopt(curl.get(), CURLOPT_POST,1);
                curl_easy_setopt(curl.get(), CURLOPT_COPYPOSTFIELDS, valueBuffer.GetData());
                curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &etcd_session::writer);
                curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &result);

                res = curl_easy_perform(curl.get());
                if (res == CURLE_OK)
                {
                    shouldContinue = false;
                    simdjson::dom::parser parser;


                    simdjson::simdjson_result<simdjson::dom::element> element =  parser.parse(LD::StringView{result.c_str()});
                    //todo do error checking
                    if (element.error())
                    {
                        return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
                    }
                    //std::cout << "ABC: " << result << std::endl;
                    return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<Args>(arguements)...);
                }
                //always pop the information related to the server_URL and request info
                for(LD::UInteger n = 0;n<numberOfBytesWritten;++n)
                {
                    urlBuffer.PopBack();
                }

            }

        }

        //curl_easy_cleanup(curl);
        return LD::MakeContext(LD::TransactionError{},LD::Forward<Args>(arguements)...);
    }
    
    /**
    * User must free returned Document *
    */
    std::unique_ptr<rapidjson::Document> set(std::string key, std::string value, int ttl);

    /**
    * User must free returned Document *
    */
    std::unique_ptr<rapidjson::Document> set(std::string key, std::string value);

   };
} //end namespace

#endif
