#ifndef TRIANGULARRNG_HPP
#define TRIANGULARRNG_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/Range.h"
#include "Memory/ElementReference.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Algorithms/Exponential.hpp"
namespace LD
{
    template<typename Rand>
    class TriangularGenerator
    {

    private:

        static const constexpr LD::Integer GS_Trng_DA = 1;
        static const constexpr LD::Integer GS_Trng_DB = 3;
        static const constexpr LD::Integer GS_Trng_DC = 0;
        // Minimum value.
        //double a;

        // Maximum value.
        //double b;
        LD::Range<LD::Float> mRange;

        // Most probable value.
        //double c;
        LD::Float c;

        // Stored constants.
        double cdfc;  // F(c)
        double baca;  // (b-a)*(c-a)
        double babc;  // (b-a)*(b-c)

        LD::Variant<Rand,LD::ElementReference<Rand>> mGenerator;
        LD::ElementReference<Rand> mReferencedGenerator;
    public:

        constexpr TriangularGenerator() noexcept :mRange{GS_Trng_DA,GS_Trng_DB}
        {
            //a = GS_Trng_DA;
            double a = mRange.Min();
            double b = mRange.Max();
            c = GS_Trng_DC;


            // Calculate constants.
            cdfc = (c - a)/(b - a);
            baca = (b - a)*(c - a);
            babc = (b - a)*(b - c);

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

        constexpr TriangularGenerator(double mn, double mx, double mp) noexcept :mRange{mn,mx}
        {


            double a = mRange.Min();
            double b = mRange.Max();
            // Check and set parameters.
            if ( (mn <= mp) && (mp <= mx) ) {
                a = mn;
                b = mx;
                c = mp;
            } else
                {
                a = GS_Trng_DA;
                b = GS_Trng_DB;
                c = GS_Trng_DC;
            }

            // Calculate constants.
            cdfc = (c - a)/(b - a);
            baca = (b - a)*(c - a);
            babc = (b - a)*(b - c);

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
        /// Set parameters to default values.

        /// \brief Getter for minimum possible value returned, as integer.
        const  LD::Range<LD::Float> & Range() const noexcept
        {
            return this->mRange;
        }

        LD::Range<LD::Float> & Range() noexcept
        {
            return this->mRange;
        }

        /// \brief Getter for mean.
        constexpr double Mean() const noexcept
        {
            return (mRange.Min() + mRange.Max() + c)/3.0;
        }

        /// \brief Getter for mode.
        constexpr double Mode() const noexcept
        {
            return c;
        }

        /// \brief Getter for median.
        constexpr double Median() const noexcept
        {
            double a = mRange.Min();
            double b = mRange.Max();
            double m = (a+b)/2;
            double sqrt2 = LD::Sqrt(2.0);
            double branch[2] = {b- LD::Sqrt(babc)/sqrt2,a + LD::Sqrt(baca)/sqrt2};

            //if (c >= m) {
                //return a + LD::Sqrt(baca)/sqrt2;
            //} else {
                //return b- LD::Sqrt(babc)/sqrt2;
            //}
            return branch[c>=m];
        }

        /// \brief Getter for variance.
        constexpr double Variance() const noexcept
        {
            double a = mRange.Min();
            double b = mRange.Max();
            return (a*a + b*b + c*c - a*b - a*c - b*c)/18.0;
        }


        /// \brief Setter for most probable.
        void setMode(double mp) noexcept
        {
            LD::Float a = mRange.Min();
            LD::Float b = mRange.Max();
            // Check consistency with already set parameters.
            if ( (a <= mp) && (mp <= b) ) {
                c = mp;
                // Recalculate constants.
                cdfc = (c - a)/(b - a);
                baca = (b - a)*(c - a);
                babc = (b - a)*(b - c);
            } else {
            }
        }


        /// \brief Calculate next value in the sequence, returns as integer.
        ///
        /// Continue values are rounded.
        constexpr LD::UInteger NextInteger() noexcept
        {
            // Floating point value is rounded.
            //double f = nextF();
            //cout << f << " ";
            return (LD::UInteger) LD::Floor(this->NextFloatingPoint() + 0.5);
        }

        /// \brief Calculate next value in the sequence, returns as floating.
        constexpr double NextFloatingPoint() noexcept
        {
            double u = this->mReferencedGenerator->NextFloatingPoint();

            LD::Float  a = mRange.Min();
            LD::Float b = mRange.Max();
            double branch[2] = {b - sqrt((1.0 - u) * babc),a + sqrt(u * baca)};
            //if (u < cdfc)
             //   return a + sqrt(u * baca);
            //else
              //  return b - sqrt((1.0 - u) * babc);
            return branch[u<cdfc];
        }

    };
}

#endif