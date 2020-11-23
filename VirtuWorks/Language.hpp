//
// Created by phoenixflower on 11/20/20.
//

#ifndef LANDESSDEVCORE_LANGUAGE_HPP
#define LANDESSDEVCORE_LANGUAGE_HPP
#include "IO/FetchRequest.h"
#include "Primitives/General/StringView.hpp"
namespace LD
{
    namespace VW
    {
        enum QueueLanguageTag{VirtuWorksLanguageEnglish,VirtuWorksLanguageSpanish,VirtuWorksLanguageKryol};
        enum CallType{eVirtuWorksCall,eVirtualWorksQueue};

        class Language
        {
        private:
            QueueLanguageTag mLanguage;
        public:
            constexpr Language() noexcept:mLanguage{VirtuWorksLanguageEnglish}
            {

            }
            constexpr Language(QueueLanguageTag tag) noexcept:mLanguage{tag}
            {

            }
        };
    }


    template<typename ... A>
    LD::QueryResult<LD::VW::Language(A...)> FromStringView(LD::Type<LD::VW::Language>, LD::StringView view, A && ... args) noexcept
    {
        LD::StringView englishQueue{"EnglishQueue"};
        LD::StringView kryolQueue{"KryolQueue"};
        LD::StringView spanishQueue{"SpanishQueue"};
        if (englishQueue == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::Language{LD::VW::VirtuWorksLanguageEnglish},LD::Forward<A>(args)...);
        }else if(kryolQueue == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::Language{LD::VW::VirtuWorksLanguageKryol},LD::Forward<A>(args)...);
        }else if(spanishQueue == view)
        {
            return LD::MakeContext(LD::TransactionResult{},LD::VW::Language{LD::VW::VirtuWorksLanguageSpanish},LD::Forward<A>(args)...);
        }
        return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(args)...);
    }
}
#endif //LANDESSDEVCORE_LANGUAGE_HPP
