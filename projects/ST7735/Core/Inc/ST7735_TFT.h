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

#ifndef _ST7735_TFT_H
#define _ST7735_TFT_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

// ******** USER OPTION 1 PCB_TYPE *********
// PCB type , pick one and one only 
#define TFT_PCB_RED
//#define TFT_PCB_GREEN
//#define TFT_PCB_BLACK
//#define  TFT_PCB_GENERIC
// ****************************************

// ****** USER OPTION 2 FONT ******
#define TFT_Font_One
#define TFT_Font_Two
#define TFT_Font_Three
#define TFT_Font_Four
#define TFT_Font_Five

// ******** USER OPTION 3 SPI TYPE ***********
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

// Offsets + screen size Misc
#define TFT_ASCII_OFFSET 0x20 //0x20, ASCII character for Space, The font table starts with this character
#define OFFSET_COL 2  // These offsets can be adjusted for any issues with manfacture tolerance 
#define OFFSET_ROW 0
#define _swap(a, b) { int16_t t; t = a; a = b; b = t;}
#define _width         128
#define _height        128

enum ST7735_modes {
    NORMAL = 0, PARTIAL, IDLE, SLEEP, INVERT, DISP_ON, DISP_OFF
}; //0,1,2,3,4,5,6

// ******** FUNCTION  PROTOTYPES ************ 

// SPI 
void spiwrite(uint8_t);
void write_command(uint8_t);
void write_data(uint8_t);

// Init routines 
void TFT_ResetPIN(void);
void Rcmd1(void);
void Rcmd2red(void);
void Rcmd3(void);

#ifdef TFT_PCB_GENERIC
void TFT_ST7735B_Initialize(void);
void Bcmd();
#endif

#ifdef TFT_PCB_GREEN
void TFT_GreenTab_Initialize(void);
void Rcmd2green();
#endif

#ifdef TFT_PCB_BLACK
void TFT_BlackTab_Initialize(void);
#endif

#ifdef TFT_PCB_RED
void TFT_RedTab_Initialize(void);
#endif

#ifdef TFT_SPI_HARDWARE
	void SPIHWInitialize(SPI_HandleTypeDef * );
#endif

// Misc + Screen related
void setAddrWindow(uint8_t, uint8_t, uint8_t, uint8_t);
void fillScreen(uint16_t color);
void pushColor(uint16_t color);
void setRotation(uint8_t);
void changeMode(const enum ST7735_modes m);

//Scroll 
void setScrollDefinition(uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction);
void VerticalScroll(uint8_t _vsp);

// Shapes and lines
void drawPixel(uint8_t, uint8_t, uint16_t);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);

void drawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void fillRectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
void fillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

// Text
void drawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
void setTextWrap(bool w);
void drawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);
void TFTFontNum(uint8_t FontNumber);

// Bitmap & Icon
void drawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);
void drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const unsigned char bitmap[]);

#endif // file header guard 

// ********************** EOF *********************
