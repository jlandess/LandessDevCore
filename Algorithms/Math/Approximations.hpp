//
// Created by phoenixflower on 9/26/21.
//

#ifndef LANDESSDEVCORE_APPROXIMATIONS_HPP
#define LANDESSDEVCORE_APPROXIMATIONS_HPP
namespace LD
{
    namespace Approx
    {
        static inline double fastersin (float x)
        {
            static constexpr float fouroverpi = 1.2732395447351627f;
            static constexpr float fouroverpisq = 0.40528473456935109f;
            static constexpr float q = 0.77633023248007499f;
            union { float f; uint32_t i; } p = { 0.22308510060189463f };

            union { float f; uint32_t i; } vx = { x };
            uint32_t sign = vx.i & 0x80000000;
            vx.i &= 0x7FFFFFFF;

            float qpprox = fouroverpi * x - fouroverpisq * x * vx.f;

            p.i |= sign;

            return qpprox * (q + p.f * qpprox);
        }

        static inline float sin (float x)
        {
            static constexpr float twopi = 6.2831853071795865f;
            static constexpr float invtwopi = 0.15915494309189534f;

            int k = x * invtwopi;
            float half = (x < 0) ? -0.5f : 0.5f;
            return fastersin ((half + k) * twopi - x);
        }


        static inline float fastercos (float x)
        {
            static constexpr float twooverpi = 0.63661977236758134f;
            static constexpr float p = 0.54641335845679634f;

            union { float f; uint32_t i; } vx = { x };
            vx.i &= 0x7FFFFFFF;

            float qpprox = 1.0f - twooverpi * vx.f;

            return qpprox + p * qpprox * (1.0f - qpprox * qpprox);
        }

        static inline float cos (float x)
        {
            static constexpr float halfpi = 1.5707963267948966f;
            return sin (x + halfpi);
        }



        static inline float tan (float x)
        {
            static constexpr float twopi = 6.2831853071795865f;
            static constexpr float invtwopi = 0.15915494309189534f;

            int k = x * invtwopi;
            float half = (x < 0) ? -0.5f : 0.5f;
            float xnew = x - (half + k) * twopi;

            return LD::Approx::sin (xnew) / LD::Approx::cos (xnew);
        }

        static inline float csc(float value)
        {
            return 1.0f/LD::Approx::sin(value,amountOfIterations);
        }
        static inline float sec(float value)
        {
            return 1.0f/LD::Approx::cos(value,amountOfIterations);
        }

        static inline float log2 (float x)
        {
            union { float f; uint32_t i; } vx = { x };
            float y = vx.i;
            y *= 1.1920928955078125e-7f;
            return y - 126.94269504f;
        }

        static inline float log (float x)
        {
            //  return 0.69314718f * fasterlog2 (x);

            union { float f; uint32_t i; } vx = { x };
            float y = vx.i;
            y *= 8.2629582881927490e-8f;
            return y - 87.989971088f;
        }

        static inline float pow2 (float p)
        {
            float clipp = (p < -126) ? -126.0f : p;
            union { uint32_t i; float f; } v = { uint32_t ( (1 << 23) * (clipp + 126.94269504f) ) };
            return v.f;
        }

        static inline float pow (float x,
                                 float p)
        {
            return Expedited::pow2 (p * Expedited::log2 (x));
        }


        static inline float
        fastererfc (float x)
        {
            static const float k = 3.3509633149424609f;

            return 2.0f / (1.0f + Expedited::pow2 (k * x));
        }
        static inline float ererf (float x)
        {
            return 1.0f - fastererfc (x);
        }

        float QRSqrt( float number) noexcept
        {
            long i;
            float x2, y;
            const float threehalfs = 1.5f;

            x2 = number * 0.5f;
            y = number;
            i = *(long *)&y;
            i = 0x5f3759df - ( I >> 1);
            y = *(float*)&i;
            y = y * (threehalfs - (x2* y*y ));
            return y;
        }
    }
}
#endif //LANDESSDEVCORE_APPROXIMATIONS_HPP
