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
#include "Async/Channel.hpp"
#include "Patterns/Observer.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Algorithms/Visitation.hpp"
#include "Algorithms/FromString.hpp"
#include "Reflection/Reflection.hpp"
#include "IO/json.hpp"
#include "Algorithms/ToJSON.hpp"
#include "Algorithms/FromJSON.hpp"
#include "IO/DataLink.hpp"
namespace LD
{
    class OpenDHTRegisterEvent
    {

    };

    class OpenDHTDeRegisterEvent
    {

    };
    class OpenDHTBackend;


    template<typename Topic, typename Queue, typename LocalBuffer>
    class TopicalOpenDHTConnection: public LD::DataLink
    {
    private:
        OpenDHTBackend * mInstance;
        Topic mTopic;
        Queue mQueue;
    public:
        TopicalOpenDHTConnection(OpenDHTBackend * back, dht::InfoHash hash, Topic && topic) noexcept;

        virtual LD::UInteger Write(unsigned char * data, LD::UInteger size) noexcept;
        virtual LD::UInteger Read(unsigned char * data, LD::UInteger size) noexcept;
        virtual LD::UInteger Read(LD::LightWeightDelegate<bool(unsigned char)>) noexcept;
    };
    class OpenDHTBackend
    {
    private:
        mutable dht::DhtRunner mNode;
        mutable std::shared_ptr<std::mutex> mMutex;
        mutable std::shared_ptr<std::condition_variable> mCondition;
        mutable std::shared_ptr<bool> mReady;
        LD::Atomic<LD::Integer> mRegistrationCount;
    public:

        template<typename V, typename Buffer, class = void>
        class SubscriptionTokenOne;
        template<typename V, typename Buffer>
        class SubscriptionTokenOne<V,Buffer,LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::Type<V>{})>>>
        {
        private:
            Buffer mBuffer;
            dht::InfoHash mHash;
            std::size_t mToken;
            LD::ElementReference<OpenDHTBackend> mBackend;
            LD::BasicObserver<LD::ElementReference<OpenDHTBackend>(LD::Variant<OpenDHTDeRegisterEvent,OpenDHTDeRegisterEvent>)> mObservee;
        public:

            using type = V;
            SubscriptionTokenOne() noexcept{}
            SubscriptionTokenOne(LD::ElementReference<OpenDHTBackend> backend, dht::InfoHash hash,Buffer && buffer) noexcept:mBackend{backend},mHash{hash},mObservee{LD::ElementReference<OpenDHTBackend> {backend}},mBuffer{buffer}
            {

                auto listenLambda  = [&](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
                {
                    LD::BackInserter inserter{this->mBuffer};
                    for(const auto & answer: answers)
                    {
                        auto response = LD::StringView {(const char*)answer->data.data(),answer->data.size()};

                        simdjson::dom::parser parser;
                        simdjson::dom::element parsedResponse = parser.parse(response);
                        auto possibleRes = parsedResponse["value"];
                        if (!possibleRes.error())
                        {
                            V result;
                            possibleRes.get(result);
                            inserter = result;
                        }
                    }
                    return true;
                };
                backend->mNode.listen(hash,listenLambda).get();
                mObservee(LD::OpenDHTDeRegisterEvent{});

            }

            SubscriptionTokenOne & operator >> (LD::Optional<V> & possibleValue) noexcept
            {
                this->mBuffer >> possibleValue;
                return (*this);
            }
        };
        template<typename BackInserterType>
        class SubscriptionToken
        {
        private:
            dht::InfoHash mHash;
            std::size_t mToken;
            LD::ElementReference<OpenDHTBackend> mBackend;
            LD::BasicObserver<LD::ElementReference<OpenDHTBackend>(LD::Variant<OpenDHTDeRegisterEvent,OpenDHTDeRegisterEvent>)> mObservee;
        public:

            SubscriptionToken():mBackend{nullptr},mToken{0}{}
            SubscriptionToken(LD::ElementReference<OpenDHTBackend> backend, dht::InfoHash hash, BackInserterType backInserter) noexcept:mBackend{backend},mHash{hash},mObservee{LD::ElementReference<OpenDHTBackend> {backend}}
            {
                auto listenLambda  = [backInserter](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
                {
                    BackInserterType inserter = backInserter;
                    for(const auto & answer: answers)
                    {

                        auto response = LD::StringView {(const char*)answer->data.data(),answer->data.size()};
                        inserter = response;
                    }
                    return true;
                };
                backend->mNode.listen(hash,listenLambda).get();
                //mObservee(LD::Variant<OpenDHTDeRegisterEvent,OpenDHTDeRegisterEvent>{LD::OpenDHTDeRegisterEvent{}});
                mObservee(LD::OpenDHTDeRegisterEvent{});
            }

            SubscriptionToken(const SubscriptionToken &) = delete;


            SubscriptionToken & operator = (const SubscriptionToken & token) = delete;

            SubscriptionToken(SubscriptionToken && subscriptionToken) noexcept
            {
                (*this) = LD::Move(subscriptionToken);
            }

            SubscriptionToken & operator = (SubscriptionToken && token) noexcept
            {
                this->mToken = token.mToken;
                this->mBackend = token.mBackend;
                this->mHash = token.mHash;

                token.mBackend = LD::ElementReference<OpenDHTBackend>{nullptr};
                token.mHash = dht::InfoHash {};
                token.mToken = std::size_t {};

                return (*this);
            }

            ~SubscriptionToken()
            {
                mObservee(LD::OpenDHTDeRegisterEvent{});
            }
        };

