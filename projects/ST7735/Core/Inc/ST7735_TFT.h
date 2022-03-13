/*
 * Project Name: ST7735, 128 by 128, 1.44", red pcb,  SPI TFT module, library.  
 * File: ST7735_TFT.h
 * Description: library header file   
 * Author: Gavin Lyons.
 * IC: STM32F070RB
 * IDE:  STM32cubeIDE
 * Created May 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */

#ifndef ST7735_TFT_H
#define ST7735_TFT_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"


// ****** USER OPTION 3 FONT ******
// Comment in the fonts you want
#define TFT_Font_One
#define TFT_Font_Two
#define TFT_Font_Three
#define TFT_Font_Four
#define TFT_Font_Five

// ******** USER OPTION 4 SPI TYPE ***********
// SPI TFT module connections
// *** If Hardware SPI module is used, comment this line in ***
#define TFT_SPI_HARDWARE
// *******************************************
// HW_SPI ((SW SPI MSSP1)) Connections
// CS = PC2
// DC = PC1
// SDATA = PA7 ((PA0  ))
// SCLK = PB3 (( PA1  ))
// RST = PC0

#ifdef TFT_Font_One
#include "Font_One.h" // Default 5X8
#endif
#ifdef TFT_Font_Two
#include "Font_Two.h" // Thick 7X8  (NO LOWERCASE LETTERS)
#endif
#ifdef TFT_Font_Three
#include "Font_Three.h" // Seven segment 4X8
#endif
#ifdef TFT_Font_Four
#include "Font_Four.h" // Wide  8X8 (NO LOWERCASE LETTERS)
#endif
#ifdef TFT_Font_Five
#include "Font_Five.h"// Tiny 3X8
#endif

// *********** DEFINES ****************


// ******** ST7735 registers ********
#define ST7735_NOP     0x00 // Non operation
#define ST7735_SWRESET 0x01 // Soft Reset
#define ST7735_RDDID   0x04 
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10 // Sleep ON
#define ST7735_SLPOUT  0x11 // Sleep OFF
#define ST7735_PTLON   0x12 // Partial mode
#define ST7735_NORON   0x13 // Normal Display
#define ST7735_INVOFF  0x20 // Display invert off
#define ST7735_INVON   0x21 // Display Invert on 
#define ST7735_DISPOFF 0x28 // Display off
#define ST7735_DISPON  0x29 // Display on 

#define ST7735_IDLEON 0x39 //Idle Mode ON	
#define ST7735_IDLEOF  0x38 //Idle Mode OFF	

#define ST7735_CASET   0x2A // Column address set
#define ST7735_RASET   0x2B // Page address set
#define ST7735_RAMWR   0x2C // Memory write
#define ST7735_RAMRD   0x2E // Memory read
#define ST7735_PTLAR   0x30 // Partial Area
#define ST7735_VSCRDEF 0x33 // Vertical scroll def
#define ST7735_COLMOD  0x3A // Interface Pixel Format
#define ST7735_MADCTL  0x36 // Memory Access Control
#define ST7735_VSCRSADD 0x37 // Vertical Access Control

// Frame Rate Control 
#define ST7735_FRMCTR1 0xB1 // Normal  
#define ST7735_FRMCTR2 0xB2 // idle
#define ST7735_FRMCTR3 0xB3 // Partial

#define ST7735_INVCTR  0xB4 // Display Inversion control 
#define ST7735_DISSET5 0xB6 // Display Function set 

// Power_Control
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_PWCTR6  0xFC

#define ST7735_VMCTR1  0xC5 // VCOM_Control 1	

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_GMCTRP1 0xE0 // Positive Gamma Correction Setting
#define ST7735_GMCTRN1 0xE1 // Negative Gamma Correction Setting

#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

// Color definitions 16-Bit Color Values R5G6B5
#define   ST7735_BLACK   0x0000
#define   ST7735_BLUE    0x001F
#define   ST7735_RED     0xF800
#define   ST7735_GREEN   0x07E0
#define   ST7735_CYAN    0x07FF
#define   ST7735_MAGENTA 0xF81F
#define   ST7735_YELLOW  0xFFE0
#define   ST7735_WHITE   0xFFFF
#define   ST7735_TAN     0xED01
#define   ST7735_GREY    0x9CD1
#define   ST7735_BROWN   0x6201

