//
//  TermBoxRenderingContext.hpp
//  DataStructures
//
//  Created by James Landess on 2/13/18.
//  Copyright © 2018 James Landess. All rights reserved.
//

#ifndef TermBoxRenderingContext_hpp
#define TermBoxRenderingContext_hpp


#include "Primitives/General/Vec2.h"
#include "Definitions/Integer.h"
#include "Definitions/Float.h"
#include "Definitions/Common.h"
#include "TermBox/termbox.h"
#include "Primitives/General/mapboxvariant.h"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/StaticArray.h"
#include "Primitives/General/Immutable.h"
#include "Algorithms/CompileTimeControlFlow.h"
#include "RenderingContext.h"
#include "MVC/SchedulingEvent.h"
#include "TermBoxEnumerations.h"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/VariadicPack.h"
#include "TypeTraits/TypeList.h"
#include "Primitives/General//ctre.hpp"
#include "RenderableConcept.h"
#include "Renderable.h"
#include "Primitives/General/StringView.h"
namespace LD
{


    class TermBox
    {
        
    };
    
    template<>
    class RenderingContext<TermBox>
    {
    private:
        PDP::Detail::tVec2<LD::UInteger> Cursor;
        LD::UInteger LineWidth;
        PDP::TermBoxConsoleApplicationColorSpectrum CurrentSpectrum;
        PDP::TermBoxConsoleApplicationColorModifier CurrentForegroundModifier;
        PDP::TermBoxConsoleApplicationColorModifier CurrentBackgroundModifier;
        unsigned short CurrentForegroundColor;
        unsigned short CurrentBackgroundColor;
    public:
        
        RenderingContext();
        
