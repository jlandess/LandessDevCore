//
//  TermBoxConsoleApplication.cpp
//  DataStructures
//
//  Created by James Landess on 7/15/17.
//  Copyright (c) 2017 James Landess. All rights reserved.
//

#include "TermBoxConsoleApplication.hpp"


namespace PDP
{
    /*
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


                PDP::DataStructures::BasicString<char> rawrNumbers = LD::UInteger( tb_width());


                for (LD::UInteger n = 0; n<rawrNumbers.GetSize(); ++n )
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


    /*
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

    /*
    void TermBoxConsoleApplication::OnSignalHandled(const PDP::CoreContext &metaData)
    {

        //tb_event tbEvent;
        //tb_peek_event(&tbEvent, 0);


        //do the correct amount of book keeping and clear the current context.  Similiar to a rendering context we should always clear before rendering to the screen
        tb_clear();



        //copy of the CoreContext, this is to take in account possibly amount of time that has occured
        PDP::CoreContext event(metaData);


        event.SetTimeElapsed(metaData.GetTimeElapsed());



        LD::TermBoxRenderContext currentContext;//create the context to actually render stuff to the termbox buffer

        //set a default state for the output mode
        tb_select_output_mode(currentContext.GetSpectrum());
        //render all of the graphics to the rendering buffer pipeline utilizing the passed in context
        this->OnRender(event,currentContext);


        //present the actual buffer to the screen
        tb_present();
    }



    void TermBoxConsoleApplication::OnRender(const PDP::CoreContext &, LD::TermBoxRenderContext & renderingContext)
    {

    }
     */






namespace LD
{
    TermBoxRenderContext::TermBoxRenderContext():
            CurrentForegroundModifier(LD::eTBNone),CurrentForegroundColor(LD::eTBGreen),CurrentSpectrum(LD::eNormalSpectrum),LineWidth(tb_width()),CurrentBackgroundColor(LD::eTBDefault),CurrentBackgroundModifier(LD::eTBNone)
    {


    }




    /*
    TermBoxRenderContext & TermBoxRenderContext::Write(const LD::Integer & unsignedInteger, const PDP::Detail::tVec2<LD::Integer> & translation)
    {
        //PDP::DataStructures::BasicString<char> numberInIntegerForm = character;

        //this->Write(std::to_string(character).c_str(),location);
        this->Write(LD::ToImmutableString(unsignedInteger),translation);
        return (*this);
    }
    TermBoxRenderContext & TermBoxRenderContext::Write(const LD::UInteger & character, const PDP::Detail::tVec2<LD::Integer> & translation)
    {
        //PDP::DataStructures::BasicString<char> numberInIntegerForm = character;

        //this->Write(std::to_string(character).c_str(),translation);
        this->Write(LD::ToImmutableString(character),translation);
        return (*this);
    }


    TermBoxRenderContext & TermBoxRenderContext::Write(const LD::Float & floatingPointNumber, const PDP::Detail::tVec2<LD::Integer> & location)
    {
        //PDP::DataStructures::BasicString<char> floatingPointNumber = character;

        //this->Write(std::to_string(character).c_str(),location);


        this->Write(LD::ToImmutableString(floatingPointNumber),location);
        return (*this);
    }

     */
    const TermBoxRenderContext & TermBoxRenderContext::Write(const char & character, const LD::Detail::tVec2<LD::Integer> & location) const noexcept
    {
        //int x = (int)Cursor.X();
        //int y = (int)Cursor.Y();

        int x = tb_width()/2 + location.X();
        int y = tb_height()/2 - location.Y();

        tb_change_cell(x, y, character, this->CurrentForegroundColor | this->CurrentForegroundModifier, this->CurrentBackgroundColor| this->CurrentBackgroundModifier);
        return (*this);
    }

