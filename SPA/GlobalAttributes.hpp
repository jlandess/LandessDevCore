//
// Created by phoenixflower on 11/1/20.
//

#ifndef LANDESSDEVCORE_GLOBALATTRIBUTES_HPP
#define LANDESSDEVCORE_GLOBALATTRIBUTES_HPP
#include "Reflection/Reflection.hpp"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/Span.hpp"
#include "Primitives/General/Boolean.hpp"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    namespace SPA
    {
        enum InputModeTag{InputNone = 0,InputText = 1,InputDecimal = 2,InputNumeric,Tel = 3,InputSearch =4,InputEmail = 5,InputURL = 6};
        enum AutoCapitalizeTag{CapitalizeOff,CapitalizeNone,CapitalizeOn,CapitalizeSentences,CapitalizeWords,CapitalizeCharacters};
        enum DirectionTag{DirectionLeftRight,DirectionRightLeft,DirectionAuto};
    }
}

namespace LD
{
    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,LD::SPA::InputModeTag>::value>,LD::ImmutableString<10>> ToImmutableString(const T & typeOfInput) noexcept
    {
        LD::StaticArray<LD::ImmutableString<10>,6> results;


        LD::ImmutableString<10> none;
        none[0] = 'n';
        none[1] = 'o';
        none[2] = 'n';
        none[3] = 'e';


        LD::ImmutableString<10> off;
        off[0] = 'o';
        off[1] = 'f';
        off[2] = 'f';

        LD::ImmutableString<10> on;
        on[0] = 'o';
        on[1] = 'n';

        LD::ImmutableString<10> sentences;
        sentences[0] = 's';
        sentences[1] = 'e';
        sentences[2] = 'n';
        sentences[3] = 't';
        sentences[4] = 'e';
        sentences[5] = 'n';
        sentences[6] = 'c';
        sentences[7] = 'e';
        sentences[8] = 's';

        LD::ImmutableString<10> words;
        words[0] = 'w';
        words[1] = 'o';
        words[2] = 'r';
        words[3] = 'd';
        words[4] = 's';

        LD::ImmutableString<10> characters;
        characters[0] = 'c';
        characters[1] = 'h';
        characters[2] = 'a';
        characters[3] = 'r';
        characters[4] = 'a';
        characters[5] = 'c';
        characters[6] = 't';
        characters[7] = 'e';
        characters[8] = 'r';
        characters[9] = 's';



        results[0] = none;
        results[1] = off;
        results[2] = on;
        results[3] = sentences;
        results[4] = words;
        results[5] = characters;
        return results[typeOfInput];
    }

    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,LD::SPA::AutoCapitalizeTag>::value>,LD::ImmutableString<7>> ToImmutableString(const T & typeOfInput) noexcept
    {
        LD::StaticArray<LD::ImmutableString<7>,7> results;


        LD::ImmutableString<7> none;
        none[0] = 'n';
        none[1] = 'o';
        none[2] = 'n';
        none[3] = 'e';


        LD::ImmutableString<7> text;
        text[0] = 't';
        text[1] = 'e';
        text[2] = 'x';
        text[3] = 't';

        LD::ImmutableString<7> decimal;
        decimal[0] = 'd';
        decimal[1] = 'e';
        decimal[2] = 'c';
        decimal[3] = 'i';
        decimal[4] = 'm';
        decimal[5] = 'a';
        decimal[6] = 'l';

        LD::ImmutableString<7> numeric;
        numeric[0] = 'n';
        numeric[1] = 'u';
        numeric[2] = 'm';
        numeric[3] = 'e';
        numeric[4] = 'r';
        numeric[5] = 'i';
        numeric[6] = 'c';

        LD::ImmutableString<7> search;
        search[0] = 's';
        search[1] = 'e';
        search[2] = 'a';
        search[3] = 'r';
        search[4] = 'c';
        search[5] = 'h';

        LD::ImmutableString<7> email;
        email[0] = 'e';
        email[1] = 'm';
        email[2] = 'a';
        email[3] = 'i';
        email[4] = 'l';

        LD::ImmutableString<7> url;
        url[0] = 'u';
        url[1] = 'r';
        url[2] = 'l';


        results[0] = none;
        results[1] = text;
        results[2] = decimal;
        results[3] = numeric;
        results[4] = search;
        results[5] = email;
        results[6] = url;
        return results[typeOfInput];
    }

    template<typename T>
    constexpr LD::Enable_If_T<LD::Require<LD::Detail::IsSame<T,LD::SPA::DirectionTag>::value>,LD::ImmutableString<4>> ToImmutableString(const T & typeOfInput) noexcept
    {
        LD::StaticArray<LD::ImmutableString<4>,3> results;


        LD::ImmutableString<4> leftToRight;
        leftToRight[0] = 'l';
        leftToRight[1] = 't';
        leftToRight[2] = 'r';

        LD::ImmutableString<4> rightToLeft;
        rightToLeft[0] = 'r';
        rightToLeft[1] = 't';
        rightToLeft[2] = 'l';

        LD::ImmutableString<4> Auto;
        Auto[0] = 'a';
        Auto[1] = 'u';
        Auto[2] = 't';
        Auto[3] = 'u';



        results[0] = leftToRight;
        results[1] = rightToLeft;
        results[2] = Auto;
        return results[typeOfInput];
    }
}
namespace LD
{
    namespace SPA
    {


