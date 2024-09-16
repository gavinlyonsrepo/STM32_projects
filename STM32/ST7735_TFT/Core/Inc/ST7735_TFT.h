/*
 * Project Name: ST7735
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
#include <stdio.h>                      // vsprintf
#include <stdarg.h>                     // varg
#include "main.h"



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
#define   ST7735_ORANGE  0xFC00

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
	TFTFont_Homespun = 6,
	TFTFont_Bignum = 7,		/**< Bignum numbers only */
	TFTFont_Mednum = 8,		/**< Mednum number only */
	TFTFont_ArialRound = 9, /**< Arial round font */
	TFTFont_ArialBold = 10, /**< Arial bold font */
	TFTFont_Mia = 11,		/**< Mia font */
	TFTFont_Dedica = 12		/**< dedica font */
}ST7735_FontType_e; // Font type 1-6

/*! Width of the font in bits each representing a bytes sized column*/
typedef enum
{
	TFTFont_width_3 = 3,  /**< 3 tiny font */
	TFTFont_width_4 = 4,  /**< 4 seven segment font */
	TFTFont_width_5 = 5,  /**< 5 default font */
	TFTFont_width_6 = 6,  /**< dedica font  */
	TFTFont_width_7 = 7,  /**< 7 homespun & thick font*/
	TFTFont_width_8 = 8,  /**< 8 wide & mia font*/
	TFTFont_width_16 = 16 /**< 16 font 7-10*/
}TFT_Font_width_e ;

/*! font offset in the ASCII table*/
typedef enum
{
	TFTFont_offset_none = 0x00,	 /**< extended ASCII */
	TFTFont_offset_space = 0x20, /**< Starts at Space, alphanumeric */
	TFTFont_offset_minus = 0x2D, /**< Starts at Minus, extended numeric */
	TFTFont_offset_zero = 0x30	 /**< Starts at zero, numeric */
}TFT_Font_offset_e;

/*! Height of the font in bits*/
typedef enum
{
	TFTFont_height_8 = 8,	/**< 8 bit font 1-6 at size 1*/
	TFTFont_height_12 = 12, /**< 12 bit font 12 */
	TFTFont_height_16 = 16, /**< 16 bit font 8, 10 11*/
	TFTFont_height_24 = 24, /**< 24 bit font 9 */
	TFTFont_height_32 = 32	/**< 32 bit font 7 */
}ST7735_Font_height_e;

/*! Number of ASCII characters in Font */
typedef enum
{
	TFTFontLenNumeric = 14,			/**< extended Numeric 0x2D - 0x3A */
	TFTFontLenAlphaNumNoLCase = 59, /**< reduced Alphanumeric 0x20-0x5A*/
	TFTFontLenAlphaNum = 95,		/**< Full Alphanumeric 0x20-0x7E */
	TFTFontLenAll = 255				/**< Full Range  0-0xFF */
}ST7735_Font_Length_e ;

typedef enum
{
	TFT_BMP_16 = 16, // 16 bit per pixel
	TFT_BMP_24 =24   // 24 bit per pixel
}ST7735_BMPType_e; // bits per pixel of BMP.


// ******** FUNCTION  PROTOTYPES ************ 


// SPI 
#ifdef TFT_SPI_HARDWARE
	void TFTSPIHWInitialize(SPI_HandleTypeDef * );
#endif
void TFTwriteCommand(uint8_t);
void TFTwriteData(uint8_t);
void TFTspiWriteByte(uint8_t);
void TFTspiWriteSoftware(uint8_t spidata);
void TFTspiWriteBuffer(uint8_t* spidata, uint32_t len);

// Init routines 
void TFTInitPCBType(ST7735_PCBtype_e);
void TFTInitScreenSize(uint8_t xOffset, uint8_t yOffset, uint16_t w, uint16_t h);
void TFTResetPIN(void);

void TFTRcmd1(void);
void TFTRcmd2red(void);
void TFTRcmd3(void);
void TFTBcmd(void);
void TFTRcmd2green(void);

void TFTST7735BInitialize(void);
void TFTGreenTabInitialize(void);
void TFTBlackTabInitialize(void);
void TFTRedTabInitialize(void);


// Misc + Screen related
void TFTsetAddrWindow(uint8_t, uint8_t, uint8_t, uint8_t);
void TFTpushColor(uint16_t color);
void TFTsetRotation(ST7735_rotate_e r);
void TFTchangeMode(ST7735_modes_e m);
uint16_t TFTgetLibVer(void);

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
uint8_t TFTfillRectangleBuffer(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void TFTfillScreen(uint16_t color);
void TFTfillScreenBuffer(uint16_t color);

void TFTdrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
void TFTfillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);

void TFTdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void TFTdrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void TFTfillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void TFTfillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void TFTdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void TFTfillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

// Text
uint8_t TFTdrawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
uint8_t TFTdrawChar2(uint8_t x, uint8_t y, uint8_t character, uint16_t color, uint16_t bg);
uint8_t TFTdrawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);
uint8_t TFTdrawText2(uint8_t x, uint8_t y, char *pText, uint16_t color, uint16_t bg);
void TFTFontNum(ST7735_FontType_e FontNumber);
void TFTsetTextWrap(bool w);
int TFTPrintf(uint8_t x, uint8_t y, uint16_t color, uint16_t bg, uint8_t size, const char *fmt, ...);

// Bitmap & Icon
uint8_t TFTdrawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);
uint8_t TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const uint8_t bitmap[], uint16_t sizeOfBitmap);
uint8_t TFTdrawBitmapBuffer(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const uint8_t *bitmap, uint16_t sizeOfBitmap);
uint8_t TFTdrawBitmap1624Buffer(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *pBmp, ST7735_BMPType_e);

#endif // file header guard 

// ********************** EOF *********************