    const TermBoxRenderContext & TermBoxRenderContext::Write(const wchar_t & character, const LD::Detail::tVec2<LD::Integer> & location) const noexcept
    {
        //int x = (int)Cursor.X();
        //int y = (int)Cursor.Y();

        int x = tb_width()/2 + location.X();
        int y = tb_height()/2 - location.Y();

        tb_change_cell(x, y, character, this->CurrentForegroundColor | this->CurrentForegroundModifier, this->CurrentBackgroundColor| this->CurrentBackgroundModifier);
        return (*this);
    }
    /*
    TermBoxRenderContext & TermBoxRenderContext::Write(unsigned char & character, const PDP::Detail::tVec2<LD::Integer> & location)
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
    TermBoxRenderContext & TermBoxRenderContext::Write(const wchar_t & character, const PDP::Detail::tVec2<LD::Integer> & location)
    {
        if (character == '\n')
        {
            this->Cursor.X() = 0;
            ++Cursor.Y();

        }else if(character == '\t')
        {
            Cursor.X()+=5;
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







    TermBoxRenderContext & TermBoxRenderContext::Write(const wchar_t * characters, const PDP::Detail::tVec2<LD::Integer> & location)
    {
        //LD::UInteger size =  PDP::DataStructures::BasicString<wchar_t>::GetLength(characters);

        LD::UInteger size = wcslen(characters);

        for (LD::UInteger n = 0; n<size; ++n )
        {
            this->Write(*(characters+n),location);
        }
        return (*this);
    }
    TermBoxRenderContext & TermBoxRenderContext::Write(const char * characters, const PDP::Detail::tVec2<LD::Integer> & location)
    {


        LD::UInteger size = strlen(characters);

        //PDP::DataStructures::BasicString<char> number(Cursor.X());



        for (LD::UInteger n = 0; n<size; ++n )
        {
            //tb_change_cell((int)n, 0, number[n], TB_MAGENTA|TB_BOLD, 0);
            //Cursor.X()++;

            //this->Write(*(characters+n),location);
            //this->Cursor.X()++;
        }








        return (*this);
    }
*/



    void TermBoxRenderContext::SetForegroundColor(const unsigned short &defaultColor)
    {
        this->CurrentForegroundColor = defaultColor;
    }

    void TermBoxRenderContext::SetForegroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier &modifier)
    {
        this->CurrentForegroundModifier = modifier;
    }

    void TermBoxRenderContext::SetBackgroundColor(const unsigned short & defaultColor)
    {
        this->CurrentBackgroundColor = defaultColor;
    }

    void TermBoxRenderContext::SetBackgroundCurrentModifier(const LD::TermBoxConsoleApplicationColorModifier &modifier)
    {
        this->CurrentBackgroundModifier = modifier;
    }

    void TermBoxRenderContext::SetCurrentSpectrum(const LD::TermBoxConsoleApplicationColorSpectrum &spectrum)
    {
        this->CurrentSpectrum = spectrum;
    }

    const LD::TermBoxConsoleApplicationColorSpectrum & TermBoxRenderContext::GetSpectrum() const
    {
        return this->CurrentSpectrum;
    }


    const LD::TermBoxConsoleApplicationColorModifier & TermBoxRenderContext::GetForegroundModifier() const
    {
        return this->CurrentForegroundModifier;
    }

    const unsigned short & TermBoxRenderContext::GetForegroundColor() const
    {
        return this->CurrentForegroundColor;
    }

    const LD::TermBoxConsoleApplicationColorModifier & TermBoxRenderContext::GetBackgroundModifier() const
    {
        return this->CurrentBackgroundModifier;
    }

    const unsigned short & TermBoxRenderContext::GetBackgroundColor() const
    {
        return this->CurrentBackgroundColor;
    }

    void TermBoxRenderContext::Clear(const unsigned short & foregroundColor, const unsigned short & backgroundColor, const LD::TermBoxConsoleApplicationColorModifier & foregroundModifier, const LD::TermBoxConsoleApplicationColorModifier & backgroundModifier, const LD::TermBoxConsoleApplicationColorSpectrum & spectrum)
    {
        //this->mCursor = {0,0};
        this->LineWidth = tb_width();
    }
}
