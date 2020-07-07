//
// Created by phoenixflower on 7/2/20.
//

#ifndef LANDESSDEVCORE_FULLADDER_HPP
#define LANDESSDEVCORE_FULLADDER_HPP
#include "Definitions/Integer.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/BitSet.hpp"
namespace LD
{
    template<LD::UInteger Bits>
    class LookAheadGenerator
    {
    private:

    public:
        LookAheadGenerator()
        {

        }

        constexpr LD::BitSet<Bits> operator()(
                const LD::BitSet<Bits> & a,
                const LD::BitSet<Bits> & b,
                const bool & cinInput = false) const noexcept
        {

            bool cin = cinInput;
            LD::BitSet<Bits> cg = {false};
            LD::BitSet<Bits> cp = {false};
            LD::BitSet<Bits> cinArray = {false};

            for(LD::UInteger  i=Bits-1, n = 0;i!=0;i--,++n)
            {
                cinArray[i] = cin;
                cg[i] = ((a[i]) and (b[i]));// Gi = Ai . Bi
                cp[i] = ((a[i]) or (b[i]));// Pi = Ai (+) Bi
                cin = ((cg[i]) or ((cp[i]) and cin));// Cin =Gi + PiCi
            }
            return cinArray;
        }
        constexpr LD::ImmutableArray<unsigned char,Bits> operator()(const LD::ImmutableArray<unsigned char,Bits> & a,
                                                                    const LD::ImmutableArray<unsigned char,Bits> & b,
                                                                    const bool & cinInput = false) const noexcept
        {

            bool cin = cinInput;
            LD::ImmutableArray<unsigned char,Bits> cg = {'0'};
            LD::ImmutableArray<unsigned char,Bits> cp = {'0'};
            LD::ImmutableArray<unsigned char,Bits> cinArray = {'0'};

            for(LD::UInteger  i=Bits-1, n = 0;i!=0;i--,++n)
            {
                cinArray[i] = '0'+cin;
                cg[i] = '0'+((a[i] == '1') and (b[i] == '1'));// Gi = Ai . Bi
                cp[i] = '0'+((a[i] == '1') or (b[i] == '1'));// Pi = Ai (+) Bi
                cin = ((cg[i] == '1') or ((cp[i] == '1') and cin));// Cin =Gi + PiCi
            }

            return cinArray;
        }
    };
    template<LD::UInteger Bits>
    class FullAdder
    {
    private:
        //LD::ImmutableArray<unsigned char,Bits> mResult;
    public:

        constexpr FullAdder() noexcept //:mResult({'0'})
        {


        }

        constexpr LD::BitSet<Bits> operator()(const LD::BitSet<Bits> & a,
                                              const LD::BitSet<Bits> & b,
                                              const bool & value = false) noexcept
        {


            LD::BitSet<Bits> cin= {false};
            LD::BitSet<Bits> mResult = {false};
            cin[0] = value;
            constexpr const LD::UInteger ax = Bits;
            constexpr const LD::UInteger bx = Bits;
            for(int i = 0; i < Bits; i++)
            {
                //LD::UInteger x = 0,y = 0;
                LD::UInteger c = cin[i];
                LD::UInteger x = (ax-1-i < 0)*0 + (!(ax-1-i < 0))*((a[ax-1-i]));
                LD::UInteger y = (bx-1-i < 0)*0 + (!(bx-1-i < 0))*(b[bx-1-i]);
                //if(ax-1-i < 0) x = 0;
                //else x = (int)(a[ax-1-i]-'0');
                //if(bx-1-i < 0) y = 0;
                //else y = (int)(b[bx-1-i]-'0');

                //s = (char)(x xor y xor c + '0') + s;
                mResult[Bits-i-1] = (x xor y xor c);
                cin[i+1] = ((x&y)|(y&c)|(x&c));
            }
            return mResult;
        }

