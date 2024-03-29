#include "Async/RingBuffer.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Examples/IMGUIExample1.h"
#include "Examples/IMGUIExample2.h"
#include "Examples/WebDavExample.h"
#include "Examples/WebServerExample.hpp"
#include "Examples/WebServerExample.hpp"
#include "REST/CPR/cpr.h"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ctre.hpp"
#include <IO/UnqliteDatabaseBackend.h>
#include <Algorithms/Benchmark.hpp>
#include "Core/Unit.hpp"
#include "Network/IPV4Address.hpp"
#include "Core/RequestResponse.hpp"
#include "Memory/Optional.h"
#include "IaaS/DockerManifest.hpp"
#include "Algorithms/ToJSON.hpp"
#include <webdav/client.hpp>
#include <opendht.h>
#include <PBX/Mirta/Date.hpp>
#include <MVC/Cusor.h>
#include <MVC/TermboxTextLabel.hpp>
#include "IO/DHTBackend.hpp"
#include "IO/TieredDatabase.h"
#include "IaaS/ArtifactRepository.hpp"
#include "Algorithms/FromJSON.hpp"
#include "IaaS/HashiVault/HashiVault.hpp"

#include "Patterns/DependencyInjection.hpp"
#include "Algorithms/GetDecimalPlaces.hpp"
#include "Patterns/Observer.hpp"
#include "Patterns/Scheduling.hpp"
#include "MVC/TermBoxConsoleApplication.hpp"
#include "Primitives/General/CircularQueue.hpp"
#include "IO/PublisherSubscriber.h"

#include "Services/Logging.hpp"
#include "Examples/DependencyInjectionExample.hpp"
#include "Core/CompileTimeMap.hpp"
#include "MVC/TermBoxLogger.hpp"
#include "Examples/PublisherSubscriberExample.h"

template<typename KeyType, typename FunctionType>
class OpenDHTSubscriptionMember
{
private:
    KeyType mKey;
    FunctionType mFunction;
    LD::UInteger mPacketNumber;
};

class OpenDHTSubscriptionBucket
{
private:
    dht::InfoHash mHash;
    LD::UInteger mOpenDHTID;
    LD::UInteger mID;
public:
};
template<typename KeyType, typename IDType,typename ItemQueueType, typename RemovalQueueType, typename ReuseableKeyQueueType = RemovalQueueType>
class BasicOpenDHTSubscription
{
private:
    LD::ElementReference<LD::OpenDHTBackend> mBackend;
    LD::StaticArray<LD::Tuple<dht::InfoHash,LD::UInteger,LD::UInteger>,15> mFunctions;
    LD::CircularQueue<LD::Tuple<KeyType,LD::fixed_size_function<void(LD::StringView)>,LD::UInteger>,15> mItemsToBeAdded;
    LD::CircularQueue<LD::UInteger ,15> mItemsToBeRemoved;
    LD::CircularQueue<LD::UInteger ,15> mReusableKeys;

    LD::BasicChannel<LD::CircularQueue<LD::UInteger ,15>&> mItemsToBeRemovedChannel;
    LD::BasicChannel<LD::CircularQueue<LD::Tuple<KeyType,LD::fixed_size_function<void(LD::StringView)>,LD::UInteger>,15>&> mItemsToBeAddedChannel;
    IDType mNextID;
    LD::UInteger mNumberOfListeners;
public:

    class SubscriptionToken
    {
    private:
        LD::UInteger mID;
        LD::BasicObserver<LD::ElementReference<BasicOpenDHTSubscription>(LD::Variant<LD::OpenDHTDeRegisterEvent,LD::OpenDHTDeRegisterEvent>,LD::UInteger)> mObservee;
    public:

        SubscriptionToken(LD::ElementReference<BasicOpenDHTSubscription> backend, LD::UInteger id):mObservee{LD::ElementReference<BasicOpenDHTSubscription> {backend}}
        {
            this->mObservee(LD::OpenDHTRegisterEvent{},this->mID);
        }

        SubscriptionToken() = delete;
        SubscriptionToken(SubscriptionToken && ) = delete;
        SubscriptionToken(const SubscriptionToken &) = delete;

