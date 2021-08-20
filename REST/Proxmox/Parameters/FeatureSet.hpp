//
// Created by phoenixflower on 1/27/21.
//

#ifndef LANDESSDEVCORE_FEATURESET_HPP
#define LANDESSDEVCORE_FEATURESET_HPP
#include "Fuse.hpp"
#include "KeyCTL.hpp"
#include "MkNode.hpp"
#include "ForceSWSys.hpp"
#include "Nesting.hpp"
#include "TypeTraits/IsUnique.hpp"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    namespace PVE
    {
        class FeatureSet
        {
        private:
            Nesting mNesting;
            MkNode mMkNode;
            KeyCTL mKeyCTL;
            Fuse mFuse;
            ForceSWSys mSys;
        public:
            FeatureSet() noexcept:mNesting{false},mMkNode{false},mKeyCTL{false},mFuse{false},mSys{false}
            {

            }

            template<typename ... A,
                    typename TL = LD::CT::TypeList<A...>,
                    typename = LD::Enable_If_T<
                    LD::Require<
                            LD::CT::IsUnique<A...>,
                            (LD::CT::GetTypeCountInTypeList<LD::Detail::Decay_T<A>,LD::CT::TypeList<LD::PVE::Nesting,LD::PVE::MkNode,LD::PVE::KeyCTL,LD::PVE::Fuse,LD::PVE::ForceSWSys>>  && ...)
                            >>>
            FeatureSet(A && ... objects) noexcept
            {
                auto featureSet = LD::MakeTuple(LD::Forward<A>(objects)...);
                LD::For<sizeof...(A)>([](
                        auto I,
                        FeatureSet * instance,
                        const auto & features)
                {
                    using CurrentType = LD::CT::TypeAtIndex<I,TL>;
                    if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<LD::PVE::Nesting>{},LD::Type<CurrentType>{}))
                    {
                        instance->mNesting = LD::PVE::Nesting{LD::Get(LD::Get<I>(features).Status())};
                    }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<LD::PVE::MkNode>{},LD::Type<CurrentType>{}))
                    {
                        instance->mMkNode = LD::PVE::MkNode{LD::Get(LD::Get<I>(features).Status())};
                    }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<LD::PVE::KeyCTL>{},LD::Type<CurrentType>{}))
                    {
                        instance->mKeyCTL = LD::PVE::KeyCTL{LD::Get(LD::Get<I>(features).Status())};
                    }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<LD::PVE::Fuse>{},LD::Type<CurrentType>{}))
                    {
                        instance->mFuse = LD::PVE::Fuse{LD::Get(LD::Get<I>(features).Status())};
                    }else if constexpr (LD::CT::IsSameWithoutQualifiers(LD::Type<LD::PVE::ForceSWSys>{},LD::Type<CurrentType>{}))
                    {
                        instance->mSys = LD::PVE::ForceSWSys{LD::Get(LD::Get<I>(features).Status())};
                    }
                    return true;
                },this,featureSet);
            }

            auto Key() const noexcept
            {
                return LD::ImmutableString{"features"};
            }
            auto Value() const noexcept
            {
                auto forceLabel = LD::ImmutableString{"force_rw_sys="};
                auto fuseLabel = LD::ImmutableString{"fuse="};
                auto keyCTLLabel = LD::ImmutableString{"keyctl="};
                auto nestingLabel = LD::ImmutableString{"nesting="};
                auto mkNodeLabel = LD::ImmutableString{"mknod="};
                auto sysStatus = LD::ToImmutableString(LD::UInteger(this->mSys.Status()));
                auto fuseValue = LD::ToImmutableString(LD::UInteger(this->mFuse.Status()));
                auto keyCTLValue = LD::ToImmutableString(LD::UInteger(this->mKeyCTL.Status()));
                auto nestingValue = LD::ToImmutableString(LD::UInteger(this->mNesting.Status()));
                auto mkNodeValue = LD::ToImmutableString(LD::UInteger(this->mMkNode.Status()));
                auto comma = LD::ImmutableString{","};
                return forceLabel+sysStatus+comma+fuseLabel+fuseValue+comma+keyCTLLabel+keyCTLValue+comma+nestingLabel+nestingValue+comma+mkNodeLabel+mkNodeValue;
            }
        };
    }
}
#endif //LANDESSDEVCORE_FEATURESET_HPP
