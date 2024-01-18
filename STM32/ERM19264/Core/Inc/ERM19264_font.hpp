/*!
	@file ERM19264_UC1609_font.hpp
	@brief LCD driven by UC1609 controller. font data file 12 fonts.
	@author Gavin Lyons.
*/

#pragma once

#include "cstdint"

// **********************************************
// ****** USER FONT OPTION ONE *************
// ****** FONT DEFINE SECTION *************** 
// Comment in the fonts YOU want, One is default. 

#define UC1609_Font_One  // (1) default  (FUll ASCII with mods)
#define UC1609_Font_Two  // (2) thick (NO LOWERCASE)
#define UC1609_Font_Three // (3) seven segment
#define UC1609_Font_Four // (4) wide (NO LOWERCASE)
#define UC1609_Font_Five // (5) tiny
#define UC1609_Font_Six // (6) HomeSpun
#define UC1609_Font_Seven // (7) big numbers ( extended Nnumeric )
#define UC1609_Font_Eight // (8) Med numbers (extended Nnumeric )
#define UC1609_Font_Nine // (9) Arial round
#define UC1609_Font_Ten   // (9) Arial bold
#define UC1609_Font_Eleven  //(10) Mia
#define UC1609_Font_Twelve  // (12) dedica

// ****** END OF FONT DEFINE SECTION ******  
// **********************************************
 
// Font data is in the cpp file accessed thru extern pointers.

#ifdef UC1609_Font_One 
extern const unsigned char * pFontDefaultptr;  /**< Pointer to default font data */
#endif
#ifdef UC1609_Font_Two 
extern const unsigned char * pFontThickptr;  /**< Pointer to thickfont data */
#endif
#ifdef UC1609_Font_Three
extern const unsigned char * pFontSevenSegptr;  /**< Pointer to seven seg font data */
#endif
#ifdef UC1609_Font_Four
extern const unsigned char * pFontWideptr;  /**< Pointer to wide font data */
#endif
#ifdef UC1609_Font_Five
extern const  unsigned char * pFontTinyptr;  /**< Pointer to tinyfont data */
#endif
#ifdef UC1609_Font_Six
extern const unsigned char * pFontHomeSpunptr;  /**< Pointer to homespun font data */
#endif
#ifdef UC1609_Font_Seven
extern const uint8_t (* pFontBigNum16x32ptr)[64];  /**< Pointer to bignum font data */
#endif
#ifdef UC1609_Font_Eight
extern const uint8_t (* pFontMedNum16x16ptr)[32];  /**< Pointer to mednum font data */
#endif
#ifdef UC1609_Font_Nine
extern const uint8_t (* pFontArial16x24ptr)[48]; /**< Pointer to Arial Round font data */
#endif
#ifdef UC1609_Font_Ten
extern const uint8_t (* pFontArial16x16ptr)[32]; /**< Pointer to Arial bold font data */
#endif
#ifdef UC1609_Font_Eleven
extern const uint8_t (* pFontMia8x16ptr)[16]; /**< Pointer to Mia font data */
#endif
#ifdef UC1609_Font_Twelve
extern const uint8_t (* pFontDedica8x12ptr)[12]; /**< Pointer to dedica font data */
#endif

