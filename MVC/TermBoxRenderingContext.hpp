//
//  TermBoxRenderingContext.hpp
//  DataStructures
//
//  Created by James Landess on 2/13/18.
//  Copyright © 2018 James Landess. All rights reserved.
//

#ifndef TermBoxRenderingContext_hpp
#define TermBoxRenderingContext_hpp


#include "Primitives/General/Vec2.hpp"
#include "Definitions/Integer.hpp"
#include "Definitions/Float.hpp"
//#include "Definitions/Common.hpp"
#include "TermBox/termbox.h"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/StaticArray.hpp"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/CompileTimeControlFlow.hpp"
#include "RenderingContext.hpp"
#include "MVC/SchedulingEvent.hpp"
#include "TermBoxEnumerations.hpp"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/VariadicPack.hpp"
#include "TypeTraits/TypeList.hpp"
#include "Primitives/General//ctre.hpp"
#include "RenderableConcept.hpp"
#include "Renderable.hpp"
#include "Primitives/General/StringView.hpp"
#include "Primitives/General/ContextualVariant.h"
namespace LD
{


    class TermBox
    {
        
    };
    
    template<>
    class RenderingContext<TermBox>
    {
    private:
        LD::Detail::tVec2<LD::UInteger> Cursor;
        LD::UInteger LineWidth;
        LD::TermBoxConsoleApplicationColorSpectrum CurrentSpectrum;
        LD::TermBoxConsoleApplicationColorModifier CurrentForegroundModifier;
        LD::TermBoxConsoleApplicationColorModifier CurrentBackgroundModifier;
        unsigned short CurrentForegroundColor;
        unsigned short CurrentBackgroundColor;
    public:
        
        RenderingContext();

        LD::Detail::tVec2<LD::UInteger> & GetCursor() {return this->Cursor;}
        const LD::Detail::tVec2<LD::UInteger> & GetCursor() const {return this->Cursor;}
        void SetCurrentSpectrum(const LD::TermBoxConsoleApplicationColorSpectrum & spectrum);
        
        
        void SetForegroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier & modifier);
        void SetForegroundColor(const unsigned short & defaultColor);
        
        
        void SetBackgroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier & modifier);
        void SetBackgroundColor(const unsigned short & defaultColor);
        
        const LD::TermBoxConsoleApplicationColorSpectrum & GetSpectrum() const;
        
        
        const LD::TermBoxConsoleApplicationColorModifier & GetForegroundModifier() const;
        const unsigned short & GetForegroundColor() const;
        
        const LD::TermBoxConsoleApplicationColorModifier & GetBackgroundModifier() const;
        const unsigned short & GetBackgroundColor() const;
        
        
        void Clear(const unsigned short & foregroundColor = LD::eTBGreen, const unsigned short & backgroundColor = LD::eTBDefault, const LD::TermBoxConsoleApplicationColorModifier & foregroundModifier = LD::eTBNone, const LD::TermBoxConsoleApplicationColorModifier & backgroundModifier = LD::eTBNone, const LD::TermBoxConsoleApplicationColorSpectrum & spectrum = LD::eNormalSpectrum);
        
        
        RenderingContext<TermBox> & Write(const wchar_t * characters, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const char * characters, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::Integer & character, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::UInteger & character, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::Float & character, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const char & character, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(unsigned char & character, const LD::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const wchar_t & character, const LD::Detail::tVec2<LD::UInteger> & translation);
    };
}
namespace LD
{
    class TermBoxKeyboardEvent
    {
    private:
        uint8_t ModifierValue; /* modifiers to either 'key' or 'ch' below */
        uint16_t KeyValue; /* one of the TB_KEY_* constants */
        uint32_t CharacterValue; /* unicode character */
    public:
        inline explicit constexpr TermBoxKeyboardEvent(const uint8_t & mod, const uint16_t & key,const uint32_t & character) noexcept :CharacterValue(character),KeyValue(key),ModifierValue(mod){}
        
