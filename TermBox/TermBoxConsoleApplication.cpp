//
//  TermBoxConsoleApplication.cpp
//  DataStructures
//
//  Created by James Landess on 7/15/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#include "TermBoxConsoleApplication.h"
#include "termbox.h"

namespace PDP
{
    static void print_combinations_table(int sx, int sy, const uint16_t *attrs, int attrs_n)
    {
        int i, c;
        //for (i = 0; i < attrs_n; i++)
        {
            //for (c = TB_DEFAULT; c <= TB_WHITE; c++)
            {
                
                //uint16_t bg = attrs[0] | c;
                //draw_line(0, 0, bg);
                tb_change_cell(0, 1, 'a', TB_MAGENTA|TB_BOLD, 0);
                tb_change_cell(1, 0, 'b', TB_RED, 0);
                tb_change_cell(1, 1, ' ', TB_RED, 0);
                tb_change_cell(3, 1, ' ', TB_RED, 0);
                tb_change_cell(3, 1, '+', TB_RED, 0);
                tb_change_cell(4, 1, 'y', TB_RED, 0);
                
                
                PDP::DataStructures::BasicString<char> rawrNumbers = PDP::UInteger( tb_width());
                
                
                for (PDP::UInteger n = 0; n<rawrNumbers.GetSize(); ++n )
                {
                    tb_change_cell(5+n, 0, rawrNumbers[n], TB_RED, 0);
                    
                }
                
                sy++;
            }
        }
    }
    
    static void draw_all()
    {
        
        tb_clear();
        
        tb_cell currentCell;
        
        tb_select_output_mode(TB_OUTPUT_NORMAL);
        
        
        static const uint16_t col1[] = {0, TB_BOLD};
        static const uint16_t col2[] = {TB_REVERSE};
        
        
        print_combinations_table(1, 1, col1, 2);
        
        
        //print_combinations_table(2 + strlen(chars), 1, col2, 1);
        tb_present();
        
        
        /*
         tb_select_output_mode(TB_OUTPUT_GRAYSCALE);
         int c, x, y;
         for (x = 0, y = 23; x < 24; ++x) {
         tb_change_cell(x, y, '@', x, 0);
         tb_change_cell(x+25, y, ' ', 0, x);
         }
         tb_present();
         
         
         tb_select_output_mode(TB_OUTPUT_216);
         y++;
         for (c = 0, x = 0; c < 216; ++c, ++x) {
         if (!(x%24)) {
         x = 0;
         ++y;
         }
         tb_change_cell(x, y, '@', c, 0);
         tb_change_cell(x+25, y, ' ', 0, c);
         }
         tb_present();
         
         tb_select_output_mode(TB_OUTPUT_256);
         y++;
         for (c = 0, x = 0; c < 256; ++c, ++x) {
         if (!(x%24)) {
         x = 0;
         ++y;
         }
         tb_change_cell(x, y, '+', c | ((y & 1) ? TB_UNDERLINE : 0), 0);
         tb_change_cell(x+25, y, ' ', 0, c);
         }
         tb_present();
         */
    }
    
    
    TermBoxConsoleApplication::TermBoxConsoleApplication()
    {
        //:Cursor({0,0}),CurrentSetting(setting)
        //this->Cursor = {0,0};
        //this->CurrentSetting = setting;
        
        //this->InitializationStatus =  tb_init();
        if (tb_init())
        {
            printf("Can't initialize the tb windowing system\n");
            exit(0);
        }
    }
    TermBoxConsoleApplication::~TermBoxConsoleApplication()
    {
        tb_shutdown();
    }
    
    /*
    void TermBoxConsoleApplication::OnSignalHandled(const PDP::CoreContext & eventData)
    {
        tb_clear();
        
        
        tb_select_output_mode(CurrentSetting.GetSpectrum());
        
        PDP::CoreContext event(eventData);
        event.SetTimeElapsed(event.GetTimeElapsed());
        
        
        this->OnRender(event);
        
        tb_present();
        
        
        Cursor.X() = 0;
        Cursor.Y() = 0;
    }
    */
    
