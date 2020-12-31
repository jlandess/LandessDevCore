#ifndef BASICRNG_LANDESSCORE_HPP
#define BASICRNG_LANDESSCORE_HPP
#include "Primitives/General/mapboxvariant.hpp"
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
#include "TypeTraits/TypeList.hpp"
#include "TypeTraits/Limit.hpp"
#include "TypeTraits/Detection.hpp"
#include "Memory/ElementReference.h"
namespace LD
{
    class BasicSystemRandom
    {
    private:
        static constexpr const LD::UInteger RandMax = RAND_MAX;
        static constexpr const auto Infinity = LD::Limit<double>::GetMax();
        static constexpr const auto DefSeed = 87923;
        static constexpr const auto MaxRejections = 100;
        /// \brief Seed of the random sequence.
        unsigned long mSeed;
        /// \brief Current value in the sequence.
        unsigned long mCurr;
    public:

        inline  BasicSystemRandom() noexcept:mSeed(0),mCurr(0)
        {
            srand(DefSeed);
        }
        inline BasicSystemRandom(const LD::Integer & seed) noexcept :mSeed(seed),mCurr(seed)
        {
            srand(mSeed);
        }
        inline unsigned long NextInteger() noexcept
        {
            this->mCurr = rand();
            return this->mCurr;
        }

        /// \brief Calculate next value in the sequence (as floating point value).
        ///
        /// Calculate the next integer value in the sequence, which is available
        /// by getCurr(), but returns a floating point normalized value in [0,1).
        /// Due to the implementation of the standard library rand(),
        /// this affects all the instances of GS_SysR_BRNG.
        inline double NextFloatingPoint() noexcept
        {
            this->mCurr = rand();
            return (double) double(this->mCurr)/(double(RandMax) + 1.0);
        }

        /// \brief Get the max random value constant.
        inline unsigned long maxRand()
        {
            return RandMax;
        }

        /// \brief Reset sequence to current seed.
        ///
        /// Due to the implementation of the standard library srand(),
        /// this affects all the instances of GS_SysR_BRNG.
        inline void reSeed(const unsigned long & seed) noexcept
        {
            srand(seed);
        }

        /// \brief Reset sequence with specified seed.
        ///
        /// Due to the implementation of the standard library srand(),
        /// this affects all the instances of GS_SysR_BRNG.
        inline void reSeed() noexcept
        {
            srand(this->mSeed);
        }
    };

    class LinearCongruentGenerator
    {
    private:
        static constexpr const LD::UInteger MVal = 65535;
        static constexpr const LD::UInteger AVal = 2011;
        static constexpr const LD::UInteger CVal = 313;
        LD::Integer mModulus;  // Modulus     (aka m).
        LD::Integer mMultiplier;  // Multiplier  (aka a).
        LD::Integer mIncrement;  // Increment   (aka c).
        LD::UInteger mCurrent;
    public:
        /// \brief Default constructor.
        ///
        /// Set the seed to the default.
        inline LinearCongruentGenerator(const LD::UInteger & seed = {}) noexcept :mCurrent(seed)
        {
            mModulus = MVal;
            mMultiplier = AVal;
            mIncrement = CVal;
        }

        /// \brief Constructor with custom LCG parameters.
        ///
        /// Set modulus, multiplier and increment to the specified values.
        inline LinearCongruentGenerator(const LD::Integer  & m, const LD::Integer  & a, const LD::Integer & c) noexcept :mModulus(m),mMultiplier(a),mIncrement(c)
        {

        }


        /// \brief Get the max random value constant.
        inline unsigned long maxRand()
        {
            return this->mModulus-1;
        }

        /// \brief Reset sequence with specified seed.
        inline void reSeed(unsigned long & s) noexcept
        {
            this->mCurrent = s;
        }

        /// \brief Calculate the next value in the sequence.
        inline unsigned long NextInteger() noexcept
        {
            this->mCurrent = ((this->mCurrent * this->mMultiplier) + this->mIncrement) % this->mModulus;
            return this->mCurrent;
        }

        /// \brief Calculate the next value in the sequence as float in [0,1).
        inline double NextFloatingPoint() noexcept
        {
            this->mCurrent = ((this->mCurrent * this->mMultiplier) + this->mIncrement) % this->mModulus;
            return (double) this->mCurrent/(this->mModulus + 1.0);
        }
    };

    class QuickAndDirty32BitGenerator
    {
    private:
        static constexpr const LD::UInteger RMax = 4294967295UL;
        static constexpr const LD::UInteger AVal = 1664525UL;
        static constexpr const LD::UInteger CVal = 1013904223UL;
        LD::UInteger mCurrent;
    public:

