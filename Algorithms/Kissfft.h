//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_KISSFFT_H
#define LANDESSDEVCORE_KISSFFT_H
/*
#include <complex>
#include <utility>
#include <vector>
#include "Array.h"
#include "Complex.h"
#include "Per.h"
#include "Exp.h"
template <typename scalar_t>
class KissFFT
{
public:

    using cpx_t = PDP::Complex<scalar_t>;

    KissFFT( const PDP::UInteger nfft,
             const bool inverse )
            :_nfft(nfft)
            ,_inverse(inverse)
    {
        // fill twiddle factors
        _twiddles.Reserve(_nfft);


        const scalar_t phinc =  (_inverse?2:-2)* PDP::ACos( (scalar_t) -1)  / _nfft;


        for (PDP::UInteger i=0;i<_nfft;++i)
        {
            _twiddles[i] = PDP::Exp( cpx_t(0,i*phinc) );
        }


        //factorize
        //start factoring out 4's, then 2's, then 3,5,7,9,...
        PDP::UInteger n= _nfft;
        PDP::UInteger p=4;
        do {
            while (n % p) {
                switch (p) {
                    case 4: p = 2; break;
                    case 2: p = 3; break;
                    default: p += 2; break;
                }
                if (p*p>n)
                    p = n;// no more factors
            }
            n /= p;
            _stageRadix.PushBack(p);
            _stageRemainder.PushBack(n);
        }while(n>1);
    }


    /// Changes the FFT-length and/or the transform direction.
    ///
    /// @post The @c kissfft object will be in the same state as if it
    /// had been newly constructed with the passed arguments.
    /// However, the implementation may be faster than constructing a
    /// new fft object.


    //void assign( const std::size_t nfft,const bool inverse )
    //{
        //if ( nfft != _nfft )
        //{
            //kissfft tmp( nfft, inverse ); // O(n) time.
            //std::swap( tmp, *this ); // this is O(1) in C++11, O(n) otherwise.
        //}
        //else if ( inverse != _inverse )
        //{
            // conjugate the twiddle factors.
            //for ( typename std::vector<cpx_t>::iterator it = _twiddles.begin();
              //    it != _twiddles.end(); ++it )
                //it->imag( -it->imag() );
        //}
    //}


    /// Calculates the complex Discrete Fourier Transform.
    ///
    /// The size of the passed arrays must be passed in the constructor.
    /// The sum of the squares of the absolute values in the @c dst
    /// array will be @c N times the sum of the squares of the absolute
    /// values in the @c src array, where @c N is the size of the array.
    /// In other words, the l_2 norm of the resulting array will be
    /// @c sqrt(N) times as big as the l_2 norm of the input array.
    /// This is also the case when the inverse flag is set in the
    /// constructor. Hence when applying the same transform twice, but with
    /// the inverse flag changed the second time, then the result will
    /// be equal to the original input times @c N.
    void transform(const cpx_t * fft_in, cpx_t * fft_out, const PDP::UInteger stage = 0, const PDP::UInteger fstride = 1, const PDP::UInteger in_stride = 1) const
    {
        const PDP::UInteger p = _stageRadix[stage];
        const PDP::UInteger m = _stageRemainder[stage];
        cpx_t * const Fout_beg = fft_out;
        cpx_t * const Fout_end = fft_out + p*m;

        if (m==1)
        {
            do
            {

                *fft_out = *fft_in;
                fft_in += fstride*in_stride;
            }while(++fft_out != Fout_end );
        }else
        {
            do
            {
                // recursive call:
                // DFT of size m*p performed by doing
                // p instances of smaller DFTs of size m,
                // each one takes a decimated version of the input

                transform(fft_in, fft_out, stage+1, fstride*p,in_stride);

                fft_in += fstride*in_stride;

            }while( (fft_out += m) != Fout_end );
        }

        fft_out=Fout_beg;

        // recombine the p smaller DFTs
        switch (p)
        {
            case 2: kf_bfly2(fft_out,fstride,m); break;
            case 3: kf_bfly3(fft_out,fstride,m); break;
            case 4: kf_bfly4(fft_out,fstride,m); break;
            case 5: kf_bfly5(fft_out,fstride,m); break;
            default: kf_bfly_generic(fft_out,fstride,m,p); break;
        }
    }

    /// Calculates the Discrete Fourier Transform (DFT) of a real input
    /// of size @c 2*N.
    ///
    /// The 0-th and N-th value of the DFT are real numbers. These are
    /// stored in @c dst[0].real() and @c dst[1].imag() respectively.
    /// The remaining DFT values up to the index N-1 are stored in
    /// @c dst[1] to @c dst[N-1].
    /// The other half of the DFT values can be calculated from the
    /// symmetry relation
    ///     @code
    ///         DFT(src)[2*N-k] == conj( DFT(src)[k] );
    ///     @endcode
    /// The same scaling factors as in @c transform() apply.
    ///
    /// @note For this to work, the types @c scalar_t and @c cpx_t
    /// must fulfill the following requirements:
    ///
    /// For any object @c z of type @c cpx_t,
    /// @c reinterpret_cast<scalar_t(&)[2]>(z)[0] is the real part of @c z and
    /// @c reinterpret_cast<scalar_t(&)[2]>(z)[1] is the imaginary part of @c z.
    /// For any pointer to an element of an array of @c cpx_t named @c p
    /// and any valid array index @c i, @c reinterpret_cast<T*>(p)[2*i]
    /// is the real part of the complex number @c p[i], and
    /// @c reinterpret_cast<T*>(p)[2*i+1] is the imaginary part of the
    /// complex number @c p[i].
    ///
    /// Since C++11, these requirements are guaranteed to be satisfied for
    /// @c scalar_ts being @c float, @c double or @c long @c double
    /// together with @c cpx_t being @c std::complex<scalar_t>.
    void transform_real( const scalar_t * const src,
                         cpx_t * const dst ) const
    {
        const PDP::UInteger N = _nfft;
        if ( N == 0 )
            return;

        // perform complex FFT
        transform( reinterpret_cast<const cpx_t*>(src), dst );

        // post processing for k = 0 and k = N
        dst[0] = cpx_t( dst[0].GetReal() + dst[0].GetImaginary(),
                        dst[0].GetReal() - dst[0].GetImaginary() );

        // post processing for all the other k = 1, 2, ..., N-1
        const scalar_t pi = acos( (scalar_t) -1);

        const scalar_t half_phi_inc = ( _inverse ? pi : -pi ) / N;


        const cpx_t twiddle_mul = PDP::Exp( cpx_t(0, half_phi_inc) );

        for ( PDP::UInteger k = 1; 2*k < N; ++k )
        {
            const cpx_t w = (scalar_t)0.5 * cpx_t(
                    dst[k].GetReal() + dst[N-k].GetReal(),
                    dst[k].GetImaginary() - dst[N-k].GetImaginary() );
            const cpx_t z = (scalar_t)0.5 * cpx_t(
                    dst[k].GetImaginary() + dst[N-k].GetImaginary(),
                    -dst[k].GetReal() + dst[N-k].GetReal() );
            const cpx_t twiddle =
                    k % 2 == 0 ?
                    _twiddles[k/2] :
                    _twiddles[k/2] * twiddle_mul;
            dst[  k] =       w + twiddle * z;
            //dst[N-k] = conj( w - twiddle * z );
            dst[N-k] = (w-twiddle * z).Conjugate();
        }
        if ( N % 2 == 0 )
        {
            //dst[N/2] = conj( dst[N/2] );
            dst[N/2] = dst[N/2].Conjugate();
        }

    }

private:

    void kf_bfly2( cpx_t * Fout, const PDP::UInteger fstride, const PDP::UInteger m) const
    {
        for (PDP::UInteger k=0;k<m;++k)
        {
            //possible spot for SIMD
            const cpx_t t = Fout[m+k] * _twiddles[k*fstride];

            //possible spot for SIMD
            Fout[m+k] = Fout[k] - t;
            Fout[k] += t;
        }
    }

    void kf_bfly3( cpx_t * Fout, const PDP::UInteger fstride, const PDP::UInteger m) const
    {
        PDP::UInteger k=m;
        const PDP::UInteger m2 = 2*m;
        const cpx_t *tw1,*tw2;
        cpx_t scratch[5];
        const cpx_t epi3 = _twiddles[fstride*m];

        tw1=tw2=&_twiddles[0];

        do{
            scratch[1] = Fout[m]  * *tw1;
            scratch[2] = Fout[m2] * *tw2;

            scratch[3] = scratch[1] + scratch[2];
            scratch[0] = scratch[1] - scratch[2];
            tw1 += fstride;
            tw2 += fstride*2;

            Fout[m] = Fout[0] - scratch[3]*scalar_t(0.5);
            scratch[0] *= epi3.GetImaginary();

            Fout[0] += scratch[3];

            Fout[m2] = cpx_t(  Fout[m].GetReal() + scratch[0].GetImaginary() , Fout[m].GetImaginary() - scratch[0].GetReal() );

            Fout[m] += cpx_t( -scratch[0].GetImaginary(),scratch[0].GetReal() );
            ++Fout;
        }while(--k);
    }

    void kf_bfly4( cpx_t * const Fout, const PDP::UInteger fstride, const PDP::UInteger m) const
    {
        cpx_t scratch[7];
        const scalar_t negative_if_inverse = _inverse ? -1 : +1;
        for (PDP::UInteger k=0;k<m;++k)
        {
            //possible SIMD zone
            scratch[0] = Fout[k+  m] * _twiddles[k*fstride  ];
            scratch[1] = Fout[k+2*m] * _twiddles[k*fstride*2];
            scratch[2] = Fout[k+3*m] * _twiddles[k*fstride*3];


            scratch[5] = Fout[k] - scratch[1];

            Fout[k] += scratch[1];
            scratch[3] = scratch[0] + scratch[2];
            scratch[4] = scratch[0] - scratch[2];
            scratch[4] = cpx_t( scratch[4].GetImaginary()*negative_if_inverse ,
                                -scratch[4].GetReal()*negative_if_inverse );

            Fout[k+2*m]  = Fout[k] - scratch[3];
            Fout[k    ]+= scratch[3];
            Fout[k+  m] = scratch[5] + scratch[4];
            Fout[k+3*m] = scratch[5] - scratch[4];
        }
    }

    void kf_bfly5( cpx_t * const Fout, const PDP::UInteger fstride, const PDP::UInteger m) const
    {
        cpx_t *Fout0,*Fout1,*Fout2,*Fout3,*Fout4;
        cpx_t scratch[13];
        const cpx_t ya = _twiddles[fstride*m];
        const cpx_t yb = _twiddles[fstride*2*m];

        Fout0=Fout;

        //Possible SIMD zone
        Fout1=Fout0+m;
        Fout2=Fout0+2*m;
        Fout3=Fout0+3*m;
        Fout4=Fout0+4*m;

        for ( PDP::UInteger u=0; u<m; ++u ) {
            scratch[0] = *Fout0;

            //possible SIMD zone
            scratch[1] = *Fout1 * _twiddles[  u*fstride];
            scratch[2] = *Fout2 * _twiddles[2*u*fstride];
            scratch[3] = *Fout3 * _twiddles[3*u*fstride];
            scratch[4] = *Fout4 * _twiddles[4*u*fstride];

            scratch[7] = scratch[1] + scratch[4];
            scratch[10]= scratch[1] - scratch[4];
            scratch[8] = scratch[2] + scratch[3];
            scratch[9] = scratch[2] - scratch[3];

            *Fout0 += scratch[7];
            *Fout0 += scratch[8];

            scratch[5] = scratch[0] + cpx_t(
                    scratch[7].GetReal()*ya.GetReal() + scratch[8].GetReal()*yb.GetReal(),
                    scratch[7].GetImaginary()*ya.GetReal() + scratch[8].GetImaginary()*yb.GetReal()
            );

            scratch[6] =  cpx_t(
                    scratch[10].GetImaginary()*ya.GetImaginary() + scratch[9].GetImaginary()*yb.GetImaginary(),
                    -scratch[10].GetReal()*ya.GetImaginary() - scratch[9].GetReal()*yb.GetImaginary()
            );

            *Fout1 = scratch[5] - scratch[6];
            *Fout4 = scratch[5] + scratch[6];

            scratch[11] = scratch[0] +
                          cpx_t(
                                  scratch[7].GetReal()*yb.GetReal() + scratch[8].GetReal()*ya.GetReal(),
                                  scratch[7].GetImaginary()*yb.GetReal() + scratch[8].GetImaginary()*ya.GetReal()
                          );

            scratch[12] = cpx_t(
                    -scratch[10].GetImaginary()*yb.GetImaginary() + scratch[9].GetImaginary()*ya.GetImaginary(),
                    scratch[10].GetReal()*yb.GetImaginary() - scratch[9].GetReal()*ya.GetImaginary()
            );

            *Fout2 = scratch[11] + scratch[12];
            *Fout3 = scratch[11] - scratch[12];

            ++Fout0;
            ++Fout1;
            ++Fout2;
            ++Fout3;
            ++Fout4;
        }
    }

    // perform the butterfly for one stage of a mixed radix FFT
    void kf_bfly_generic(
            cpx_t * const Fout,
            const PDP::UInteger fstride,
            const PDP::UInteger m,
            const PDP::UInteger p
    ) const
    {
        const cpx_t * twiddles = &_twiddles[0];
        PDP::DataStructures::Array<cpx_t> scratchbuf;
        scratchbuf.Reserve(p);
        //cpx_t scratchbuf[p];

        for ( PDP::UInteger u=0; u<m; ++u )
        {
            PDP::UInteger k = u;
            for ( PDP::UInteger q1=0 ; q1<p ; ++q1 )
            {
                scratchbuf[q1] = Fout[ k  ];
                k += m;
            }

            k=u;
            for ( PDP::UInteger q1=0 ; q1<p ; ++q1 )
            {
                PDP::UInteger twidx=0;
                Fout[ k ] = scratchbuf[0];
                for ( PDP::UInteger q=1;q<p;++q )
                {

                    twidx += fstride * k;
                    if (twidx>=_nfft)
                    {
                        twidx-=_nfft;
                    }

                    //possible SIMD zone
                    Fout[ k ] += scratchbuf[q] * twiddles[twidx];
                }
                k += m;
            }
        }
    }

    PDP::UInteger _nfft;
    bool _inverse;

    PDP::DataStructures::Array<cpx_t> _twiddles;
    PDP::DataStructures::Array<PDP::UInteger> _stageRadix;
    PDP::DataStructures::Array<PDP::UInteger> _stageRemainder;
};
 */
#endif //LANDESSDEVCORE_KISSFFT_H
