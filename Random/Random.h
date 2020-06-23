//
//  Random.h
//  DataStructures
//
//  Created by James Landess on 10/28/14.
//  Copyright (c) 2014 LandessDev. All rights reserved.
//

#ifndef __DataStructures__Random__
#define __DataStructures__Random__

#include <climits>
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"

namespace LD
{
    template <typename T >
    class MaximumRandom
    {
    public:
        static constexpr T GetMaximum() noexcept
        {
            return T();
        }
    };
    template<>
    class MaximumRandom<int>
    {
    public:
        static constexpr int GetMaximum() noexcept
        {
            return INT_MAX;
        }
    };
    template<>
    class MaximumRandom<long>
    {
    public:
        static constexpr long GetMaximum() noexcept
        {
            return LONG_MAX;
        }
    };

    template<>
    class MaximumRandom<unsigned long>
    {
    public:
        static constexpr long GetMaximum() noexcept
        {
            return ULONG_MAX;
        }
    };

    template<>
    class MaximumRandom<unsigned int>
    {
    public:
        static constexpr long GetMaximum() noexcept
        {
            return UINT_MAX;
        }
    };

    /**
     @addtogroup DataStructures
     @{
     */

    /**
     @brief A standard random number generator which was developed for the sole purpose of implementing the PDP::DataStructures::Treap datastructure.  It can also serve as a trivial random number generator.  If you are looking for advanced feature sets such as uniform random numbers, or random numbers which allow for customization, then it is best to look under the PDP::Math namespace of the Postponed Productions libraries
     */
    class Random
    {
    public:
        /**
         @brief A constructor which allows you to provide the initial value (which determines the state) of this classical Random Number generator
         */
        Random(LD::UInteger initialValue = 1 ) noexcept ;

        /**
         @brief Gets the next integer from the Random number generator
         @return PDP::Integer - The next random integer
         @code
         #include <PostponedProductions/DataStructures/Random.h>
         int main()
         {
            PDP::DataStructures::Random randomNumberGenerator;
            for(PDP::UInteger n = 0;n<100;++n)
            {
                PDP::Integer nextRandomInteger = randomNumberGenerator.NextInt();
            }
            return 0;
         }
         @endcode
         */
        LD::UInteger NextInt( ) noexcept ;


        /**
         @brief Gets the next floating point number from the Random number generator
         @return PDP::Integer - The next random integer
         @code
         #include <PostponedProductions/DataStructures/Random.h>
         int main()
         {
            PDP::DataStructures::Random randomNumberGenerator;
            for(PDP::UInteger n = 0;n<100;++n)
            {
                LD::Float nextRandomInteger = randomNumberGenerator.NextFloat();
            }
            return 0;
         }
         @endcode
         */
        LD::Float NextFloat( ) noexcept ;

        LD::Float NextFloat(LD::UInteger low, LD::UInteger high ) noexcept ;

        /**
         @brief Gets the next integer between the specifies values [low,high]
         @param low - The beginning of the range you care about
         @param high - The end of the range you care about
         @return PDP::Integer - The next random integer
         @code
         #include <PostponedProductions/DataStructures/Random.h>
         int main()
         {
            PDP::DataStructures::Random randomNumberGenerator;
            for(PDP::UInteger n = 0;n<100;++n)
            {
                LD::Float nextRandomInteger = randomNumberGenerator.NextInt(0,55);
            }
            return 0;
         }
         @endcode
         */
        LD::UInteger NextInt( LD::UInteger low, LD::UInteger high ) noexcept;

        /**
         @brief Provides a copy to the current internal state of the random number generator, this is here simply for serialization reasons to maintain the random number generator's "random-ness" between application start-ups
         */
        LD::UInteger GetState() const noexcept ;
    private:
        LD::UInteger State;
    };
}


#endif /* defined(__DataStructures__Random__) */