        constexpr const uint8_t & Modifier() const noexcept {return this->ModifierValue;}
        constexpr const uint16_t & Key() const noexcept {return this->KeyValue;}
        constexpr const uint32_t & Character() const noexcept {return this->CharacterValue;}

        template<typename ... Args>
        constexpr LD::ContextualVariant<LD::Variant<uint16_t,uint32_t>(Args...)> operator()(Args && ... arguements) const noexcept
        {
            LD::ContextualVariant<LD::Variant<uint16_t,uint32_t>(Args...)> ret[2];
            ret[0] = LD::MakeContext(uint16_t{this->KeyValue},LD::Forward<Args>(arguements)...);
            ret[1] = LD::MakeContext(uint32_t{this->CharacterValue},LD::Forward<Args>(arguements)...);
            return ret[this->CharacterValue != 0];
        }
    };
    
    
    class TermBoxMouseEvent
    {
    private:
        int32_t XValue;
        int32_t YValue;
    public:

        inline explicit constexpr TermBoxMouseEvent(const int32_t & x, const int32_t & y) noexcept :XValue(x),YValue(y) {}
        constexpr const int32_t & X() const noexcept {return this->XValue;}

        constexpr const int32_t & Y() const noexcept {return this->YValue;}
    };
    
    
    class TermBoxResizingEvent
    {
    private:
        int32_t mWidth;
        int32_t mHeight;
    public:

        inline explicit constexpr TermBoxResizingEvent(const int32_t & width, const int32_t & height) noexcept :mWidth(width),mHeight(height) {}

        constexpr const int32_t & GetWidth() const noexcept {return this->mWidth;}
        
        constexpr const int32_t & GetHeight() const noexcept {return this->mHeight;}
        
        
    };
    
    class TermBoxEmptyEvent
    {
        
    };
    class TermBoxContext
    {
    private:
        mapbox::util::variant<TermBoxEmptyEvent,TermBoxKeyboardEvent,TermBoxMouseEvent,TermBoxResizingEvent> Event;
    public:
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::Exists<LD::CallableType, F,TermBoxKeyboardEvent>
        >,LD::CallableType<F, TermBoxKeyboardEvent>> OnKeyboardEvent(const LD::CallableType<F, TermBoxKeyboardEvent> & initialState,F && keyboardFunctor)
        {
            return LD::Match(this->Event,keyboardFunctor,[&](auto & event){return initialState;});
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::Exists<LD::CallableType, F,TermBoxResizingEvent>
        >,LD::CallableType<F, TermBoxMouseEvent>> OnMouseEvent(const LD::CallableType<F, TermBoxMouseEvent> & initialState,F && mouseFunctor)
        {
            
            return LD::Match(this->Event,mouseFunctor,[&](auto & event){return initialState;});
        }
        
        template<typename F>
        LD::Enable_If_T<
        LD::Require<
        LD::Exists<LD::CallableType, F,TermBoxResizingEvent>
        >,LD::CallableType<F, TermBoxResizingEvent>> OnResizing(const LD::CallableType<F, TermBoxResizingEvent> & initialState,F && resizingFunctor)
        {
            
            return LD::Match(this->Event,resizingFunctor,[&](auto & event){return initialState;});
        }
    };
    class TermBoxRenderContext;



