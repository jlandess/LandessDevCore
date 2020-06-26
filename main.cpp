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
#include "Algorithms/FuzzySearch.h"
#include "TypeTraits/IsSame.hpp"
#include <unistd.h>
#include <stdio.h>


class RowBackingStore
{
private:
    FILE * mHandle;
    char * mLineBuffer;
    size_t mLineBufferSize;
    off64_t mCachedHandle;
public:
    RowBackingStore(const LD::StringView & fileToOpen) noexcept:mLineBuffer{nullptr}
    {
        mHandle = fopen(fileToOpen.data(),"r");
        this->mCachedHandle = ftello(this->mHandle);
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
    template<typename ... Args>
    LD::QueryResult<bool(Args...)> Begin(Args && ... arguments) noexcept
    {
        return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
    }
    template<typename ... Args>
    LD::QueryResult<bool(Args...)> Commit(Args && ... arguments) noexcept
    {
        return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
    }

    template<typename ... Args>
    LD::QueryResult<bool(Args...)> RollBack(Args && ... arguments) noexcept
    {
        fflush(this->mHandle);
        return LD::MakeContext(LD::DatabaseTransactionResult{},bool{true},LD::Forward<Args>(arguments)...);
    }
    RowBackingStore & Flush() noexcept
    {
        fflush(this->mHandle);
        return (*this);
    }
    template<typename ... Args>
    LD::QueryResult<LD::StringView(Args...)> GetLine(Args && ... arguments) noexcept
    {
        //this looks like a problem, getline will realloc and alloc on demand.  Trust the getline implementation
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
        //there's no formatting, or meta data to let us know which byte offsets the rows are at, just read them one at a time until we get to the
        //indicated number, or indicate we got a snag along the way and indicate that with the return of LD::QueryResult
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
    LD::ElementReference<BackingStore> mBackingStore;
public:

    template<typename Object>
    class Iterator;
    template<typename ... Reflectables, typename ... Context>
    class Iterator<LD::Variant<Reflectables...>(Context...)>
    {
    private:
        LD::QueryResult<LD::Variant<Reflectables...>(Context...)> mContext;
        LD::ElementReference<BasicDelimeterSeperatedFile> mInstance;

        static LD::QueryResult<LD::Variant<Reflectables...>(Context...)> OnRowError(const LD::Tuple<LD::DatabaseError,Context...> & context) noexcept
        {
            return {};
        }
        template<typename T>
        static LD::QueryResult<LD::Variant<Reflectables...>(Context...)> OnRowCompletion(const LD::Tuple<LD::DatabaseTransactionResult,T,Context...> & context) noexcept
        {
            return {};
        }
    public:
        Iterator(const LD::ElementReference<BasicDelimeterSeperatedFile> & instance, Context && ... context) noexcept :mInstance{instance}
        {

            using CurrentTypeList = LD::CT::TypeList<Reflectables...>;
            this->mContext = LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);
            LD::Context<Context...> passableContext = LD::MakeContext(LD::Forward<Context>(context)...);
            LD::QueryResult<LD::StringView(LD::Context<Context...>&)> query = this->mInstance->mBackingStore->GetLine(passableContext);


            auto onError = [](const LD::Context<LD::DatabaseError,LD::Context<Context...>&> & context) noexcept
            {
                //return an empty row to indicate something bad happened
                return LD::StringView{"",0};
                //return LD::QueryResult<LD::Variant<Reflectables...>(Context...)>{LD::DatabaseError{},LD::Get(LD::Get<1>(context))};
            };


            auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView,LD::Context<Context...>&> & context) noexcept
            {
                return LD::StringView {LD::Get(LD::Get<1>(context))};
            };


            //this->mContext = LD::Match(query,onError,onRow);


            LD::StringView row = LD::Match(query,onError,onRow);
            if (!row.empty())
            {
                this->mContext = RowToObject(LD::CT::TypeList<Reflectables...>{},row,LD::Forward<Context>(context)...);
            }

        }
        constexpr Iterator(Context && ... context) noexcept :mInstance{nullptr}
        {
            this->mContext = LD::MakeContext(LD::DatabaseError{},LD::Forward<Context>(context)...);
        }

        Iterator & operator ++ () noexcept
        {
            return (*this);
        }

        Iterator & operator ++ (int) noexcept
        {
            return (*this);
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
    BasicDelimeterSeperatedFile(const LD::ElementReference<BackingStore> & backingStore) noexcept :mBackingStore{backingStore}//, mLineBuffer(nullptr)
    {

    }


    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> Begin(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        //we're starting at the beginning so we should probably reflect that in the backing store
        this->mBackingStore->Reset();
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::ElementReference<BasicDelimeterSeperatedFile>{this},LD::Forward<Args>(arguments)...};
    }

    template<typename ... Reflectables, typename ... Args>
    Iterator<LD::Variant<Reflectables...>(Args...)> End(const LD::CT::TypeList<Reflectables...> &,Args && ... arguments) noexcept
    {
        //it just works as a dummy sentinal to stop the possible loop
        return Iterator<LD::Variant<Reflectables...>(Args...)>{LD::Forward<Args>(arguments)...};
    }


    template<typename ... Reflectables, typename ... Args>
    static LD::Enable_If_T<
    LD::Require<
    (LD::IsReflectable<Reflectables> && ...)
    >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> RowToObject(const LD::CT::TypeList<Reflectables...> & tl,const LD::StringView  & row,Args && ... arguments) noexcept
    {
        using ResultSetType = decltype(ctre::range<MemberPattern>(LD::Declval<LD::StringView>()));
        using ResultSetIteratorType = decltype(LD::Declval<ResultSetType>().begin());

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
                auto && row,
                auto && onClassReanimate,
                auto && result,
                Args && ... arguments)
        {


            using CurrentType = LD::CT::TypeAtIndex<I,CurrentTypeList>;
            using Var = LD::CT::RebindList<LD::CT::ReflectiveTransformation<CurrentType ,LD::AccessWriteOnly>,LD::Variant>;


            //get the types which compose the hiarchacy of the current Type we're looking at.
            //using ExpandedMembers = LD::CT::GenerateNamedReflectiveTypeStructure<decltype(""_ts),CurrentType>;

            //LD::StaticArray<int,ExpandedMembers::size()/2> membersInStringForm;
            auto currentView = row;

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
                    auto matchSet = ctre::range<MemberPattern>(LD::StringView {row.data()+offset});
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
            },row,onClassReanimate,result,LD::Forward<Args>(arguments)...);

