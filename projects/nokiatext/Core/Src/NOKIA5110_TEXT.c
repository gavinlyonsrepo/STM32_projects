/*
 * Project Name: Nokia 5110 ASCII text 
 * File: NOKIA5110_TEXT.c
 * Description: Nokia library c file
 * Author: Gavin Lyons.
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */

#include "NOKIA5110_TEXT.h"
#include "main.h"
#include  "NOKIA5110_TEXT_FONT_DATA.h"

uint8_t _contrast = LCD_CONTRAST;
uint8_t _bias = LCD_BIAS;
bool _sleep = false;
bool _inverse = false;
uint8_t _Block = 0;
uint8_t _Col = 0;

uint8_t	_CurrentFontNumber = LCDFont_Default;
uint8_t _CurrentFontWidth =  5;
uint8_t _CurrentFontOffset = 0x20 ;


/*Function : LCDinit
This sends the  commands to the PCD8544 to  init LCD
 */
void LCDInit(bool Inverse, uint8_t Contrast, uint8_t Bias) {

    _inverse = Inverse;
    _contrast = Contrast;
    _bias = Bias;

    //Configure control pins
    LCD_DIN_SetLow();
    LCD_CLK_SetLow();
    LCD_DC_SetLow();
    //Reset the LCD to a known state
    LCD_RST_SetLow();
    LCD_RST_SetHigh();
    LCDWrite(LCD_COMMAND, LCD_COMMAND_MODE); //Tell LCD that extended commands follow
    LCDWrite(LCD_COMMAND, _contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    LCDWrite(LCD_COMMAND, LCD_TEMP_COEF); //Set Temp coefficent
    LCDWrite(LCD_COMMAND, _bias); //LCD bias mode 1:48: Try 0x13 or 0x14
    LCDWrite(LCD_COMMAND, LCD_FUNCTIONSET); //We must send 0x20 before modifying the display control mode
    if (_inverse == false)
        LCDWrite(LCD_COMMAND, LCD_DISPLAYCONTROL); //Set display control, normal mode. 0x0D for inverse
    else
        LCDWrite(LCD_COMMAND, LCD_DISPLAYCONTROL_INVERSE); //Set display control, normal mode. 0x0D for inverse
}

/* Function: LCDFont
Passed a enum to set between fonts , 1-9
 */
void LCDFont(LCDFontType_e FontNumber) {

	typedef enum {
		LCDFont_W_3 = 3, // tiny font
		LCDFont_W_4 = 4, // seven segment font
		LCDFont_W_5 = 5, // Default
		LCDFont_W_7 = 7, // thick + homeSpun
		LCDFont_W_8 = 8, // wide
		LCDFont_W_12 = 12, // large , no lowercase letters
		LCDFont_W_16 = 16 // mega and huge , numbers only
	} LCDFontWidth_e; // Size width of fonts in pixels, add padding for font_width < 9

	typedef enum {
		LCDFont_O_Full = 0x00, //   full ASCII
		LCDFont_O_Space = 0x20, // Starts at Space
		LCDFont_O_Number = 0x2E, // // ASCII code for . is 0X2E (. / 0 1 etc)
	} LCDFontOffset_e; // font offset in the ASCII table

		LCDFontOffset_e offsetFont;
		LCDFontWidth_e  widthFont;

		_CurrentFontNumber = FontNumber;

			switch (_CurrentFontNumber) {
			case LCDFont_Default:  // default
				_CurrentFontWidth = (widthFont = LCDFont_W_5);
				_CurrentFontOffset =  ( offsetFont =  LCDFont_O_Space );
			break;
			case LCDFont_Thick: // Thick
			case LCDFont_HomeSpun:  // homespun
				_CurrentFontWidth = (widthFont = LCDFont_W_7);
				_CurrentFontOffset =  ( offsetFont = LCDFont_O_Space);
			break;
			case LCDFont_Seven_Seg:  //seven seg
				_CurrentFontWidth = (widthFont = LCDFont_W_4);
				_CurrentFontOffset =  ( offsetFont = LCDFont_O_Space);
			break;
			case LCDFont_Wide :  //wide
				_CurrentFontWidth = (widthFont = LCDFont_W_8);
				_CurrentFontOffset =   ( offsetFont = LCDFont_O_Space);
			break;
			case LCDFont_Tiny : // tiny
				_CurrentFontWidth = (widthFont = LCDFont_W_3);
				_CurrentFontOffset =  ( offsetFont = LCDFont_O_Space);
			break;
			case LCDFont_Large : // large
				_CurrentFontWidth = (widthFont = LCDFont_W_12);
				_CurrentFontOffset = ( offsetFont = LCDFont_O_Space);
			break;
			case LCDFont_Huge: // huge
				_CurrentFontWidth = (widthFont = LCDFont_W_16);
				_CurrentFontOffset =  ( offsetFont =LCDFont_O_Number );
			break;
			case LCDFont_Mega: // mega
				_CurrentFontWidth = (widthFont = LCDFont_W_16);
				_CurrentFontOffset =  ( offsetFont =LCDFont_O_Number ) ;
			break;
		}
}

/* Function: LCDClear 
Clears the LCD by writing zeros to the entire screen
 */
void LCDClear(uint8_t FillData) {
    for (uint16_t index = 0; index < (LCD_X * LCD_Y / 8); index++) {
        LCDWrite(LCD_DATA, FillData);
    }
    LCDgotoXY(0, 0); //After we clear the display, return to the home position
}

/* Function: LCDClear 
Clears one of the 6 row blocks(one byte height) on LCD 
by writing zeros to the line. Send the row block number 0-5
 */
void LCDClearBlock(uint8_t RowBlockNum) {
    LCDgotoXY(0, RowBlockNum);
    for (uint8_t index = 0; index < (LCD_X); index++) {
        LCDWrite(LCD_DATA, 0x00);
    }
}

/* Function: gotoXY gotoXY routine to position cursor 
 x - range: 0 to 84 (0 to 0x53)
 y - range: 0 to 5 ( 6 blocks one byte each 6*8 = 48*/
void LCDgotoXY(uint8_t x, uint8_t y) {
    LCDWrite(LCD_COMMAND, 0x80 | x); // Column. (result 0x80 to 0xD3)
    LCDWrite(LCD_COMMAND, 0x40 | y); // Row.
    _Block = y;
    _Col = x;
}

/* Function: LCDWrite 
There are two  banks in the LCD, data and commands. This
function sets the DC pin high or low depending, and then sends
the data byte
 */
void LCDWrite(unsigned char data_or_command, unsigned char data) {
    unsigned char i, d;
    d = data;
    if (data_or_command == 0)LCD_DC_SetLow();
    else LCD_DC_SetHigh();
    //data_or_command; 
    //Tell the LCD that we are writing either to data or a command
    //Send the data
    LCD_CE_SetLow();
    for (i = 0; i < 8; i++) {
        LCD_DIN_SetLow();
        if (d & 0x80)LCD_DIN_SetHigh(); // b1000000 Mask with 0 & all zeros out.
        LCD_CLK_SetHigh();
        d <<= 1;
        LCD_CLK_SetLow();
    }
    LCD_CE_SetHigh();
}

/* Function: LCDCharacter.*/
void LCDCharacter(char character) {
    switch (_CurrentFontNumber) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: LCDdraw_fonts_1TO6(character);
            break;
        case 7: LCDdraw_fonts_7(character);
            break;
        case 8: LCDdraw_fonts_8TO9(character);
            break;
        case 9: LCDdraw_fonts_8TO9(character);
            break;
    }
}

