/*
 * Project Name: Nokia 5110 ASCII text 
 * File: NOKIA5110_TEXT.c
 * Description: Nokia library c file
 * Author: Gavin Lyons.
 * IC: PIC_18F47K42 
 * IDE:  MPLAB 5.30 Xc8 2.10
 * Created March 2019
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/pic_18F47K42_projects
 */

#include "NOKIA5110_TEXT.h"
#include "main.h"

uint8_t _contrast = LCD_CONTRAST;
uint8_t _FontNumber = LCD_FONTNUMBER;
uint8_t _bias = LCD_BIAS;
bool _sleep = false;
bool _inverse = false;
uint8_t _Block = 0;
uint8_t _Col = 0;

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
Passed a int to set between fonts , 1-6
default font is 1, Font_two is 2. Font_three is 3 etc
 */
void LCDFont(uint8_t FontNumber) {
    _FontNumber = FontNumber;
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

/* Function: LCDCharacter.
 This function takes in a character, looks it up in the font table/array
And writes it to the screen
Each character is 8 bits tall and 5 bits wide. We pad one blank column of
pixels on each side of the character for readability.
 */
void LCDCharacter(char character) {
    switch (_FontNumber) {
        case 1: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_1);
            break;
        case 2: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_2);
            break;
        case 3: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_3);
            break;
        case 4: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_4);
            break;
        case 5: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_5);
            break;
        case 6: LCDdraw_fonts_1TO6(character, LCD_FONT_WIDTH_6);
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
 Parameter 3: Blank vertical line padding  padding 4 values
 0 = no padding 
 1 = Left hand side padding only
 2 = Right hand side padding only 
 3 = LHS + RHS

 */
void LCDCustomChar(const unsigned char character[], uint16_t numChars, uint8_t padding) {

    uint16_t column = 0; // max 504 bytes.
    if (padding == 1 || padding == 3) {
        LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding , LHS
    }

    for (column = 0; column < numChars; column++) {
        LCDWrite(LCD_DATA, character[column]); //pgm TODO
    }

    if (padding == 2 || padding == 3) {
        LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding RHS
    }
}

void LCDdraw_fonts_1TO6(char character, uint8_t font_width) {
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding , LHS
    for (uint8_t column = 0; column < font_width; column++) {
        switch (_FontNumber) {
            case 1:
                #ifdef NOKIA5110_FONT_1
                      LCDWrite(LCD_DATA, ASCII_ONE[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
            case 2:
                #ifdef NOKIA5110_FONT_2
                     LCDWrite(LCD_DATA, ASCII_TWO[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
            case 3:
                #ifdef NOKIA5110_FONT_3
                     LCDWrite(LCD_DATA, ASCII_THREE[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
            case 4:
                #ifdef NOKIA5110_FONT_4
                    LCDWrite(LCD_DATA, ASCII_FOUR[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
            case 5:
                #ifdef NOKIA5110_FONT_5
                    LCDWrite(LCD_DATA, ASCII_FIVE[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
            case 6:
                #ifdef NOKIA5110_FONT_6
                   LCDWrite(LCD_DATA, ASCII_SIX[character - LCD_ASCII_OFFSET][column]);
                #endif
            break;
        }
    }
    LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding RHS
}

void LCDdraw_fonts_7(char character) {
#ifdef NOKIA5110_FONT_7
    uint16_t totalchar = 0;
    uint8_t topchar = 0;
    uint8_t botchar = 0;
    uint8_t column = 0;
    //print upper byte
    for (column = 0; column < LCD_FONT_WIDTH_7; column++) {
        totalchar = ASCII_SEVEN[character - LCD_ASCII_OFFSET][column];
        topchar = totalchar & 0x00FF;
        LCDWrite(LCD_DATA, topchar);
    }
    //Move to next block
    LCDgotoXY(_Col, _Block + 1);
    //print lowerbyte
    for (column = 0; column < LCD_FONT_WIDTH_7; column++) {
        totalchar = ASCII_SEVEN[character - LCD_ASCII_OFFSET][column];
        botchar = (totalchar >> 8) & 0xFF;
        LCDWrite(LCD_DATA, botchar);
    }
    //move back to upper block  
    LCDgotoXY(_Col + LCD_FONT_WIDTH_7, _Block - 1);
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
    for (column = 0; column < LCD_FONT_WIDTH_8; column++) {
        if (_FontNumber == 8) {
        #ifdef NOKIA5110_FONT_8
            totaldata = ASCII_EIGHT[character - LCD_ASCII_OFFSET_ZERO][column];
        #endif
        } else {
            #ifdef NOKIA5110_FONT_9
            totaldata = ASCII_NINE[character - LCD_ASCII_OFFSET_ZERO][column];
            #endif
        }
        topbyte = totaldata & 0xFF;
        LCDWrite(LCD_DATA, topbyte);
    }
    totaldata = 0;
    LCDgotoXY(_Col, _Block + 1); //goto next block

    // print middle upper byte CC
    for (column = 0; column < LCD_FONT_WIDTH_8; column++) {
        if (_FontNumber == 8) {
            #ifdef NOKIA5110_FONT_8
            totaldata = ASCII_EIGHT[character - LCD_ASCII_OFFSET_ZERO][column];
            #endif
        } else {
            #ifdef NOKIA5110_FONT_9
            totaldata = ASCII_NINE[character - LCD_ASCII_OFFSET_ZERO][column];
            #endif
        }
        middleupperbyte = (totaldata >> 8) & 0xFF;
        LCDWrite(LCD_DATA, middleupperbyte);
    }
    totaldata = 0;
    LCDgotoXY(_Col, _Block + 1); //goto next block

    // print middle lower byte BB
    for (column = 0; column < LCD_FONT_WIDTH_8; column++) {
        if (_FontNumber == 8) {
            #ifdef NOKIA5110_FONT_8
            totaldata = ASCII_EIGHT[character - LCD_ASCII_OFFSET_ZERO][column];
            #endif
        } else {
            #ifdef NOKIA5110_FONT_9
            totaldata = ASCII_NINE[character - LCD_ASCII_OFFSET_ZERO][column];
            #endif
        }
        middlelowerbyte = (totaldata >> 16) & 0xFF;
        LCDWrite(LCD_DATA, middlelowerbyte);
    }

    // print lower byte AA, no need if printing font 8 
    if (_FontNumber == 8)
        LCDgotoXY(_Col + LCD_FONT_WIDTH_8, _Block - 2); //move back for next character 
    else {
        totaldata = 0;
        LCDgotoXY(_Col, _Block + 1); //goto next block
        for (column = 0; column < LCD_FONT_WIDTH_8; column++) {
            #ifdef NOKIA5110_FONT_9
            totaldata = ASCII_NINE[character - LCD_ASCII_OFFSET_ZERO][column];
            lowerbyte = (totaldata >> 24) & 0xFF;
            LCDWrite(LCD_DATA, lowerbyte);
            #endif
        }
        LCDgotoXY(_Col + LCD_FONT_WIDTH_8, _Block - 3); //move back for next character 
    }

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
/* =========== EOF ===========*/
