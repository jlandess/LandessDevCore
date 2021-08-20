//
// Created by phoenixflower on 1/3/21.
//

#ifndef LANDESSDEVCORE_MIRTA_HPP
#define LANDESSDEVCORE_MIRTA_HPP
#include "Primitives/General/Immutable.hpp"
#include "REST/CPR/cpr.h"
#include "PBX/Mirta/Date.hpp"
#include "Primitives/General/DateTime.h"
#include "IO/EphemiralRowBackingStore.hpp"
#include "Functor/FunctionView.h"
#include "VirtuWorks/QueueLog.hpp"
#include "IO/CSVParser.hpp"
namespace LD
{
    namespace PBX
    {
        template<typename KeyType,typename TennantType>
        class Mirta
        {
        private:
            KeyType mAPIKey;
            TennantType mTennant;
        public:
            Mirta(KeyType && apiKey, TennantType && tennant) noexcept:mAPIKey{apiKey},mTennant{tennant}
            {

            }
            LD::EphemiralRowBackingStore FetchQueueInfo(LD::BasicDate<LD::Year<LD::UInteger>,LD::Month<LD::UInteger>,LD::Day<LD::UInteger>> mStart, LD::PBX::MT::Date mEnd) const noexcept
            {

                std::cout << "Date Start: " << LD::ToImmutableString(mStart).Data() << std::endl;
                cpr::Response r = cpr::Get(cpr::Url{"https://voip.virtuworks.net/pbx/proxyapi.php"},
                                           cpr::Parameters
                                                   {{"key", this->mAPIKey.Data()},
                                                    {"reqtype", "INFO"},
                                                    {"info","QUEUELOGS"},
                                                    {"format","csv"},
                                                    {"tenant",this->mTennant.Data()},
                                                    {"start",LD::ToImmutableString(mStart).Data()},//2020-11-17 LD::ToImmutableString(mStart).Data()
                                                    {"end",LD::ToImmutableString(mStart+LD::Day<LD::UInteger>{1ul}).Data()}});//2020-11-18 LD::ToImmutableString(mEnd).Data()


                 std::cout << r.text << std::endl;



                return LD::EphemiralRowBackingStore{std::make_unique<std::string>(r.text)};
            }

            template<typename F, typename ... A>
            void MissedCalls(LD::BasicDate<LD::Year<LD::UInteger>,LD::Month<LD::UInteger>,LD::Day<LD::UInteger>> date, F && callback, A && ... args) const noexcept
            {
                using FunctionViewType = LD::FunctionView<void(const LD::VW::QueueLog &, A && ... args)>;
                FunctionViewType functionView{LD::Forward<F>(callback)};
                LD::EphemiralRowBackingStore rows = this->FetchQueueInfo(date,++date);
                LD::CSVParser csvParser{rows,LD::CT::TypeList<LD::VW::QueueLog>{}};
                for(auto beingOfCSV = csvParser.Begin(FunctionViewType{functionView});beingOfCSV!=csvParser.End(FunctionViewType{functionView});++beingOfCSV)
                {
                    auto object = (*beingOfCSV);

                    auto onError = [](
                            const LD::Context<LD::TransactionError,FunctionViewType> & context
                    ) noexcept
                    {};


                    auto onLog = [&](
                            const LD::Context<LD::TransactionResult,LD::VW::QueueLog,FunctionViewType> & context
                    ) noexcept
                    {
                        const LD::VW::QueueLog & log = LD::Get(LD::Get<1>(context));
                        FunctionViewType functor = LD::Get(LD::Get<2>(context));
                        //std::cout << "being called" << std::endl;
                        if (log.Status() == LD::VW::Abandoned || log.Status() == LD::VW::Timeout)
                        {
                            functor(log,LD::Forward<A>(args)...);
                        }
                    };
                    LD::MultiMatch(LD::Overload{onLog,onError},object);
                }
            }
        };
        template<typename KeyType, typename TennantType> Mirta(KeyType &&, TennantType &&) -> Mirta<KeyType,TennantType>;
    }
}
#endif //LANDESSDEVCORE_MIRTA_HPP