        ~SubscriptionToken()
        {
            this->mObservee(LD::OpenDHTDeRegisterEvent{},this->mID);
        }
    };
    BasicOpenDHTSubscription(LD::ElementReference<LD::OpenDHTBackend> backend) noexcept:mBackend{backend},mNextID{0},mNumberOfListeners{0},
                                                                                        mItemsToBeRemovedChannel{mItemsToBeRemoved},mItemsToBeAddedChannel{mItemsToBeAdded}
    {

    }
    ~BasicOpenDHTSubscription()
    {

        while (this->mNumberOfListeners > 0){};
    }
    void operator()( LD::Variant<LD::OpenDHTRegisterEvent,LD::OpenDHTDeRegisterEvent>  event, LD::UInteger id) noexcept
    {
        auto onRegistration = [](LD::OpenDHTRegisterEvent, LD::UInteger id, LD::ElementReference<LD::CircularQueue<LD::UInteger ,15>> itemsToBeRemoved, LD::ElementReference<LD::UInteger> numberOfListeners)
        {
            (*numberOfListeners)++;
        };

        auto onDeRegistration = [](LD::OpenDHTDeRegisterEvent, LD::UInteger id, LD::ElementReference<LD::CircularQueue<LD::UInteger ,15>> itemsToBeRemoved, LD::ElementReference<LD::UInteger> numberOfListeners)
        {
            (*numberOfListeners)--;
            LD::CircularQueue<LD::UInteger ,15> & referenceToItems = *itemsToBeRemoved;
            LD::BackInserter itemsToBeRemovedInserter{referenceToItems};
            itemsToBeRemovedInserter = id;
            //itemsToBeRemoved->Insert(id);
        };
        LD::VisitWithContext(LD::Overload{onRegistration,onDeRegistration},
                             LD::MakeTuple(LD::UInteger{id},LD::ElementReference<LD::CircularQueue<LD::UInteger ,15>>{this->mItemsToBeRemoved},LD::ElementReference<LD::UInteger>{this->mNumberOfListeners}),
                             event);
    }
    void operator()() noexcept
    {
        LD::BackInserter inserter{this->mFunctions};
        LD::Optional<LD::Tuple<KeyType,LD::fixed_size_function<void(LD::StringView)>,LD::UInteger>> possibleItemToBeAdded;
        mItemsToBeAddedChannel >> possibleItemToBeAdded;
        if(possibleItemToBeAdded)
        {
            auto listener = *possibleItemToBeAdded;
            LD::Pair<dht::InfoHash,LD::UInteger> res = this->mBackend->ListenWithFunction(LD::StringView{LD::Get<0>(listener).Data()},LD::Get<1>(listener));
            inserter = LD::MakeTuple(res.GetFirst(),res.GetSecond(),LD::Get<2>(listener));
        }
        LD::Optional<LD::UInteger> possibleItemToBeRemoved;
        mItemsToBeRemovedChannel >> possibleItemToBeRemoved;

        if (possibleItemToBeRemoved)
        {
            LD::UInteger indexToRemove = (*possibleItemToBeRemoved);
        }
        //if (!this->mItemsToBeAdded.empty())
        //{
            //LD::Tuple<KeyType,LD::fixed_size_function<void(LD::StringView)>,LD::UInteger> listener = this->mItemsToBeAdded.remove();
            //LD::Pair<dht::InfoHash,LD::UInteger> res = this->mBackend->ListenWithFunction(LD::StringView{LD::Get<0>(listener).Data()},LD::Get<1>(listener));
            //inserter = LD::MakeTuple(res.GetFirst(),res.GetSecond(),LD::Get<2>(listener));
        //}
        //if (!this->mItemsToBeRemoved.empty())
        //{
            //this->mItemsToBeRemoved.remove();
        //}
    }

    template<typename Inserter>
    BasicOpenDHTSubscription::SubscriptionToken Subscribe(KeyType key, Inserter backInserter) noexcept
    {
        auto callBack = [=](LD::StringView value) noexcept
        {
            Inserter currentInserter = backInserter;
            currentInserter = value;
        };

        LD::BackInserter itemsToBeAddedInserter{this->mItemsToBeAdded};
        itemsToBeAddedInserter = LD::MakeTuple(KeyType{key},LD::fixed_size_function<void(LD::StringView)>{callBack},LD::UInteger{this->mNextID});

        return BasicOpenDHTSubscription::SubscriptionToken{LD::ElementReference<BasicOpenDHTSubscription>{this},this->mNextID++};

    }
};

using OpenDHTSubscription = BasicOpenDHTSubscription<LD::ImmutableString<256>,LD::UInteger ,LD::StaticArray<LD::Tuple<dht::InfoHash,LD::UInteger,LD::UInteger>,15>,LD::CircularQueue<LD::UInteger ,15>>;

class ExampleBasicApplication
{
private:
    LD::Second<LD::Float> mTimeElapsed;
public:
    ExampleBasicApplication() noexcept:mTimeElapsed(0_s)
    {

    }
    bool operator()(const LD::ApplicationStartedEvent<> & frameStarted) noexcept
    {
        return true;
    }
    bool operator()(const LD::ApplicationFrameStartedEvent<> & frameStarted) noexcept
    {
        this->mTimeElapsed += frameStarted.TimeElapsed();

        std::cout <<  LD::ToImmutableString(this->mTimeElapsed).Data()<< "\n";
        return this->mTimeElapsed < 10_s;
    }

