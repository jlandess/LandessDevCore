//
//  Pair.h
//  DataStructures
//
//  Created by James Landess on 9/5/15.
//  Copyright (c) 2015 James Landess. All rights reserved.
//

#ifndef DataStructures_Pair_h
#define DataStructures_Pair_h
#include "TypeTraits/Type.h"
namespace LD
{
    /**
         @addtogroup DataStructures
         */

    /**
     @brief The implementation of the pair data structure, allows one to return two data types at the same time without creating either a C Array or invoking array based datastructures
     @tparam T - The first type of object you want to return
     @tparam Q - The second type of object you want to return
     @code
     #include <PostponedProductions/DataStructures/Pair.h>
     int main()
     {
        PDP::DataStructures::Pair<int,double> pair;
        return 0;
     }
     @endcode
     */
    template<typename T,typename Q> class Pair
    {
    private:
        T First;
        Q Second;

    public:

        inline Pair();
        inline Pair(const T & first, const Q & second);

        inline T & GetFirst();
        inline Q & GetSecond() ;

        inline const T & GetFirst() const;
        inline const Q & GetSecond() const;

        inline void SetFirst(const T & first);
        inline void SetSecond(const Q & second);
    };
    /**
     @}
     */

    template<typename T, typename Q> Pair<T,Q>::Pair(){}



    template<typename T, typename Q> Pair<T,Q>::Pair(const T & first, const Q & second):First(first),Second(second){}


    template<typename T,typename Q> T & Pair<T,Q>::GetFirst(){return this->First;}

    template<typename T, typename Q> Q & Pair<T,Q>::GetSecond(){return this->Second;}

    template<typename T,typename Q> const T & Pair<T,Q>::GetFirst() const {return this->First;}

    template<typename T, typename Q> const Q & Pair<T,Q>::GetSecond() const {return this->Second;}

    template<typename T, typename Q> void Pair<T,Q>::SetFirst(const T & first){this->First = first;}

    template<typename T, typename Q> void Pair<T,Q>::SetSecond(const Q & second){this->Second = second;}

    template<typename T,typename V> Pair(T &&, V &&) -> Pair<T,V>;

    namespace CT
    {
        template<typename T, typename U>
        constexpr LD::Type<T> Key(LD::Type<LD::Pair<T,U>> ) noexcept
        {
            return LD::Type<T>{};
        }

        template<typename T, typename U>
        constexpr LD::Type<U> Value(LD::Type<LD::Pair<T,U>> ) noexcept
        {
            return LD::Type<U>{};
        }
    }

}


#endif