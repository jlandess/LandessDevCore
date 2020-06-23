#ifndef LandessDevCore_Twister_Hpp
#define LandessDevCore_Twister_Hpp
namespace LD
{
    class TwisterRandomNumberGenerator
    {
    private:

        constexpr static const auto N = 624;
        constexpr static const auto M = 397;
        constexpr static const auto Mag0 = 0x00000000UL;
        constexpr static const auto Mag1 = 0x9908b0dfUL;
        constexpr static const auto InitM = 1812433253UL;
        constexpr static const auto Lmsk = 0x7fffffffUL;
        constexpr static const auto Umsk = 0x80000000UL;
        constexpr static const auto Defs = 5489UL;
        constexpr static const auto RMax = 4294967295UL;
        constexpr static const unsigned long mg[2] = {Mag0, Mag1};

        unsigned long mSeed;
        unsigned long mCurrent;
        unsigned long t[N];
        int iT;
    public:


        constexpr TwisterRandomNumberGenerator(const unsigned long & seed = {}) noexcept
        {
            this->ReSeed(seed);
        }

        constexpr void ReSeed(const unsigned long & seed) noexcept
        {
            // Set the seed.
            mSeed = seed;

            // Set the first value of the table to the default seed.
            t[0] = seed;

            // Fill the table.
            for ( iT=1; iT<N; iT++ )
            {
                t[iT] = (InitM * (t[iT-1] ^ (t[iT-1] >> 30)) + iT);
            }
        }

        constexpr unsigned long NextInteger() noexcept {

            int jT;

            if (iT == N) {

                // The current set of values was completely used.
                // This happens immediately after initialization too.
                // Generate a new set.

                // First part, from 0 to pivot.
                for (jT=0; jT<(N-M); jT++) {
                    // Apply masks to calculate curr from table.
                    this->mCurrent = (t[jT] & Umsk) | (t[jT+1] & Lmsk);
                    // Use curr and magic table to update table.
                    t[jT] = t[jT+M] ^ (this->mCurrent>>1) ^ mg[this->mCurrent&0x1UL];
                }

                // Second part, from pivot to end-1.
                for ( ; jT<(N-1); jT++) {
                    // Apply masks to calculate curr from table.
                    this->mCurrent = (t[jT] & Umsk) | (t[jT+1] & Lmsk);
                    // Use curr and magic table to update table.
                    t[jT] = t[jT+(M-N)] ^ (this->mCurrent>>1) ^ mg[this->mCurrent&0x1UL];
                }

                // Last element, closing the cycle with the first one.
                // Apply masks to calculate curr from table.
                this->mCurrent = (t[jT] & Umsk) | (t[0] & Lmsk);
                // Use curr and magic table to update table.
                t[jT] = t[M-1] ^ (this->mCurrent>>1) ^ mg[this->mCurrent&0x1UL];

                // Reset iT.
                iT = 0;
            }

            // Set the current value and temper it.
            this->mCurrent = t[iT];
            this->mCurrent ^= (this->mCurrent >> 11);
            this->mCurrent ^= (this->mCurrent << 7) & 0x9d2c5680UL;
            this->mCurrent ^= (this->mCurrent << 15) & 0xefc60000UL;
            this->mCurrent ^= (this->mCurrent >> 18);

            // Increment the iterator;
            iT++;

            return this->mCurrent;
        }

        constexpr double NextFloatingPoint() noexcept  {

            return (double) this->NextInteger()/(RMax + 1.0);
        }
    };
}
#endif