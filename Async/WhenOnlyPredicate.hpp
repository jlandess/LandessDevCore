//
// Created by phoenixflower on 9/15/21.
//

#ifndef LANDESSDEVCORE_WHENONLYPREDICATE_HPP
#define LANDESSDEVCORE_WHENONLYPREDICATE_HPP
#include "Primitives/General/Tuple.hpp"
namespace LD
{
    namespace Async
    {
        template<typename ... T>
        class WhenAnyPredicate
        {
        private:
            LD::Tuple<T...> mValue;
        public:

            using PredicateType = LD::Tuple<T...>;

            WhenAnyPredicate() noexcept
            {

            }

            WhenAnyPredicate(const LD::Tuple<T...> & value) noexcept:mValue{value}
            {

            }

            const LD::Tuple<T...> & Value() const noexcept
            {
                return this->mValue;
            }
        };


    }
}
#endif //LANDESSDEVCORE_WHENONLYPREDICATE_HPP