    class TermBoxRenderContext
    {
    public:
        tb_event CurrentEvent;
    private:
        LD::UInteger GetFormatedIndices(const char * data, LD::UInteger * indices) const noexcept
        {
            static constexpr auto pattern = ctll::fixed_string{"\\{\\}"};
            LD::UInteger amountOfDelimeters = 0;
            for (const auto & match: ctre::range<pattern>(std::string_view {data}))
            {
                indices[amountOfDelimeters] = (match.get_end_position()-data)-2;//the -2 is the size of the delimeter
                ++amountOfDelimeters;
            }
            return amountOfDelimeters;
        }
        LD::Detail::tVec2<LD::Integer> mCursor;
        LD::Detail::tVec2<LD::Integer> mMouse;
        LD::UInteger LineWidth;
        LD::TermBoxConsoleApplicationColorSpectrum CurrentSpectrum;
        LD::TermBoxConsoleApplicationColorModifier CurrentForegroundModifier;
        LD::TermBoxConsoleApplicationColorModifier CurrentBackgroundModifier;
        unsigned short CurrentForegroundColor;
        unsigned short CurrentBackgroundColor;
        unsigned short mMouseEnabled;

        const TermBoxRenderContext & Write(const char & character, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept;
        const TermBoxRenderContext & Write(const wchar_t & character, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept;
    public:




        template<typename T>
        constexpr static LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<LD::Detail::Decay_T<T>,LD::Detail::tVec2<LD::UInteger>()>::Value(),
                        LD::IsClass<T>
                        >
        ,LD::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept (noexcept(LD::Declval<T>()(LD::Declval<LD::RenderableDimensionEvent>())))
        {
            return object(LD::RenderableDimensionEvent{});
        }


        template<typename T>
        constexpr static LD::Enable_If_T<LD::Require<!LD::IsClass<T>>,LD::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept
        {
            return LD::Detail::GetRenderableDimensions(LD::Forward<T>(object));
        }
        template<typename T,class sfinae>
        friend class BasicTermBoxApplication;

        TermBoxRenderContext();
        void EnableMouse() {this->mMouseEnabled = true;}
        void DisableMouse() {this->mMouseEnabled = false;}
        bool IsMouseEnabled() const {return this->mMouseEnabled;}
        const tb_event & GetEvent() const{return this->CurrentEvent;}

        template<typename ... Args,
                typename Ret = LD::ContextualVariant<LD::Variant<LD::TermBoxEmptyEvent,LD::TermBoxResizingEvent,LD::TermBoxMouseEvent,LD::TermBoxKeyboardEvent>(Args...)>>
        constexpr Ret Event(Args && ... arguements)  noexcept
        {
            //An event, single interaction from the user. The 'mod' and 'ch' fields are
            // valid if 'type' is TB_EVENT_KEY. The 'w' and 'h' fields are valid if 'type'
//is TB_EVENT_RESIZE. The 'x' and 'y' fields are valid if 'type' is
            // TB_EVENT_MOUSE. The 'key' field is valid if 'type' is either TB_EVENT_KEY
            // or TB_EVENT_MOUSE. The fields 'key' and 'ch' are mutually exclusive; only
            // one of them can be non-zero at a time.
            Ret eventable[4];

            //tb_peek_event(&CurrentEvent,0);
            eventable[0] = LD::MakeContext(LD::TermBoxEmptyEvent{},LD::Forward<Args>(arguements)...);
            eventable[1] = LD::MakeContext(LD::TermBoxMouseEvent{this->GetEvent().x,this->GetEvent().y},LD::Forward<Args>(arguements)...);
            eventable[2] = LD::MakeContext(LD::TermBoxKeyboardEvent{this->GetEvent().mod,this->GetEvent().key,this->GetEvent().ch},LD::Forward<Args>(arguements)...);
            eventable[3] = LD::MakeContext(LD::TermBoxResizingEvent{this->GetEvent().w,this->GetEvent().h},LD::Forward<Args>(arguements)...);


            if(this->GetEvent().type == TB_EVENT_MOUSE && (this->GetEvent().ch == 0))
            {
                return eventable[1];

            }else if(this->GetEvent().type == TB_EVENT_KEY)
            {
                return eventable[2];

            }else if(this->GetEvent().type == TB_EVENT_RESIZE)
            {

                return eventable[3];
            }
            //LD::UInteger sum = (this->GetEvent().type == TB_EVENT_MOUSE)*1 + (this->GetEvent().type == TB_EVENT_RESIZE)*2 + (this->GetEvent().type == TB_EVENT_RESIZE)+3;
            //LD::UInteger sum = (this->GetEvent().type == TB_EVENT_MOUSE)*1 + (this->GetEvent().type == TB_EVENT_KEY)*2 + (this->GetEvent().type == TB_EVENT_RESIZE)+3;
            //return eventable[2];
            return {};
        }


        //tb_event & GetEvent() {return this->CurrentEvent;}
        LD::Detail::tVec2<LD::Integer > & GetCursor() {return this->mCursor;}
        const LD::Detail::tVec2<LD::Integer > & GetCursor() const {return this->mCursor;}
        LD::Detail::tVec2<LD::Integer > & GetMouse() {return this->mMouse;}
        const LD::Detail::tVec2<LD::Integer > & GetMouse() const {return this->mMouse;}
        void SetCurrentSpectrum(const LD::TermBoxConsoleApplicationColorSpectrum & spectrum);
        void SetForegroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier & modifier);
        void SetForegroundColor(const unsigned short & defaultColor);
        void SetBackgroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier & modifier);
        void SetBackgroundColor(const unsigned short & defaultColor);
        const LD::TermBoxConsoleApplicationColorSpectrum & GetSpectrum() const;
        const LD::TermBoxConsoleApplicationColorModifier & GetForegroundModifier() const;
        const unsigned short & GetForegroundColor() const;
        const LD::TermBoxConsoleApplicationColorModifier & GetBackgroundModifier() const;
        const unsigned short & GetBackgroundColor() const;
        /**
         CurrentForegroundModifier(eTBNone),CurrentForegroundColor(eTBGreen),CurrentSpectrum(eNormalSpectrum),LineWidth(tb_width()),CurrentBackgroundColor(eTBDefault),CurrentBackgroundModifier(eTBNone)
         */
        void Clear(const unsigned short & foregroundColor = LD::eTBGreen, const unsigned short & backgroundColor = LD::eTBDefault, const LD::TermBoxConsoleApplicationColorModifier & foregroundModifier = LD::eTBNone, const LD::TermBoxConsoleApplicationColorModifier & backgroundModifier = LD::eTBNone, const LD::TermBoxConsoleApplicationColorSpectrum & spectrum = LD::eNormalSpectrum);
        template<LD::UInteger StringSize>
        const TermBoxRenderContext& Render(const char (&lit)[StringSize], const LD::Detail::tVec2<LD::Integer> & translation) const
        {
            return this->Render(LD::ToImmutableString(lit),translation);
        }
        template<typename T>
        constexpr LD::Enable_If_T<
        LD::Require<
        LD::Either<
        LD::Detail::IsSame<T, char>::value,
        LD::IsInteger<T>,
        LD::IsUnsignedInteger<T>,
        LD::Detail::IsSame<T, unsigned char>::value
        >,
        LD::IsPrimitive<T>
        >,const TermBoxRenderContext&> Render(const T & integer, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
            return this->Render(LD::ToImmutableString((integer)),translation);
        }
        template<typename T>
        constexpr LD::Enable_If_T<LD::Require<LD::IsTypeString<T>>,const TermBoxRenderContext&> Render(const T & typeString, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
            return this->Render(LD::ToImmutableString((typeString)),translation);
        }
        template<typename T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Either<
                                LD::Detail::IsSame<T, float>::value,
                                LD::Detail::IsSame<T, double>::value>
                >,const TermBoxRenderContext&> Render(const T & floatingPointNumber, const LD::Detail::tVec2<LD::Integer> & translation, const LD::UInteger & precision = 0) const
        {
          return this->Render(LD::ToImmutableString(floatingPointNumber,precision),translation);
        }
        template<LD::UInteger Size>
        constexpr const TermBoxRenderContext & Render(const LD::ImmutableString<Size> & str, const LD::Detail::tVec2<LD::Integer > & translation) const noexcept
        {
            LD::For<Size>([](auto Index,const LD::ImmutableString<Size> & str,const TermBoxRenderContext * instance, const LD::Detail::tVec2<LD::Integer > & translation)
            {
                const char & currentCharacter = str[Index];
                instance->Write(currentCharacter,translation+LD::Detail::tVec2<LD::Integer>{Index,0});
                return (Index < str.GetSize());
            },str,this,translation);
            return (*this);
        }

