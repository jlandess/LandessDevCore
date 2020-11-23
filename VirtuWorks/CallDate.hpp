//
// Created by phoenixflower on 11/20/20.
//

#ifndef LANDESSDEVCORE_CALLDATE_HPP
#define LANDESSDEVCORE_CALLDATE_HPP
#include "Primitives/General/DateTime.h"
#include "IO/FetchRequest.h"
namespace LD
{
    namespace VW
    {
        class CallDate
        {
        private:
            LD::Date mDate;
        public:
            CallDate(const LD::Date & date) noexcept:mDate{date}
            {

            }

            const LD::Date & Date() const noexcept
            {
                return this->mDate;
            }
        };
    }

}
#endif //LANDESSDEVCORE_CALLDATE_HPP
