//
// Created by phoenixflower on 10/23/21.
//
#ifndef LANDESSDEVCORE_LOGGING_HPP
#define LANDESSDEVCORE_LOGGING_HPP
#include "Primitives/General/mapboxvariant.hpp"
#include "TypeTraits/Detection.hpp"
#include "Algorithms/Visitation.hpp"
#include "Patterns/DependencyInjection.hpp"
#include "TypeTraits/CanBeAnImmutableString.hpp"
namespace LD
{
    class DebugLog
    {

    };

    class ErrorLog
    {

    };

    class InfoLog
    {

    };

    class WarningLog
    {

    };

    class AbstractLogger
    {
    public:
        virtual void PrintInfo(LD::StringView view) noexcept = 0;
        virtual void PrintError(LD::StringView view) noexcept = 0;
        virtual void PrintDebug(LD::StringView view) noexcept = 0;
        virtual void PrintWarning(LD::StringView view) noexcept = 0;
    };

    class PrintFLogger: public AbstractLogger
    {
    private:
        virtual void PrintInfo(LD::StringView view) noexcept
        {
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                printf("%c",view[n]);
            }
        }
        virtual void PrintError(LD::StringView view) noexcept
        {
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                printf("%c",view[n]);
            }
        }
        virtual void PrintDebug(LD::StringView view) noexcept
        {
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                printf("%c",view[n]);
            }
        }
        virtual void PrintWarning(LD::StringView view) noexcept
        {
            for(LD::UInteger n = 0;n<view.size();++n)
            {
                printf("%c",view[n]);
            }
        }
    };
    class BasicLogger: public LD::DependencyInjection<BasicLogger(AbstractLogger&)>
    {
    private:
        LD::Variant<DebugLog,ErrorLog,InfoLog,WarningLog> mState;
        AbstractLogger & mLogger;
    public:

        BasicLogger(AbstractLogger & logger) noexcept:mLogger{logger}
        {

        }
        template<typename U>
        LD::Enable_If_T<LD::Require<LD::CT::CanBeAnImmutableString(LD::Type<U>{})>,BasicLogger &> operator << (U && object) noexcept
        {
            auto immutableString = LD::ToImmutableString(LD::Forward<U>(object));

            return (*this) << LD::StringView{immutableString.Data()};
        }

        BasicLogger & operator << (DebugLog style) noexcept
        {
            this->mState = style;
            return (*this);
        }
        BasicLogger & operator << (ErrorLog style) noexcept
        {
            this->mState = style;
            return (*this);
        }
        BasicLogger & operator << (InfoLog style) noexcept
        {
            this->mState = style;
            return (*this);
        }
        BasicLogger & operator << (WarningLog style) noexcept
        {
            this->mState = style;
            return (*this);
        }
        BasicLogger & operator << (LD::StringView view) noexcept
        {
            auto onInfoLog = [](InfoLog, LD::ElementReference<AbstractLogger> logger, LD::StringView log)
            {
                logger->PrintInfo(log);
            };

            auto onWarningLog = [](WarningLog, LD::ElementReference<AbstractLogger> logger, LD::StringView log)
            {
                logger->PrintWarning(log);
            };

            auto onErrorLog = [](ErrorLog, LD::ElementReference<AbstractLogger> logger, LD::StringView log)
            {
                logger->PrintError(log);
            };

            auto onDebugLog = [](DebugLog, LD::ElementReference<AbstractLogger> logger, LD::StringView log)
            {
                logger->PrintDebug(log);
            };

            auto overloadVisitor = LD::Overload{onInfoLog,onWarningLog,onErrorLog,onDebugLog};
            LD::VisitWithContext(overloadVisitor,LD::MakeTuple(LD::ElementReference<AbstractLogger>{this->mLogger},LD::StringView{view}),this->mState);
            return (*this);
        }

        template<LD::UInteger Size>
        BasicLogger & operator << (const char (&lit)[Size]) noexcept
        {
            return (*this) << LD::StringView{lit};
        }

    };
}
#endif //LANDESSDEVCORE_LOGGING_HPP
