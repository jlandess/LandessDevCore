//
// Created by James Landess on 1/18/20.
//

#ifndef LANDESSDEVCORE_TERMBOXENUMERATIONS_HPP
#define LANDESSDEVCORE_TERMBOXENUMERATIONS_HPP
namespace PDP
{
    enum TermBoxConsoleApplicationColorSpectrum{eCurrentSpectrum = 0,eNormalSpectrum = 1, e256Spectrum =2 , e216Spectrum =3 , eGrayScaleSpectrum = 4};
    enum TermBoxConsoleApplicationColorModifier{eTBold = 0x0100,eTBUnderline = 0x0200,eTBReverse = 0x0400,eTBNone = 0x0};


    enum TermBoxConsoleDefaultColorSet{eTBDefault = 0x00,eTBlack = 0x01,eTBRed = 0x02,eTBGreen = 0x03,eTBYellow = 0x04,eTBlue = 0x05,eTBMagenta = 0x06,eTBCyan = 0x07,eTBWhite = 0x08};
}
#endif //LANDESSDEVCORE_TERMBOXENUMERATIONS_HPP