        return result;
    }
    template<typename ... Reflectables, typename ... Args>
    LD::Enable_If_T<
            LD::Require<
                    (LD::IsReflectable<Reflectables> && ...)
                    >,LD::QueryResult<LD::Variant<Reflectables...>(Args...)>> operator()(const LD::CT::TypeList<Reflectables...> & tl,const LD::UInteger & index, Args && ... arguments) noexcept
    {
        //auto cachedOffset = ftello(this->mHandle);
        //fseeko(this->mHandle,0,SEEK_SET);
        //fflush(this->mHandle);
        //this->mBackingStore->Reset();
        //LD::QueryResult<LD::StringView()> query;

        this->mBackingStore->Reset();
        auto onError = [](const LD::Context<LD::DatabaseError> & context) noexcept
        {
            //
            return LD::StringView{"",0};
        };

        auto onRow = [](const LD::Context<LD::DatabaseTransactionResult,LD::StringView> & context)
        {
            LD::StringView row = LD::Get<1>(context);
            return LD::Get<1>(context);
        };

        //get a specific row from the underlying BackingStore
        LD::StringView row = LD::Match((*this->mBackingStore)(index),onError,onRow);
        if (!row.empty())
        {
            //cool the row isn't empty, get the object
            return RowToObject(tl,row,LD::Forward<Args>(arguments)...);
        }
        //by default it will return an error
        return {};
        //auto match = ctre::range<MemberPattern>(input);

        /*
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
        //fseeko(this->mHandle,cachedOffset,SEEK_SET);
        return result;
         */
    }



    ~BasicDelimeterSeperatedFile() noexcept
    {
        //fclose(mHandle);
        //free(this->mLineBuffer);
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


    RowBackingStore backingStore{"/proc/stat"};
    SpaceSpeerateValueFile<RowBackingStore> file{backingStore};
    LD::UInteger previous_idle_time=0, previous_total_time=0;
    SpaceSpeerateValueFile<RowBackingStore>::Iterator<LD::Variant<LD::CPUPackageMetric>(LD::UInteger&,LD::UInteger&)> queryResult = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{},previous_idle_time,previous_total_time);


    auto onError = [](const LD::Context<LD::DatabaseError,LD::UInteger&,LD::UInteger&> & context) noexcept
    {

    };

    auto onCPUPackageMetric = [](const LD::Context<LD::DatabaseTransactionResult,LD::CPUPackageMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
    {
        LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(context));
        LD::UInteger previous_idle_time = LD::Get(LD::Get<2>(context));
        LD::UInteger previous_total_time = LD::Get(LD::Get<3>(context));
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

    LD::Match(*queryResult,onError,onCPUPackageMetric);
    //file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{});


    /*
    file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{});

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
     */


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