    void TermBoxConsoleApplication::OnSignalHandled(const PDP::CoreContext &metaData)
    {
        
        //tb_event tbEvent;
        //tb_peek_event(&tbEvent, 0);
        
        
        //do the correct amount of book keeping and clear the current context.  Similiar to a rendering context we should always clear before rendering to the screen
        tb_clear();
        
        
        
        //copy of the CoreContext, this is to take in account possibly amount of time that has occured
        PDP::CoreContext event(metaData);
        
        
        event.SetTimeElapsed(metaData.GetTimeElapsed());
        
        
        
        PDP::TB::TermBoxRenderContext currentContext;//create the context to actually render stuff to the termbox buffer
        
        //set a default state for the output mode
        tb_select_output_mode(currentContext.GetSpectrum());
        //render all of the graphics to the rendering buffer pipeline utilizing the passed in context
        this->OnRender(event,currentContext);
        
        
        //present the actual buffer to the screen
        tb_present();
    }
    
    
    
    void TermBoxConsoleApplication::OnRender(const PDP::CoreContext &, PDP::TB::TermBoxRenderContext & renderingContext)
    {
        
    }
    

    PDP::TB::TermBoxRenderContext::TermBoxRenderContext():CurrentForegroundModifier(eTBNone),CurrentForegroundColor(eTBGreen),CurrentSpectrum(eNormalSpectrum),LineWidth(tb_width()),CurrentBackgroundColor(eTBDefault),CurrentBackgroundModifier(eTBNone)
   
