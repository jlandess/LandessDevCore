#ifndef LANDESSDEVCOREUNIFORMRNG_HPP
#define LANDESSDEVCOREUNIFORMRNG_HPP
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Memory/ElementReference.h"
#include "Primitives/General/Range.h"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{
    template<typename Rand>
    class UniformRandomNumberGenerator
    {
    private:
        static const constexpr LD::Integer UniformDConMin = -5;
        static const constexpr LD::Integer UniformDConMax = 5;
        static const constexpr LD::Integer UniformDDMin = 1;
        static const constexpr LD::Integer UniformDDMax = 6;
        LD::Range<LD::Float> mRange;
        double mSpan;
        LD::UInteger mModulus;
        LD::Variant<Rand,LD::ElementReference<Rand>> mGenerator;
        LD::ElementReference<Rand> mReferencedGenerator;
    public:
        inline UniformRandomNumberGenerator() noexcept :mRange{UniformDDMin,UniformDDMax},mModulus{LD::UInteger (mRange.Span()+1)}
        {
            auto onRValue = [](Rand & generator) noexcept
            {
                return LD::ElementReference<Rand>{generator};
            };
            auto onReference = [](LD::ElementReference<Rand> & a) noexcept
            {
                return LD::ElementReference<Rand>{a};
            };

            this->mReferencedGenerator = LD::Match(this->mGenerator,onRValue,onReference);
        }
        /// \brief Default constructor.
        ///
        /// Set the max and min to the default values.
        inline UniformRandomNumberGenerator(const LD::ElementReference<Rand> & g) noexcept :mGenerator{g},mRange{UniformDDMin,UniformDDMax},mModulus{LD::UInteger (mRange.Span()+1)}
        {
            this->mGenerator = LD::ElementReference<Rand>{g};
            auto onRValue = [](Rand & generator) noexcept
            {
                return LD::ElementReference<Rand>{generator};
            };
            auto onReference = [](LD::ElementReference<Rand> & a) noexcept
            {
                return LD::ElementReference<Rand>{a};
            };

            this->mReferencedGenerator = LD::Match(this->mGenerator,onRValue,onReference);
        }

        /// \brief Constructor with custom min and max.
        inline UniformRandomNumberGenerator(const LD::ElementReference<Rand> & g, const LD::Float & mn, const LD::Float & mx) noexcept:mGenerator{g},mRange{mn,mx},mModulus{LD::UInteger (mRange.Span()+1)}
        {
            this->mGenerator = LD::ElementReference<Rand>{g};
            auto onRValue = [](Rand & generator) noexcept
            {
                return LD::ElementReference<Rand>{generator};
            };
            auto onReference = [](LD::ElementReference<Rand> & a) noexcept
            {
                return LD::ElementReference<Rand>{a};
            };

            this->mReferencedGenerator = LD::Match(this->mGenerator,onRValue,onReference);
        }
        /// \brief Getter for minimum possible value returned, as floating.
        constexpr LD::Range<LD::Float> Range() const noexcept{return this->mRange;}
        LD::Range<LD::Float> & Range() noexcept {return this->mRange;}

        constexpr double Mean() const noexcept
        {
            return (this->mRange.Max()+this->mRange.Min())/2.0;
        }

        constexpr double Mode() const noexcept
        {
            return 0;
        }

        constexpr double Median() const noexcept
        {
            return (this->mRange.Max()+this->mRange.Min())/2.0;
        }

        constexpr double Variance() const noexcept
        {
            return (this->mSpan.Span()*this->mSpan.Span())/12.0;
        }


        unsigned long NextInteger() noexcept
        {
            return ( (mRange.Min())) + (this->mReferencedGenerator->NextInteger() % this->mModulus);
        }

        double NextFloatingPoint() noexcept
        {
            return mRange.Min() + (this->mReferencedGenerator->NextFloatingPoint() * (this->mRange.Span()));
        }

    };


    /// \brief Uniform random number generator in a defined discrete range.
    ///
    /// Generate an uniform deviate sequence of discrete values (long integer)
    /// in the defined range.
    /// The generator is also able to generate continue values. Difference in
    /// performance are trascurable, however, due to the type for the range
    /// specification parameters, the discrete generator is not able to generate
    /// values in ranges like [1.2, 1.6].
    class DiscreteUniformRandomNumberGenerator  {

    private:

        // Min value in the sequence.
        LD::UInteger mMin;

        // Max value in the sequence.
        LD::UInteger mMax;

        // Range span.
        LD::UInteger mSpan;

        // Number of generated values.
        LD::UInteger mMod;

    public:

        /// \brief Default constructor.
        ///
        /// Set the max and min to the default values.
        //GS_DUniformRNG(GS_BasicRNG* g);

        /// \brief Constructor with custom min and max.
        //GS_DUniformRNG(GS_BasicRNG* g, long mn, long mx);

        /// \brief Getter for minimum possible value returned, as integer.
        LD::UInteger getRangeMinI();

        /// \brief Getter for maximum possible value returned, as integer.
        LD::UInteger getRangeMaxI();

        /// \brief Getter for minimum possible value returned, as floating.
        double getRangeMinF();

        /// \brief Getter for maximum possible value returned, as floating.
        double getRangeMaxF();

        /// \brief Getter for range.
        ///
        /// For discrete generators the domain range is different from the number of
        /// generated values;
        /// e.g. the domain [1, 6] has range 5, but generates 6 values.
        LD::UInteger getRange();

        /// \brief Getter for number of generated values.
        ///
        /// For discrete generators the number of generated values is different from
        /// the domain range;
        /// e.g. the domain [1, 6] has range 5, but generates 6 values.
        LD::UInteger getNumVal();

        /// \brief Setter for min.
        void setRangeMin(const LD::UInteger & m);

        /// \brief Setter for max.
        void setRangeMax(const LD::UInteger & m);

        /// \brief Getter for mean.
        double getMean();

        /// \brief Getter for mode.
        double getMode();

        /// \brief Getter for median.
        double getMedian();

        /// \brief Getter for variance.
        double getVariance();

        /// \brief Calculate next value in the sequence, returns as integer.
        long nextI();

        /// \brief Calculate next value in the sequence, returns as floating.
        double nextF();

    };
}
#endif