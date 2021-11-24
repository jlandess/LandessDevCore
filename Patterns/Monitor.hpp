//
// Created by phoenixflower on 11/19/21.
//

#ifndef LANDESSDEVCORE_MONITOR_HPP
#define LANDESSDEVCORE_MONITOR_HPP
#include "IO/DataLink.hpp"
#include "Memory/ElementReference.h"
#include "Primitives/General/StringView.hpp"
#include "Memory/UniquePointer.h"
#include "Memory/shared_ptr.hpp"
#include "Memory/MemoryResource.hpp"
#include "Functor/LightWeightDelegate.h"
#include "Primitives/General/StaticArray.hpp"
namespace LD
{
    class Monitor
    {
    public:
        virtual LD::Optional<LD::BasicStringView<unsigned char>> operator()() noexcept = 0;
    };
    template<typename T>
    class BasicMonitor;



    template<typename T>
    class BasicMonitor: public LD::Monitor
    {
    private:
        T  & mObject;
        LD::Optional<T> mCache;
        LD::StaticArray<unsigned char,16+64> mBuffer;
    public:
        Monitor(T & object) noexcept:mObject{object}
        {

        }

        virtual LD::Optional<LD::BasicStringView<unsigned char>> operator()() noexcept
        {

            return LD::Optional<LD::BasicStringView<unsigned char>>{LD::BasicStringView<unsigned char>{this->mBuffer.Data(),this->mBuffer.GetSize()}};
        }
    };

    class MonitorRegistry
    {
    private:

    public:

        virtual void operator()() noexcept = 0;
        virtual LD::Optional<LD::UniquePointer<Monitor,LD::MemoryResourceDeleter>> RegisterUniquely(LD::StringView name, LD::LightWeightDelegate<LD::BasicStringView<unsigned char>()>, LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept= 0;
        virtual LD::Optional<LD::SharedPointer<Monitor>> RegisterShared(LD::StringView name, LD::LightWeightDelegate<LD::BasicStringView<unsigned char>()>, LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept= 0;

        virtual LD::Monitor & Register(LD::StringView name,LD::SharedPointer<Monitor>) noexcept = 0;
        virtual LD::Monitor & Register(LD::StringView name,LD::UniquePointer<Monitor,LD::MemoryResourceDeleter>, LD::Mem::MemoryResource * resource = LD::Mem::GetNewDeleteResource()) noexcept = 0;
    };

    template<typename Storage>
    class BasicMonitorRegistry
    {
    private:
        LD::DataLink & mDataLink;
        LD::Mem::MemoryResource & mMemoryResource;
        Storage mStorage;
        LD::MonitorRegistry & mMonitoryRegistry;
        LD::StaticArray<LD::UniquePointer<Monitor,LD::MemoryResourceDeleter>,12> mMonitors;
    public:
        BasicMonitorRegistry(LD::DataLink & link,LD::MonitorRegistry & registry,LD::Mem::MemoryResource & mem = LD::Mem::GetNewDeleteResourceReference()) noexcept:mDataLink{link},mMemoryResource{mem},mMonitoryRegistry{registry}
        {

        }

        template<typename T>
        virtual LD::Monitor & Register(LD::StringView name,LD::SharedPointer<Monitor>) noexcept
        {
            LD::MakeUnique<LD::Monitor>(LD::BasicMonitor<T>{object});
        }
        virtual void operator()() noexcept
        {

        }

    };
}
#endif //LANDESSDEVCORE_MONITOR_HPP
