//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_CHANNELACCESSOR_H
#define LANDESSDEVCORE_CHANNELACCESSOR_H
#include "Primitives/General/Span.hpp"
#include "Definitions/Integer.hpp"
namespace LD
{

    class ChannelIndex
    {
    private:
        PDP::UInteger CurrentChannelIndex;
    public:

        inline ChannelIndex(const PDP::UInteger & index);
        inline const PDP::UInteger & GetChannelIndex() const;
        inline void SetChannelIndex(const PDP::UInteger & index);
    };


    ChannelIndex::ChannelIndex(const PDP::UInteger & index):CurrentChannelIndex(index)
    {

    }

    const PDP::UInteger & ChannelIndex::GetChannelIndex() const
    {
        return this->CurrentChannelIndex;
    }

    void ChannelIndex::SetChannelIndex(const PDP::UInteger & index)
    {
        this->CurrentChannelIndex = index;
    }

    template<typename  ComponentType,PDP::UInteger AmountOfComponents>
    class ChannelAccessor
    {
    private:
        LD::Span<ComponentType> Buffer;
        PDP::UInteger Stride;
        PDP::UInteger Start;
        PDP::UInteger Size;
    public:

        //inline ChannelAccessor(ComponentType * buffer,const PDP::UInteger & start, const PDP::UInteger & stride, const PDP::UInteger & size);

        inline ChannelAccessor(const LD::Span<ComponentType> & buffer,
                const PDP::UInteger & start,
                const ChannelIndex & channel,
                const PDP::UInteger & size);
        //inline ChannelAccessor(ComponentType * buffer,const PDP::UInteger & start, const ChannelIndex & channel, const PDP::UInteger & size);
        //inline ChannelAccessor(ComponentType * buffer, const ChannelIndex & channel, const PDP::UInteger & size);
        //inline ChannelAccessor(ComponentType * buffer,const PDP::UInteger & start, const PDP::UInteger & size);
        //inline ChannelAccessor(ComponentType * buffer, const PDP::UInteger & size);
        inline void SetChannel(const ChannelIndex & channel);
        //template<typename T,typename Alloc>
        //inline ChannelAccessor(const PDP::DataStructures::Array<T,Alloc> & array, const PDP::UInteger & start, const PDP::ChannelIndex & channel);

        //template<typename T,typename Alloc>
        //inline ChannelAccessor(const PDP::DataStructures::Array<T,Alloc> & array, const PDP::ChannelIndex & channel);


        //template<typename T, PDP::UInteger Amount>
        //inline ChannelAccessor(const PDP::StaticArray<T,Amount> & array, const PDP::UInteger & start, const PDP::ChannelIndex & channel);


        //template<typename T, PDP::UInteger Amount>
        //inline ChannelAccessor(const PDP::StaticArray<T,Amount> & array, const PDP::ChannelIndex & channel);

        inline ComponentType & operator[](const PDP::UInteger & index);


        inline const ComponentType & operator[](const PDP::UInteger & index) const;

        ComponentType & operator* ();
        const ComponentType& operator* () const;

        ChannelAccessor & operator ++();
        ChannelAccessor & operator ++(int);

        inline const PDP::UInteger & GetSize() const {return this->Buffer.Size();}
        //inline const PDP::UInteger GetSize() {return this->Size;}

    };

    /*
    template<typename ComponentType,PDP::UInteger AmountOfChannels, template<typename> class Functor = PDP::LightWeightDelegate>
    class DelegatedChannelAccessor
    {
    private:


    public:
    };
     */


    /*
    template<PDP::UInteger AmountOfChannels,typename ComponentType, template<typename> class Functor, typename ... Args>
    class DelegatedChannelAccessor<ComponentType(Args...), AmountOfChannels, Functor>
    {
    private:
        //Functor<ComponentType(const PDP::ChannelIndex &,const PDP::UInteger & index,Args...)> Delegate;
        LD::ChannelIndex CurrentChannel;
    public:


        inline void SetChannel(const LD::ChannelIndex & channel);
        inline const LD::ChannelIndex & GetChannel() const;

        //inline void SetDelegate(const Functor<ComponentType(const PDP::ChannelIndex &,const PDP::UInteger & index,Args...)> &);


        inline ComponentType & operator[](const PDP::UInteger & index);


        inline const ComponentType & operator[](const PDP::UInteger & index) const;
    };


    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents>::ChannelAccessor(
            const LD::Span<ComponentType> & buffer,
            const PDP::UInteger & start,
            const ChannelIndex & channel,
            const PDP::UInteger & size):Buffer(buffer),Start(start),Size(size)
    {
        this->Start = channel.GetChannelIndex();
        this->Stride = AmountOfComponents;
    }
    /*
    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents>::ChannelAccessor(ComponentType * buffer, const PDP::UInteger & start, const ChannelIndex & channel, const PDP::UInteger & size):Buffer(buffer),Start(start),Size(size)
    {
        this->Start = channel.GetChannelIndex();
        this->Stride = AmountOfComponents;
    }

    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents>::ChannelAccessor(ComponentType * buffer, const PDP::UInteger & start, const PDP::UInteger & size):Buffer(buffer),Start(start),Size(size)
    {
        this->Start = start;
        this->Stride = AmountOfComponents;
    }


    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents>::ChannelAccessor(ComponentType * buffer, const PDP::UInteger & size):Buffer(buffer),Size(size)
    {
        this->Start = 0;
        this->Stride = AmountOfComponents;
    }
     */


    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    void ChannelAccessor<ComponentType, AmountOfComponents>::SetChannel(const LD::ChannelIndex &channel)
    {
        this->Start = channel.GetChannelIndex();
        this->Stride = AmountOfComponents;
    }

    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    inline ComponentType & ChannelAccessor<ComponentType, AmountOfComponents>::operator[](const PDP::UInteger & index)
    {
        //return *(this->Buffer+(Start + index*Stride));
        return *(this->Buffer+(Start + index*Stride));
    }
    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    inline const ComponentType & ChannelAccessor<ComponentType, AmountOfComponents>::operator[](const PDP::UInteger & index) const
    {
        //return *(this->Buffer+(Start + index*Stride));
        return *(this->Buffer+(Start + index*Stride));
    }

    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ComponentType & ChannelAccessor<ComponentType, AmountOfComponents>::operator*()
    {
        return (*this->Buffer);
    }

    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    const ComponentType & ChannelAccessor<ComponentType, AmountOfComponents>::operator*() const
    {
        return (*this->Buffer);
    }

    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents> & ChannelAccessor<ComponentType, AmountOfComponents>::operator++()
    {
        this->Buffer += (this->Stride);

        return (*this);
    }


    template<typename ComponentType, PDP::UInteger AmountOfComponents>
    ChannelAccessor<ComponentType, AmountOfComponents> & ChannelAccessor<ComponentType, AmountOfComponents>::operator++(int)
    {
        this->Buffer += (this->Stride);

        return (*this);
    }
}
#endif //LANDESSDEVCORE_CHANNELACCESSOR_H
