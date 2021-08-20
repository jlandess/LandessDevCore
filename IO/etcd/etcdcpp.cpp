
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "IO/rapidjson/document.h"
#include <simdjson/simdjson.h>
#include <memory>
#include "etcdcpp.h"
#include "Primitives/General/StringView.hpp"
//#include "Definitions/Common.hpp"



template<typename T>
std::string sappend(std::string s, T t) {
  std::ostringstream sstream;
  sstream << s << t;
  return sstream.str();
}

/* curl uses a callback to read urls. It passes the result buffer reference as an argument */
int writer(char *data, size_t size, size_t nmemb, std::string *buffer){
  int result = 0;
  if(buffer != NULL) {
    buffer -> append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
} 

template <typename fun>
std::unique_ptr<rapidjson::Document> with_curl(std::vector<etcdcpp::etcd_host> server_list, fun process) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  std::string result;
  if (curl) {
    for (auto server : server_list)
    {
      process(server, curl);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
      res = curl_easy_perform(curl);
      if (res != CURLE_OK)
      {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        throw res;
      }
      curl_easy_cleanup(curl);

      rapidjson::Document *d = new rapidjson::Document;
      d->Parse(result.c_str());
      return std::unique_ptr<rapidjson::Document>(std::move(d));
    }
  } 
  throw "Curl failed to initialize";
}

template <typename fun>
std::unique_ptr<simdjson::dom::element> with_curl1(std::vector<etcdcpp::etcd_host> server_list, fun process) noexcept {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string result;
    if (curl)
    {
        for (auto server : server_list)
        {
            process(server, curl);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writer);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK){
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                throw res;
            }
            curl_easy_cleanup(curl);

            //rapidjson::Document *d = new rapidjson::Document;
            //d->Parse(result.c_str());
            simdjson::dom::parser parser;
            //simdjson::dom::element * parsedDocument = new simdjson::dom::element{};
            //*parsedDocument = parser.parse(LD::StringView{result.c_str()});
            //std::cout << "Document 1 : " << parsedDocument << std::endl;
            return std::make_unique<simdjson::dom::element>(parser.parse(LD::StringView{result.c_str()}));
        }
    }
    throw "Curl failed to initialize";
}


std::string build_url(etcdcpp::etcd_host &host, std::string key) {
  std::ostringstream url;
  url << "http://" << host.get_host() << ":" <<  host.get_port() << "/v2/keys" << key;
  return url.str();
}

etcdcpp::etcd_session::etcd_session(std::vector<etcd_host> server_list) {
  if (server_list.size() == 0) {
    throw "Startup failed: At least one host is required";
  }
  this->server_list = server_list;
  curl_global_init(CURL_GLOBAL_ALL);
}

etcdcpp::etcd_session::etcd_session(etcd_host host) {
  std::vector<etcd_host> hosts;
  hosts.push_back(host);
  this->server_list = hosts;
  curl_global_init(CURL_GLOBAL_ALL);
}

std::unique_ptr<rapidjson::Document> etcdcpp::etcd_session::get(std::string key) {
  return with_curl(server_list, [=](etcd_host server, CURL *curl) {
    std::string url = build_url(server, key) + "?sorted=true&recursive=true";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  });
}

std::unique_ptr<simdjson::dom::element> etcdcpp::etcd_session::get1(std::string key)
{
    return with_curl1(server_list, [=](etcd_host server, CURL *curl) {
        std::string url = build_url(server, key) + "?sorted=true&recursive=true";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    });
}

 
std::unique_ptr<rapidjson::Document> etcdcpp::etcd_session::set(std::string key, std::string value, int ttl) {
  std::string buffer = value + sappend("&ttl=", ttl);
  return this->set(key, buffer);
}

std::unique_ptr<rapidjson::Document> etcdcpp::etcd_session::set(std::string key, std::string value)
{
  return with_curl(server_list, [=] (etcd_host server, CURL *curl)
  {
    std::string url = build_url(server, key);
    std::string buffer = "value=" + value;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST,1);
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, buffer.c_str());
  });
}

