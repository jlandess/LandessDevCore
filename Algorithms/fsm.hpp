// The MIT License (MIT)
//
// Copyright (c) 2017 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "Memory/Optional.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General//mapboxvariantvisitor.h"
#include "Definitions/Common.hpp"
#include "Primitives/General/StaticArray.hpp"
#include "Functor/LightWeightDelegate.h"
namespace mp
{


    template<typename Derived, typename StateVariant>
    class fsm
    {

        StateVariant state_;

    public:
        const StateVariant& get_state() const { return state_; }

        StateVariant& get_state() { return state_; }

        template<typename Event>
        void Dispatch(Event&& event)
        {
            Derived& child = static_cast<Derived&>(*this);

            auto new_state = LD::Match(state_,[&](auto & s)->PDP::Optional<StateVariant>
            {

                return child.OnEvent(s, LD::Forward<Event>(event));
            });
            LD::StaticArray<PDP::LightWeightDelegate<void(StateVariant & state, const PDP::optional_lite::Optional<StateVariant> & newState)>,2> codePaths;
            auto emptyCodePath = [](StateVariant & state, const PDP::optional_lite::Optional<StateVariant> & newState){};
            auto executingCodePath = [](StateVariant & state, const PDP::Optional<StateVariant> & newState)
            {
                state = *LD::Move(newState);
            };
            bool shouldTransition = bool(new_state);
            decltype(emptyCodePath) * ptr;
            codePaths[0] = PDP::LightWeightDelegate<void(StateVariant & state, const PDP::optional_lite::Optional<StateVariant> & newState)>{&emptyCodePath, &decltype(emptyCodePath)::operator()};
            codePaths[1] = PDP::LightWeightDelegate<void(StateVariant & state, const PDP::optional_lite::Optional<StateVariant> & newState)>{&executingCodePath, &decltype(executingCodePath)::operator()};
            codePaths[shouldTransition](state_,new_state);
        }
    };

}  // namespace mp