        constexpr LD::ImmutableArray<unsigned char,Bits>  operator()(const LD::ImmutableArray<unsigned char,Bits> & a,
                                                          const LD::ImmutableArray<unsigned char,Bits> & b,
                                                          const bool & carryin = 0) const noexcept
        {

            LD::ImmutableArray<unsigned char,Bits> mResult;
            //LD::ImmutableArray<unsigned char,Bits> ret = {'0'};
            constexpr const LD::UInteger ax = Bits;
            constexpr const LD::UInteger bx = Bits;
            LD::ImmutableArray<unsigned char,Bits> cin= {'0'};
            cin[0] = '0'+carryin;
            if constexpr(Bits < 1024)//do at compile time if we can get away with it
            {
                LD::For<Bits>([](
                        auto I,
                        LD::ImmutableArray<unsigned char,Bits> & cin,
                        LD::ImmutableArray<unsigned char,Bits> & ret,
                        const LD::ImmutableArray<unsigned char,Bits> & a,
                        const LD::ImmutableArray<unsigned char,Bits> & b)
                {

                    constexpr const LD::UInteger ax = Bits;
                    constexpr const LD::UInteger bx = Bits;
                    LD::UInteger c = cin[I]-'0';
                    LD::UInteger x = (ax-1-I < 0)*0 + (!(ax-1-I < 0))*((a[ax-1-I]-'0'));
                    LD::UInteger y = (bx-1-I < 0)*0 + (!(bx-1-I < 0))*(b[bx-1-I]-'0');
                    ret[Bits-I-1] = (char)(x xor y xor c + '0');
                    cin[I+1] = ((x&y)|(y&c)|(x&c) + '0');
                    return true;
                },cin,mResult,a,b);

            }else
            {
                for(int i = 0; i < Bits; i++)
                {
                    //LD::UInteger x = 0,y = 0;
                    LD::UInteger c = cin[i]-'0';
                    LD::UInteger x = (ax-1-i < 0)*0 + (!(ax-1-i < 0))*((a[ax-1-i]-'0'));
                    LD::UInteger y = (bx-1-i < 0)*0 + (!(bx-1-i < 0))*(b[bx-1-i]-'0');
                    //if(ax-1-i < 0) x = 0;
                    //else x = (int)(a[ax-1-i]-'0');
                    //if(bx-1-i < 0) y = 0;
                    //else y = (int)(b[bx-1-i]-'0');

                    //s = (char)(x xor y xor c + '0') + s;
                    mResult[Bits-i-1] = (char)(x xor y xor c + '0');
                    cin[i+1] = ((x&y)|(y&c)|(x&c) + '0');
                }
            }
            /*
            for(int i = 0; i < Bits; i++)
            {
                //LD::UInteger x = 0,y = 0;
                LD::UInteger c = cin[i]-'0';
                LD::UInteger x = (ax-1-i < 0)*0 + (!(ax-1-i < 0))*((a[ax-1-i]-'0'));
                LD::UInteger y = (bx-1-i < 0)*0 + (!(bx-1-i < 0))*(b[bx-1-i]-'0');
                //if(ax-1-i < 0) x = 0;
                //else x = (int)(a[ax-1-i]-'0');
                //if(bx-1-i < 0) y = 0;
                //else y = (int)(b[bx-1-i]-'0');

                //s = (char)(x xor y xor c + '0') + s;
                ret[Bits-i-1] = (char)(x xor y xor c + '0');
                cin[i+1] = ((x&y)|(y&c)|(x&c) + '0');
            }
             */
            return mResult;
        }

