#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Algorithms/StringAsNumber.h"
#include "Chrono/Timer.h"
#include "Reflection/reflectable.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include <unistd.h>
#include <stdio.h>


class RowBackingStore
{
private:
    FILE * mHandle;
    char * mLineBuffer;
    size_t mLineBufferSize;
public:
    RowBackingStore() noexcept:mLineBuffer{nullptr}
    {
        mHandle = fopen("/proc/stat","r");
    }
    RowBackingStore & Reset() noexcept
    {
        fseeko(this->mHandle,0,SEEK_SET);
        fflush(this->mHandle);
        return (*this);
    }

    ~RowBackingStore() noexcept
    {
        fclose(this->mHandle);
        free(this->mLineBuffer);
    }
    RowBackingStore & Flush() noexcept
    {
        fflush(this->mHandle);
        return (*this);
    }
    template<typename ... Args>
    LD::QueryResult<LD::StringView(Args...)> GetLine(Args && ... arguments) noexcept
    {
        LD::UInteger readAmount = getline(&mLineBuffer,&mLineBufferSize,this->mHandle);
        LD::QueryResult<LD::StringView(Args...)> results[2];
        results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
        results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::StringView{this->mLineBuffer,this->mLineBufferSize},LD::Forward<Args>(arguments)...);
        return results[readAmount > 0];
    }

    template<typename ... Args>
    LD::QueryResult<LD::StringView (Args...)> operator()(const LD::UInteger & index, Args && ... arguments) noexcept
    {
        LD::UInteger n = {};
        do
        {
            if (!getline(&this->mLineBuffer,&this->mLineBufferSize,this->mHandle))
            {
                break;
            }
        }while(n<index);
        LD::QueryResult<LD::StringView(Args...)> results[2];
        results[0] = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
        results[1] = LD::MakeContext(LD::DatabaseTransactionResult{},LD::StringView{this->mLineBuffer,this->mLineBufferSize},LD::Forward<Args>(arguments)...);
        return results[n == index];
    }
};


