//
// Created by phoenixflower on 6/23/20.
//

#ifndef LANDESSDEVCORE_GAUSSIANUNIFORMRNG_H
#define LANDESSDEVCORE_GAUSSIANUNIFORMRNG_H
#include "Definitions/Float.hpp"
#include "Memory/ElementReference.h"
#include "Algorithms/Exponential.hpp"
#include "Algorithms/Periodic.hpp"
namespace LD
{
    template<typename Random>
    class BoxMullerGenerator
    {
    private:
        LD::Float mStandardDeviation;
        LD::Float mMean;
        LD::ElementReference<Random> mReferenceGenerator;
        constexpr static const auto PI = 3.1415926535;
        constexpr static const auto TwoPI = 6.2831853071795864769252866;
        
    public:

        BoxMullerGenerator(const LD::Float & mean = 1,const LD::Float & standardDeviation = 1):mMean{mean},mStandardDeviation{standardDeviation}
        {

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
#endif //LANDESSDEVCORE_GAUSSIANUNIFORMRNG_H