        template<typename T>
        auto Render(const T & wideCharacter, const LD::Detail::tVec2<LD::Integer > & translation) noexcept -> LD::TypedRequires<LD::TermBoxRenderContext&,LD::Detail::IsSame<T, wchar_t >::value>
        {
            this->Write(wideCharacter,translation);
            return (*this);
        }


        constexpr const TermBoxRenderContext & Render(const LD::StringView & view, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept
        {

            LD::Integer verticalOffset = {};
            LD::UInteger horizontalOffset = {};
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                const char & currentCharacter = view[n];
                if(currentCharacter != '\t' && currentCharacter != '\n')
                {
                    this->Write(currentCharacter,translation+LD::Detail::tVec2<LD::Integer>{horizontalOffset,verticalOffset});
                    ++horizontalOffset;
                }else if(currentCharacter == '\t')
                {
                    horizontalOffset+=5;

                }else if(currentCharacter == '\n')
                {
                    --verticalOffset;
                    horizontalOffset = {};
                }


            }
            return (*this);
        }


        template<LD::UInteger Size,typename ... Arguements>
        LD::Enable_If_T<
        LD::Require<
                (LD::IsRenderable2D<Arguements,const LD::TermBoxRenderContext&> && ...),
                (sizeof...(Arguements) > 0),
                (!LD::Detail::IsPointer<Arguements>::value && ...)
        >,const TermBoxRenderContext &> RenderWithFormat(const LD::ImmutableString<Size> & str,const LD::Detail::tVec2<LD::Integer> & translation,Arguements && ... objects) const noexcept
        {
            using ArguementList = LD::CT::TypeList<LD::Detail::Decay_T<Arguements>...>;
            using RenderableContextTypeList = LD::CT::TypeList<LD::NullClass,LD::ElementReference <LD::Detail::Decay_T<Arguements>>...,LD::ElementReference <char>>;
            using DeDuplicatedRederableContextTypeList = typename LD::CT::DeDuplicateTypeList<RenderableContextTypeList> ;
            using Renderable = LD::Rebind<DeDuplicatedRederableContextTypeList,LD::Variant>;
            using ContextTypeList = LD::CT::TypeList<LD::ElementReference<LD::Detail::Decay_T <Arguements>>...>;
            using DeDuplicatedContextTypeList =  LD::CT::DeDuplicateTypeList<ContextTypeList> ;
            using Arguement = LD::Rebind<DeDuplicatedContextTypeList,LD::Variant>;

            LD::StaticArray<LD::UInteger ,Size> delimeterIndices = {Size};
            LD::UInteger amountOfDelimeters = 0;


            LD::StaticArray<Renderable,Size> renderables;


            LD::StaticArray<Arguement,sizeof...(objects)> arguements;
            LD::ReferenceableTuple<Arguements...> packedArguements = LD::MakeReferenceableTuple(LD::Forward<Arguements>(objects)...);

            LD::For<sizeof...(objects)>([](auto Index, LD::StaticArray<Arguement,sizeof...(objects)> & arguements,  LD::ReferenceableTuple<Arguements...> & context)
            {
                arguements[Index] = LD::ElementReference<typename LD::TypeAtIndex<Index,ArguementList >::type >{LD::Get(LD::Get<Index>(context))};
                return true;
            },arguements,packedArguements);
            LD::UInteger numberOfDelimeters = GetFormatedIndices(str.Data(),delimeterIndices.GetData());
            LD::UInteger currentCount = 0;
            LD::UInteger currentDelimeterToCheck = 0;
            constexpr LD::UInteger delimeterSize = 2;
            for(LD::UInteger n = 0;n<Size;++n)
            {

                if( ((numberOfDelimeters > 0 && currentDelimeterToCheck < numberOfDelimeters) && delimeterIndices[currentDelimeterToCheck] == n) && (currentDelimeterToCheck < sizeof...(objects)))
                {
                    LD::Match(arguements[currentDelimeterToCheck],[&](const auto & object)
                    {
                        renderables[n] = object;
                    });
                }
                else if(((numberOfDelimeters > 0 && currentDelimeterToCheck < numberOfDelimeters) && (delimeterIndices[currentDelimeterToCheck]+(delimeterSize-1) < n)) && (currentDelimeterToCheck < sizeof...(objects)))
                {
                    //renderables[n] = LD::ElementReference<char>{buffer1};

                }else if(((numberOfDelimeters > 0 && currentDelimeterToCheck < numberOfDelimeters) && (delimeterIndices[currentDelimeterToCheck]+(delimeterSize-1) == n)) && (currentDelimeterToCheck < sizeof...(objects)))
                {
                    currentDelimeterToCheck++;
                    //renderables[n] = LD::ElementReference<char>{buffer2};
                }
                else
                {
                    renderables[n] = LD::ElementReference<char>{str[currentCount]};
                }
                currentCount++;
            }
            LD::Detail::tVec2<LD::Integer> offsetTranslation = {0,0};
            LD::For<Size>([](
                    auto Index,
                    const TermBoxRenderContext * instance,
                    const LD::StaticArray<Renderable ,Size> & renderables,
                    const LD::Detail::tVec2<LD::Integer> & translation,
                    LD::Detail::tVec2<LD::Integer> & offsetTranslation,
                    const LD::ReferenceableTuple<Arguements...> & packedArguements)
            {

                LD::Match(renderables[Index],[&]( auto && object)->LD::Enable_If_T<LD::Require<!LD::IsSame<LD::Detail::Decay_T <decltype(object)>,LD::NullClass>>,void>
                {
                    instance->Render(LD::Get(object),translation+LD::Detail::tVec2<LD::Integer>{Index,0}+offsetTranslation);
                    offsetTranslation.X() += (LD::TermBoxRenderContext::GetRenderableDimensions(LD::Get(object)).X()-1);
                },[&](const LD::NullClass&){offsetTranslation.X()-=1;});
                return true;
            },this,renderables,translation,offsetTranslation,packedArguements);
            return (*this);
        }


        template<typename T>
        constexpr LD::Enable_If_T<
         LD::Require<
         LD::ConvertiblyCallable<LD::Detail::Decay_T<T>,const LD::TermBoxRenderContext&(const LD::TermBoxRenderContext&, const LD::Detail::tVec2<LD::Integer> &)>::Value()
        >,const LD::TermBoxRenderContext&> Render(T && object, const LD::Detail::tVec2<LD::Integer> & translation) const noexcept (noexcept(LD::Declval<LD::Detail::Decay_T<T>>()(LD::Declval<const LD::TermBoxRenderContext&>(),LD::Declval<LD::Detail::tVec2<LD::Integer>>())))
        {
            return object(*this,translation);
        }

        template<typename T>
        auto Render(T && object) noexcept(noexcept(LD::Declval<T>()(LD::Declval<LD::TermBoxRenderContext&>()))) -> LD::TypedRequires<LD::TermBoxRenderContext&,LD::ConvertiblyCallable<T,LD::TermBoxRenderContext&( LD::TermBoxRenderContext&)>::Value()>
        {
            return object(*this);
        }
    };


}
#endif /* TermBoxRenderingContext_hpp */