        OpenDHTBackend(OpenDHTBackend && dht) noexcept
        {

        }

        OpenDHTBackend & operator = (OpenDHTBackend && dht) noexcept
        {
            return (*this);
        }
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

        void operator()( LD::Variant<OpenDHTDeRegisterEvent,OpenDHTDeRegisterEvent>  event) noexcept
        {
            auto onRegister = [](LD::OpenDHTRegisterEvent) noexcept
            {
                return 1;
            };

            auto onDeRegister = [](LD::OpenDHTDeRegisterEvent) noexcept
            {
                return -1;
            };

            this->mRegistrationCount.store(this->mRegistrationCount.load(LD::MemoryOrder::AcquireRelease)+LD::Visit(LD::Overload{onRegister,onDeRegister},event),LD::MemoryOrder::AcquireRelease);
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

            std::scoped_lock scopedLock{*this->mMutex};
            this->mNode.listen(room.data(), listenLambda);
        }

        auto Publish(LD::StringView key, LD::StringView value) noexcept
        {
            return this->Insert(key,value);
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::CT::IsPrimitive(LD::Type<T>{})>,LD::RequestResponse<bool()>> Publish(LD::StringView key, T && value) noexcept
        {
            nlohmann::json document;
            document["value"] = LD::Forward<T>(value);
            return this->Insert(key,LD::StringView{document.dump()});
        }

        template<typename T>
        LD::Enable_If_T<LD::Require<LD::CT::IsReflectable(LD::Type<T>{})>,LD::RequestResponse<bool()>> Publish(LD::StringView key, T && value) noexcept
        {
            nlohmann::json document;
            LD::ToJson(document,LD::Forward<T>(value));
            //document["value"] = LD::Forward<T>(value);
            return this->Insert(key,LD::StringView{document.dump()});
        }


        template<typename BackInserterType>
        LD::Pair<dht::InfoHash,LD::UInteger> Listen(LD::StringView room,BackInserterType backInserter) noexcept
        {
            auto listenLambda  = [backInserter](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
            {
                BackInserterType inserter = backInserter;
                for(const auto & answer: answers)
                {

                    auto response = LD::StringView {(const char*)answer->data.data(),answer->data.size()};
                    inserter = response;
                }
                return true;
            };
            dht::InfoHash hash = dht::InfoHash{}.get(room.data());
            LD::UInteger token = this->mNode.listen(hash,listenLambda).get();
            return LD::Pair<dht::InfoHash,LD::UInteger>{hash,token};
        }

        template<typename F>
        LD::Pair<dht::InfoHash,LD::UInteger> ListenWithFunction(LD::StringView room,F function) noexcept
        {
            auto listenLambda  = [function](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
            {
                F inserter = function;
                for(const auto & answer: answers)
                {

                    auto response = LD::StringView {(const char*)answer->data.data(),answer->data.size()};
                    inserter(response);
                }
                return true;
            };
            dht::InfoHash hash = dht::InfoHash{}.get(room.data());
            LD::UInteger token = this->mNode.listen(hash,listenLambda).get();
            return LD::Pair<dht::InfoHash,LD::UInteger>{hash,token};
        }

        void StopListen(dht::InfoHash hash, LD::UInteger token) noexcept
        {

        }

        template<typename Topic, typename Queue, typename LocalBuffer>
        LD::SharedPointer<LD::TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>>  TopicalDataLink(Topic && topic, LD::Type<Queue>, LD::Type<LocalBuffer>) noexcept;
        template<typename BackInserterType>
        SubscriptionToken<BackInserterType>  Subscribe(LD::StringView room, BackInserterType inserter) noexcept
        {
            auto successHandler = [=](bool success) noexcept
            {
                //std::cout << "Success : " << success << "\n";
                this->DoneDB(success);
            };
            //this->Wait();
            auto ret = SubscriptionToken<BackInserterType>{
                LD::ElementReference<OpenDHTBackend>{this},
                dht::InfoHash {}.get(room.data()),
                inserter};

            //this->DoneDB(true);

            return ret;
        }

        template<typename ChannelType, typename ConstrainedType>
        SubscriptionTokenOne<ConstrainedType,ChannelType>  SubscribeOne(LD::StringView room, LD::Type<ConstrainedType>,ChannelType && inserter) noexcept
        {
            auto successHandler = [=](bool success) noexcept
            {
                //std::cout << "Success : " << success << "\n";
                this->DoneDB(success);
            };
            //this->Wait();
            //auto ret = SubscriptionTokenOne<ConstrainedType,BackInserterType>{
                    //LD::ElementReference<OpenDHTBackend>{this},
                    //dht::InfoHash {}.get(room.data()),
                    //inserter};

            //this->DoneDB(true);

            return SubscriptionTokenOne<ConstrainedType,ChannelType>{
                    LD::ElementReference<OpenDHTBackend>{this},
                    dht::InfoHash {}.get(room.data()),
                    LD::Forward<ChannelType>(inserter)
            };
        }
        template<typename ConstrainedType, typename ChannelType>
        SubscriptionTokenOne<ConstrainedType,ChannelType>  SubscribeOne(LD::StringView room, LD::Type<ConstrainedType>, LD::Type<ChannelType>) noexcept
        {
            return SubscriptionTokenOne<ConstrainedType,ChannelType>{
                    LD::ElementReference<OpenDHTBackend>{this},
                    dht::InfoHash {}.get(room.data()),
                    ChannelType{}
            };
        }

        LD::ImmutableString<128> NodeIdentification() const noexcept
        {
            auto identity = this->mNode.getNodeId().toString();

            return LD::ImmutableString<128>{LD::StringView {identity.data(),identity.size()}};
        }

        template<typename ... Args>
        LD::RequestResponse<bool(Args...)> Insert(const LD::StringView key, const LD::StringView data, Args && ... arguments) const noexcept
        {
            auto successHandler = [=](bool success) noexcept
            {
                //std::cout << "Success : " << success << "\n";
                this->DoneDB(success);
            };
            //printf("key %s , value %s \n",key.data(),data.data());


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

            LD::Optional<ReturnType> returnable;

            std::future<std::vector<std::shared_ptr<dht::Value>>> info = this->mNode.get(dht::InfoHash{}.get(key.data()));

            auto result = info.get();
            if (result.size() > 0)
            {
                returnable = functor(key,LD::StringView{
                                             (const char*)result[result.size()-1]->data.data(),
                                             result[result.size()-1]->data.size()},
                                     LD::Forward<Args>(arguments)...);
            }

            /*
            this->mNode.get(key.data(),
                     [&](const std::vector<std::shared_ptr<dht::Value>>& values)
            {
                returnable = functor(key,LD::StringView{
                    (const char*)values[values.size()-1]->data.data(),
                    values[values.size()-1]->data.size()},
                    LD::Forward<Args>(arguments)...);
                //std::cout << "Memes: " << *values[values.size()-1] << "\n";

                return true; // return false to stop the search
            },
             [&] (bool success)
             {
                successful = success;

                this->DoneDB(success);
             });

            this->Wait();
             */

            if (result.size() > 0 && returnable)
            {
                return LD::CreateResponse(LD::Type<ReturnType>{},ReturnType {*returnable},LD::Forward<Args>(arguments)...);
            }

            return LD::CreateResponse(LD::Type<ReturnType>{},LD::TransactionError{},LD::Forward<Args>(arguments)...);
        }

        template<typename Stream, typename IndexStreamer, typename Comparator>
        LD::Variant<LD::TransactionError,LD::UInteger> QueryWithStream(
                LD::StringView key,
                Stream streamer,
                IndexStreamer indexStreamer,
                Comparator comparator,
                LD::UInteger packetCount = 0) const noexcept //-> LD::RequestResponse<decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()))(Args&&...)>
        {

            //using ReturnType = decltype(LD::Declval<F>()(LD::Declval<LD::StringView>(),LD::Declval<LD::StringView>()));
            bool successful = false;


            //LD::UInteger max = 0;
            this->mNode.get(key.data(),
                            [&,streamer,packetCount,indexStreamer,comparator](const std::vector<std::shared_ptr<dht::Value>>& values)
            {
                Stream stream = streamer;
                IndexStreamer indexStream = indexStreamer;
                Comparator currentComparator = comparator;
                for(LD::UInteger n = 0;n< values.size();++n)
                {
                    //max = LD::Max(max,values[n]->id);
                    //std::cout << "Sequence: " << values[n]-><< "\n";

                    if (currentComparator(values[n]->id))
                    {
                        std::cout << "Packet ID: " << values[n]->id << "\n";
                        stream = LD::StringView{(const char*)values[n]->data.data(),values[n]->data.size()};
                    }
                    if (values[n]->id > packetCount)
                    {

                    }

                }
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
                return LD::UInteger {0};
            }

            return LD::TransactionError{};
        }

        template<typename Q, typename R, typename S>
        friend class TopicalOpenDHTConnection;
    };

    //inline LD::Channel<LD::ImmutableString<64>> * biteMe = new LD::Channel<LD::ImmutableString<64>>{};
    template<typename Topic, typename Queue, typename LocalBuffer>
    TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>::TopicalOpenDHTConnection(OpenDHTBackend * backend,dht::InfoHash hash, Topic && topic) noexcept :mInstance{backend},mTopic{LD::Forward<Topic>(topic)}
    {
        auto listenLambda  = [&](const std::vector<std::shared_ptr<dht::Value> > & answers) -> bool
        {
            LD::BackInserter inserter{this->mQueue};
            for(const auto & answer: answers)
            {
                //std::cout << "Reply: " << LD::StringView {(const char*)answer->data.data(),answer->data.size()} << "\n";
                auto response = LD::StringView {(const char*)answer->data.data(),answer->data.size()};
                LocalBuffer buffer{response};
                inserter = buffer;
            }
            return true;
        };
        backend->mNode.listen(hash,listenLambda).get();
    }

    template<typename Topic, typename Queue, typename LocalBuffer>
    LD::UInteger TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>::Write(unsigned char *data, LD::UInteger size) noexcept
    {
        auto transaction = this->mInstance->Insert(ViewAsStringView(this->mTopic),LD::StringView{(char*)data,size});
        auto onWrite = [=](auto) noexcept
        {
            return size;
        };

        auto onFailure = [](LD::TransactionError) noexcept
        {
            return LD::UInteger {};
        };
        return LD::InvokeVisitation(LD::Overload{onWrite,onFailure},transaction);
    }

    template<typename Topic, typename Queue, typename LocalBuffer>
    LD::UInteger TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>::Read(unsigned char *data, LD::UInteger size) noexcept
    {
        LD::Optional<LocalBuffer> localBuffer;

        this->mQueue >> localBuffer;

        if(localBuffer)
        {
            auto & buffer = *localBuffer;
            LD::UInteger indexesTraversed = 0;
            for(LD::UInteger n = 0;n<size;++n)
            {
                data[n] = '\0';
            }

            for(auto it = LD::Begin(buffer);it!=LD::End(buffer) && indexesTraversed<size;++it,++indexesTraversed)
            {
                data[indexesTraversed] = *it;
            }
            return indexesTraversed;
        }

        return 0;
    }

    template<typename Topic, typename Queue, typename LocalBuffer>
    LD::UInteger TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>::Read(LD::LightWeightDelegate<bool(unsigned char)> callback) noexcept
    {
        LD::Optional<LocalBuffer> localBuffer;

        this->mQueue >> localBuffer;

        if(localBuffer)
        {
            auto & buffer = *localBuffer;
            LD::UInteger indexesTraversed = 0;

            for(auto it = LD::Begin(buffer);it!=LD::End(buffer);++it,++indexesTraversed)
            {
                callback(*it);

            }
            return indexesTraversed;
        }

        return 0;
    }

    template<typename Topic, typename Queue, typename LocalBuffer>
    LD::SharedPointer<LD::TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>>  OpenDHTBackend::TopicalDataLink(Topic && topic, LD::Type<Queue>, LD::Type<LocalBuffer>) noexcept
    {
        auto hash = dht::InfoHash{}.get(topic.Data());

        return LD::MakeShared<LD::TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>>(this,hash,LD::Forward<Topic>(topic));//TopicalOpenDHTConnection<Topic,Queue,LocalBuffer>{this,hash,LD::Forward<Topic>(topic)};
    }
}

namespace LD
{

    namespace CT
    {

        template<typename IteratorType>
        constexpr bool IsSubsriptionToken(LD::Type<LD::OpenDHTBackend::SubscriptionToken<IteratorType>> ) noexcept
        {
            return true;
        }

        template<typename T>
        LD::Type<void> SubscriptionType(LD::Type<T>) noexcept
        {
            return LD::Type<void>{};
        }

        template<typename V, typename B>
        LD::Type<V> SubscriptionType(LD::Type<OpenDHTBackend::SubscriptionTokenOne<V,B>>) noexcept
        {
            return LD::Type<V>{};
        }
    }
}
#endif //LANDESSDEVCORE_DHTBACKEND_HPP
