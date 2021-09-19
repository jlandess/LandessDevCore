//
// Created by phoenixflower on 9/15/21.
//

#ifndef LANDESSDEVCORE_WHENALLPREDICATE_HPP
#define LANDESSDEVCORE_WHENALLPREDICATE_HPP
#include "Primitives/General/Tuple.hpp"
namespace LD
{
    namespace Async
    {
        template<typename ... T>
        class WhenAllPredicate
        {
        private:
            LD::Tuple<T...> mValue;
        public:

            using PredicateType = LD::Tuple<T...>;

            WhenAllPredicate() noexcept
            {

            }

            WhenAllPredicate(const LD::Tuple<T...> & value) noexcept:mValue{value}
            {

            }

            const LD::Tuple<T...> & Value() const noexcept
            {
                return this->mValue;
            }
        };


        namespace CT
        {

        }
    }
}
#endif //LANDESSDEVCORE_WHENALLPREDICATE_HPP