        class AccessKey
        {
        private:
            char mValue;
        public:
            AccessKey() = default;
            AccessKey(const char key) noexcept:mValue{key}
            {

            }
            char & Value() noexcept
            {
                return this->mValue;
            }

            const char & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class AutoCapitalize
        {
        private:
            LD::SPA::AutoCapitalizeTag mValue;
        public:
            AutoCapitalize() = default;
            AutoCapitalize(const LD::SPA::AutoCapitalizeTag key) noexcept:mValue{key}
            {

            }
            LD::SPA::AutoCapitalizeTag & Value() noexcept
            {
                return this->mValue;
            }

            const LD::SPA::AutoCapitalizeTag & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ClassName
        {
        private:
            String mValue;
        public:

            ClassName() = default;
            ClassName(String && value) noexcept:mValue{LD::Forward<String>(value)}
            {

            }
            String & Value() noexcept
            {
                return this->mValue;
            }

            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class ContentEditable
        {
        private:
            bool mState;
        public:
            ContentEditable(const bool  enabled = false) noexcept:mState(enabled)
            {

            }

            bool & Value() noexcept
            {
                return this->mState;
            }

            const bool & Value() const noexcept
            {
                return this->mState;
            }
        };

        class Direction
        {
        private:
            //static constexpr LD::StringView States[3] = {"ltr","rtl","auto"};
            LD::SPA::DirectionTag mValue;
        public:

            Direction(const LD::SPA::DirectionTag  value) noexcept:mValue{value}
            {

            }

            LD::SPA::DirectionTag & Value() noexcept
            {
                return this->mValue;
            }

            const LD::SPA::DirectionTag & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class Draggable
        {
        private:
            bool mState;
        public:


            bool & Value() noexcept
            {
                return this->mState;
            }

            const bool & Value() const noexcept
            {
                return this->mState;
            }
        };

        template<typename String>
        class ID
        {
        private:
            String mValue;
        public:
            ID() = default;

            ID(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class InputMode
        {
        private:
            LD::SPA::InputModeTag mValue;
        public:
            InputMode() = default;

            InputMode(const LD::SPA::InputModeTag & value) noexcept:mValue{value}
            {

            }
            LD::SPA::InputModeTag & Value() noexcept
            {
                return this->mValue;
            }

            const LD::SPA::InputModeTag & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ItemID
        {
        private:
            String mValue;
        public:
            ItemID() = default;

            ItemID(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ItemProp
        {
        private:
            String mValue;
        public:
            ItemProp() = default;

            ItemProp(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ItemRef
        {
        private:
            String mValue;
        public:
            ItemRef() = default;

            ItemRef(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ItemScope
        {
        private:
            String mValue;
        public:
            ItemScope() = default;

            ItemScope(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class ItemType
        {
        private:
            String mValue;
        public:
            ItemType() = default;

            ItemType(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        template<typename String>
        class Language
        {
        private:
            String mValue;
        public:
            Language() = default;

            Language(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class SpellCheck
        {
        private:
            bool mState;
        public:

            bool & Value() noexcept
            {
                return this->mState;
            }

            const bool & Value() const noexcept
            {
                return this->mState;
            }
        };

        template<typename String>
        class Style
        {
        private:
            String mValue;
        public:
            Style() = default;

            Style(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };


        class TabIndex
        {
        private:
            LD::UInteger mValue;
        public:
            LD::UInteger & Value()  noexcept
            {
                return this->mValue;
            }
            const LD::UInteger & Value() const noexcept
            {
                return this->mValue;
            }
        };


        template<typename String>
        class Title
        {
        private:
            String mValue;
        public:
            Title() = default;

            Title(String && string) noexcept:mValue{LD::Forward<String>(string)}
            {

            }
            String & Value()  noexcept
            {
                return this->mValue;
            }
            const String & Value() const noexcept
            {
                return this->mValue;
            }
        };

        class Translate
        {
        private:
            LD::YesNo mState;
        public:
            constexpr Translate(const bool state = true) noexcept:mState((LD::YesNo) state)
            {

            }

            LD::YesNo & Value()  noexcept
            {
                return this->mState;
            }
            const LD::YesNo & Value() const noexcept
            {
                return this->mState;
            }
        };
    }
}
//-------------------------------------------------------------------Beginning of Attribute Registration-------------------------------------------------------------------------
namespace LD
{
    namespace SPA
    {


    }
}
//-----------------------------------------------------------------End of Attribute Registration----------------------------------------------------------------------------------


//------------------------------------------------------------------------Beginning of Attribute Reflection------------------------------------------------------------------------------
template<>
struct LD::CT::TypeDescriptor<LD::SPA::AccessKey>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("accesskey");
public:
    static constexpr auto ClassName = ctll::fixed_string{"key"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<char & (LD::SPA::AccessKey::*)(),&LD::SPA::AccessKey::Value>(),LD::CT::SelectOverload<const char & (LD::SPA::AccessKey::*)() const,&LD::SPA::AccessKey::Value>()>
    >;


    static constexpr MemberList Members{  };

};
template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::Language<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("lang");
public:
    static constexpr auto ClassName = ctll::fixed_string{"attribute"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::Language<T>::*)(),&LD::SPA::Language<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::Language<T>::*)() const,&LD::SPA::Language<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};


template<>
struct LD::CT::TypeDescriptor<LD::SPA::AutoCapitalize>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("accesskey");
public:
    static constexpr auto ClassName = ctll::fixed_string{"key"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<LD::SPA::AutoCapitalizeTag & (LD::SPA::AutoCapitalize::*)(),&LD::SPA::AutoCapitalize::Value>(),LD::CT::SelectOverload<const LD::SPA::AutoCapitalizeTag & (LD::SPA::AutoCapitalize::*)() const,&LD::SPA::AutoCapitalize::Value>()>
    >;


    static constexpr MemberList Members{  };

};


template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::ClassName<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("class");
public:
    static constexpr auto ClassName = ctll::fixed_string{"name"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::ClassName<T>::*)(),&LD::SPA::ClassName<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::ClassName<T>::*)() const,&LD::SPA::ClassName<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};
template<>
struct LD::CT::TypeDescriptor<LD::SPA::ContentEditable>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("contenteditable");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<bool & (LD::SPA::ContentEditable::*)(),&LD::SPA::ContentEditable::Value>(),LD::CT::SelectOverload<const bool & (LD::SPA::ContentEditable::*)() const,&LD::SPA::ContentEditable::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::SPA::Direction>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("dir");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<LD::SPA::DirectionTag & (LD::SPA::Direction::*)(),&LD::SPA::Direction::Value>(),LD::CT::SelectOverload<const LD::SPA::DirectionTag & (LD::SPA::Direction::*)() const,&LD::SPA::Direction::Value>()>
    >;


    static constexpr MemberList Members{  };

};
template<>
struct LD::CT::TypeDescriptor<LD::SPA::Draggable>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("draggable");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<bool & (LD::SPA::Draggable::*)(),&LD::SPA::Draggable::Value>(),LD::CT::SelectOverload<const bool & (LD::SPA::Draggable::*)() const,&LD::SPA::Draggable::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::ID<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("id");
public:
    static constexpr auto ClassName = ctll::fixed_string{"name"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::ID<T>::*)(),&LD::SPA::ID<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::ID<T>::*)() const,&LD::SPA::ID<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::SPA::InputMode>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("inputmode");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<LD::SPA::InputModeTag & (LD::SPA::InputMode::*)(),&LD::SPA::InputMode::Value>(),LD::CT::SelectOverload<const LD::SPA::InputModeTag & (LD::SPA::InputMode::*)() const,&LD::SPA::InputMode::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::ItemID<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("itemid");
public:
    static constexpr auto ClassName = ctll::fixed_string{"name"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::ItemType<T>::*)(),&LD::SPA::ItemID<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::ItemID<T>::*)() const,&LD::SPA::ItemID<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::SPA::SpellCheck>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("spellcheck");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<bool & (LD::SPA::SpellCheck::*)(),&LD::SPA::SpellCheck::Value>(),LD::CT::SelectOverload<const bool & (LD::SPA::SpellCheck::*)() const,&LD::SPA::SpellCheck::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::Style<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("style");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::ItemType<T>::*)(),&LD::SPA::ItemID<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::ItemID<T>::*)() const,&LD::SPA::ItemID<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::SPA::TabIndex>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("tabindex");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<LD::UInteger & (LD::SPA::TabIndex::*)(),&LD::SPA::TabIndex::Value>(),LD::CT::SelectOverload<const LD::UInteger & (LD::SPA::TabIndex::*)() const,&LD::SPA::TabIndex::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<typename T>
struct LD::CT::TypeDescriptor<LD::SPA::Title<T>>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("title");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<T & (LD::SPA::Title<T>::*)(),&LD::SPA::Title<T>::Value>(),LD::CT::SelectOverload<const T & (LD::SPA::Title<T>::*)() const,&LD::SPA::Title<T>::Value>()>
    >;


    static constexpr MemberList Members{  };

};

template<>
struct LD::CT::TypeDescriptor<LD::SPA::Translate>
{
private:
    static constexpr auto TagName = ctll::basic_fixed_string("translate");
public:
    static constexpr auto ClassName = ctll::fixed_string{"value"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<TagName,LD::CT::SelectOverload<LD::YesNo & (LD::SPA::Translate::*)(),&LD::SPA::Translate::Value>(),LD::CT::SelectOverload<const LD::YesNo & (LD::SPA::Translate::*)() const,&LD::SPA::Translate::Value>()>
    >;


    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_GLOBALATTRIBUTES_HPP