        constexpr LD::BitSet<Bits>  operator()(
                const LD::BitSet<Bits> & a,
                const LD::BitSet<Bits> & b,
                const LD::LookAheadGenerator<Bits> & generatoredCin) const noexcept
        {
            LD::BitSet<Bits> mResult;

            constexpr auto cinArray = generatoredCin(a,b);
            if constexpr(Bits < 1024)
            {
                LD::For<Bits>([](
                        auto I,
                        const LD::BitSet<Bits> & a,
                        const LD::BitSet<Bits> & b,
                        const LD::BitSet<Bits> & cin,
                        LD::BitSet<Bits> & result)
                {
                    //bool index = ((cin[I]) xor ((a[I]) xor (b[I])));
                    result[I] = ((cin[I]) xor ((a[I]) xor (b[I])));;
                    return true;
                },a,b,cinArray,mResult);

            }else
            {

                for(LD::UInteger i = 0; i < Bits; i++)
                {
                    //bool index = ((cinArray[i]) xor ((a[i]) xor (b[i])));
                    mResult[i] = ((cinArray[i]) xor ((a[i]) xor (b[i])));
                }
            }

            return mResult;
        }
        constexpr LD::ImmutableArray<unsigned char,Bits>  operator()(const LD::ImmutableArray<unsigned char,Bits> & a,
                                                                      const LD::ImmutableArray<unsigned char,Bits> & b,
                                                                      const LD::LookAheadGenerator<Bits> & generatoredCin) const noexcept
        {

            LD::ImmutableArray<unsigned char,Bits> mResult;
            //LD::ImmutableArray<unsigned char,Bits> ret = {'0'};
            //constexpr const LD::UInteger ax = Bits;
            //constexpr const LD::UInteger bx = Bits;
            LD::ImmutableArray<unsigned char,Bits> cin= {'0'};
            if constexpr(Bits < 1024)//do at compile time if we can get away with it
            {
                LD::For<Bits>([](
                        auto I,
                        const LD::ImmutableArray<unsigned char,Bits> & cin,
                        LD::ImmutableArray<unsigned char,Bits> & ret,
                        const LD::ImmutableArray<unsigned char,Bits> & a,
                        const LD::ImmutableArray<unsigned char,Bits> & b)
                {
                    bool index = ((cin[I] == '1') xor ((a[I] == '1') xor (b[I] == '1')));
                    ret[I] = (index)*'1' + (!index)*'0';
                    return true;

                },generatoredCin(a,b),mResult,a,b);

            }else
            {
                constexpr auto cinArray = generatoredCin(a,b);
                for(LD::UInteger i = 0; i < Bits; i++)
                {
                    bool index = ((cinArray[i] == '1') xor ((a[i] == '1') xor (b[i] == '1')));
                    mResult[i] = (index)*'1' + (!index)*'0';
                }
            }
            return mResult;
        }
    };
}
/**
 * template<LD::UInteger Bits>
LD::ImmutableArray<unsigned char,Bits> CarryLookAheadExample(const LD::ImmutableArray<unsigned char,Bits> & aInput, const LD::ImmutableArray<unsigned char,Bits> & bInput, const bool & cinInput = false) noexcept
{
    bool cin = cinInput;
    LD::ImmutableArray<unsigned char,Bits> sum = {'0'};
    LD::ImmutableArray<unsigned char,Bits> cg = {'0'};
    LD::ImmutableArray<unsigned char,Bits> cp = {'0'};
    LD::ImmutableArray<unsigned char,Bits> cinArray = {'0'};

    //generate the lookahead
    for(LD::UInteger  i=Bits-1, n = 0;i!=0;i--,++n)
    {
        cinArray[i] = '0'+cin;
        cg[i] = '0'+((aInput[i] == '1') and (bInput[i] == '1'));// Gi = Ai . Bi
        cp[i] = '0'+((aInput[i] == '1') or (bInput[i] == '1'));// Pi = Ai (+) Bi
        cin = ((cg[i] == '1') or ((cp[i] == '1') and cin));// Cin =Gi + PiCi
    }

    //perform the addition
    for(LD::UInteger  i=Bits-1, n = 0;i!=0;i--,++n)
    {

        bool index = ((cinArray[i] == '1') xor ((aInput[i] == '1') xor (bInput[i] == '1')));
        //std::cout << index << std::endl;
        sum[i] = (index)*'1' + (!index)*'0';
        //cg[i]=and(n1[i],n2[i]); // Gi = Ai . Bi
        //cg[i] = '0'+((aInput[i] == '1') and (bInput[i] == '1'));
        //cp[i] = '0'+((aInput[i] == '1') or (bInput[i] == '1'));
    }
    return sum;
}
 */
#endif //LANDESSDEVCORE_FULLADDER_HPP
