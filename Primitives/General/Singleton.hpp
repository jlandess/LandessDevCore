//
//  Singleton.h
//  DataStructures
//
//  Created by James Landess on 4/28/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef DataStructures_Singleton_h
#define DataStructures_Singleton_h
//#include "Definitions/Common.hpp"
namespace LD
{
    /**
        @addtogroup Patterns
        */
    /**
     @brief
     You can call the constructor as many times as you see fit, but you can only call the Release method once throughout the entire program to keep the semantics simple.  This object utilizes lazy insansiation.  It can be threadsafe if and only if the allocation sequence is thread-safe.  By default it is not thread-safe.
     @tparam T the Data to be used within the Singleton
     @tparam Alloc - The allocation mechanics
     @code
     void DiscussionMethod()
     {
     Patterns::Singleton<int> sameSingletonAsCurrentSingleton;
     }
     in main(const char ** arguements, const int argcv)
     {
     Patterns::Singleton<int> currentSingleton;
     }
     @endcode

     */
    template<typename T>
    class Singleton
    {
    public:

        template<typename ... A>
         static T & GetInstance(A && ... a) noexcept
        {
            static T    instance{LD::Forward<A>(a)...}; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
    private:
        inline constexpr Singleton() noexcept
        {

        }

        // C++ 03
        // ========
        // Dont forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        //S(S const&);              // Don't Implement
        //void operator=(S const&); // Don't implement

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    public:
        Singleton(Singleton const&)               = delete;
        void operator=(Singleton const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
    };
}
#endif