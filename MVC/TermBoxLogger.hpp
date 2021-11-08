//
// Created by phoenixflower on 11/5/21.
//

#ifndef LANDESSDEVCORE_TERMBOXLOGGER_HPP
#define LANDESSDEVCORE_TERMBOXLOGGER_HPP
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Services/Logging.hpp"
namespace LD
{
    struct TermBoxLoggerMessage
    {
        LD::ImmutableString<256> mMessage;
        LD::Variant<DebugLog,ErrorLog,InfoLog,WarningLog> mState;
    };
    class TermBoxLogger: public AbstractLogger
    {
    private:
        LD::CircularQueue<LD::TermBoxLoggerMessage,15> mMessages;
        LD::BasicChannel<LD::CircularQueue<LD::TermBoxLoggerMessage,15>&> mMessageChannel;
    public:
        TermBoxLogger() noexcept: mMessageChannel{mMessages}
        {

        }
        virtual void PrintInfo(LD::StringView view) noexcept
        {
            this->mMessageChannel << TermBoxLoggerMessage{LD::ImmutableString<256>{view},LD::InfoLog{}};
        }
        virtual void PrintError(LD::StringView view) noexcept
        {
            this->mMessageChannel << TermBoxLoggerMessage{LD::ImmutableString<256>{view},LD::ErrorLog{}};
        }
        virtual void PrintDebug(LD::StringView view) noexcept
        {
            this->mMessageChannel << TermBoxLoggerMessage{LD::ImmutableString<256>{view},LD::DebugLog{}};
        }
        virtual void PrintWarning(LD::StringView view) noexcept
        {
            this->mMessageChannel << TermBoxLoggerMessage{LD::ImmutableString<256>{view},LD::WarningLog{}};
        }

        template<typename ... A>
        bool operator()(const LD::ApplicationStartedEvent<A...> & applicaitonStartedEvent)
        {
            return true;
        }

        template<typename ... A>
        bool operator()(const LD::ApplicationFrameStartedEvent<A...> & frameStarted) noexcept
        {
            return true;
        }

        template<typename ... A>
        LD::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<A...> & frameStarted) noexcept
        {
            return 1.0_s;
        }

        template<typename ... A>
        void operator()(const LD::ApplicationExecutionEvent<A...> & applicationExecutionEvent) noexcept
        {
            LD::Optional<LD::TermBoxLoggerMessage> possibleMessage;
            this->mMessageChannel >> possibleMessage;
            if(possibleMessage)
            {
                LD::ImmutableString<256> message = (*possibleMessage).mMessage;
                LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);

                LD::TUI::ImmutableTextLabel debugOutput(LD::BasicVec2D<LD::Integer>{5,5},message);
                renderingContext->Render(debugOutput);

            }
            //LD::ElementReference<LD::TermBoxRenderContext> renderingContext = LD::Get<LD::TermBoxRenderContext>(applicationExecutionEvent);
            //LD::TUI::ImmutableTextLabel debugOutput(LD::BasicVec2D<LD::Integer>{0,0},LD::ImmutableString{"debugString"});
            //renderingContext->Render(debugOutput);
        }

        template<typename ... A>
        void operator()(const LD::ApplicationFrameEndedEvent<A...> &) noexcept
        {

        }

        template<typename ... A>
        void operator()(const LD::ApplicationQuittingEvent<A...> &) noexcept
        {

        }
    };
}
#endif //LANDESSDEVCORE_TERMBOXLOGGER_HPP
