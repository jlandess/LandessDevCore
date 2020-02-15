#ifndef DataStructures_Float_h
#define DataStructures_Float_h


namespace PDP
{
    
#if defined(_WIN64) || defined(__LP64__)
    
    typedef double Float;
    typedef float Float32;
    typedef double Float64;
    
#else
    typedef float Float;
    typedef float Float32;
    typedef double Float64;
    
#endif
}

namespace LD
{

#if defined(_WIN64) || defined(__LP64__)

    typedef double Float;
    typedef float Float32;
    typedef double Float64;

#else
    typedef float Float;
    typedef float Float32;
    typedef double Float64;

#endif
}

#endif