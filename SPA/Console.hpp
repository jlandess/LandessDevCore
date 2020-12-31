//
// Created by phoenixflower on 12/29/20.
//

#ifndef LANDESSDEVCORE_CONSOLE_HPP
#define LANDESSDEVCORE_CONSOLE_HPP
#include "Variable.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace SPA
    {
        template<typename RandomNumberGenerator>
        class Console
        {
        private:
            LD::ElementReference<RandomNumberGenerator> mGenerator;
        public:
            Console() = default;
            Console(RandomNumberGenerator & gen) noexcept:mGenerator{gen}
            {

            }
            //template<typename T,typename Op,typename OpType,typename ... A>
            template<typename T,typename Op,typename OpType,LD::UInteger Amount, typename ... A>
            auto Assert(LD::SPA::VarExpression<T,Op,OpType,A...> expression,LD::ImmutableString<Amount> message)  noexcept
            {
                LD::SPA::Var varType4{LD::ImmutableString<Amount>{message},*this->mGenerator};
                LD::SPA::VarExpression expression5{
                    LD::Type<void>{},
                    LD::SPA::ExternalObjectSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                    LD::ImmutableString{"assert"},
                    LD::SPA::VarExpression<T,Op,OpType,A...>{expression},
                    decltype(varType4){varType4}};
                return expression5;
            }

            void Clear() const noexcept
            {

            }
        };

        template<typename RandomNumber> Console(RandomNumber & generator) -> Console<RandomNumber>;
    }
}
#endif //LANDESSDEVCORE_CONSOLE_HPP
