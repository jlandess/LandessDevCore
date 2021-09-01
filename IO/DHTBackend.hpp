//
// Created by phoenixflower on 4/13/21.
//

#ifndef LANDESSDEVCORE_DHTBACKEND_HPP
#define LANDESSDEVCORE_DHTBACKEND_HPP
#include <opendht.h>
#include "Primitives/General/StringView.hpp"
#include "Functor/fixed_size_function.hpp"
#include "Core/RequestResponse.hpp"
#include "Network/IPV4Address.hpp"
#include "Network/IPV6Address.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/Context.h"
namespace LD
{
    class OpenDHTBackend
    {
    private:
        mutable dht::DhtRunner mNode;
        mutable std::shared_ptr<std::mutex> mMutex;
        mutable std::shared_ptr<std::condition_variable> mCondition;
        mutable std::shared_ptr<bool> mReady;
    public:
        OpenDHTBackend(
                const LD::Variant<LD::IPV4Address,LD::IPV6Address> & address,
                        LD::Port listeningPort,
                        LD::Port bootstrapPort) noexcept
        {
            this->mReady = std::make_shared<bool>(false);
            this->mMutex = std::make_shared<std::mutex>();
            this->mCondition = std::make_shared<std::condition_variable>();
            this->mNode.run(listeningPort.Value(), dht::crypto::generateIdentity(), true);
            auto onIPV4 = [](const LD::IPV4Address & address) noexcept -> LD::ImmutableString<64>
            {
                return LD::ToImmutableString(address);
            };

            auto onIPV6 = [](const LD::IPV6Address & address) noexcept -> LD::ImmutableString<64>
            {
                return LD::ToImmutableString(address);
            };

            auto stringifiedAddress = LD::MultiMatch(LD::Overload{onIPV4,onIPV6},address);
            auto stringifiedPort = LD::ToImmutableString(bootstrapPort.Value());
            this->mNode.bootstrap(stringifiedAddress.Data(), stringifiedPort.Data());
            //fd00:1700:81b8:401e:0:d9:191:4a34
            //4222
            //this->mNode.bootstrap("127.0.0.1", "47008");
            //this->mNode.bootstrap("fd00:1700:81b8:401e:0:d9:191:4a34", "4222");
        }

        void Wait() const
        {
            *this->mReady = true;
            std::unique_lock<std::mutex> lk(*this->mMutex);
            this->mCondition->wait(lk);
            *this->mReady = false;
        }

        void DoneDB(bool success) const
        {
            if (success) {
                //std::cout << "success!" << std::endl;
            } else {
                //std::cout << "failed..." << std::endl;
            }
            std::unique_lock<std::mutex> lk(*this->mMutex);
            this->mCondition->wait(lk, [=]{ return *this->mReady; });
            this->mCondition->notify_one();
        }

        template<typename F, typename ... A>
        LD::Enable_If_T<LD::Require<LD::ConvertiblyCallable<F,void(LD::StringView,A && ...)>::Value()>,void>  Subscribe(LD::StringView room, F && function, A && ... args) noexcept
        {

            auto copyableContext = LD::MakeTuple(LD::StringView {},args...);
            auto listenLambda  = [=](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
            {
                for(const auto & answer: answers)
                {

                    LD::Get(LD::Get<0>(copyableContext)) = LD::StringView {(const char*)answer->data.data(),answer->data.size()};
                    LD::Invoke(function,copyableContext);
                    //values[n]->data.size()
                    //function(LD::StringView {(const char*)answer->data.data(),answer->data.size()},args...);
                    //std::cout << *answer << std::endl;
                }
                return true;
            };

            this->mNode.listen(room.data(), listenLambda);
        }

        template<typename ... Args>
        LD::RequestResponse<bool(Args...)> Insert(const LD::StringView key, const LD::StringView data, Args && ... arguments) const noexcept
        {
            auto successHandler = [=](bool success) noexcept
            {
                std::cout << "Success : " << success << "\n";
                this->DoneDB(success);
            };
            printf("key %s , value %s \n",key.data(),data.data());


            /*
            auto managedValue = std::make_shared<dht::Value>(
                    dht::ValueType::USER_DATA.USER_DATA,
                    std::vector<uint8_t> {data.begin(), data.end()});
            managedValue->user_type = "text/plain";

             */

            std::vector<uint8_t> some_data(data.begin(), data.end());
            this->mNode.put(key.data(), some_data,successHandler);
            /*
            this->mNode.put(
                    dht::InfoHash(std::string{key.begin(),key.end()}),
                    managedValue,
                    successHandler);
                    */
            this->Wait();
            return LD::CreateResponse(LD::Type<bool>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }
        template<typename ... Args, typename F>
        auto Query(LD::StringView key, F && functor, Args && ... arguments) const noexcept -> LD::RequestResponse<decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()))(Args&&...)>
        {

            using ReturnType = decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()));
            bool successful = false;

            this->mNode.get(key.data(),
                     [&](const std::vector<std::shared_ptr<dht::Value>>& values)
            {
                functor(key,LD::StringView{
                    (const char*)values[values.size()-1]->data.data(),
                    values[values.size()-1]->data.size()},
                    LD::Forward<Args>(arguments)...);
                std::cout << "Memes: " << *values[values.size()-1] << "\n";

                return true; // return false to stop the search
            },
             [&] (bool success)
             {
                successful = success;

                this->DoneDB(success);
             });

            this->Wait();

            if (successful)
            {
                return LD::CreateResponse(LD::Type<ReturnType>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
            }

            return LD::CreateResponse(LD::Type<ReturnType>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }
    };
}
#endif //LANDESSDEVCORE_DHTBACKEND_HPP