/* Function: LCDString.
 Given a string of characters, one by one is passed to the LCD
 */
void LCDString(const char *characters) {
    while (*characters)
        LCDCharacter(*characters++);
}

/* Function: LCDSetPixel
 Function to set one pixel on
 Passed two byte X and Y , Column and row position to set the the bit
 X = 0-83 , Y = 0-47.
 */
void LCDSetPixel(uint8_t col, uint8_t row) {
    uint8_t rowblock = row / 8; //0 -5
    LCDgotoXY(col, rowblock);
    LCDWrite(LCD_DATA, 1 << (row - (rowblock * 8)));
}

/* Function: LCDSetContrast
 Function to set contrast passed a byte 
 Set LCD VOP Contrast, range = ((0x00-0x7F) |0x80) 0xB5 = (0x35|0x80) try B1 - BF normally. 
 */
void LCDsetContrast(uint8_t vop_contrast) {
    _contrast = vop_contrast;
    LCDWrite(LCD_COMMAND, LCD_COMMAND_MODE); //Tell LCD that extended commands follow
    LCDWrite(LCD_COMMAND, _contrast); //Set LCD Vop (Contrast):
    LCDWrite(LCD_COMMAND, LCD_FUNCTIONSET); //We must send 0x20 before modifying the display control mode
}