template<typename BackingStore,typename MembersSeperator, typename ObjectSeperator, class  = void>
class BasicDelimeterSeperatedFile;
template<typename BackingStore,typename MembersSeperator, typename ObjectSeperator>
class BasicDelimeterSeperatedFile<BackingStore,MembersSeperator,ObjectSeperator,LD::Enable_If_T<LD::Require<
        LD::IsTypeString<MembersSeperator>,
        LD::IsTypeString<ObjectSeperator>
        >>>
{
private:

    static constexpr auto MemberPattern = ctll::fixed_string{MembersSeperator{}};
    FILE * mHandle;
    char * mLineBuffer;
    size_t mLineBufferSize;
    LD::ElementReference<BackingStore> mBackingStore;
public:

    template<typename Object>
    class Iterator;
    template<typename ... Reflectables, typename ... Context>
    class Iterator<LD::Variant<Reflectables...>(Context...)>
    {
    private:
        LD::QueryResult<LD::Variant<Reflectables...>(Context...)> mContext;
        LD::UInteger mLineBufferSize;
        LD::ElementReference<BasicDelimeterSeperatedFile> mInstance;
    public:
        constexpr Iterator(const LD::ElementReference<BasicDelimeterSeperatedFile> & instance, Context && ... context) noexcept :mLineBufferSize{0},mInstance{instance}
        {

            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            this->mContext = LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);

            LD::For<sizeof...(Reflectables)>([](auto I)
            {
                using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;

                return true;
            });
        }
        constexpr Iterator(Context && ... context) noexcept :mLineBufferSize{0},mInstance{nullptr}
        {
            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            LD::For<sizeof...(Reflectables)>([](auto I)
            {
                  using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;


                  return true;

            });
        }

        constexpr bool operator != (const Iterator & it) noexcept
        {
            return this->mLineBufferSize != 0 ||it.mInstance == nullptr;
        }
        const LD::QueryResult<LD::Variant<Reflectables...>(Context...)> & operator*() const noexcept
        {
            return this->mContext;
        }

        template<typename ... Args>
        LD::QueryResult<LD::Variant<Reflectables...>(Args...)> operator()(Args && ... arguments) noexcept
        {
            return {};
        }
    };
    BasicDelimeterSeperatedFile(const LD::ElementReference<BackingStore> & backingStore) noexcept :mLineBuffer{nullptr},mBackingStore{backingStore}//, mLineBuffer(nullptr)
    {
        mHandle = fopen("/proc/stat","r");
        //getline(&mLineBuffer,&mLineBufferSize,this->mHandle);



        //printf("%s \n",mLineBuffer);
    }


    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> Begin(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::ElementReference<BasicDelimeterSeperatedFile>{this},LD::Forward<Args>(arguments)...};
    }

    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> End(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        //it just works as a dummy sentinal to stop the possible loop
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::Forward<Args>(arguments)...};
    }


    template<typename ... Reflectables, typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    (LD::IsReflectable<Reflectables> && ...)
                    >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> operator()(const LD::CT::TypeList<Reflectables...> &,const LD::UInteger & index, Args && ... arguments) noexcept
    {
        using ResultSetType = decltype(ctre::range<MemberPattern>(LD::Declval<LD::StringView>()));
        using ResultSetIteratorType = decltype(LD::Declval<ResultSetType>().begin());
        auto cachedOffset = ftello(this->mHandle);
        fseeko(this->mHandle,0,SEEK_SET);
        fflush(this->mHandle);
        //this->mBackingStore->Reset();
        LD::UInteger n = 0;
        LD::QueryResult<LD::StringView()> query;
        /*
        do
        {
            getline(&mLineBuffer,&mLineBufferSize,this->mHandle);
            //querythis->mBackingStore->GetLine();
        }while(n < index);
         */
        this->mBackingStore->Reset();
        auto onError = [](const LD::Context<LD::DatabaseError> & context) noexcept
        {
            //
            return LD::StringView{"",0};
        };

        auto onString = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView> & context)
        {
            //std::cout << LD::Get<1>(context) << std::endl;
            return LD::Get<1>(context);
        };



        //auto input = LD::StringView {mLineBuffer};
        //LD::StringView input = LD::Match(this->mBackingStore->GetLine(),onError,onString);
        //(*this->mBackingStore)(0);
        LD::StringView input = LD::Match((*this->mBackingStore)(index),onError,onString);
        //auto match = ctre::range<MemberPattern>(input);

        LD::QueryResult<LD::Variant<Reflectables...>(Args...)> result;
        result = LD::MakeContext(LD::DatabaseError{},LD::Forward<Args>(arguments)...);
        using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
        //iterate through all the types we care about at compile time
        auto onClassReanimate = [](const LD::Context<LD::StringView,LD::StringView> & context) noexcept -> bool
        {
            return true;
        };
        LD::For<sizeof...(Reflectables)>([](
                auto I,
                auto && input,
                auto && onClassReanimate,
                auto && result,
                Args && ... arguments)
        {

            using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<CurrentType ,LD::AccessWriteOnly>,LD::Variant>;


            //get the types which compose the hiarchacy of the current Type we're looking at.
            //using ExpandedMembers = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),CurrentType>;

            //LD::StaticArray<int,ExpandedMembers::size()/2> membersInStringForm;
            auto currentView = input;

            LD::UInteger offset = 0;

            auto && classNameSearch = ctre::range<MemberPattern>(currentView);

            bool keepSearching = true;
             //we found the type associated with the current row
            if (classNameSearch.begin() != classNameSearch.end())
            {
                keepSearching = false;
                using CurrentClassName = decltype(CurrentType::GetClassNameTypeString());

                auto  classNameMatch = classNameSearch.begin().current_match;

                //LD::UInteger indexOfIncidence = (classNameMatch.template get<0>().begin()-input.data());
                offset = classNameMatch.view().size();
                //we found the class we care about
                auto foundClassName = LD::StringView{(const char*)classNameMatch.view().data(),classNameMatch.view().size()};
                if(foundClassName == LD::StringView{CurrentClassName::data(),CurrentClassName::size()})
                {
                    //iterate through the members to deserialize
                    auto matchSet = ctre::range<MemberPattern>(LD::StringView {input.data()+offset});
                    auto regexIterator = matchSet.begin();
                    auto onMemberReanimate = [&](const LD::ContextualVariant<Var(LD::StringView)> & context) noexcept
                    {
                        auto onPrimitiveAction = [&](auto && context) noexcept
                        {
                            using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                            if (regexIterator != matchSet.end())
                            {
                                if constexpr(LD::IsPrimitive<MemberType>)
                                {

                                    //it's a handle to the instance variable itself
                                    LD::ElementReference<MemberType> memberReference = LD::Get<0>(context);
                                    //turn the string view into a primtive (hopefully!) if not just return the default value of the primitive type (which is usually 0)
                                    auto resultVariant = LD::StringAsNumber<MemberType>(LD::StringView{(const char *)regexIterator.current_match.view().data(),regexIterator.current_match.view().size()});
                                    MemberType result = LD::Match(resultVariant,[](const MemberType & obj){ return obj;},[](auto &&){ return MemberType {};});
                                    LD::Get(memberReference) = result;
                                    //std::cout << "Member : " << regexIterator.current_match.view() << std::endl;

                                }
                                ++regexIterator;

                            }
                        };

                        LD::Match(context,onPrimitiveAction);
                        //using MemberType = LD::Detail::Decay_T<decltype(LD::Get(LD::Get<0>(context)))>;
                        //does our current model have more members that are actually in the file? probably shouldn't try to deserialize what's not there
                        return true;
                    };

                    //walk through the object's hiarchachy, the following function will call the callbacks on events like finding a new class or member
                    CurrentType typeToDeserialize;
                    //we're writing to the object, use the write only transform optimizations
                    LD::CT::ReflectiveWalk(""_ts,typeToDeserialize,onClassReanimate,onMemberReanimate,LD::AccessWriteOnly{});

                    result = LD::MakeContext(LD::DatabaseTransactionResult{},CurrentType{typeToDeserialize},LD::Forward<Args>(arguments)...);

                }
            }
             return keepSearching;
        },input,onClassReanimate,result,LD::Forward<Args>(arguments)...);
        fseeko(this->mHandle,cachedOffset,SEEK_SET);
        return result;
    }



    ~BasicDelimeterSeperatedFile() noexcept
    {
        fclose(mHandle);
        free(this->mLineBuffer);
    }
};