    LD::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<> & frameStarted) noexcept
    {
        return 1.0_s;
    }

    void operator()(const LD::ApplicationExecutionEvent<> & execution) noexcept
    {

    }

    void operator()(const LD::ApplicationFrameEndedEvent<> & execution) noexcept
    {

    }

    void operator()(const LD::ApplicationQuittingEvent<> & execution) noexcept
    {

    }
};


template<typename T>
class MultiApplication;

class MultiApplicationParameter
{
private:
    LD::Second<LD::Float> AccumulatedPeriod;
    LD::Second<LD::Float> CachedPeriod;
};
template<typename ... T, typename ... A>
class MultiApplication<LD::CT::TypeList<T...>(A...)>
{
private:
    LD::Tuple<T...> mExecutors;
    LD::StaticArray<MultiApplicationParameter,sizeof...(T)> mExecutorParams;
public:
    MultiApplication(T && ... executors) noexcept:mExecutors{LD::Forward<T>(executors)...}
    {

    }

    template<typename State, typename Event>
    auto OnEvent(State&, const Event&)
    {

        return LD::nullopt;

    }

    LD::ApplicationState OnEvent(const LD::InitialApplicationState & initialApplicationState, const LD::ApplicationStartedEvent<A...> & applicationStartedEvent)
    {

        bool shouldRun = true;
        //const bool shouldRun = BooleanEventDispatcher(this->Events, applicationStartedEvent);
        return LD::ApplicationHasStartedState{shouldRun};

    }

    LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState & applicationHasStartedState, const LD::ApplicationFrameStartedEvent<A...> & applicationFrameStartedEvent)
    {
        //resolve any initial conditions that have to met for the application to run, if not then just quit
        //const bool shouldRun = BooleanEventDispatcher(this->Events, applicationFrameStartedEvent);
        bool shouldRun = true;
        return LD::ApplicationFrameStartedState{shouldRun};
    }

    LD::ApplicationState OnEvent(const LD::ApplicationFrameEndedState & applicationFrameEndedState, const LD::ApplicationFrameStartedEvent<A...> & newFrameEvent)
    {
        //decide betweent the end of the last frame, and the beginning of the new frame if the application should continue to run
        //const bool shouldRun = BooleanEventDispatcher(this->Events, newFrameEvent);
        bool shouldRun = true;
        return LD::ApplicationFrameStartedState{shouldRun};
    }
    LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState & frameStartedState, const LD::ApplicationPeriodEvent<A...> & sleepingEvent)
    {

        //LD::Second<LD::Float> period =  PeriodEventDispatcher(this->Events,sleepingEvent);

        LD::For<sizeof...(T)>([](
                auto Index,
                const LD::ApplicationPeriodEvent<A...> & periodEvent,
                LD::Tuple<T...> & executors) noexcept
        {
            LD::Second<LD::Float> requiredTime =  LD::Get(LD::Get<Index>(executors))(periodEvent);
            return true;
        },sleepingEvent,this->mExecutors);
        LD::Second<LD::Float> period = 1_s;
        return LD::ApplicationPeriodState{period};
    }
    LD::ApplicationState OnEvent(const LD::ApplicationPeriodState & periodState, const LD::ApplicationExecutionEvent<A...> & executionEvent)
    {
        //EventDispatcher(this->Events,executionEvent);
        LD::For<sizeof...(T)>([](
                auto Index,
                const LD::ApplicationExecutionEvent<A...> & executionEvent,
                LD::Tuple<T...> & executors) noexcept
        {
            LD::Get(LD::Get<Index>(executors))(executionEvent);
            return true;
        },executionEvent,this->mExecutors);
        return LD::ApplicationExecutionState{};
    }

    LD::ApplicationState OnEvent(const LD::ApplicationExecutionState &, const LD::ApplicationFrameEndedEvent<A...> & frameEndedEvent)
    {
        //EventDispatcher(this->Events,frameEndedEvent);
        LD::For<sizeof...(T)>([](
                auto Index,
                const LD::ApplicationFrameEndedEvent<A...> &  frameEndedEvent,
                LD::Tuple<T...> & executors) noexcept
        {
            LD::Get(LD::Get<Index>(executors))(frameEndedEvent);
            return true;
        },frameEndedEvent,this->mExecutors);
        return LD::ApplicationFrameEndedState{};
    }

    LD::ApplicationState OnEvent(const LD::ApplicationFrameStartedState &, const LD::ApplicationQuittingEvent<A...> & quittingEvent)
    {
        LD::For<sizeof...(T)>([](
                auto Index,
                const LD::ApplicationQuittingEvent<A...> & quittingEvent,
                LD::Tuple<T...> & executors) noexcept
        {
            LD::Get(LD::Get<Index>(executors))(quittingEvent);
            return true;
        },quittingEvent,this->mExecutors);
        //EventDispatcher(this->Events,quittingEvent);
        return LD::QuittingState{};
    }
    LD::ApplicationState OnEvent(const LD::ApplicationHasStartedState &, const LD::ApplicationQuittingEvent<A...> & quittingEvent)
    {
        //EventDispatcher(this->Events,quittingEvent);
        LD::For<sizeof...(T)>([](
                auto Index,
                const LD::ApplicationQuittingEvent<A...> & quittingEvent,
                LD::Tuple<T...> & executors) noexcept
        {
            LD::Get(LD::Get<Index>(executors))(quittingEvent);
            return true;
        },quittingEvent,this->mExecutors);
        return LD::QuittingState{};
    }
};
int TermBoxApplicationExample(LD::Configuration & config,LD::OpenDHTBackend & dht, LD::AbstractLogger & passedInLogger, LD::TermBoxLogger & passedInTBLogger)
{

    LD::BasicServiceExecutor<void(LD::AbstractLogger&)> coreServices{config,passedInLogger};

    auto logger = coreServices.Make<LD::BasicLogger>();

    bool shouldContinue = true;
    PDP::Second<LD::Float> timeElapsed;
    LD::Second<LD::Float> timeEla = 0_s;

    timeEla*5.0;
    //timeEla.Multiply(5);
    OpenDHTSubscription subscription{dht};

    LD::StaticArray<LD::ImmutableString<125>,25> subscriptionResults;
    LD::BackInserter inserter{subscriptionResults};
    subscription.Subscribe(LD::ImmutableString{"abc"},inserter);




    auto TUIApplicationStarted = [](const LD::ApplicationStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> & applicaitonStartedEvent) noexcept ->bool
    {
        LD::Get<LD::TermBoxRenderContext>(applicaitonStartedEvent)->EnableMouse();

        return LD::ApplicationRunningPredicate{};
    };
    auto TUIApplicationFrameStarted = [&](const LD::ApplicationFrameStartedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> & context ) noexcept -> bool
    {
        LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(context);
        timeEla += context.TimeElapsed();
        timeElapsed += context.GetElapsedTime();


        LD::Float res = timeEla.NativeRepresentation().Value();
        LD::TUI::ImmutableTextLabel cpuLabel(LD::BasicVec2D<LD::Integer>{0,0},LD::ToImmutableString(res));
        renderingContext->Render(cpuLabel);
        return LD::ApplicationQuittingPredicate{ timeEla > 10_s};
    };
    auto TUIApplicationPeriod = [](const LD::ApplicationPeriodEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> &) noexcept -> LD::Second<LD::Float>
    {
        return 1.0_s;
    };

    auto TUIApplicationExecution = [&](const LD::ApplicationExecutionEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> & applicationExecutionEvent ) noexcept
    {

        LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);

        for(LD::UInteger n = 0;n<subscriptionResults.GetSize();++n)
        {
            LD::TUI::ImmutableTextLabel cpuLabel(LD::BasicVec2D<LD::Integer>{-tb_width()/2,(tb_height()/2)-2*n-1},subscriptionResults[n]);
            renderingContext->Render(cpuLabel);
        }


        logger << LD::InfoLog{} << rand() << "\n";
        subscription();
        passedInTBLogger(applicationExecutionEvent);

    };

    auto TUIApplicationFrameEnded = [](const LD::ApplicationFrameEndedEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> &) noexcept
    {

    };

    auto TUIApplicationQuit = [](const LD::ApplicationQuittingEvent<LD::TermBoxRenderContext,LD::Cursor<LD::Integer>> &) noexcept
    {

    };
    auto generatedApp = LD::Overload{TUIApplicationStarted,TUIApplicationFrameStarted,TUIApplicationPeriod,TUIApplicationExecution,TUIApplicationFrameEnded,TUIApplicationQuit};


    //LD::BasicApplication<LD::CT::TypeList<decltype(generatedApp)>(LD::TermBoxRenderContext,LD::Cursor<LD::Integer>)> comboApp{generatedApp};
    MultiApplication<LD::CT::TypeList<decltype(generatedApp)&>(LD::TermBoxRenderContext,LD::Cursor<LD::Integer>)> app{generatedApp};
    LD::BasicApplication<decltype(generatedApp)(LD::TermBoxRenderContext,LD::Cursor<LD::Integer>)> application{generatedApp};
    LD::BasicTermBoxApplication<decltype(generatedApp)(LD::TermBoxRenderContext,LD::Cursor<LD::Integer>)> termBoxApplication{generatedApp};
    LD::BasicApplication<decltype(termBoxApplication)(LD::TermBoxRenderContext,LD::Cursor<LD::Integer>)> application1{termBoxApplication};
    LD::Cursor<LD::Integer> cursor;
    LD::TermBoxRenderContext cntxt;
    LD::Timer currentTimer;
    return LD::ApplicationLoop(application1,currentTimer,cntxt,cursor);
}