        /// \brief Default constructor.
        ///
        /// Set the seed to the default.
        QuickAndDirty32BitGenerator(const LD::UInteger & seed = {}) noexcept :mCurrent(seed)
        {

        }


        /// \brief Get the max random value constant.
        inline unsigned long maxRand() noexcept
        {
            return RMax;
        }

        /// \brief Reset sequence with specified seed.
        inline void reSeed(unsigned long & s) noexcept
        {
            this->mCurrent = s;
        }

        /// \brief Calculate the next value in the sequence.
        inline unsigned long NextInteger() noexcept
        {

            this->mCurrent = AVal*this->mCurrent + CVal;
            return this->mCurrent;
        }

        /// \brief Calculate next value in the sequence (as floating point value).
        ///
        /// Calculate the next integer value in the sequence, which is available
        /// by getCurr(), but returns a floating point normalized value in [0,1).
        inline double NextFloatingPoint() noexcept
        {

            // The mod operation is not done, 32 bit overflow is exploited.
            this->mCurrent = AVal*this->mCurrent + CVal;
            // A even dirty trick can be used to avoid a divide by a double 2^32
            // by masking the exponential part of the representation, but the
            // resulting code is heavy machine dependent.
            return (double) this->mCurrent/(RMax + 1.0);
        }
    };

    class ParkAndMillarMinimalStandardGenerator
    {
    private:
        static constexpr const LD::UInteger MVal = 2147483647;
        static constexpr const LD::UInteger AVal = 16807;
        static constexpr const LD::UInteger Stiq = 127773;
        static constexpr const LD::UInteger Mask = 123459876;
        static constexpr const LD::UInteger Stir = 2836;
        static constexpr const LD::Float Inverse = LD::Float(1.0)/LD::Float (MVal);
        LD::UInteger mCurrent;
    public:
        ParkAndMillarMinimalStandardGenerator(const LD::UInteger & seed = {}) noexcept :mCurrent(seed)
        {

        }
        /// \brief Get the max random value constant.
        inline unsigned long maxRand()
        {
            return  MVal - 1;
        }
        /// \brief Reset sequence with specified seed.
        inline void reSeed(unsigned long & s) noexcept
        {
            this->mCurrent = s;
        }

        /// \brief Calculate the next value in the sequence.
        inline unsigned long NextInteger() noexcept
        {
            long k;                        // Temp variable for Schrage trick.
            long c = (long) this->mCurrent;          // Need negative values.

            // XORring with mask to prevent 0 seed.
            //c ^= GS_PMMS_MASK;

            // Schrage: divide curr by q.
            k = c / Stiq;

            // Schrage: compute curr in two steps.
            c = AVal * (c - k*Stiq) - Stir*k;
            if (c < 0)
                c += MVal;

            // XORring with mask again.
            //c ^= GS_PMMS_MASK;
            this->mCurrent = (unsigned long) c;
            return this->mCurrent;
        }

        /// \brief Calculate next value in the sequence (as floating point value).
        ///
        /// Calculate the next integer value in the sequence, which is available
        /// by getCurr(), but returns a floating point normalized value in [0,1).
        inline double NextFloatingPoint() noexcept
        {
            return (double) Inverse * NextInteger();
        }
    };


    template<typename T, typename Random, class = void>
    class BasicRandomGenerator;
    template<typename T, typename Random>
    class BasicRandomGenerator<T,Random,LD::Enable_If_T<
            LD::Require<
            LD::Either<LD::IsUnsignedInteger<T>,LD::IsInteger<T>>
            >>>
    {
    private:
        LD::ElementReference<Random> mGenerator;
    public:
        constexpr BasicRandomGenerator(const LD::ElementReference<Random> & gen) noexcept :mGenerator{gen}
        {

        }

        constexpr T operator()() noexcept
        {
            return this->mGenerator->NextInteger();
        }

    };

    template<typename T, typename Random>
    class BasicRandomGenerator<T,Random,LD::Enable_If_T<
            LD::Require<
                    LD::IsFloatingPoint<T>
            >>>
    {
    private:
        LD::ElementReference<Random> mGenerator;
    public:
        constexpr BasicRandomGenerator(const LD::ElementReference<Random> & gen) noexcept :mGenerator{gen}
        {

        }

        constexpr T operator()() noexcept
        {
            return this->mGenerator->NextFloatingPoint();
        }
    };

    template<typename T, typename ... RandomTypes>
    class BasicRandomGenerator<T,LD::CT::TypeList<RandomTypes...>>
    {
    private:
        LD::Variant<RandomTypes...> mPolymorphicGenerator;
    public:
    };
}
#endif