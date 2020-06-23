//
// Created by phoenixflower on 6/23/20.
//

#ifndef LANDESSDEVCORE_GAUSSIANUNIFORMRANDONNUMBERGENERATOR_H
#define LANDESSDEVCORE_GAUSSIANUNIFORMRANDONNUMBERGENERATOR_H
#include "Definitions/Float.hpp"
#include "Memory/ElementReference.h"
#include "Algorithms/Exponential.hpp"
#include "Algorithms/Periodic.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    template<typename Random, class = void>
    class BoxMullerGenerator;
    /**
     *
     * @tparam Random The Core Generator that will be used to generate random numbers
     * @brief Provides a Gaussian distribution
     */
    template<typename Random>
    class BoxMullerGenerator<Random,LD::Enable_If_T<
            LD::Require<
                    true
                    >>>
    {
    private:
        LD::Float mStandardDeviation;
        LD::Float mMean;
        LD::ElementReference<Random> mReferenceGenerator;
        LD::Variant<Random,LD::ElementReference<Random>> mGenerator;
        constexpr static const auto PI = 3.1415926535;
        constexpr static const auto TwoPI = 6.2831853071795864769252866;

    public:

        /**
         *
         * @param mean the Centerpoint (mean) of the distribution
         * @param standardDeviation How far the distribution should "deviate" from the mean
         * @brief Default behavior will provide a distribution between [0,100] with 50 as the mean
         */
        constexpr BoxMullerGenerator(const LD::Float & mean = 50,const LD::Float & standardDeviation = 50) noexcept :mMean{mean},mStandardDeviation{standardDeviation}
        {
            auto onRValue = [](Random & generator) noexcept
            {
                return LD::ElementReference<Random>{generator};
            };
            auto onReference = [](LD::ElementReference<Random> & a) noexcept
            {
                return LD::ElementReference<Random>{a};
            };

            this->mReferencedGenerator = LD::Match(this->mGenerator,onRValue,onReference);

        }

        constexpr BoxMullerGenerator(const LD::ElementReference<Random> & generator,const LD::Float & mean = 1,const LD::Float & standardDeviation = 1) noexcept :mMean{mean},mStandardDeviation{standardDeviation},mGenerator{generator}
        {
            auto onRValue = [](Random & generator) noexcept
            {
                return LD::ElementReference<Random>{generator};
            };
            auto onReference = [](LD::ElementReference<Random> & a) noexcept
            {
                return LD::ElementReference<Random>{a};
            };

            this->mReferencedGenerator = LD::Match(this->mGenerator,onRValue,onReference);
        }

        constexpr LD::UInteger NextInteger() noexcept
        {
            return this->NextFloatingPoint();
        }
        constexpr double NextFloatingPoint() noexcept
        {

            double   u, theta;           // Variables for Box-Muller method

            // Generate u
            u = 0.0;
            while (u == 0.0)
                u = mReferenceGenerator->GetNextFloatingPoint();

            // Compute r
            double r = LD::Sqrt(-2.0 * LD::Ln(u));

            // Generate theta
            theta = 0.0;
            while (theta == 0.0)
                theta = TwoPI * mReferenceGenerator->GetNextFloatingPoint();


            // Generate x value
            double x = r * LD::Cos(theta);// Normal(0, 1) rv

            // Adjust x value for specified mean and variance
            double norm_rv = (x * this->mStandardDeviation) + this->mMean;

            // Return the normally distributed RV value
            return(norm_rv);
        }
    };
}
#endif //LANDESSDEVCORE_GAUSSIANUNIFORMRANDONNUMBERGENERATOR_H