// Put the LCD to Sleep function
void LCDenableSleep() {
    _sleep = true;
    for (uint16_t index = 0; index < (LCD_X * LCD_Y / 8); index++) {
        LCDWrite(LCD_DATA, 0x00);
    }
    LCDgotoXY(0, 0);
    LCDWrite(LCD_COMMAND, LCD_POWERDOWN);
}

// Wake the LCD up from Sleep function
void LCDdisableSleep() {
    _sleep = false;
    LCDWrite(LCD_COMMAND, LCD_COMMAND_MODE);
    LCDWrite(LCD_COMMAND, _contrast);
    LCDWrite(LCD_COMMAND, LCD_TEMP_COEF);
    LCDWrite(LCD_COMMAND, _bias);
    LCDWrite(LCD_COMMAND, LCD_FUNCTIONSET);
    if (_inverse == false)
        LCDWrite(LCD_COMMAND, LCD_DISPLAYCONTROL); //Set display control, normal mode. 0x0D for inverse
    else
        LCDWrite(LCD_COMMAND, LCD_DISPLAYCONTROL_INVERSE); //Set display control, normal mode. 0x0D for inverse
}

/* Function: LCDCustomChar
 Write a custom character to screen X by Y. Y is fixed at 8
 Parameters 1 Example: 5 by 8 = || || = Char array unsigned char power[5] = {0xFF, 0xFF, 0x00, 0xFF, 0xFF}; 
 Parameters 2: Size of array,  sizeof(power)/sizeof(unsigned char)
 Parameter 3: Blank vertical line padding  padding 4 values enum
 0 = no padding 
 1 = Left hand side padding only
 2 = Right hand side padding only 
 3 = LHS + RHS

 */
void LCDCustomChar(const unsigned char character[], uint16_t numChars, LCDPaddingType_e padding) {

    uint16_t column = 0; // max 504 bytes.
    if (padding == LCDPadding_LHS || padding == LCDPadding_Both) {
        LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding , LHS
    }

    for (column = 0; column < numChars; column++) {
        LCDWrite(LCD_DATA, character[column]); //pgm TODO
    }

    if (padding == LCDPadding_RHS || padding == LCDPadding_Both) {
        LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding RHS
    }
}

void LCDdraw_fonts_1TO6(char character) {
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding , LHS
	for (uint8_t  column = 0 ; column < _CurrentFontWidth ; column++)
	{
		switch(_CurrentFontNumber)
		{
		case LCDFont_Default:
		#ifdef NOKIA5110_FONT_Default
			LCDWrite(LCD_DATA, FontDefault[character - _CurrentFontOffset][column]);
		#endif
		break;
		case LCDFont_Thick:
		#ifdef NOKIA5110_FONT_Thick
			LCDWrite(LCD_DATA, FontThick[character - _CurrentFontOffset][column]);
		#endif
		break;
		case LCDFont_HomeSpun:
		#ifdef NOKIA5110_FONT_HomeSpun
			LCDWrite(LCD_DATA, FontHomeSpun[character - _CurrentFontOffset][column]);
		#endif
		break;
		case LCDFont_Seven_Seg:
		#ifdef  NOKIA5110_FONT_SevenSeg
			LCDWrite(LCD_DATA, FontSevenSegment[character - _CurrentFontOffset][column]);
		#endif
		break;
		case LCDFont_Wide:
		#ifdef NOKIA5110_FONT_Wide
			LCDWrite(LCD_DATA, FontWide[character - _CurrentFontOffset][column]);
		#endif
		break;
		case LCDFont_Tiny:
		#ifdef NOKIA5110_FONT_Tiny
			LCDWrite(LCD_DATA, FontTiny[character - _CurrentFontOffset][column]);
		#endif
		break;
		}
	}
	LCDWrite(LCD_DATA, 0x00); // Blank vertical line padding RHS
}

void LCDdraw_fonts_7(char character) {
#ifdef NOKIA5110_FONT_Large
    uint16_t totalchar = 0;
    uint8_t topchar = 0;
    uint8_t botchar = 0;
    uint8_t column = 0;
    //print upper byte
    for (column = 0; column < _CurrentFontWidth; column++) {
        totalchar = FontLarge[character - _CurrentFontOffset][column];
        topchar = totalchar & 0x00FF;
        LCDWrite(LCD_DATA, topchar);
    }
    //Move to next block
    LCDgotoXY(_Col, _Block + 1);
    //print lowerbyte
    for (column = 0; column < _CurrentFontWidth; column++) {
        totalchar = FontLarge[character - _CurrentFontOffset][column];
        botchar = (totalchar >> 8) & 0xFF;
        LCDWrite(LCD_DATA, botchar);
    }
    //move back to upper block  
    LCDgotoXY(_Col + _CurrentFontWidth, _Block - 1);
#endif
}

