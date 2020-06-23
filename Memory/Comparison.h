//
// Created by phoenixflower on 6/22/20.
//

#ifndef LANDESSDEVCORE_COMPARISON_H
#define LANDESSDEVCORE_COMPARISON_H
#include "Definitions/Integer.hpp"
#include "TypeTraits/Limit.hpp"
#include "Primitives/General/StringView.hpp"
#include "Algorithms/Exponential.hpp"
#include <ctype.h>
namespace LD
{
    inline constexpr double StringMatch(const LD::StringView & leftView, const LD::StringView & rightView, const float mismatchValue = 0.90f) noexcept
    {
        // get the length of the left, right, and longest string
        // (to use as a basis for comparison in value calculateions)
        LD::UInteger leftSize = leftView.size();
        LD::UInteger rightSize = rightView.size();
        LD::UInteger largerSize = (leftSize > rightSize) ?
                            leftSize : rightSize;
        char * left = (char*)leftView.data();
        char * right = (char*)rightView.data();
        char const *leftPtr = leftView.data();
        char const *rightPtr = rightView.data();
        double matchVal = 0.0f;
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
            else if(tolower(*leftPtr) == tolower(*rightPtr))
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
                        if(tolower(*lp) == tolower(*rp))
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
        if(matchVal > 0.99)
            matchVal = 1.0;
        else if(matchVal < 0.01)
            matchVal = 0.0;
        return matchVal;
    }

    inline  float Fuzzymath(const LD::StringView  & s1, const LD::StringView  & s2) noexcept
    {
        unsigned long s1len, s2len, x, y, lastdiag, olddiag;
        s1len = s1.size();
        s2len = s1.size();
        unsigned long column[s1len+1];
        for (y = 1; y <= s1len; y++)
            column[y] = y;


        for (x = 1; x <= s2len; x++) {
            column[0] = x;
            for (y = 1, lastdiag = x-1; y <= s1len; y++) {
                olddiag = column[y];
                column[y] = LD::Min(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
                lastdiag = olddiag;
            }
        }
        long result = column[s1len];

        return  1.0f-((float)result/(float)s1len);
    }
}
#endif //LANDESSDEVCORE_COMPARISON_H
