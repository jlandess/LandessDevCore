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
                return LD::SPA::VarExpression {
                    LD::Type<void>{},
                    LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                    LD::ImmutableString{"assert"},
                    LD::SPA::VarExpression<T,Op,OpType,A...>{expression},
                    LD::SPA::Var{LD::ImmutableString{message},*this->mGenerator}};
            }

            auto Clear() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"clear"}};
            }

            auto Count() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"count"}};
            }
            template<LD::UInteger MessageSize>
            auto Error(LD::ImmutableString<MessageSize> message) noexcept
            {
                //LD::SPA::Var varType4{LD::ImmutableString{message},*this->mGenerator};
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"error"},LD::SPA::Var{LD::ImmutableString{message},*this->mGenerator}};
            }
            auto Group() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"group"}};
            }
            auto GroupCollapsed() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"groupCollapsed"}};
            }

            auto GroupEnd() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"groupEnd"}};
            }

            template<LD::UInteger MessageSize>
            void Info(LD::ImmutableString<MessageSize> message)
            {
                //LD::SPA::Var varType4{LD::ImmutableString{message},*this->mGenerator};
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"info"},LD::SPA::Var{LD::ImmutableString{message},*this->mGenerator}};
            }

            template<LD::UInteger MessageSize>
            void Log(LD::ImmutableString<MessageSize> message)
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"log"},LD::SPA::Var{LD::ImmutableString{message},*this->mGenerator}};
            }

            void Table()
            {

            }

            auto Time() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"time"}};
            }

            auto TimeEnd() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"timeEnd"}};
            }

            auto Trace() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"trace"}};
            }

            auto Warn() const noexcept
            {
                return LD::SPA::VarExpression {
                        LD::Type<void>{},
                        LD::SPA::ExternalObjectFunctionalSymbol{LD::Type<Console>{},LD::ImmutableString{"console"}},
                        LD::ImmutableString{"warn"}};
            }
        };

        template<typename RandomNumber> Console(RandomNumber & generator) -> Console<RandomNumber>;
    }
}
#endif //LANDESSDEVCORE_CONSOLE_HPP