template<typename F, typename Integrand>
void Integrate(F && function, LD::Type<Integrand>) noexcept
{

}

#include "Examples/FiniteStateMachineExample.hpp"
int main(int argc, char **argv)
{
    LD::PrintFLogger printfLogger;
    LD::BasicLogger logger{printfLogger};
    LD::Example::DependencyInjectionExample(printfLogger);
    LD::Example::PublisherSubscriber(printfLogger);
    //LD::Example::FiniteStateMachineExample();
    /*
    LD::PrintFLogger printfLogger;
    LD::BasicLogger logger{printfLogger};
    //logger << LD::InfoLog{} << "abc" << 7 << LD::ImmutableString{"cde"} << "\n";
    LD::Second<LD::Float> rawrbunnies = 0_ms;
    LD::Exp(rawrbunnies);
    rawrbunnies+=1_s;
    LD::CircularQueue<LD::UInteger ,15> mReusableKeys;
    LD::BasicChannel channel{mReusableKeys};
    channel << LD::UInteger {};
    LD::UInteger memezsters;
    channel >> memezsters;


    LD::Example::DependencyInjectionExample(printfLogger);

    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}};



    LD::TermBoxLogger termBoxLogger;
     auto j2 = R"(
                {
                "happy": true,
                "pi": 3.141,
                "key": 12.97,
                "object" : {"key": 7 }
                }
            )"_json;
            nlohmann::json mConfiguration{j2};

            LD::JsonConfiguration configuration(LD::Mem::GetNewDeleteResourceReference(),mConfiguration);

            LD::Configuration & config = (Configuration &) configuration;
    //TermBoxApplicationExample(config,mBackend,termBoxLogger,termBoxLogger);

    /*
    LD::StaticArray<LD::ImmutableString<125>,25> subscriptionResults;
    LD::BackInserter inserter{subscriptionResults};
    OpenDHTSubscription subscription{mBackend};
    subscription.Subscribe(LD::ImmutableString{"abc"},inserter);

    LD::Timer timer;
    timer.Start();
    while (timer.GetElapsedTime() < 10)
    {
        subscription();
        sleep(1);
    }
    /*
    TermBoxApplicationExample(mBackend);
    ExampleBasicApplication applicationExecutor;
    LD::BasicApplication<ExampleBasicApplication()> application{applicationExecutor};
    LD::Timer currentTimer;
    LD::ApplicationLoop(application,currentTimer);
    mpark::variant<int,int>{};
    std::string nodeID = "5d3c9c159c9db04b27f2a961cc8f03fef8b18f35";
   // std::cout << base64_encode((unsigned char*)nodeID.data(), nodeID.size()) << "\n";
    std::cout << LD::Base64Encode(LD::ImmutableString{"5d3c9c159c9db04b27f2a961cc8f03fef8b18f35"}).Data() << "\n";
    std::cout << LD::Base64Decode(LD::ImmutableString{"NWQzYzljMTU5YzlkYjA0YjI3ZjJhOTYxY2M4ZjAzZmVmOGIxOGYzNQ=="}).Data() << "\n";


    LD::Ringbuffer<int> buffer;
    LD::BackInserter ringBufferInserter{buffer};

    ringBufferInserter = 7;
    moodycamel::ConcurrentQueue<int> queue;

    LD::Async::RingBuffer<int,5> ringerBuffer;


    LD::BackInserter ringerInserter{buffer};
    ringerInserter = 7;
    LD::BasicChannel basicChannel{buffer};

    basicChannel << 7;


    int rawrBunnie123;

    basicChannel >> rawrBunnie123;

    //LD::Variant<int>{}.get<0>();

    //LD::WebServerExample();

    LD::VisitWithContext(LD::Overload{[](int, std::string){},[](float, std::string){}},LD::MakeTuple(std::string{}),mpark::variant<int,float>{});


    OpenDHTSubscription subscription{mBackend};

    subscription.Subscribe(LD::ImmutableString{"abc"},LD::LazyLambda{[](LD::StringView value){ std::cout << "Subscription Value: " << value << "\n";}});


    subscription();



    //d05a3a0a907bfe16959e1b84ed116bf1e1dbb904

    LD::Channel<LD::ImmutableString<256>> nameChannel;
    //auto token =  mBackend.Subscribe(LD::StringView{"room"},LD::BackInserter<LD::Channel<LD::ImmutableString<256>>>{&nameChannel});

    //LD::Publish(mBackend,LD::ImmutableString{"node101"},7);




    LD::Channel<LD::UInteger> mIntegers;

    //LD::Subscribe123(mBackend,LD::ImmutableString{"node101"},LD::Type<LD::UInteger>{},LD::BackInserter{mIntegers},LD::Async::NewWorker(scheduler));




    LD::Async::SharedState tokenizedState{nameChannel};

    LD::Async::SharedState tokenizedState1{mIntegers};



    auto letSharedState = [&](auto  sharedState) noexcept
    {

        //LD::UInteger number;
        //(*sharedState) >> number;
        //std::cout << "Number: " << number << "\n";

        mBackend.Query(LD::StringView{"room2"},[](auto , auto value)
        {
            std::cout << "Value laawlz: " << value << "\n";
            return 8;
        });


        //mBackend.QueryWithStream(LD::StringView{"room1"},LD::LazyLambda{[](LD::StringView number) noexcept
        //{
        //    std::cout << "room1 stream: " << number << "\n";
        //}});


        mBackend.QueryWithStream(LD::StringView{"room1"},LD::LazyLambda{[](LD::StringView number) noexcept
        {
            std::cout << "room1.1 stream: " << number << "\n";
        }});

        auto response = LD::Fetch(mBackend,LD::ImmutableString{"room1"},LD::Type<LD::UInteger>{});


        //while (!LD::IsTransactionalResponse(response))
        //{
            //response = LD::Fetch(mBackend,LD::ImmutableString{"room1"},LD::Type<LD::UInteger>{});
        //}
        auto onNumber = [](LD::UInteger number) noexcept
        {
            std::cout << "Number2342342: " << number << "\n";
        };


        auto onError = [](LD::TransactionError) noexcept
        {
            std::cout << "error has occured\n";
        };

        LD::InvokeVisitation(LD::Overload{onNumber,onError},response);

        //LD::ImmutableString<256> string;
        //(*sharedState) >> string;

        //std::cout << "Room Value: " << string.Data() << "\n";

        //LD::ImmutableString<256> string;
        //(*sharedState) >> string;

        //std::cout << "Room Value: " << string.Data() << "\n";

        return 8;
    };

    auto letSharedState1 = [](auto  sharedState) noexcept
    {


        LD::UInteger number;
        (*sharedState) >> number;

        std::cout << "Integer Value: " << number << "\n";

        //(*sharedState) >> number;

        std::cout << "Second Integer: " << number << "\n";

        //LD::ImmutableString<256> string;
        //(*sharedState) >> string;

        //std::cout << "Room Value: " << string.Data() << "\n";

        return 8;
    };

    //LD::Async::SyncWait(LD::Async::LetValue(LD::Async::NewWorker(scheduler),tokenizedState1,letSharedState1));
    LD::Async::SyncWait(LD::Async::LetValue(LD::Async::NewWorker(scheduler),tokenizedState1,letSharedState));


    LD::Channel<LD::UInteger> channel;
    LD::BackInserter<LD::Channel<LD::UInteger>> inserter{&channel};
    LD::LazyLambda lambda{[](LD::UInteger index)
    {
         std::cout << "Found DHT Value: " << index << "\n";
    }};

    //LD::Tuple<decltype(lambda)> rawr;
    using LambdaType = decltype(lambda);




    //sleep(5);

    //while (channel.Size() > 0)
    //{
        //LD::UInteger integer;
        //channel >> integer;
        //std::cout << integer << "\n";
    //}
    //printf("Aysnc Execution lol: %li \n", SyncWait<LD::UInteger>(f1));

    //auto f4 = then(f1, [](LD::UInteger i){return i;});
    //printf("Aysnc Execution lol: %s \n", SyncWait<LD::UInteger>(f4));
    /*
    nlohmann::json json123;
    LD::JsonBackend jsonBackend123{json123};





    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"key1"},
            LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});


    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"keyone"},
            LD::MakeTuple(1.9,7,'a'));


    LD::StaticArray<LD::Pyramid,5> memeArray;
    memeArray.PushBack(LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});
    memeArray.PushBack(LD::Pyramid{LD::Square{19},LD::Triangle{79,22}});
    memeArray.PushBack(LD::Pyramid{LD::Square{25},LD::Triangle{19,9}});
    memeArray.PushBack(LD::Pyramid{LD::Square{9},LD::Triangle{12,36}});
    memeArray.PushBack(LD::Pyramid{LD::Square{8},LD::Triangle{7,98}});
    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"staticArray"},
            memeArray);

    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"keytwo"},
            LD::Variant<LD::Square,int>{LD::Square{96}});

    LD::Pyramid testPyramid;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"key1"},
            testPyramid);

    LD::Insert(
            jsonBackend123,
            LD::ImmutableString{"abc"},
            23);
    LD::UInteger memesz;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"abc"},
            memesz);

    std::cout << "ABC Number: " << memesz << std::endl;

    std::cout << json123.dump(2) << std::endl;
    std::cout << testPyramid.Side().Base() << std::endl;

    LD::Tuple<LD::Float,LD::UInteger,char,LD::Square> tupleToDeserialize;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"keyone"},
            tupleToDeserialize);

    std::cout <<
    LD::Get(LD::Get<0>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<1>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<2>(tupleToDeserialize)) <<
    "," <<
    LD::Get(LD::Get<3>(tupleToDeserialize)).Length() <<
    "\n";

    LD::Variant<LD::Square,int> mMemeVariant;
    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"keytwo"},
            mMemeVariant);

    auto onMemeSquare = [](const LD::Square & square) noexcept
    {
        std::cout << "Meme Square: " << square.Length() << "\n";
    };


    LD::MultiMatch(LD::Overload{onMemeSquare,[](auto){}},mMemeVariant);


    //fd00:1700:81b8:401e:0:d9:191:4a34
    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4225},LD::Port{4222}};


    LD::Insert(
            mBackend,
            LD::ImmutableString{"dhttuple"},
            LD::MakeTuple(1.9,7,'a'));

    mBackend.Query(LD::StringView{"room.Side.Height"},[](LD::StringView key, LD::StringView value)->int
    {
        std::cout << "Key : " << key << "     |   Value: " << value << std::endl;
        return 5;
    });

    LD::Insert(mBackend,LD::ImmutableString{"room"},
               LD::Pyramid{LD::Square{8},LD::Triangle{7,105}});





    std::cout << "ClassName: " << LD::CT::GetClassName(LD::Type<int>{}).Data() << std::endl;

    LD::Channel<LD::UInteger> queue;
    auto subscriptionResponse = LD::Subscribe(
             mBackend,
             LD::ImmutableString{"room.Side.Height"},
             LD::Type<LD::UInteger>{});


    auto onSuccessfulSubscription = [&](LD::Channel<LD::UInteger> channel) noexcept
    {
        queue = channel;
    };

    auto onSubscriptionFailure = [](LD::TransactionError) noexcept
    {

    };

    LD::InvokeVisitation(LD::Overload{onSuccessfulSubscription,onSubscriptionFailure},subscriptionResponse);


    LD::SharedLock<LD::Mutex> sharedLock;

   LD::Subscribe(
            mBackend,
            LD::ImmutableString{"memez1"},
            LD::Type<LD::Variant<LD::UInteger,bool>>{},
            [](LD::Variant<LD::UInteger,bool> variant, LD::SharedLock<LD::Mutex> sharedLock)
            {
                auto onInteger = [](LD::UInteger value) noexcept
                {
                    std::cout << "Found UInteger in Variant: " << value << "\n";
                };

                auto onBool = [](bool value) noexcept
                {

                };

                LD::MultiMatch(LD::Overload{onInteger,onBool}, variant);

            },sharedLock);


    LD::Subscribe(
            mBackend,
            LD::ImmutableString{"keyone"},
            LD::Type<LD::Tuple<double,int,char>>{},
            [](LD::Tuple<double,int,char> tuple, LD::SharedLock<LD::Mutex> sharedLock)
            {
                LD::For<3>([](
                        auto I,
                        LD::Tuple<double,int,char> tuple)
                {
                    std::cout << "Found Tuple: " << LD::UInteger(I) << " : " << LD::Get(LD::Get<I>(tuple)) << "\n";
                    return true;
                },tuple);
                printf("found tuple from data set \n");

            },sharedLock);


    LD::UInteger abc = 0;
    LD::Subscribe(mBackend,LD::ImmutableString{"room.Side.Height"},LD::Type<LD::UInteger>{},
    [&](LD::UInteger response, LD::SharedLock<LD::Mutex> sharedLock)
    {
        LD::ScopeLock<LD::SharedLock<LD::Mutex>> scopedLock{sharedLock};

        abc = response;
        std::cout << "the wheels on the bus go round and round all through the town!" << "\n";
    },sharedLock);


    LD::Tuple<int> adsfsadfs;
    queue << 7;

    queue << 7;

    queue << 7;

    queue << 7;
    LD::StaticArray<LD::Pyramid,5> usableMemeArray;

    LD::Fetch(
            jsonBackend123,
            LD::ImmutableString{"staticArray"},
            usableMemeArray);

    std::cout << usableMemeArray[1].Base().Length() << std::endl;
    /*
    LD::HashiVault vault{
        LD::ImmutableString{"http://192.168.30.50:8200"},
        LD::ImmutableString{"s.w8hIFCNwl59CMABCvaZ1qlGV"}};

    auto vaultResponse = vault.Query(
            LD::Type<LD::Square>{},
            LD::ImmutableString{"demoapplications"},
            LD::ImmutableString{"square"});


    auto onVaultSquare = [](const LD::Square & square) noexcept
    {
        std::cout << "Vault Square Length: " << square.Length() << std::endl;
    };

    LD::InvokeVisitation(LD::Overload{onVaultSquare,[](auto){}},vaultResponse);


    nlohmann::json jsonToJsonTest;
    LD::ToJson(jsonToJsonTest,LD::Pyramid{LD::Square{7},LD::Triangle{7,9}});


    std::cout << jsonToJsonTest.dump(2) << std::endl;

    simdjson::dom::parser parser;

    simdjson::dom::element parsedResponse = parser.parse(jsonToJsonTest.dump(2));


    LD::TieredKeyValueStore<LD::RedisBackingStore,LD::CT::TypeList<LD::OpenDHTBackend>> dnsEntries;
    LD::UnQliteBackend<char> sqliteDB{"database1.db",LD::OpenAndCreateIfNotExists{}};
    LD::Timer currentTimer;
    nlohmann::json json;
    LD::JsonBackend jsonBackend{json};
    LD::OpenDHTBackend mBackend{LD::IPV6Address{"fd00:1700:81b8:401e:0:d9:191:4a34"},LD::Port{4222},LD::Port{4222}};
    auto test1 = [&](LD::UnQliteBackend<char> & db, const auto & serializableObject) -> int
    {
        LD::Insert(db,LD::ImmutableString{"key1"},serializableObject);

        mBackend.Query(LD::StringView{"lucario"},[](LD::StringView key, LD::StringView value)->int
        {
            std::cout << "Key : " << key << "     |   Value: " << value << std::endl;
            return 5;
        });

        return 5;
    };
    auto testResult = LD::TDD::Execute(
            LD::ImmutableString{"Insertion Test"},
            currentTimer,
            test1,
            5,
            sqliteDB,
            LD::Pyramid{LD::Square{928},LD::Triangle{65,50}});

    LD::ArtifactRepository repository{LD::ElementReference<LD::JsonBackend>{jsonBackend}};


    LD::Insert(jsonBackend,LD::ImmutableString{"keyone"},LD::MakeTuple(1.9,7,'a',LD::Square{7}));
    repository.Insert(
            testResult,
            testResult,
            testResult
    );
    std::cout << json.dump(2) << std::endl;
    /*
    int abc = 0;
    mBackend.Listen();
    while(abc < 25)
    {

        ++abc;
        sleep(1);
    }
     */
    /*
    // put some data on the dht
    /*
     */
    /*
    LD::Timer timer;
    timer.Start();
    // get data from the dht
    dht::InfoHash id;

    LD::UnQliteBackend<char> unqliteBackingStore{"database.db",LD::OpenAndCreateIfNotExists{}};

    unqliteBackingStore.Query(LD::StringView{"key.Length"},[](auto a, auto b)
    {
        //std::cout << "Key : " << a << " Value:" << b << std::endl;
        return 7;
    });

    LD::Insert(
            unqliteBackingStore,
            LD::ImmutableString{"key1"},
            LD::Pyramid{LD::Square{928},LD::Triangle{65,50}});
    auto fetchResponse = LD::Fetch(unqliteBackingStore,LD::ImmutableString{"key1"},LD::Type<LD::Pyramid>{});



    auto onResponse = [](LD::Pyramid geometry) noexcept
    {
        std::cout << "Geometry length: " << geometry.Base().Length() << std::endl;
        std::cout << "Side Base:" << geometry.Side().Base() << std::endl;
        std::cout << "Side Height:" << geometry.Side().Height() << std::endl;
    };

    auto onError = [](LD::TransactionError ) noexcept
    {

    };
    LD::InvokeVisitation(LD::Overload{onResponse,onError},fetchResponse);
    LD::Example::WebDavExample();
    //LD::Example::IMGUIExample1();
    //LD::Example::IMGUIExample2();
    /*
    ParseResponse("UPID:virtualhome:00004556:065341E5:6020F7FA:vzcreate:110:root@pam:");
    */


    return 0;
}