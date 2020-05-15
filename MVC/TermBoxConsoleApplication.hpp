//
//  TermBoxConsoleApplication.h
//  DataStructures
//
//  Created by James Landess on 7/15/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#ifndef __DataStructures__TermBoxConsoleApplication__
#define __DataStructures__TermBoxConsoleApplication__


#include <Definitions/Common.hpp>
#include "Definitions/PDPUnits.hpp"
#include "TermBoxRenderingContext.hpp"
#include "SchedulingEvent.hpp"
#include "TermBox/termbox.h"
namespace LD
{
    template<typename T, class = void>
    class BasicTermBoxApplication;

    template<typename T, typename ... Context>
    class BasicTermBoxApplication<T(Context...),LD::Enable_If_T<!LD::Require<LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value>>>
     {
        static_assert(LD::Require<LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value>,"The provided Context Must contain LD::TermBoxRenderContext");
     };
    template<typename T, typename ... Context>
    class BasicTermBoxApplication<T(Context...),LD::Enable_If_T<LD::Require<LD::IsInTuple<LD::TermBoxRenderContext,LD::VariadicPack<typename LD::Decay<Context>::type...>>::value>>>
    {
    private:
        T CurrentApplication;
        LD::UInteger StartStatus;
    public:
        inline BasicTermBoxApplication(const T & application):CurrentApplication(application){}
        inline ~BasicTermBoxApplication(){}
        
        const bool operator()(const LD::ApplicaitonStartedEvent<Context...> & applicationStartedEvent)
        {
            this->StartStatus = (tb_init() == 0);
            return (this->StartStatus) && this->CurrentApplication(applicationStartedEvent);
        }
        
        const bool operator()(const LD::ApplicationFrameStartedEvent<Context...> & applicationFrameStartedEvent)
        {
            tb_event tbEvent;
            tb_peek_event(&tbEvent, 0);//get the even from this frame
            LD::Get<LD::TermBoxRenderContext>(applicationFrameStartedEvent)->CurrentEvent = tbEvent;
            //tb_clear_screen();
            tb_clear();//clear the buffer at the beginning of the frame
            LD::Get<LD::TermBoxRenderContext>(applicationFrameStartedEvent)->Clear();
            return this->CurrentApplication(applicationFrameStartedEvent);
        }

        PDP::Second<LD::Float> operator()(const LD::ApplicationPeriodEvent<Context...> & sleepingEvent)
        {
            return this->CurrentApplication(sleepingEvent);
        }
        
        void operator()(const LD::ApplicationExecutionEvent<Context...> & executionEvent)
        {
            //set a default state for the output mode
            tb_select_output_mode(LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetSpectrum());
            //allow the application to render out

            //LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetCursor().X() = double(tb_width())/2.0f;
            //LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetCursor().Y() = double(tb_height())/2.0f;
            this->CurrentApplication(executionEvent);
            //present the actual buffer to the screen

            LD::UInteger possibleXValue = LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetMouse().X();
            LD::UInteger possibleYValue = LD::Get<LD::TermBoxRenderContext>(executionEvent)->GetMouse().Y();

            const LD::UInteger isMouseEnabled = LD::Get<LD::TermBoxRenderContext>(executionEvent)->IsMouseEnabled();
            //LD::Integer  xValue = (isMouseEnabled * (((tb_width()/2)+possibleXValue)+1))-1;
            LD::Integer xValue = (isMouseEnabled)*(tb_width()/2+possibleXValue) + (!isMouseEnabled)*-1;
            //LD::Integer  yValue = (isMouseEnabled * (((tb_height()/2)+possibleYValue)+1))-1;
            LD::Integer yValue = (isMouseEnabled)*(tb_height()/2-possibleYValue) + (!isMouseEnabled)*-1;



            tb_set_cursor(xValue,yValue);
            tb_present();
        }
        
        void operator()(const LD::ApplicationFrameEndedEvent<Context...> & frameEndedEvent)
        {
            this->CurrentApplication(frameEndedEvent);
        }

        void operator()(const LD::ApplicationQuittingEvent<Context...> & quittingEvent)
        {
            this->CurrentApplication(quittingEvent);
            if(this->StartStatus == true)
            {
                tb_shutdown();
            }
        }
    };
}
namespace PDP
{
    
    /*
     #define TB_OUTPUT_CURRENT   0
     #define TB_OUTPUT_NORMAL    1
     #define TB_OUTPUT_256       2
     #define TB_OUTPUT_216       3
     #define TB_OUTPUT_GRAYSCALE 4
     
     #define TB_BOLD      0x0100
     #define TB_UNDERLINE 0x0200
     #define TB_REVERSE   0x0400
     
     
     #define TB_DEFAULT 0x00
     #define TB_BLACK   0x01
     #define TB_RED     0x02
     #define TB_GREEN   0x03
     #define TB_YELLOW  0x04
     #define TB_BLUE    0x05
     #define TB_MAGENTA 0x06
     #define TB_CYAN    0x07
     #define TB_WHITE   0x08
     */
    
    //               x, y, ch,, foreground,         background
    //tb_change_cell(0, 1, 'a', TB_MAGENTA|TB_BOLD, 0         );
    
    
    
    
    /*
    class TermBoxConsoleApplication: public PDP::TimedSignalProtocol
    {
    private:
    public:
        TermBoxConsoleApplication();
        
        
        ~TermBoxConsoleApplication();
        
        
        virtual void OnSignalHandled(const PDP::CoreContext & eventData);//overidden function to help with signal handling
        
        
        virtual void OnRender(const PDP::CoreContext &, LD::TermBoxRenderContext & renderingContext);
        
    };
     */
    
    
}
#endif /* defined(__DataStructures__TermBoxConsoleApplication__) */