    {
        
    }
    
    
    
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const PDP::Integer & character)
    {
        PDP::DataStructures::BasicString<char> numberInIntegerForm = character;
        
        this->Write(numberInIntegerForm.GetCString());
        return (*this);
    }
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const PDP::UInteger & character)
    {
        PDP::DataStructures::BasicString<char> numberInIntegerForm = character;
        
        this->Write(numberInIntegerForm.GetCString());
        return (*this);
    }
    
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const PDP::Float & character)
    {
        PDP::DataStructures::BasicString<char> floatingPointNumber = character;
        
        this->Write(floatingPointNumber.GetCString());
        
        return (*this);
    }
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const char & character)
    {
        //tb_change_cell(0, 1, 'a', TB_MAGENTA|TB_BOLD, 0         );
        
        
        if (character == '\n')
        {
            this->Cursor.X() = 0;
            this->Cursor.Y()+=1;
            
        }else if(character == '\t')
        {
            this->Cursor.X()+=5;
        }
        else
        {
            int x = (int)Cursor.X();
            int y = (int)Cursor.Y();
            
            
            tb_change_cell(x, y, character, this->CurrentForegroundColor | this->CurrentForegroundModifier, this->CurrentBackgroundColor| this->CurrentBackgroundModifier);
            
            ++Cursor.X();
            
            if (Cursor.X() > tb_width())
            {
                Cursor.X() = 0;
                ++Cursor.Y();
            }
        }
        
        
        
        
        
        
        
        //tb_change_cell((int)Cursor.X(), (int)Cursor.Y(), character, TB_MAGENTA|TB_BOLD, 0);
        
        //Cursor.X() = Cursor.X()+1;
        
        
        //tb_change_cell(1, 0, 'b', TB_RED, 0);
        //tb_change_cell(1, 1, ' ', TB_RED, 0);
        //tb_change_cell(3, 1, ' ', TB_RED, 0);
        //tb_change_cell(3, 1, '+', TB_RED, 0);
        //tb_change_cell(4, 1, 'y', TB_RED, 0);
        
        
        return (*this);
    }
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(unsigned char & character)
    {
        if (character == '\n')
        {
            this->Cursor.X() = 0;
            this->Cursor.Y()+=1;
            
        }else if(character == '\t')
        {
            this->Cursor.X()+=5;
        }
        else
        {
            int x = (int)Cursor.X();
            int y = (int)Cursor.Y();
            
            
            tb_change_cell(x, y, character, this->CurrentForegroundColor | this->CurrentForegroundModifier, this->CurrentBackgroundColor| this->CurrentBackgroundModifier);
            
            ++Cursor.X();
            
            if (Cursor.X() > tb_width())
            {
                Cursor.X() = 0;
                ++Cursor.Y();
            }
        }
        
        
        return (*this);
    }
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const wchar_t & character)
    {
        if (character == '\n')
        {
            
        }else if(character == '\t')
        {
            
        }
        else
        {
            int x = (int)Cursor.X();
            int y = (int)Cursor.Y();
            
            
            tb_change_cell(x, y, character, this->CurrentForegroundColor | this->CurrentForegroundModifier, this->CurrentBackgroundColor| this->CurrentBackgroundModifier);
            
            ++Cursor.X();
            
            if (Cursor.X() >= tb_width())
            {
                Cursor.X() = 0;
                ++Cursor.Y();
            }
        }
        
        
        return (*this);
    }
    
    
    
    
    
    
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const wchar_t * characters)
    {
        PDP::UInteger size =  PDP::DataStructures::BasicString<wchar_t>::GetLength(characters);
        
        
        for (PDP::UInteger n = 0; n<size; ++n )
        {
            this->Write(*(characters+n));
        }
        return (*this);
    }
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::Write(const char * characters)
    {
        
        PDP::UInteger size = PDP::DataStructures::BasicString<char>::GetLength(characters);
        
        //PDP::DataStructures::BasicString<char> number(Cursor.X());
        
        for (PDP::UInteger n = 0; n<size; ++n )
        {
            //tb_change_cell((int)n, 0, number[n], TB_MAGENTA|TB_BOLD, 0);
            //Cursor.X()++;
            
            this->Write(*(characters+n));
        }
        
        return (*this);
    }
    
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::operator<<(const wchar_t *characters)
    {
        return this->Write(characters);
    }
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::operator<<(const char *characters)
    {
        return this->Write(characters);
    }
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::operator<<(const wchar_t & characters)
    {
        return this->Write(characters);
    }
    
    PDP::TB::TermBoxRenderContext & PDP::TB::TermBoxRenderContext::operator<<(const char & characters)
    {
        return this->Write(characters);
    }
    
    
    
    
    void PDP::TB::TermBoxRenderContext::SetForegroundColor(const unsigned short &defaultColor)
    {
        this->CurrentForegroundColor = defaultColor;
    }
    
    void PDP::TB::TermBoxRenderContext::SetForegroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier &modifier)
    {
        this->CurrentForegroundModifier = modifier;
    }
    
    void PDP::TB::TermBoxRenderContext::SetBackgroundColor(const unsigned short & defaultColor)
    {
        this->CurrentBackgroundColor = defaultColor;
    }
    
    void PDP::TB::TermBoxRenderContext::SetBackgroundCurrentModifier(const PDP::TermBoxConsoleApplicationColorModifier &modifier)
    {
        this->CurrentBackgroundModifier = modifier;
    }
    
    void PDP::TB::TermBoxRenderContext::SetCurrentSpectrum(const PDP::TermBoxConsoleApplicationColorSpectrum &spectrum)
    {
        this->CurrentSpectrum = spectrum;
    }
    
    const TermBoxConsoleApplicationColorSpectrum & PDP::TB::TermBoxRenderContext::GetSpectrum() const
    {
        return this->CurrentSpectrum;
    }
    
    
    const TermBoxConsoleApplicationColorModifier & PDP::TB::TermBoxRenderContext::GetForegroundModifier() const
    {
        return this->CurrentForegroundModifier;
    }
    
    const unsigned short & PDP::TB::TermBoxRenderContext::GetForegroundColor() const
    {
        return this->CurrentForegroundColor;
    }
    
    const TermBoxConsoleApplicationColorModifier & PDP::TB::TermBoxRenderContext::GetBackgroundModifier() const
    {
        return this->CurrentBackgroundModifier;
    }
    
    const unsigned short & PDP::TB::TermBoxRenderContext::GetBackgroundColor() const
    {
        return this->CurrentBackgroundColor;
    }
    
    void PDP::TB::TermBoxRenderContext::Clear(const unsigned short & foregroundColor, const unsigned short & backgroundColor, const TermBoxConsoleApplicationColorModifier & foregroundModifier, const TermBoxConsoleApplicationColorModifier & backgroundModifier, const TermBoxConsoleApplicationColorSpectrum & spectrum)
    {
        this->Cursor = {0,0};
        this->LineWidth = tb_width();
    }
}
