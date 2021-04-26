/*
 * Project Name: NOKIA5110_TEXT
 * File: NOKIA5110_TEXT.h
 * Description: Nokia library header file ASCII TEXT 
 * Author: Gavin Lyons.
 * URL: https://github.com/gavinlyonsrepo/pic_18F47K42_projects
 */

#ifndef NOKIA5110_TEXT_H
#define NOKIA5110_TEXT_H

#include <stdint.h>
#include <stdbool.h>
//#include "main.h"

// **** FONT DEFINE SECTION ******  
// Comment in the fonts you want, X_1 is default. 
#define NOKIA5110_FONT_1
//#define NOKIA5110_FONT_2
//#define NOKIA5110_FONT_3
//#define NOKIA5110_FONT_4
//#define NOKIA5110_FONT_5
//#define NOKIA5110_FONT_6
//#define NOKIA5110_FONT_7
//#define NOKIA5110_FONT_8
//#define NOKIA5110_FONT_9
// **** END OF FONT DEFINE SECTION ******  

#ifdef NOKIA5110_FONT_1
#include "NOKIA5110_TEXT_FONT_ONE.h" // Default 5X8
#endif
#ifdef NOKIA5110_FONT_2
#include "NOKIA5110_TEXT_FONT_TWO.h" // Thick 7X8  (NO LOWERCASE LETTERS)
#endif
#ifdef NOKIA5110_FONT_3
#include "NOKIA5110_TEXT_FONT_THREE.h" // Aurebesh 5X8
#endif
#ifdef NOKIA5110_FONT_4
#include "NOKIA5110_TEXT_FONT_FOUR.h" //seven segment 4X8
#endif
#ifdef NOKIA5110_FONT_5
#include "NOKIA5110_TEXT_FONT_FIVE.h" // Wide  8X8 (NO LOWERCASE LETTERS)
#endif
#ifdef NOKIA5110_FONT_6
#include "NOKIA5110_TEXT_FONT_SIX.h" // Tiny 3X8
#endif
#ifdef NOKIA5110_FONT_7
#include "NOKIA5110_TEXT_FONT_SEVEN.h" // Large 12 X 16 (no lowercase letters)
#endif
#ifdef NOKIA5110_FONT_8
#include "NOKIA5110_TEXT_FONT_EIGHT.h" // Huge 16 X24 (numbers + . : only)
#endif
#ifdef NOKIA5110_FONT_9
#include "NOKIA5110_TEXT_FONT_NINE.h" // Mega 16 X 32 (numbers + . : / only)
#endif

//LCD Commands PCD8544_
#define LCD_COMMAND_MODE 0x21  //FUNCTIONSET + extended instruction
#define LCD_CONTRAST     0xBF  //Set LCD VOP Contrast Try 0xB1 or 0xBF if is too dark range = ((0x00-0x7F) |0x80)
#define LCD_TEMP_COEF    0x04  //Set Temp coefficient
#define LCD_BIAS         0x14  // //LCD bias mode 1:48: Try 0x13 or 0x14
#define LCD_FUNCTIONSET  0x20  //We must send 0x20 before modifying the display control mode
#define LCD_DISPLAYCONTROL 0x0C //Set display control, normal mode. 0x0D for inverse
#define LCD_DISPLAYCONTROL_INVERSE 0x0D //Set display control, inverse mode. 0x0D for inverse
#define LCD_POWERDOWN    0x24 //LCD power off

// Misc LCD Data 
#define LCD_FONTNUMBER  0x01 // default Font number 1,  1 to 6 fonts
#define LCD_ASCII_OFFSET 0x20 //0x20, ASCII character for Space, The font table starts with this character
#define LCD_ASCII_OFFSET_ZERO 0x2E // ASCII code for . is 0X2E (. / 0 1 etc)

//The DC pin tells the LCD if sending a command or data
#define LCD_COMMAND 0
#define LCD_DATA 1

// 84 by 48 pixels screen
#define LCD_X 84
#define LCD_Y 48

// Size width of fonts in pixels, add 2 for padding 1-6
#define LCD_FONT_WIDTH_1 5
#define LCD_FONT_WIDTH_2 7
#define LCD_FONT_WIDTH_3 5
#define LCD_FONT_WIDTH_4 4
#define LCD_FONT_WIDTH_5 8
#define LCD_FONT_WIDTH_6 3
#define LCD_FONT_WIDTH_7 12 // No padding  , no lowercase letters
#define LCD_FONT_WIDTH_8 16 // No padding ,  numbers only
#define LCD_FONT_WIDTH_9 16 // No padding ,  numbers only

#define LCD_DC_SetHigh()  HAL_GPIO_WritePin(GPIOB, DC_Pin,GPIO_PIN_SET)
#define LCD_DC_SetLow()   HAL_GPIO_WritePin(GPIOB, DC_Pin,GPIO_PIN_RESET)
#define LCD_DIN_SetHigh() HAL_GPIO_WritePin(GPIOB, SWDATA_Pin,GPIO_PIN_SET)
#define LCD_DIN_SetLow()  HAL_GPIO_WritePin(GPIOB, SWDATA_Pin,GPIO_PIN_RESET)
#define LCD_CLK_SetHigh()  HAL_GPIO_WritePin(GPIOB, SWCLKB3_Pin,GPIO_PIN_SET)
#define LCD_CLK_SetLow()  HAL_GPIO_WritePin(GPIOB, SWCLKB3_Pin,GPIO_PIN_RESET)
#define LCD_CE_SetHigh() HAL_GPIO_WritePin(GPIOB, CE_Pin,GPIO_PIN_SET)
#define LCD_CE_SetLow()  HAL_GPIO_WritePin(GPIOB, CE_Pin,GPIO_PIN_RESET)
#define LCD_RST_SetHigh() HAL_GPIO_WritePin(GPIOB, Reset_Pin,GPIO_PIN_SET)
#define LCD_RST_SetLow() HAL_GPIO_WritePin(GPIOB, Reset_Pin,GPIO_PIN_RESET)

// Methods
void LCDInit(bool, uint8_t, uint8_t);
void LCDgotoXY(uint8_t, uint8_t);
void LCDClear(uint8_t);
void LCDClearBlock(uint8_t);
void LCDString(const char *characters);
void LCDsetContrast(uint8_t);
void LCDenableSleep(void);
void LCDdisableSleep(void);
void LCDCharacter(char);
void LCDWrite(unsigned char, unsigned char);
void LCDFont(uint8_t);
void LCDSetPixel(uint8_t, uint8_t);
void LCDFillBlock(uint8_t, uint8_t);
void LCDCustomChar(const unsigned char character[], uint16_t, uint8_t);
bool LCDIsSleeping(void);

void LCDdraw_fonts_1TO6(char character, uint8_t font_width); //8 bit tall fonts
void LCDdraw_fonts_7(char character); // 16 bit tall fonts
void LCDdraw_fonts_8TO9(char character); // 24 and 32 bit tall fonts

#endif