void LCDdraw_fonts_8TO9(char character) {
    // 0XAABBCCDD font 9 32 bit high,   0X00BBCCDD font 8  24 bit high
    // DD DD	Upperbyte
    // CC CC   upper middle
    // BB  BB lower middle
    // AA      lower byte

    uint32_t totaldata = 0;
    uint8_t topbyte = 0;
    uint8_t middleupperbyte = 0;
    uint8_t middlelowerbyte = 0;
    uint8_t lowerbyte = 0;
    uint8_t column = 0;
    //print upper byte  DD
    for (column = 0; column < _CurrentFontWidth; column++) {
        if (_CurrentFontNumber == LCDFont_Huge) {
        #ifdef NOKIA5110_FONT_Huge
            totaldata = FontHuge[character - _CurrentFontOffset][column];
        #endif
        } else {
            #ifdef NOKIA5110_FONT_Mega
            totaldata = FontMega[character - _CurrentFontOffset][column];
            #endif
        }
        topbyte = totaldata & 0xFF;
        LCDWrite(LCD_DATA, topbyte);
    }
    totaldata = 0;
    LCDgotoXY(_Col, _Block + 1); //goto next block

    // print middle upper byte CC
    for (column = 0; column < _CurrentFontWidth; column++) {
        if (_CurrentFontNumber == LCDFont_Huge) {
            #ifdef NOKIA5110_FONT_Huge
            totaldata = FontHuge[character - _CurrentFontOffset][column];
            #endif
        } else {
            #ifdef NOKIA5110_FONT_Mega
            totaldata = FontMega[character - _CurrentFontOffset][column];
            #endif
        }
        middleupperbyte = (totaldata >> 8) & 0xFF;
        LCDWrite(LCD_DATA, middleupperbyte);
    }
    totaldata = 0;
    LCDgotoXY(_Col, _Block + 1); //goto next block

    // print middle lower byte BB
    for (column = 0; column < _CurrentFontWidth; column++) {
        if (_CurrentFontNumber == LCDFont_Huge) {
            #ifdef NOKIA5110_FONT_Huge
            totaldata = FontHuge[character - _CurrentFontOffset][column];
            #endif
        } else {
            #ifdef NOKIA5110_FONT_Mega
            totaldata = FontMega[character - _CurrentFontOffset][column];
            #endif
        }
        middlelowerbyte = (totaldata >> 16) & 0xFF;
        LCDWrite(LCD_DATA, middlelowerbyte);
    }

    // print lower byte AA, no need if printing font 8 
    if (_CurrentFontNumber == LCDFont_Huge)
        LCDgotoXY(_Col + _CurrentFontWidth, _Block - 2); //move back for next character
    else {
        totaldata = 0;
        LCDgotoXY(_Col, _Block + 1); //goto next block
        for (column = 0; column < _CurrentFontWidth; column++) {
            #ifdef NOKIA5110_FONT_Mega
            totaldata = FontMega[character - _CurrentFontOffset][column];
            lowerbyte = (totaldata >> 24) & 0xFF;
            LCDWrite(LCD_DATA, lowerbyte);
            #endif
        }
        LCDgotoXY(_Col + _CurrentFontWidth, _Block - 3); //move back for next character
    }
    character += lowerbyte; // Get rid of unused variable warning
}

bool LCDIsSleeping() {
    return _sleep;
}

void LCDFillBlock(uint8_t FillData, uint8_t RowBlockNum) {
    LCDgotoXY(0, RowBlockNum);
    for (uint8_t index = 0; index < (LCD_X); index++) {
        LCDWrite(LCD_DATA, FillData);
    }
}

// Func Desc: vsprintf wrapper to print numerical data
// Parameters: https://www.tutorialspoint.com/c_standard_library/c_function_vsprintf.htm
// The C library function int vsprintf(char *str, const char *format, va_list arg)
// sends formatted output to a string using an argument list passed to it.
// Returns: If successful, the total number of characters written is returned,
// otherwise a negative number is returned.
// Note: requires stdio.h and stdarg.h libraries

int LCDPrintf(const char *fmt, ...) {
    int length;
    char buffer[100];
    va_list ap;

    va_start(ap, fmt);
    length = vsprintf(buffer, fmt, ap);
    va_end(ap);
    if (length > 0) {
        LCDString(buffer);
    }
    return length;
}

/* =========== EOF ===========*/
