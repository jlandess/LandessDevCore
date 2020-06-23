
#include "Random.h"



namespace LD
{
    static const LD::UInteger A = 48271;
    static const LD::UInteger M = MaximumRandom<LD::Integer>::GetMaximum();
    static const LD::UInteger Q = M / A;
    static const LD::UInteger R = M % A;

    Random::Random( LD::UInteger initialValue ) noexcept
    {


        State = (initialValue < 0)?initialValue+M:initialValue;
        State = (State == 0)?1:State;
    }

    /**
     * Return a pseudorandom int, and change the
     * internal state.
     */
    LD::UInteger Random::NextInt() noexcept
    {
        LD::UInteger tmpState = A * ( State % Q ) - R * ( State / Q );

        if( tmpState >= 0 )
            State = tmpState;
        else
            State = tmpState + M;
        return State;
    }


    /**
     * Return a pseudorandom double in the open range 0..1
     * and change the internal state.
     */
    LD::Float Random::NextFloat( ) noexcept
    {
        return (LD::Float) NextInt() / (LD::Float)M;
    }

    LD::Float Random::NextFloat( LD::UInteger low, LD::UInteger high) noexcept
    {
        return (LD::Float) NextInt(low,high) / (LD::Float)M;
    }

    /**
     * Return an int in the closed range [low,high], and
     * change the internal state.
     */
    LD::UInteger Random::NextInt( LD::UInteger low, LD::UInteger high ) noexcept
    {
        LD::Float partitionSize = (LD::Float) M / ( high - low + 1 );

        return (LD::Integer) ( NextInt( ) / partitionSize ) + low;
    }
    LD::UInteger Random::GetState() const noexcept
    {
        return this->State;
    }
}
/**
 * Construct with initialValue for the state.
 */
