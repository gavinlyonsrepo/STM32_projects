// Font data file NOkia 51110 library , 9 fonts
// Project URL https://github.com/gavinlyonsrepo/STM32_projects
#include <stdint.h>
 
#ifndef XC_HEADER_NOKIAFONT_H
#define	XC_HEADER_NOKIAFONT_H


// **** FONT DEFINE SECTION ******  
// Comment in the fonts you want, X_1 is default. 
#define NOKIA5110_FONT_Default
#define NOKIA5110_FONT_Thick
#define NOKIA5110_FONT_HomeSpun
#define NOKIA5110_FONT_SevenSeg
#define NOKIA5110_FONT_Wide
#define NOKIA5110_FONT_Tiny
#define NOKIA5110_FONT_Large
#define NOKIA5110_FONT_Huge
#define NOKIA5110_FONT_Mega
// **** END OF FONT DEFINE SECTION ******  

// Fonts
// Defined: NOKIA5110_TEXT_FONT_DATA.c
// Enabled: NOKIA5110_TEXT_FONT_DATA.h :FONT DEFINE SECTION
#ifdef NOKIA5110_FONT_Default
extern const unsigned char FontDefault[94][5];
#endif
#ifdef NOKIA5110_FONT_Thick
extern const unsigned char FontThick[59][7];
#endif
#ifdef NOKIA5110_FONT_HomeSpun
extern const unsigned char FontHomeSpun[95][7];
#endif
#ifdef NOKIA5110_FONT_SevenSeg
extern const unsigned char FontSevenSegment[92][4];
#endif
#ifdef NOKIA5110_FONT_Wide
extern const unsigned char FontWide[59][8];
#endif
#ifdef NOKIA5110_FONT_Tiny
extern const unsigned char FontTiny[95][3];
#endif
#ifdef NOKIA5110_FONT_Large
extern const uint16_t FontLarge[59][12];
#endif
#ifdef NOKIA5110_FONT_Huge
extern const uint32_t FontHuge[13][16];
#endif
#ifdef NOKIA5110_FONT_Mega
extern const uint32_t FontMega[13][16];
#endif
#endif	// endif header guard

// ** EOF **
