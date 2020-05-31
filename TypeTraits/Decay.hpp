//
// Created by James Landess on 2/4/20.
//

#ifndef LANDESSDEVCORE_DECAY_HPP
#define LANDESSDEVCORE_DECAY_HPP

#include "RemoveReference.hpp"
#include "Conditional.hpp"
#include "IsArray.hpp"
#include "RemoveExtent.hpp"
#include "IsFunction.hpp"
#include "AddPointer.hpp"
#include "RemoveCV.hpp"
namespace LD
{
    namespace Detail
    {
        template< class T >
        struct Decay {
        private:
            typedef typename LD::Detail::RemoveReference<T>::type U;
        public:
            typedef typename LD::Detail::Conditonal<
                    LD::Detail::IsArray<U>::value,
                    typename LD::Detail::RemoveExtent<U>::type*,
                    typename LD::Detail::Conditonal<
                            LD::Detail::IsFunction<U>::value,
                            typename LD::Detail::add_pointer<U>::type,
                            typename LD::Detail::RemoveCV<U>::type
                    >::type
            >::type type;
        };

    }

    template< class T >
    struct Decay {
    private:
        typedef typename LD::Detail::RemoveReference<T>::type U;
    public:
        typedef typename LD::Detail::Conditonal<
                LD::Detail::IsArray<U>::value,
                typename LD::Detail::RemoveExtent<U>::type*,
                typename LD::Detail::Conditonal<
                        LD::Detail::IsFunction<U>::value,
                        typename LD::Detail::add_pointer<U>::type,
                        typename LD::Detail::RemoveCV<U>::type
                >::type
        >::type type;
    };

//#if _LIBCPP_STD_VER > 11

//#endif
}

namespace LD
{
    namespace Detail
    {
        template <class _Tp> using Decay_T = typename LD::Detail::Decay<_Tp>::type;
    }
}
#endif //LANDESSDEVCORE_DECAY_HPP