        PDP::Detail::tVec2<LD::UInteger> & GetCursor() {return this->Cursor;}
        const PDP::Detail::tVec2<LD::UInteger> & GetCursor() const {return this->Cursor;}
        void SetCurrentSpectrum(const PDP::TermBoxConsoleApplicationColorSpectrum & spectrum);
        
        
        void SetForegroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier & modifier);
        void SetForegroundColor(const unsigned short & defaultColor);
        
        
        void SetBackgroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier & modifier);
        void SetBackgroundColor(const unsigned short & defaultColor);
        
        const PDP::TermBoxConsoleApplicationColorSpectrum & GetSpectrum() const;
        
        
        const PDP::TermBoxConsoleApplicationColorModifier & GetForegroundModifier() const;
        const unsigned short & GetForegroundColor() const;
        
        const PDP::TermBoxConsoleApplicationColorModifier & GetBackgroundModifier() const;
        const unsigned short & GetBackgroundColor() const;
        
        
        void Clear(const unsigned short & foregroundColor = PDP::eTBGreen, const unsigned short & backgroundColor = PDP::eTBDefault, const PDP::TermBoxConsoleApplicationColorModifier & foregroundModifier = PDP::eTBNone, const PDP::TermBoxConsoleApplicationColorModifier & backgroundModifier = PDP::eTBNone, const PDP::TermBoxConsoleApplicationColorSpectrum & spectrum = PDP::eNormalSpectrum);
        
        
        RenderingContext<TermBox> & Write(const wchar_t * characters, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const char * characters, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::Integer & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::UInteger & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const LD::Float & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const char & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(unsigned char & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
        RenderingContext<TermBox> & Write(const wchar_t & character, const PDP::Detail::tVec2<LD::UInteger> & translation);
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
        TermBoxKeyboardEvent(const uint8_t & mod, const uint16_t & key,const uint32_t & character):CharacterValue(character),KeyValue(key),ModifierValue(mod){}
        
        const uint8_t & Modifier() const {return this->ModifierValue;}
        const uint16_t & Key() const {return this->KeyValue;}
        const uint32_t & Character() const {return this->CharacterValue;}
    };
    
    
    class TermBoxMouseEvent
    {
    private:
        int32_t XValue;
        int32_t YValue;
    public:
        
        
        TermBoxMouseEvent(const int32_t & x, const int32_t & y):XValue(x),YValue(y){}
        const int32_t & X() const {return this->XValue;}

        const int32_t & Y() const {return this->YValue;}
    };
    
    
    class TermBoxResizingEvent
    {
    private:
        int32_t w;
        int32_t h;
    public:
        
        const int32_t & GetWidth() const {return this->w;}
        
        const int32_t & GetHeight() const {return this->h;}
        
        
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
        PDP::Detail::tVec2<LD::Integer> Cursor;
        PDP::Detail::tVec2<LD::Integer> mMouse;
        LD::UInteger LineWidth;
        PDP::TermBoxConsoleApplicationColorSpectrum CurrentSpectrum;
        PDP::TermBoxConsoleApplicationColorModifier CurrentForegroundModifier;
        PDP::TermBoxConsoleApplicationColorModifier CurrentBackgroundModifier;
        unsigned short CurrentForegroundColor;
        unsigned short CurrentBackgroundColor;
        unsigned short mMouseEnabled;
        tb_event CurrentEvent;
        const TermBoxRenderContext & Write(const char & character, const PDP::Detail::tVec2<LD::Integer> & translation) const;
    public:




        template<typename T>
        constexpr static LD::Enable_If_T<
                LD::Require<
                        LD::ConvertiblyCallable<LD::Decay_T<T>,PDP::Detail::tVec2<LD::UInteger>()>::Value(),
                        LD::IsClass<T>
                        >
        ,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept (noexcept(LD::Declval<T>()(LD::Declval<LD::RenderableDimensionEvent>())))
        {
            return object(LD::RenderableDimensionEvent{});
        }


        template<typename T>
        constexpr static LD::Enable_If_T<LD::Require<!LD::IsClass<T>>,PDP::Detail::tVec2<LD::UInteger>> GetRenderableDimensions(T && object) noexcept
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
        //tb_event & GetEvent() {return this->CurrentEvent;}
        PDP::Detail::tVec2<LD::Integer > & GetCursor() {return this->Cursor;}
        const PDP::Detail::tVec2<LD::Integer > & GetCursor() const {return this->Cursor;}
        PDP::Detail::tVec2<LD::Integer > & GetMouse() {return this->mMouse;}
        const PDP::Detail::tVec2<LD::Integer > & GetMouse() const {return this->mMouse;}
        void SetCurrentSpectrum(const PDP::TermBoxConsoleApplicationColorSpectrum & spectrum);
        void SetForegroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier & modifier);
        void SetForegroundColor(const unsigned short & defaultColor);
        void SetBackgroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier & modifier);
        void SetBackgroundColor(const unsigned short & defaultColor);
        const PDP::TermBoxConsoleApplicationColorSpectrum & GetSpectrum() const;
        const PDP::TermBoxConsoleApplicationColorModifier & GetForegroundModifier() const;
        const unsigned short & GetForegroundColor() const;
        const PDP::TermBoxConsoleApplicationColorModifier & GetBackgroundModifier() const;
        const unsigned short & GetBackgroundColor() const;
        /**
         CurrentForegroundModifier(eTBNone),CurrentForegroundColor(eTBGreen),CurrentSpectrum(eNormalSpectrum),LineWidth(tb_width()),CurrentBackgroundColor(eTBDefault),CurrentBackgroundModifier(eTBNone)
         */
        void Clear(const unsigned short & foregroundColor = PDP::eTBGreen, const unsigned short & backgroundColor = PDP::eTBDefault, const PDP::TermBoxConsoleApplicationColorModifier & foregroundModifier = PDP::eTBNone, const PDP::TermBoxConsoleApplicationColorModifier & backgroundModifier = PDP::eTBNone, const PDP::TermBoxConsoleApplicationColorSpectrum & spectrum = PDP::eNormalSpectrum);
        template<LD::UInteger StringSize>
        const TermBoxRenderContext& Render(const char (&lit)[StringSize], const PDP::Detail::tVec2<LD::Integer> & translation) const
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
        >,const TermBoxRenderContext&> Render(const T & integer, const PDP::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
            return this->Render(LD::ToImmutableString((integer)),translation);
        }
        template<typename T>
        constexpr LD::Enable_If_T<LD::Require<LD::IsTypeString<T>>,const TermBoxRenderContext&> Render(const T & typeString, const PDP::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
            return this->Render(LD::ToImmutableString((typeString)),translation);
        }
        template<typename T>
        LD::Enable_If_T<
                LD::Require<
                        LD::Either<
                                LD::Detail::IsSame<T, float>::value,
                                LD::Detail::IsSame<T, double>::value>
                >,const TermBoxRenderContext&> Render(const T & floatingPointNumber, const PDP::Detail::tVec2<LD::Integer> & translation, const LD::UInteger & precision = 0) const
        {
          return this->Render(LD::ToImmutableString(floatingPointNumber,precision),translation);
        }
        template<LD::UInteger Size>
        constexpr const TermBoxRenderContext & Render(const LD::ImmutableString<Size> & str, const PDP::Detail::tVec2<LD::Integer > & translation) const noexcept
        {
            LD::For<Size>([](auto Index,const LD::ImmutableString<Size> & str,const TermBoxRenderContext * instance, const PDP::Detail::tVec2<LD::Integer > & translation)
            {
                const char & currentCharacter = str[Index];
                instance->Write(currentCharacter,translation+PDP::Detail::tVec2<LD::Integer>{Index,0});
                return (Index < str.GetSize());
            },str,this,translation);
            return (*this);
        }


        constexpr const TermBoxRenderContext & Render(const LD::StringView & view, const PDP::Detail::tVec2<LD::Integer> & translation) const noexcept
        {
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                const char & currentCharacter = view[n];
                this->Write(currentCharacter,translation+PDP::Detail::tVec2<LD::Integer>{n,0});
            }
            return (*this);
        }


        template<LD::UInteger Size,typename ... Arguements>
        LD::Enable_If_T<
        LD::Require<
                (LD::IsRenderable2D<Arguements,const LD::TermBoxRenderContext&> && ...),
                (sizeof...(Arguements) > 0),
                (!LD::Detail::IsPointer<Arguements>::value && ...)
        >,const TermBoxRenderContext &> RenderWithFormat(const LD::ImmutableString<Size> & str,const PDP::Detail::tVec2<LD::Integer> & translation,Arguements && ... objects) const noexcept
        {
            using ArguementList = LD::TypeList<LD::Decay_T<Arguements>...>;
            using RenderableContextTypeList = LD::TypeList<LD::NullClass,LD::ElementReference <LD::Decay_T<Arguements>>...,LD::ElementReference <char>>;
            using DeDuplicatedRederableContextTypeList = typename LD::DeDuplicateTypeList<RenderableContextTypeList>::type ;
            using Renderable = LD::Rebind<DeDuplicatedRederableContextTypeList,LD::Variant>;
            using ContextTypeList = LD::TypeList<LD::ElementReference<LD::Decay_T <Arguements>>...>;
            using DeDuplicatedContextTypeList = typename LD::DeDuplicateTypeList<ContextTypeList>::type ;
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
            PDP::Detail::tVec2<LD::Integer> offsetTranslation = {0,0};
            LD::For<Size>([](
                    auto Index,
                    const TermBoxRenderContext * instance,
                    const LD::StaticArray<Renderable ,Size> & renderables,
                    const PDP::Detail::tVec2<LD::Integer> & translation,
                    PDP::Detail::tVec2<LD::Integer> & offsetTranslation,
                    const LD::ReferenceableTuple<Arguements...> & packedArguements)
            {

                LD::Match(renderables[Index],[&]( auto && object)->LD::Enable_If_T<LD::Require<!LD::IsSame<LD::Decay_T <decltype(object)>,LD::NullClass>>,void>
                {
                    instance->Render(LD::Get(object),translation+PDP::Detail::tVec2<LD::Integer>{Index,0}+offsetTranslation);
                    offsetTranslation.X() += (LD::TermBoxRenderContext::GetRenderableDimensions(LD::Get(object)).X()-1);
                },[&](const LD::NullClass&){offsetTranslation.X()-=1;});
                return true;
            },this,renderables,translation,offsetTranslation,packedArguements);
            return (*this);
        }


        template<typename T>
        constexpr LD::Enable_If_T<
         LD::Require<
         LD::ConvertiblyCallable<LD::Decay_T<T>,const LD::TermBoxRenderContext&(const LD::TermBoxRenderContext&, const PDP::Detail::tVec2<LD::Integer> &)>::Value()
        >,const LD::TermBoxRenderContext&> Render(T && object, const PDP::Detail::tVec2<LD::Integer> & translation) const noexcept (noexcept(LD::Declval<LD::Decay_T<T>>()(LD::Declval<const LD::TermBoxRenderContext&>(),LD::Declval<PDP::Detail::tVec2<LD::Integer>>())))
        {
            return object(*this,translation);
        }
    };


}
#endif /* TermBoxRenderingContext_hpp */
