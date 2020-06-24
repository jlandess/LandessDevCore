#ifndef LandessDevCoreMarsagliaRandomNumberGenerator
#define LandessDevCoreMarsagliaRandomNumberGenerator
namespace LD
{

    class MarsagliaMultiplyWithCarryGenerator
    {
    private:
        constexpr static const auto ZMC = 36969;
        constexpr static const auto WMC = 18000;
        constexpr static const auto LAC = 65535;
        constexpr static const auto SPC = 16;
        constexpr static const auto DefZ = 362436069;
        constexpr static const auto DefW = 521288629;
        constexpr static const auto RMax = 4294967295UL;

        unsigned long mZ;
        unsigned long mW;
        unsigned long mSeed;
        unsigned long mCurrent;
    public:

        constexpr MarsagliaMultiplyWithCarryGenerator() noexcept :mZ{DefZ},mW{DefW},mSeed{0},mCurrent{0}
        {

        }

        unsigned long MaxRand() {

            return  RMax;
        }

        void ReSseed() {

            mZ = (unsigned long) (DefZ + mSeed);
            mW = (unsigned long) (DefW + mSeed);
        }

        void ReSeed(unsigned long s) {

            mSeed = (unsigned long) s;
            mZ = (unsigned long) (DefZ + mSeed);
            mW = (unsigned long) (DefW + mSeed);
        }

        unsigned long NextInteger() noexcept {

            // Calculate next Z.
            mZ = ZMC * (mZ & LAC) + (mZ >> SPC);
            // Calculate next W.
            mW = WMC * (mW & LAC) + (mW >> SPC);
            // Concatenate Z and W and return the value.
            this->mCurrent = (mZ << SPC) + mW;
            return this->mCurrent;
        }

        double NextFloatingPoint() {

            return (double) this->NextInteger()/(RMax + 1.0);
        }
    };


    class MarsagliaTripleShiftRegisterGenerator
    {
    private:
        constexpr static const auto  Sh1 = 17;
        constexpr static const auto Sh2 = 13;
        constexpr static const auto Sh3 = 5;
        constexpr static const auto Def = 123456789;
        constexpr static const auto RMax = 4294967295UL;
        unsigned long mSeed;
        unsigned long mCurrent;
    public:


        constexpr MarsagliaTripleShiftRegisterGenerator(const unsigned long & seed = {}) noexcept :mSeed{seed},mCurrent{seed}
        {

        }
        constexpr unsigned long NextInteger() noexcept
        {

            // First shift & xor.
            this->mCurrent = this->mCurrent ^ (this->mCurrent << Sh1);
            // Second shift & xor.
            this->mCurrent = this->mCurrent ^ (this->mCurrent >> Sh2);
            // Third shift & xor.
            this->mCurrent = this->mCurrent ^ (this->mCurrent << Sh3);

            return this->mCurrent;
        }

        constexpr double NextFloatingPoint() {

            return (double) this->NextInteger()/(RMax + 1.0);
        }
    };

    class MarsagliaCongruentialGenerator
    {

    private:
        constexpr static const auto Aval = 69069;
        constexpr static const auto CVal = 1234567;
        constexpr static const auto Def = 380116160;
        constexpr static const auto RMax = 4294967295UL;

        unsigned long mSeed;
        unsigned long mCurrent;
    public:
        constexpr MarsagliaCongruentialGenerator(const unsigned long & seed = {}) noexcept :mSeed{seed},mCurrent{seed}
        {

        }

        constexpr unsigned long NextInteger() noexcept {

            // The mod operation is not done, 32 bit overflow is exploited.
            this->mCurrent = Aval*this->mCurrent + CVal;
            return this->mCurrent;
        }

        constexpr double NextFloatingPoint() noexcept {

            this->mCurrent = Aval*this->mCurrent + CVal;
            return (double) this->mCurrent/(RMax + 1.0);
        }
    };


    class MarsagliaLaggedFibonacci4Generator
    {
    private:
        constexpr static const auto Lag1 = (unsigned char)58;
        constexpr static const auto Lag2 = (unsigned char)119;
        constexpr static const auto Lag3 = (unsigned char)178;
        constexpr static const auto RMax = 4294967295UL;

        // Add the table of random numbers and its index.
        unsigned long t[256];
        unsigned char c;
    public:
    };

}
#endif