// GPIO
#define TFT_DC_SetHigh()  HAL_GPIO_WritePin(GPIOC, DC_TFT_PC1_Pin,GPIO_PIN_SET)
#define TFT_DC_SetLow()   HAL_GPIO_WritePin(GPIOC, DC_TFT_PC1_Pin,GPIO_PIN_RESET)
#define TFT_CS_SetHigh() HAL_GPIO_WritePin(GPIOC, CS_TFT_PC2_Pin,GPIO_PIN_SET)
#define TFT_CS_SetLow()  HAL_GPIO_WritePin(GPIOC, CS_TFT_PC2_Pin,GPIO_PIN_RESET)
#define TFT_RST_SetHigh() HAL_GPIO_WritePin(GPIOC, RESET_TFT_PC0_Pin,GPIO_PIN_SET)
#define TFT_RST_SetLow() HAL_GPIO_WritePin(GPIOC, RESET_TFT_PC0_Pin,GPIO_PIN_RESET)
#define TFT_DIN_SetHigh() HAL_GPIO_WritePin(GPIOA, SWMOSI_TFT_PA0_Pin,GPIO_PIN_SET)
#define TFT_DIN_SetLow()  HAL_GPIO_WritePin(GPIOA, SWMOSI_TFT_PA0_Pin,GPIO_PIN_RESET)
#define TFT_CLK_SetHigh()  HAL_GPIO_WritePin(GPIOA, SWCLK_TFT_PA1_Pin,GPIO_PIN_SET)
#define TFT_CLK_SetLow()  HAL_GPIO_WritePin(GPIOA, SWCLK_TFT_PA1_Pin,GPIO_PIN_RESET)

#define _swap_TFT(a, b) { int16_t t; t = a; a = b; b = t;}

// ***** Enums ******

typedef enum  {
	ST7735_Degrees_0 = 0, ST7735_Degrees_90, ST7735_Degrees_180, ST7735_Degrees_270
}ST7735_rotate_e; // TFT rotate modes in degrees 

typedef enum {
	TFT_ST7735R_Red = 0, // ST7735R Red Tab 
	TFT_ST7735R_Green, // ST7735R Green Tab
	TFT_ST7735S_Black, // ST7735S Black Tab
	TFT_ST7735B, // ST7735B controller
}ST7735_PCBtype_e; // type of PCB

typedef enum  {
    ST7735_modes_Normal = 0, 
    ST7735_modes_Partial, 
    ST7735_modes_Idle, 
    ST7735_modes_Sleep, 
    ST7735_modes_Invert, 
    ST7735_modes_DisplayOn, 
    ST7735_modes_DisplayOff
}ST7735_modes_e; //ST7735 mode select 

typedef enum 
{
	TFTFont_Default = 1,
	TFTFont_Thick = 2,
	TFTFont_Seven_Seg = 3,
	TFTFont_Wide = 4,
	TFTFont_Tiny = 5,
}ST7735_FontType_e; // Font type 1-5

ST7735_modes_e     ST7735_modes;
ST7735_rotate_e    ST7735_rotate;
ST7735_PCBtype_e   ST7735_PCBtype;
ST7735_FontType_e  ST7735_FontType;

// ******** FUNCTION  PROTOTYPES ************ 


// SPI 
void TFTspiWrite(uint8_t);
void TFTwriteCommand(uint8_t);
void TFTwriteData(uint8_t);
#ifdef TFT_SPI_HARDWARE
	void SPIHWInitialize(SPI_HandleTypeDef * );
#endif

// Init routines 
void TFTInitPCBType(ST7735_PCBtype_e);
void TFTInitScreenSize(uint8_t xOffset, uint8_t yOffset, uint16_t w, uint16_t h);
void TFT_ResetPIN(void);

void TFT_Rcmd1(void);
void TFT_Rcmd2red(void);
void TFT_Rcmd3(void);
void TFT_Bcmd(void);
void TFT_Rcmd2green(void);

void TFT_ST7735B_Initialize(void);
void TFT_GreenTab_Initialize(void);
void TFT_BlackTab_Initialize(void);
void TFT_RedTab_Initialize(void);


// Misc + Screen related
void TFTsetAddrWindow(uint8_t, uint8_t, uint8_t, uint8_t);
void TFTfillScreen(uint16_t color);
void TFTpushColor(uint16_t color);
void TFTsetRotation(ST7735_rotate_e r);
void TFTchangeMode(ST7735_modes_e m);

//Scroll 
void TFTsetScrollDefinition(uint8_t topFixHeight, uint8_t bottomFixHeight, bool scrollDirection);
void TFTVerticalScroll(uint8_t vsp);

// Shapes and lines
void TFTdrawPixel(uint8_t, uint8_t, uint16_t);
void TFTdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void TFTdrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
void TFTdrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);

void TFTdrawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void TFTfillRectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
void TFTfillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void TFTdrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
void TFTfillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);

void TFTdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void TFTdrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void TFTfillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void TFTfillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void TFTdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void TFTfillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

// Text
void TFTdrawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
void TFTsetTextWrap(bool w);
void TFTdrawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);
void TFTFontNum(ST7735_FontType_e FontNumber);

// Bitmap & Icon
void TFTdrawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);
void TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const unsigned char bitmap[]);

#endif // file header guard 

// ********************** EOF *********************
