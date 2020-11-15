//
// Created by phoenixflower on 11/11/20.
//

#ifndef LANDESSDEVCORE_EVENTHANDLER_HPP
#define LANDESSDEVCORE_EVENTHANDLER_HPP
#include "Variable.hpp"
#include "Attributes.hpp"
#include "Reflection/Reflection.hpp"
#include "TypeTraits/Detection.hpp"
namespace LD
{
    namespace SPA
    {
        struct OnAfterPrint
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onafterprint"};
        };

        struct OnBeforePrint
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onbeforeprint"};
        };

        struct OnBeforeUnload
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onbeforeunload"};
        };

        struct OnError
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onerror"};
        };

        struct OnHashChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onhashchange"};
        };

        struct OnLoad
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onload"};
        };

        struct OnMessage
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmessage"};
        };

        struct OnOffline
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onoffline"};
        };

        struct OnOnline
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ononline"};
        };

        struct OnPageHide
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onpagehide"};
        };

        struct OnPageShow
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onpageshow"};
        };

        struct OnPopState
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onpopstate"};
        };

        struct OnReSize
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onresize"};
        };

        struct OnStorage
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onstorage"};
        };

        struct OnUnLoad
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onunload"};
        };


        using WindowEventAttributes = LD::CT::TypeList<LD::SPA::OnAfterPrint,LD::SPA::OnBeforePrint,LD::SPA::OnBeforeUnload,LD::SPA::OnError,LD::SPA::OnHashChange,
        LD::SPA::OnLoad,LD::SPA::OnMessage,LD::SPA::OnOffline,LD::SPA::OnOnline,LD::SPA::OnPageHide,LD::SPA::OnPageShow,LD::SPA::OnPopState,LD::SPA::OnReSize,
        LD::SPA::OnStorage,LD::SPA::OnUnLoad>;

        struct OnBlur
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onblur"};
        };

        struct OnChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onchange"};
        };

        struct OnContextMenu
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncontextmenu"};
        };

        struct OnFocus
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onfocus"};
        };

        struct OnInput
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oninput"};
        };

        struct OnInValid
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oninvalid"};
        };

        struct OnReset
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onreset"};
        };

        struct OnSearch
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onsearch"};
        };

        struct OnSelect
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onselect"};
        };

        struct OnSubmit
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onsubmit"};
        };


        using FormEvents = LD::CT::TypeList<LD::SPA::OnBlur,LD::SPA::OnChange,LD::SPA::OnContextMenu,LD::SPA::OnFocus,LD::SPA::OnInput,
        LD::SPA::OnInValid,LD::SPA::OnReset,LD::SPA::OnSearch,LD::SPA::OnSelect,LD::SPA::OnSubmit>;

        struct OnKeyDown
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onkeydown"};
        };

        struct OnKeyPress
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onkeypress"};
        };

        struct OnKeyUp
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onkeyup"};
        };


        using KeyboardEvents = LD::CT::TypeList<LD::SPA::OnKeyDown,LD::SPA::OnKeyPress,LD::SPA::OnKeyUp>;

        struct OnClick
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onclick"};
        };

        struct OnDoubleClick
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondblclick"};
        };

        struct OnMouseDown
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmousedown"};
        };

        struct OnMouseMove
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmousemove"};
        };

        struct OnMouseOut
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmouseout"};
        };

        struct OnMouseOver
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmouseover"};
        };

        struct OnMouseUp
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onmouseup"};
        };

        struct OnWheel
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onwheel"};
        };

        using MouseEvents = LD::CT::TypeList<LD::SPA::OnClick,LD::SPA::OnDoubleClick,LD::SPA::OnMouseDown,LD::SPA::OnMouseMove,LD::SPA::OnMouseOut,
        LD::SPA::OnMouseOver,LD::SPA::OnMouseUp,LD::SPA::OnWheel>;

        struct OnDrag
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondrag"};
        };

        struct OnDragEnd
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondragend"};
        };

        struct OnDragEnter
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondragenter"};
        };

        struct OnDragLeave
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondragleave"};
        };

        struct OnDragOver
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondragover"};
        };

        struct OnDragStart
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondragstart"};
        };

        struct OnDrop
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondrop"};
        };

        struct OnScroll
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onscroll"};
        };

        using DragEvents = LD::CT::TypeList<LD::SPA::OnDrag,LD::SPA::OnDragEnd,LD::SPA::OnDragEnter,LD::SPA::OnDragLeave,LD::SPA::OnDragOver,LD::SPA::OnDragStart,
        LD::SPA::OnDrop,LD::SPA::OnScroll>;

        struct OnCopy
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncopy"};
        };

        struct OnCut
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncut"};
        };

        struct OnPaste
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onpaste"};
        };

        using ClipboardEvents = LD::CT::TypeList<LD::SPA::OnCopy,LD::SPA::OnCut,LD::SPA::OnPaste>;

        struct OnAbort
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onabort"};
        };

        struct OnCanPlay
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncanplay"};
        };

        struct OnCanPlayThrough
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncanplaythrough"};
        };

        struct OnCueChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"oncuechange"};
        };

        struct OnDurationChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ondurationchange"};
        };

        struct OnEmptied
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onemptied"};
        };

        struct OnEnded
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onended"};
        };

        struct OnLoadedData
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onloadeddata"};
        };

        struct OnLoadedMetaData
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onloadedmetadata"};
        };

        struct OnLoadStart
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onloadstart"};
        };

        struct OnPause
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onpause"};
        };

        struct OnPlay
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onplay"};
        };

        struct OnPlaying
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onplaying"};
        };

        struct OnProgress
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onplaying"};
        };

        struct OnRateChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onratechange"};
        };

        struct OnSeeked
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onseeked"};
        };

        struct OnSeeking
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onseeking"};
        };

        struct OnStalled
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onstalled"};
        };

        struct OnSuspend
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onsuspend"};
        };

        struct OnTimeUpdate
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ontimeupdate"};
        };

        struct OnVolumeChange
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onvolumechange"};
        };

        struct OnWaiting
        {
            static constexpr auto Name = ctll::basic_fixed_string{"onwaiting"};
        };

        using MediaEvents = LD::CT::TypeList<LD::SPA::OnAbort,LD::SPA::OnCanPlay,LD::SPA::OnCanPlayThrough,LD::SPA::OnCueChange,LD::SPA::OnDurationChange,
        LD::SPA::OnEmptied,LD::SPA::OnEnded,LD::SPA::OnError,LD::SPA::OnLoadedData,LD::SPA::OnLoadedMetaData,LD::SPA::OnLoadStart,LD::SPA::OnPause,LD::SPA::OnPlay,
        LD::SPA::OnPlaying,LD::SPA::OnProgress,LD::SPA::OnRateChange,LD::SPA::OnRateChange,LD::SPA::OnSeeked,LD::SPA::OnSeeking,LD::SPA::OnStalled,LD::SPA::OnSuspend,
        LD::SPA::OnTimeUpdate,LD::SPA::OnVolumeChange,LD::SPA::OnWaiting>;




        struct OnToggle
        {
            static constexpr auto Name = ctll::basic_fixed_string{"ontoggle"};
        };

        using MiscleanousEvents = LD::CT::TypeList<LD::SPA::OnToggle>;

        template<typename EventType, typename ExpressionType,class = void>
        class EventHandler;
        template<typename EventType, typename ExpressionType>
        class EventHandler<EventType,ExpressionType,LD::Enable_If_T<
                LD::Require<
                        LD::SPA::IsVarExpression(LD::CT::RemoveQualifiers(LD::Type<ExpressionType>{}))
                        >>>
        {
        private:
            ExpressionType mCurrentExpression;
        public:
            using EV = EventType;
            EventHandler(EventType &&, ExpressionType && expression) noexcept:mCurrentExpression{LD::Forward<ExpressionType>(expression)}{}

            const ExpressionType & Expression() const noexcept
            {
                return this->mCurrentExpression;
            }
            ExpressionType & Expression()  noexcept
            {
                return this->mCurrentExpression;
            }
        };



        template<typename EventType, typename Expression> EventHandler(EventType &&, Expression &&) -> EventHandler<EventType,Expression>;


    }
}
template<typename EventType, typename ExpressionType>
struct LD::CT::TypeDescriptor<LD::SPA::EventHandler<EventType,ExpressionType>>
{
private:
    static constexpr auto AmountName = LD::SPA::EventHandler<EventType,ExpressionType>::EV::Name;
    //ctll::basic_fixed_string("EventHandler");
public:
    static constexpr auto ClassName = ctll::basic_fixed_string("EventHandler");

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<ExpressionType & (LD::SPA::EventHandler<EventType,ExpressionType>::*)(),&LD::SPA::EventHandler<EventType,ExpressionType>::Expression>(),LD::CT::SelectOverload<const ExpressionType & (LD::SPA::EventHandler<EventType,ExpressionType>::*)() const,&LD::SPA::EventHandler<EventType,ExpressionType>::Expression>()>
    >;
    static constexpr MemberList Members{  };

};
#endif //LANDESSDEVCORE_EVENTHANDLER_HPP