namespace LD
{
    namespace Detail
    {
        template<typename T>
        struct GenerateSystemLoadRepresentation
        {

        };

        template<LD::UInteger ... Indices>
        struct GenerateSystemLoadRepresentation<LD::IntegerSequence<LD::UInteger ,Indices...>>
        {
            using type = LD::Variant<LD::CPUPackageMetric,LD::ContextSwitchMetric,LD::InterruptServiceMetric,LD::BootUpTimeMetric,LD::ProcessesMetric,LD::ProcessesRunningMetric,LD::ProcessesBlockedMetric,LD::SoftIRQ,LD::CPUCoreMetric<Indices>...>;
        };
    }
    template<LD::UInteger NumberOfCores>
    using GenerateSystemRepresentation = typename LD::Detail::GenerateSystemLoadRepresentation<LD::MakeIndexSequence_T<10>>::type;
}
template<typename BackingStore>
using CommaSeperateValueFile = BasicDelimeterSeperatedFile<BackingStore,decltype("[^,]+"_ts),decltype("[^\n]"_ts)>;
template<typename BackingStore>
using SpaceSpeerateValueFile = BasicDelimeterSeperatedFile<BackingStore,decltype("[^ ]+"_ts),decltype("[^\n]"_ts)>;
int main()
{


    using Type = LD::GenerateSystemRepresentation<10>;
    //LD::GenerateSystemRepresentation<8>{};
    using stuffings = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),LD::Pyramid>;

    LD::CPUCoreMetric<7>::GetClassNameTypeString();


    //LD::CT::DebugTemplate<stuffings>{};

    /*
    auto input = LD::StringView {"123,456,768"};
    static constexpr auto pattern = ctll::fixed_string{"[0-9]+"};
    auto match = ctre::range<pattern>(input);

    match.begin().current_match.get_end_position();

    for (auto match: ctre::range<pattern>(input))
    {
        //LD::StringView abc = LD::StringView {match.get<0>()};
        auto currentMatch = match.get<0>();


        currentMatch.get_end();

        //std::cout <<  LD::StringView {(const char*)currentMatch.view().data(),currentMatch.view().size()} << "\n";
        //auto abc = LD::StringView{(const char*)match.get<0>()};
        const char * indexOf = (input.data()+(currentMatch.begin()-input.data()));
        auto abc = LD::StringView{indexOf,currentMatch.size()};
        std::cout << abc << "\n";
    }

     */


    RowBackingStore backingStore;
    SpaceSpeerateValueFile<RowBackingStore> file{backingStore};
    LD::UInteger previous_idle_time=0, previous_total_time=0;
    for (int i = 0; i < 10; ++i)
    {

        LD::QueryResult<LD::Variant<LD::CPUPackageMetric>()> queryResult =  file(LD::CT::TypeList<LD::CPUPackageMetric>{},0);

        auto onError = [](const LD::Context<LD::DatabaseError> & context) noexcept
        {

        };


        auto onCPUPackageRecord = [&](const LD::Context<LD::DatabaseTransactionResult,LD::CPUPackageMetric> & context) noexcept
        {


            const LD::CPUPackageMetric & metric = LD::Get<1>(context);
            LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

            const LD::UInteger idle_time = metric.Idle();

            const double idle_time_delta = idle_time - previous_idle_time;
            const double total_time_delta = total_time - previous_total_time;
            const double utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
            std::cout << "Utilizaiton : " <<  utilization << std::endl;
            LD::Float idlePercentage = (metric.Idle()*100)/total_time;
            //std::cout << "Usage: " << ((sum-metric.Idle())*100.0)/sum << std::endl;

            previous_idle_time = idle_time;
            previous_total_time = total_time;
        };

        LD::Match(queryResult,onError,onCPUPackageRecord);
        sleep(1);
    }

    //auto bar = LD::ImmutableString<9>{'_'};
    //auto edge = LD::ToImmutableString("abc");
    //std::cout << (bar+edge).Data() << std::endl;
    //std::cout << edge.Data() << std::endl;
    //std::cout << bar.GetSize() << std::endl;
    //std::cout << edge.GetSize() << std::endl;
    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();

    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();

    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}