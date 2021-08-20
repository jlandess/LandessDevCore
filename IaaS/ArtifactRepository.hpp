//
// Created by phoenixflower on 8/1/21.
//

#ifndef LANDESSDEVCORE_ARTIFACTREPOSITORY_HPP
#define LANDESSDEVCORE_ARTIFACTREPOSITORY_HPP
#include "Algorithms/CompileTimeControlFlow.hpp"
#include "Memory/ElementReference.h"
#include "IO/Database1.h"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    template<typename RepositoryType>
    class ArtifactRepository
    {
    private:
        LD::ElementReference<RepositoryType> mRepository;
    public:
        ArtifactRepository(LD::ElementReference<RepositoryType> repository) noexcept:mRepository{repository}
        {

        }
        template<typename A,LD::UInteger N, LD::UInteger M>
        ArtifactRepository & Insert(LD::ImmutableString<N> testName,LD::TDD::Result<A,LD::ImmutableString<M>> result) noexcept
        {
            //LD::Fetch(LD::)
            auto dateTimeKey = LD::ToImmutableString(LD::DateTime{});
            auto key = testName + LD::ImmutableString{" "} + dateTimeKey;
            LD::Insert(LD::Get(this->mRepository),key,result,[](){return true;});
            return (*this);
        }

        template<typename ... Pairs,
                bool PairsHaveImmutableTestNameAsKey = (LD::CT::IsImmutableString(LD::CT::RemoveQualifiers(LD::CT::Key(LD::CT::RemoveQualifiers(LD::Type<Pairs>{})))) && ...),
                bool PairsHaveResultAsValue = (LD::TDD::CT::IsResult(LD::CT::RemoveQualifiers(LD::CT::Value(LD::CT::RemoveQualifiers(LD::Type<Pairs>{})))) && ...)>
        LD::Enable_If_T<LD::Require<PairsHaveImmutableTestNameAsKey && PairsHaveResultAsValue>,ArtifactRepository> & Insert(Pairs && ... kv) noexcept
        {
            auto context = LD::MakeContext(LD::Forward<Pairs>(kv)...);
            auto dateTimeKey = LD::ToImmutableString(LD::DateTime{});

            LD::For<sizeof...(Pairs)>([](
                    auto I,
                    const auto date ,
                    LD::ElementReference<RepositoryType> repo,
                    const auto & context) noexcept
                                      {
                                          const auto & kvPair = LD::Get(LD::Get<I>(context));
                                          auto key = kvPair.GetFirst() + LD::ImmutableString{" "} + date;
                                          LD::Insert(LD::Get(repo),key,LD::Get(kvPair.GetSecond()),[](){return true;});
                                          return true;
                                      },dateTimeKey,this->mRepository,context);
            return (*this);
        }
    };
}
#endif //LANDESSDEVCORE_ARTIFACTREPOSITORY_HPP
