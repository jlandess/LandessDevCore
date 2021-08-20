//
//  Sampler.h
//  DataStructures5
//
//  Created by James Landess on 2/14/19.
//  Copyright © 2019 James Landess. All rights reserved.
//

#ifndef Sampler_h
#define Sampler_h

//#include "Definitions/Common.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "TypeTraits/StaticallySized.h"
#include "Core/NullClass.hpp"
namespace LD
{
    template<typename T, LD::UInteger N = 4, class Filter = LD::NullClass, class sfinae = void>
    class Sampler;




    template<LD::UInteger Index,typename T, LD::UInteger N>
    static void ComputeAverageSamplerBase(const LD::StaticArray<T,N> & buffer, T & sum)
    {
        sum+=buffer[Index];

    }


    template<typename  T , LD::UInteger N, UInteger ... Index>
    static T ComputeAverageSamplerFold(LD::IndexSequence<Index...>, const LD::StaticArray<T,N> & buffer)
    {
        T sum = 0;
        (...,ComputeAverageSamplerBase<Index>(buffer,sum));
        return sum;
    }

    template<typename T, LD::UInteger N>
    static T ComputeAverageSampler(const LD::StaticArray<T,N> & buffer)
    {
        return ComputeAverageSamplerFold(typename LD::MakeIndexSequence<N>::type{},buffer)/N;
    }



    template<typename T, LD::UInteger N>
    class Sampler<T,N,LD::NullClass,LD::Enable_If_T<
            LD::Require<
                    LD::Concept::Arithmetical<T>,
                    LD::IsFloatingPoint<T>
            >>>
    {
    private:
        LD::StaticArray<T, N> Buffer;
        T Sampled;
        LD::UInteger Index;
    public:
        inline Sampler()
        {
            this->Index = 0;
            this->Sampled = T();

        }

        Sampler & operator()(const T & sample)
        {
            this->Buffer[this->Index++] = sample;

            this->Index%=N;

            const bool ifExpression = (N-this->Index == N);
            this->Sampled =  (ifExpression)*ComputeAverageSampler(this->Buffer) + (!(ifExpression))*this->Sampled;

            return (*this);
        }


        Sampler & Sample(const T & sample)
        {
            this->Buffer[this->Index++] = sample;

            this->Index%=N;

            const bool ifExpression = (N-this->Index == N);
            this->Sampled =  (ifExpression)*ComputeAverageSampler(this->Buffer) + (!(ifExpression))*this->Sampled;

            return (*this);
        }


        const T & operator()() const
        {
            return this->Sampled;
        }
    };

    template<typename T, typename Filter, LD::UInteger N>
    class Sampler<T,N,Filter,LD::Enable_If_T<
            LD::Require<
                    LD::Concept::Arithmetical<T>,
                    LD::IsFloatingPoint<T>
            >>>
    {
    private:
        LD::StaticArray<T, N> Buffer;
        T Sampled;
        LD::UInteger Index;
    public:
        inline Sampler():Index(0),Sampled(T())
        {


        }

        Sampler & operator()(const T & sample)
        {
            this->Buffer[this->Index++] = sample;

            this->Index%=N;

            const bool ifExpression = (N-this->Index == N);
            this->Sampled =  (ifExpression)*ComputeAverageSampler(this->Buffer) + (!(ifExpression))*this->Sampled;

            return (*this);
        }
        Sampler & Sample(const T & sample)
        {
            this->Buffer[this->Index++] = sample;

            this->Index%=N;

            const bool ifExpression = (N-this->Index == N);
            this->Sampled =  (ifExpression)*ComputeAverageSampler(this->Buffer) + (!(ifExpression))*this->Sampled;

            return (*this);
        }


        const T & operator()() const
        {
            return this->Sampled;
        }
    };


}

namespace LD
{
    namespace Detail
    {
        template<typename T, LD::UInteger Size>
        struct StaticallySized<LD::Sampler<T,Size>>: public LD::Detail::IntegralConstant<bool,true>
        {

        };
    }
}
#endif /* Sampler_h */
