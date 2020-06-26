//
// Created by phoenixflower on 6/25/20.
//

#ifndef LANDESSDEVCORE_FUZZYSEARCH_H
#define LANDESSDEVCORE_FUZZYSEARCH_H
#include "TypeTraits/Limit.hpp"
#include "Definitions/Float.hpp"
#include "Definitions/Integer.hpp"
#include "Primitives/General/StringView.hpp"
#include <ctype.h>
namespace LD
{
    inline LD::Float StringMatch(const LD::StringView &leftView, const LD::StringView & rightView, const float mismatchValue = 0.90f)
    {
        // get the length of the left, right, and longest string
        // (to use as a basis for comparison in value calculateions)
        LD::UInteger leftSize = leftView.size();
        LD::UInteger rightSize = rightView.size();
        LD::UInteger largerSize = (leftSize > rightSize) ?
                            leftSize : rightSize;
        const char * left = leftView.data();
        const char * right = rightView.data();
        char const *leftPtr = leftView.data();
        char const *rightPtr = rightView.data();
        LD::Float matchVal = 0.0f;
        // Iterate through the left string until we run out of room
        while(leftPtr != (left + leftSize) &&
              rightPtr != (right + rightSize))
        {
            // First, check for a simple left/right match
            if(*leftPtr == *rightPtr)
            {
                // If it matches, add a proportional character's value
                // to the match total.
                matchVal += 1.0f / largerSize;
                // Advance both pointers
                ++leftPtr;
                ++rightPtr;
            }
                // If the simple match fails,
                // try a match ignoring capitalization
            else if(::tolower(*leftPtr) == ::tolower(*rightPtr))
            {
                // We'll consider a capitalization mismatch worth 90%
                // of a normal match
                matchVal += mismatchValue / largerSize;
                // Advance both pointers
                ++leftPtr;
                ++rightPtr;
            }
            else
            {
                char const *lpbest = left + leftSize;
                char const *rpbest = right + rightSize;
                int totalCount = 0;
                int bestCount = INT_MAX;
                int leftCount = 0;
                int rightCount = 0;
                // Here we loop through the left word in an outer loop,
                // but we also check for an early out by ensuring we
                // don't exceed our best current count
                for(char const *lp = leftPtr; (lp != (left + leftSize)
                                               && ((leftCount + rightCount) < bestCount)); ++lp)
                {
                    // Inner loop counting
                    for(char const *rp = rightPtr; (rp != (right +
                                                           rightSize) && ((leftCount + rightCount) <
                                                                          bestCount)); ++rp)
                    {
                        // At this point, we don't care about case
                        if(::tolower(*lp) == ::tolower(*rp))
                        {
                            // This is the fitness measurement
                            totalCount = leftCount + rightCount;
                            if(totalCount < bestCount)
                            {
                                bestCount = totalCount;
                                lpbest = lp;
                                rpbest = rp;
                            }
                        }
                        ++rightCount;
                    }
                    ++leftCount;
                    rightCount = 0;
                }
                leftPtr = lpbest;
                rightPtr = rpbest;
            }
        }
        // clamp the value in case of floating-point error
        if(matchVal > 0.99f)
            matchVal = 1.0f;
        else if(matchVal < 0.01f)
            matchVal = 0.0f;
        return matchVal;
    }
}
#endif //LANDESSDEVCORE_FUZZYSEARCH_H
