/*
 * Project Name: ST7735
 * File: ST7735_TFT.c
 * Description: Source code file
 * Author: Gavin Lyons.
 * IC: STM32F070RB
 * IDE:  STM32cubeIDE
 * Created May 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */

#include "ST7735_TFT.h"
#include "ST7735_TFT_Font.h"

// ********* Variables  **********

#ifdef TFT_Font_Default  // defined in ST7735_TFT_FONT.h
extern const char * pFontDefaultptr; // defined in ST7735_TFT_FONT.c
#endif
#ifdef TFT_Font_Thick
extern const char * pFontThickptr;
#endif
#ifdef TFT_Font_SevenSeg
extern const char * pFontSevenptr;
#endif
#ifdef TFT_Font_Wide
extern const char * pFontWideptr;
#endif
#ifdef TFT_Font_Tiny
extern const char * pFontTinyptr;
#endif
#ifdef TFT_Font_HomeSpun
extern const char * pFontHomeptr;
#endif

uint8_t _TFTFontNumber = TFTFont_Default ;
uint8_t _TFTCurrentFontWidth = 5;
bool _TFTwrap = true;

uint8_t _TFTcurrentMode = ST7735_modes_Normal;
uint8_t _TFTType; 
uint8_t _TFTrotation = 0;

uint8_t _TFTcolstart; 
uint8_t _TFTrowstart;
uint8_t  _TFTxstart;  //  never change after first init
uint8_t  _TFTystart ; //  never change after first init

uint16_t _widthTFT;
uint16_t _heightTFT;
uint16_t _widthStartTFT; //  never change after first init
uint16_t _heightStartTFT; // never change after first init

SPI_HandleTypeDef * _hspi1;

// ********* Function Space *************

// Desc: Write to SPI both Software and hardware SPI supported
// define TFT_SPI_HARDWARE toggles(see top of header file)
// Param1:  byte to send

void TFTspiWriteByte(uint8_t spidata) {
#ifndef TFT_SPI_HARDWARE
		TFTspiWriteSoftware(spidata);
#else
		HAL_SPI_Transmit(_hspi1, &spidata, 1, 100);
#endif

}

// Desc: Write an SPI command
// Param1: command byte to send

void TFTwriteCommand(uint8_t cmd_) {
    TFT_DC_SetLow();
    TFT_CS_SetLow();
    TFTspiWriteByte(cmd_);
    TFT_CS_SetHigh();
}

// Desc: Write SPI data
// Param1: data byte to send

void TFTwriteData(uint8_t data_) {
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    TFTspiWriteByte(data_);
    TFT_CS_SetHigh();
}

// Desc: Function for Hardware Reset pin

void TFTResetPIN() {
    TFT_RST_SetHigh();
    HAL_Delay(10);
    TFT_RST_SetLow();
    HAL_Delay(10);
    TFT_RST_SetHigh();
    HAL_Delay(10);
}

// Desc: init sub-routine ST7735R Green Tab

void TFTGreenTabInitialize() {
    TFTResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFTRcmd1();
    TFTRcmd2green();
    TFTRcmd3();
    _TFTType = TFT_ST7735R_Green;
}

// Desc: init sub-routine ST7735R Green Tab

void TFTRcmd2green() {
    TFTwriteCommand(ST7735_CASET);
    TFTwriteData(0x00);
    TFTwriteData(0x02);
    TFTwriteData(0x00);
    TFTwriteData(0x7F + 0x02);
    TFTwriteCommand(ST7735_RASET);
    TFTwriteData(0x00);
    TFTwriteData(0x01);
    TFTwriteData(0x00);
    TFTwriteData(0x9F + 0x01);
}

// Desc: ST7735R Red Tab Init Red PCB version

void TFTRedTabInitialize() {
    TFTResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFTRcmd1();
    TFTRcmd2red();
    TFTRcmd3();
    _TFTType = TFT_ST7735R_Red ;

}

// Desc: Init Routine ST7735R Black Tab (ST7735S)

void TFTBlackTabInitialize() {
    TFTResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFTRcmd1();
    TFTRcmd2red();
    TFTRcmd3();
    TFTwriteCommand(ST7735_MADCTL);
    TFTwriteData(0xC0);
    _TFTType = TFT_ST7735S_Black;
}


// Desc: init routine for ST7735B controller

void TFTST7735BInitialize() {
    TFTResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFTBcmd();
    _TFTType = TFT_ST7735B;
}


// Desc: init routine for ST7735B controller

void TFTBcmd() {
	uint8_t seq1[] = {0x00, 0x06, 0x03};
	uint8_t seq2[] = {0x09, 0x16, 0x09, 0x20, 0x21, 0x1B, 0x13, 0x19, 0x17, 0x15, 0x1E, 0x2B, 0x04, 0x05, 0x02, 0x0E};
    uint8_t seq3[] = {0x0B, 0x14, 0x08, 0x1E, 0x22, 0x1D, 0x18, 0x1E, 0x1B, 0x1A, 0x24, 0x2B, 0x06, 0x06, 0x02, 0x0F};
    uint8_t seq4[] = {0x00, 0x02, 0x08, 0x81};
    uint8_t seq5[] = {0x00, 0x01, 0x08, 0xA0};
    TFTwriteCommand(ST7735_SWRESET);
    HAL_Delay(50);
    TFTwriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);
    TFTwriteCommand(ST7735_COLMOD);
    TFTwriteData(0x05);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_FRMCTR1);
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    HAL_Delay(10);
    TFTwriteCommand(ST7735_MADCTL);
    TFTwriteData(0x08);
    TFTwriteCommand(ST7735_DISSET5);
    TFTwriteData(0x15);
    TFTwriteData(0x02);
    TFTwriteCommand(ST7735_INVCTR);
    TFTwriteData(0x00);
    TFTwriteCommand(ST7735_PWCTR1);
    TFTwriteData(0x02);
    TFTwriteData(0x70);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_PWCTR2);
    TFTwriteData(0x05);
    TFTwriteCommand(ST7735_PWCTR3);
    TFTwriteData(0x01);
    TFTwriteData(0x02);
    TFTwriteCommand(ST7735_VMCTR1);
    TFTwriteData(0x3C);
    TFTwriteData(0x38);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_PWCTR6);
    TFTwriteData(0x11);
    TFTwriteData(0x15);
    TFTwriteCommand(ST7735_GMCTRP1);
    TFTspiWriteBuffer(seq2, sizeof(seq2));
    TFTwriteCommand(ST7735_GMCTRN1);
    TFTspiWriteBuffer(seq3, sizeof(seq3));
    HAL_Delay(10);
    TFTwriteCommand(ST7735_CASET);
    TFTspiWriteBuffer(seq4, sizeof(seq4));
    TFTwriteCommand(ST7735_RASET);
    TFTspiWriteBuffer(seq5, sizeof(seq5));
    TFTwriteCommand(ST7735_NORON);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_DISPON);
    HAL_Delay(500);
}


// Desc: init routine

void TFTRcmd1() {
	uint8_t seq1[] = {0x01, 0x2C, 0x2D};
	uint8_t seq2[] = {0xA2, 0x02, 0x84};
	TFTwriteCommand(ST7735_SWRESET);
    HAL_Delay(150);
    TFTwriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);
    TFTwriteCommand(ST7735_FRMCTR1);
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTwriteCommand(ST7735_FRMCTR2);
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTwriteCommand(ST7735_FRMCTR3);
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTwriteCommand(ST7735_INVCTR);
    TFTwriteData(0x07);
    TFTwriteCommand(ST7735_PWCTR1);
    TFTspiWriteBuffer(seq2, sizeof(seq2));
    TFTwriteCommand(ST7735_PWCTR2);
    TFTwriteData(0xC5);
    TFTwriteCommand(ST7735_PWCTR3);
    TFTwriteData(0x0A);
    TFTwriteData(0x00);
    TFTwriteCommand(ST7735_PWCTR4);
    TFTwriteData(0x8A);
    TFTwriteData(0x2A);
    TFTwriteCommand(ST7735_PWCTR5);
    TFTwriteData(0x8A);
    TFTwriteData(0xEE);
    TFTwriteCommand(ST7735_VMCTR1);
    TFTwriteData(0x0E);
    TFTwriteCommand(ST7735_INVOFF);
    TFTwriteCommand(ST7735_MADCTL);
    TFTwriteData(0xC8);
    TFTwriteCommand(ST7735_COLMOD);
    TFTwriteData(0x05);
}

// Desc: init sub-routine

void TFTRcmd2red() {
	uint8_t seq1[] = {0x00, 0x00, 0x00, 0x7F};
	uint8_t seq2[] = {0x00, 0x00, 0x00, 0x9F};
	TFTwriteCommand(ST7735_CASET);
	TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTwriteCommand(ST7735_RASET);
    TFTspiWriteBuffer(seq2, sizeof(seq2));
}

// Desc: init sub-routine

void TFTRcmd3() {

    uint8_t seq1[] = {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
    uint8_t seq2[] = {0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
    TFTwriteCommand(ST7735_GMCTRP1);
    TFTspiWriteBuffer(seq1, sizeof(seq1));
    TFTwriteCommand(ST7735_GMCTRN1);
    TFTspiWriteBuffer(seq2, sizeof(seq2));
    TFTwriteCommand(ST7735_NORON);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_DISPON);
    HAL_Delay(100);
}

/*
  Desc SPI displays set an address window rectangle for blitting pixels
  Param1:  Top left corner x coordinate
  Param2:  y  Top left corner x coordinate
  Param3:  w  Width of window
  Param4:  h  Height of window
 https://en.wikipedia.org/wiki/Bit_blit
 */
void TFTsetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    TFTwriteCommand(ST7735_CASET);
    TFTwriteData(0);
    TFTwriteData(x0 + _TFTxstart);
    TFTwriteData(0);
    TFTwriteData(x1 + _TFTxstart);
    TFTwriteCommand(ST7735_RASET);
    TFTwriteData(0);
    TFTwriteData(y0 + _TFTystart);
    TFTwriteData(0);
    TFTwriteData(y1 + _TFTystart);
    TFTwriteCommand(ST7735_RAMWR); // Write to RAM
}

//Desc: Draw a pixel to screen
//Param1: X co-ord
//Param2 Y  co-ord
//Param3 color 565 16-bit

void TFTdrawPixel(uint8_t x, uint8_t y, uint16_t color) {
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    TFTsetAddrWindow(x, y, x + 1, y + 1);
    TFTwriteData(color >> 8);
    TFTwriteData(color & 0xFF);
}

// Desc: fills a rectangle starting from coordinates (x,y) with width of w and height of h.

void TFTfillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    if ((x + w - 1) >= _widthTFT)
        w = _widthTFT - x;
    if ((y + h - 1) >= _heightTFT)
        h = _heightTFT - y;
    TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);
    hi = color >> 8;
    lo = color;
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            TFTspiWriteByte(hi);
            TFTspiWriteByte(lo);
        }
    }
    TFT_CS_SetHigh();
}

// Desc: Fills the whole screen with a given color.
// wraps  TFTfillRectangle
void TFTfillScreen(uint16_t color) {
    TFTfillRectangle(0, 0, _widthTFT, _heightTFT, color);
}

// Desc: Fills the whole screen with a given color.
// wraps TFTfillRectangleBuffer which uses a heap buffer
// much faster(5*) than TFTfillScreen
void TFTfillScreenBuffer(uint16_t color)
{
	TFTfillRectangleBuffer(0, 0, _widthTFT, _heightTFT, color);
}

// Desc: Draws a vertical line starting at (x,y) with height h.

void TFTdrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    if ((y + h - 1) >= _heightTFT)
        h = _heightTFT - y;
    hi = color >> 8;
    lo = color;
    TFTsetAddrWindow(x, y, x, y + h - 1);
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    while (h--) {
        TFTspiWriteByte(hi);
        TFTspiWriteByte(lo);
    }
    TFT_CS_SetHigh();
}

// Desc: Draws a horizontal line starting at (x,y) with width w.

void TFTdrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    if ((x + w - 1) >= _widthTFT)
        w = _widthTFT - x;
    hi = color >> 8;
    lo = color;
    TFTsetAddrWindow(x, y, x + w - 1, y);
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    while (w--) {
        TFTspiWriteByte(hi);
        TFTspiWriteByte(lo);
    }
    TFT_CS_SetHigh();
}

// Desc: draws a circle where (x0,y0) are center coordinates an r is circle radius.

void TFTdrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    TFTdrawPixel(x0, y0 + r, color);
    TFTdrawPixel(x0, y0 - r, color);
    TFTdrawPixel(x0 + r, y0, color);
    TFTdrawPixel(x0 - r, y0, color);
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        TFTdrawPixel(x0 + x, y0 + y, color);
        TFTdrawPixel(x0 - x, y0 + y, color);
        TFTdrawPixel(x0 + x, y0 - y, color);
        TFTdrawPixel(x0 - x, y0 - y, color);
        TFTdrawPixel(x0 + y, y0 + x, color);
        TFTdrawPixel(x0 - y, y0 + x, color);
        TFTdrawPixel(x0 + y, y0 - x, color);
        TFTdrawPixel(x0 - y, y0 - x, color);
    }
}

// Desc : used internally by drawRoundRect

void TFTdrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            TFTdrawPixel(x0 + x, y0 + y, color);
            TFTdrawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            TFTdrawPixel(x0 + x, y0 - y, color);
            TFTdrawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            TFTdrawPixel(x0 - y, y0 + x, color);
            TFTdrawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            TFTdrawPixel(x0 - y, y0 - x, color);
            TFTdrawPixel(x0 - x, y0 - y, color);
        }
    }
}

// Desc : used internally by fill circle fillRoundRect

void TFTfillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            TFTdrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            TFTdrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            TFTdrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            TFTdrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

void TFTfillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    TFTdrawFastVLine(x0, y0 - r, 2 * r + 1, color);
    TFTfillCircleHelper(x0, y0, r, 3, 0, color);
}

// Desc: draws rectangle at (x,y) where h is height and w is width of the rectangle.

void TFTdrawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    TFTdrawFastHLine(x, y, w, color);
    TFTdrawFastHLine(x, y + h - 1, w, color);
    TFTdrawFastVLine(x, y, h, color);
    TFTdrawFastVLine(x + w - 1, y, h, color);
}

// Desc : draws a line from (x0,y0) to (x1,y1).

void TFTdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep, dx, dy, err, ystep;
    steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_TFT(x0, y0);
        _swap_TFT(x1, y1);
    }
    if (x0 > x1) {
        _swap_TFT(x0, x1);
        _swap_TFT(y0, y1);
    }
    dx = x1 - x0;
    dy = abs(y1 - y0);

    err = dx / 2;
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            TFTdrawPixel(y0, x0, color);
        } else {
            TFTdrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

// Desc : fills a rectangle starting from coordinates (x,y) with width of w and height of h.

void TFTfillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    int16_t i;
    for (i = x; i < x + w; i++) {
        TFTdrawFastVLine(i, y, h, color);
    }
}

// Desc: draws a rectangle with rounded edges. h: height, w:width, r: radius of the rounded edges.

void TFTdrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color) {
    TFTdrawFastHLine(x + r, y, w - 2 * r, color);
    TFTdrawFastHLine(x + r, y + h - 1, w - 2 * r, color);
    TFTdrawFastVLine(x, y + r, h - 2 * r, color);
    TFTdrawFastVLine(x + w - 1, y + r, h - 2 * r, color);
    TFTdrawCircleHelper(x + r, y + r, r, 1, color);
    TFTdrawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    TFTdrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    TFTdrawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Desc: draws a filled rectangle with rounded edges. h: height, w:width, r: radius of the rounded edges.

void TFTfillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color) {
    TFTfillRect(x + r, y, w - 2 * r, h, color);
    TFTfillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    TFTfillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}


// Desc: draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).

void TFTdrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    TFTdrawLine(x0, y0, x1, y1, color);
    TFTdrawLine(x1, y1, x2, y2, color);
    TFTdrawLine(x2, y2, x0, y0, color);
}

// Desc: draws a filled triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).

void TFTfillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12, sa, sb;
    if (y0 > y1) {
        _swap_TFT(y0, y1);
        _swap_TFT(x0, x1);
    }
    if (y1 > y2) {
        _swap_TFT(y2, y1);
        _swap_TFT(x2, x1);
    }
    if (y0 > y1) {
        _swap_TFT(y0, y1);
        _swap_TFT(x0, x1);
    }
    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        TFTdrawFastHLine(a, y0, b - a + 1, color);
        return;
    }
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;
    sa = 0;
    sb = 0;
    if (y1 == y2) last = y1;
    else last = y1 - 1;
    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b) _swap_TFT(a, b);
        TFTdrawFastHLine(a, y, b - a + 1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) _swap_TFT(a, b);
        TFTdrawFastHLine(a, y, b - a + 1, color);
    }
}

// Desc: writes a char (c) on the TFT
// Param 1 , 2 : coordinates (x, y).
// Param 3: The ASCII character
// Param 4: color 565 16-bit
// Param 5: background color
// Param 6: size.

void TFTdrawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size) {
    const uint8_t ASCIIOffset = 0x20;
    int8_t i, j;
    
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    if (size < 1) size = 1;
    if ((c < ' ') || (c > '~'))
        c = '?';
    for (i = 0; i < _TFTCurrentFontWidth; i++) {
        uint8_t line;
        switch (_TFTFontNumber) {
            case 1:
#ifdef TFT_Font_Default
                line = pFontDefaultptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 2:
#ifdef TFT_Font_Thick
                line = pFontThickptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 3:
#ifdef TFT_Font_SevenSeg
                line = pFontSevenptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 4:
#ifdef TFT_Font_Wide
                line = pFontWideptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 5:
#ifdef TFT_Font_Tiny
                line = pFontTinyptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 6:
#ifdef TFT_Font_HomeSpun
                line = pFontHomeptr[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
        }
        for (j = 0; j < 7; j++, line >>= 1) {
            if (line & 0x01) {
                if (size == 1) TFTdrawPixel(x + i, y + j, color);
                else TFTfillRect(x + (i * size), y + (j * size), size, size, color);
            } else if (bg != color) {
                if (size == 1) TFTdrawPixel(x + i, y + j, bg);
                else TFTfillRect(x + i*size, y + j*size, size, size, bg);
            }
        }
    }
}

// Desc :  turn on or off wrap of the text
//Param1 ; TRUE or FALSE

void TFTsetTextWrap(bool w) {
    _TFTwrap = w;
}

// Desc: Writes text (*text) on the TFT at coordinates (x, y). size: text size.

void TFTdrawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size) {
    uint8_t cursor_x, cursor_y;
    uint16_t textsize, i;
    cursor_x = x, cursor_y = y;
    textsize = strlen(_text);
    for (i = 0; i < textsize; i++) {
        if (_TFTwrap && ((cursor_x + size * _TFTCurrentFontWidth) > _widthTFT)) {
            cursor_x = 0;
            cursor_y = cursor_y + size * 7 + 3;
            if (cursor_y > _heightTFT) cursor_y = _heightTFT;
            if (_text[i] == 0x20) goto _skip;
        }
        TFTdrawChar(cursor_x, cursor_y, _text[i], color, bg, size);
        cursor_x = cursor_x + size * (_TFTCurrentFontWidth + 1);
        if (cursor_x > _widthTFT) cursor_x = _widthTFT;
_skip:
        ;
    }
}

// Desc: This command defines the Vertical Scrolling Area of the display where:
// Param 1: top_fix_height: describes the Top Fixed Area,
// Param 2: bottom_fix_height: describes the Bottom Fixed Area and
// Param 3: _scroll_direction: is scroll direction (0 for top to bottom and 1 for bottom to top).

void TFTsetScrollDefinition(uint8_t topFixHeight, uint8_t bottomFixHeight, bool scrollDirection) {
    uint8_t scroll_height;
    scroll_height = _heightTFT - topFixHeight - bottomFixHeight;
    TFTwriteCommand(ST7735_VSCRDEF);
    TFTwriteData(0x00);
    TFTwriteData(topFixHeight);
    TFTwriteData(0x00);
    TFTwriteData(scroll_height);
    TFTwriteData(0x00);
    TFTwriteData(bottomFixHeight);
    TFTwriteCommand(ST7735_MADCTL);
    if (scrollDirection) {
        if ((_TFTType == TFT_ST7735R_Red)  || (_TFTType == TFT_ST7735R_Green)) {
            TFTwriteData(0xD8);
        }
        if (_TFTType == TFT_ST7735S_Black) {
            TFTwriteData(0xD0);
        }
        if (_TFTType == TFT_ST7735B) {
            TFTwriteData(0x18);
        }
    } else {
        if ((_TFTType == TFT_ST7735R_Red)  || (_TFTType == TFT_ST7735R_Green)) {
            TFTwriteData(0xC8);
        }
        if (_TFTType == TFT_ST7735S_Black) {
            TFTwriteData(0xC0);
        }
        if (_TFTType == TFT_ST7735B) {
            TFTwriteData(0x08);
        }
    }
}

// Desc: This method is used together with the setScrollDefinition.
// These two methods describe the scrolling area and the scrolling mode.

void TFTVerticalScroll(uint8_t vsp) {
    TFTwriteCommand(ST7735_VSCRSADD);
    TFTwriteData(0x00);
    TFTwriteData(vsp);
}

/*
Desc: This changes the mode of the display as:
 * Param1: ST7735_modes_e   enum
   ST7735_modes_Normal : Normal mode.
   ST7735_modes_Partial: Enable partial mode to work in portions of display 
   ST7735_modes_Idle: IDLE consume less current and shows less color
   ST7735_modes_Sleep: Put display driver section in sleep but rest of the logic works.
   ST7735_modes_Invert: invert the display
   ST7735_modes_DisplayOn: Turn on display
   ST7735_modes_DisplayOff: Turn off display
 */
void TFTchangeMode(ST7735_modes_e mode) {
    switch (mode) {
        case ST7735_modes_Normal:
            if (_TFTcurrentMode == ST7735_modes_DisplayOff) {//was in off display?
                TFTwriteCommand(ST7735_DISPON);
            }
            if (_TFTcurrentMode ==  ST7735_modes_Idle) {//was in idle?
                TFTwriteCommand(ST7735_IDLEOF);
            }
            if (_TFTcurrentMode == ST7735_modes_Sleep) {//was in sleep?
                TFTwriteCommand(ST7735_SLPOUT);
                HAL_Delay(120);
            }
            if (_TFTcurrentMode == ST7735_modes_Invert) {//was inverted?
                _TFTcurrentMode = 0;
                TFTwriteCommand(ST7735_INVOFF);
            }
            TFTwriteCommand(ST7735_NORON);
            _TFTcurrentMode = ST7735_modes_Normal;
            break;
        case ST7735_modes_Partial:
            TFTwriteCommand(ST7735_PTLON);
            _TFTcurrentMode = ST7735_modes_Partial;
            break;
        case ST7735_modes_Idle:
            TFTwriteCommand(ST7735_IDLEON);
            _TFTcurrentMode = ST7735_modes_Idle;
            break;
        case ST7735_modes_Sleep:
            TFTwriteCommand(ST7735_SLPIN);
            _TFTcurrentMode = ST7735_modes_Sleep;
            HAL_Delay(5);
            return;
        case ST7735_modes_Invert:
            TFTwriteCommand(ST7735_INVON);
            _TFTcurrentMode = ST7735_modes_Invert;
            break;
        case ST7735_modes_DisplayOn:
            TFTwriteCommand(ST7735_DISPON);
            _TFTcurrentMode = ST7735_modes_DisplayOn;
            break;
        case ST7735_modes_DisplayOff:
            TFTwriteCommand(ST7735_DISPOFF);
            _TFTcurrentMode = ST7735_modes_DisplayOff;
            break;
    }//switch
}


// Desc: Convert 24-bit color to 16-bit color

int16_t Color565(int16_t r, int16_t g, int16_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Desc:  16-bit type to 8-bit types.

void TFTpushColor(uint16_t color) {
    uint8_t hi, lo;
    hi = color >> 8;
    lo = color;
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    TFTspiWriteByte(hi);
    TFTspiWriteByte(lo);
    TFT_CS_SetHigh();
}


//  Desc :  Set the font number
// Param1: fontnumber 1-6 enum ST7735_FontType_e
// 1=default 2=thick 3=seven segment 4=wide 5=tiny 6=homespun
// Fonts must be enabled at top of header file.
void TFTFontNum(ST7735_FontType_e FontNumber) {

    _TFTFontNumber = FontNumber;

    switch (_TFTFontNumber) {
        case 1: _TFTCurrentFontWidth = TFTFont_width_5;
            break; // Norm default 5 by 8
        case 2: _TFTCurrentFontWidth = TFTFont_width_7;
            break; //Thick 7 by 8 (NO LOWERCASE LETTERS)
        case 3: _TFTCurrentFontWidth = TFTFont_width_4;
            break; //Seven segment 4 by 8
        case 4: _TFTCurrentFontWidth = TFTFont_width_8;
            break; // Wide  8 by 8 (NO LOWERCASE LETTERS)
        case 5: _TFTCurrentFontWidth = TFTFont_width_3;
            break; //Tiny 3 by 8
        case 6: _TFTCurrentFontWidth = TFTFont_width_7;
             break; //homespun 7 by 8
    }
}

// Desc: change rotation of display.
// Param1 : mode can be 0-3 , enum ST7735_rotate_e 
// 0 = Normal
// 1=  90 rotate
// 2 = 180 rotate
// 3 =  270 rotate

void TFTsetRotation(ST7735_rotate_e mode) {
    uint8_t madctl = 0;

    _TFTrotation = mode % 4;

    switch (_TFTrotation) {
        case ST7735_Degrees_0:
           if (_TFTType == TFT_ST7735S_Black ){
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR;
			}
			_widthTFT =_widthStartTFT;
			_heightTFT = _heightStartTFT;   
            _TFTxstart = _TFTcolstart;
            _TFTystart = _TFTrowstart;
            break;
        case ST7735_Degrees_90:
            if (_TFTType  == TFT_ST7735S_Black )
			{
				madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
			}
			_widthTFT  = _heightStartTFT;
			_heightTFT = _widthStartTFT;
            _TFTystart = _TFTcolstart;
            _TFTxstart = _TFTrowstart;
            break;
        case ST7735_Degrees_180:
            if (_TFTType  == TFT_ST7735S_Black)
			{
				madctl = ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_BGR;
			}
			_widthTFT =  _widthStartTFT;
			_heightTFT = _heightStartTFT;
            _TFTxstart = _TFTcolstart;
            _TFTystart = _TFTrowstart;
            break;
        case ST7735_Degrees_270:
		if (_TFTType  == TFT_ST7735S_Black){
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
			}else{
				madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
			}
            _TFTxstart = _TFTcolstart;
            _TFTystart = _TFTrowstart;
			_widthTFT =_heightStartTFT;
			_heightTFT = _widthStartTFT;
            break;
    }
    TFTwriteCommand(ST7735_MADCTL);
    TFTwriteData(madctl);
}

// Desc: Draws an custom Icon of X by 8  size to screen , where X = 0 to maxY
// Param 1,2  X,Y screen co-ord
// Param 3 0-(MAX_Y) possible values width of icon in pixels , height is fixed at 8 pixels
// Param 4,5 icon colors ,is bi-color
// Param 6: an array of unsigned chars containing icon data vertically addressed.
void TFTdrawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t backcolor, const unsigned char character[]) {
    if ((x >= _widthTFT) || (y >= _heightTFT))
        return;
    uint8_t value;
    for (uint8_t byte = 0; byte < w; byte++) 
    {
        for (uint8_t mybit = 0; mybit < 8; mybit++)
        {
            value = !!(character[byte]& (1 << mybit));
            if (value) 
            {
                TFTdrawPixel(x + byte, y + mybit, backcolor);
            } else 
            {
                TFTdrawPixel(x + byte, y + mybit, color);
            }
            value = 0;
        }
    }
}

// Desc: Draws an bitmap to screen
// Param 1,2  X,Y screen co-ord
// Param 3,4 0-127 possible values width and height of bitmap in pixels 
// Param 4,5 bitmap colors ,bitmap is bi-color
// Param 6: an array of data containing bitmap data vertically addressed.
void TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const uint8_t bitmap[]) {
    int16_t byteWidth = (w + 7) / 8;
    uint8_t byte = 0;
    for (int16_t row = 0; row < h; row++, y++) {
        for (int16_t col = 0; col < w; col++) {
            if (col & 7)
                byte <<= 1;
            else
                byte = bitmap[row * byteWidth + col / 8];
            TFTdrawPixel(x + col, y, (byte & 0x80) ? color : bgcolor);
        }
    }
}

// Func Desc: initialise the variables that define the size of the screen
// These offsets can be adjusted for any issues with manufacture tolerance/defects
// Param 1: Column offset
// Param 2: Row offset
// Param 3: Screen width in pixels
// Param 4: Screen height in pixels
void TFTInitScreenSize(uint8_t colOffset, uint8_t rowOffset, uint16_t width_TFT, uint16_t height_TFT)
{
	_TFTcolstart = colOffset; 
	_TFTrowstart= rowOffset;
	_TFTxstart = colOffset;  // Holds init value will never change after this point
	_TFTystart = rowOffset; // Holds init value will never change after this point
	
	_widthTFT = width_TFT;
	_heightTFT = height_TFT;
	_widthStartTFT = width_TFT; // Holds init value will never change after this point
	_heightStartTFT = height_TFT; // Holds init value will never change after this point
}

// Func Screen : intialise PCBtype
// Param 1: Enum TFT_PCBtype_e , 4 choices 0-3
void TFTInitPCBType(ST7735_PCBtype_e pcbType)
{
	uint8_t choice = pcbType;
	switch(choice)
	{
		case TFT_ST7735R_Red : TFTRedTabInitialize(); break;
		case TFT_ST7735R_Green: TFTGreenTabInitialize(); break;
		case TFT_ST7735S_Black: TFTBlackTabInitialize(); break;
		case TFT_ST7735B : TFTST7735BInitialize(); break;
		default: asm("NOP");; break;
	}
}

void TFTSPIHWInitialize(SPI_HandleTypeDef  * hspi1)
{
	_hspi1 = hspi1 ;
}


// Write a buffer to Screen
// Param1 Pointer to the buffer
// Param2 size of buffer
void TFTspiWriteBuffer(uint8_t* spidata, uint32_t len) {
	TFT_DC_SetHigh();
	TFT_CS_SetLow();
#ifndef TFT_SPI_HARDWARE
		for(uint32_t i=0; i<len; i++) {
			TFTspiWriteSoftware(spidata[i]);
		}
#else
		HAL_SPI_Transmit(_hspi1, spidata, len, 100);
#endif
		TFT_CS_SetHigh();
}

// Desc: Write a byte to SPI using software SPI
// Param1: byte to send

void TFTspiWriteSoftware(uint8_t spidata) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		TFT_DIN_SetLow();
		if (spidata & 0x80)TFT_DIN_SetHigh();
		TFT_CLK_SetHigh();
		spidata <<= 1;
		TFT_CLK_SetLow();
	}
}

// Writes a bi-clour bitmap to screen using a buffer
// Same as TFTdrawBitmap but uses a buffered write, pixel row by row
// Param 1,2  X,Y screen co-ord
// Param 3,4 width and height of bitmap in pixels
// Param 4,5 bitmap colors ,bitmap is bi-color,, a foreground and a background
// Param 6: pointer an array of data containing bitmap data vertically addressed.
// Note :: ~25 times faster than TFTdrawBitmap
// Buffer used is W*2 (256 bytes for 128*2) and is assigned on the heap using malloc
// each pixel needs  two bytes , Buffer holds data for single row of pixels
void TFTdrawBitmapBuffer(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const uint8_t *pBmp)
{
		int16_t byteWidth = (w + 7) / 8;
		uint8_t byte = 0;
		uint16_t mycolor = 0;
		uint16_t ptr;

		// Check bounds
		if ((x >= _widthTFT) || (y >= _heightTFT)) return;
		if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
		if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

		// Create bitmap buffer
		uint8_t* rowBuffer = (uint8_t*)malloc(w * 2);
		if (rowBuffer == NULL){return;}
		ptr = 0;
		TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);

		for (int16_t row = 0; row < h; row++)
		{
			for (int16_t col = 0; col < w; col++)
			{
				if (col & 7)
					byte <<= 1;
				else
					byte = (pBmp[row * byteWidth + col / 8]);
				mycolor = (byte & 0x80) ? color : bgcolor;
				rowBuffer[ptr++] = mycolor >> 8;
				rowBuffer[ptr++] = mycolor;
			}
			TFTspiWriteBuffer(rowBuffer, w*sizeof(uint16_t));
			ptr = 0;
		}
		free(rowBuffer);

}

// Writes a filled rect to the screen using buffered data
// smae as  TFTfillRectangle but uses bugger
// Param 1,2  X,Y screen co-ord
// Param 3,4 possible values width and height of bitmap in pixels
// Param 4,5 bitmap colors ,565.
// Note :: ~5 times faster than TFTfillRectangle
// Buffer used is W*2 (256 bytes for 128*2) and is assigned on the heap using malloc
// each pixel needs  two bytes , BUffer holds data for single row of pixels
void TFTfillRectangleBuffer(uint8_t x , uint8_t y , uint8_t w, uint8_t h, uint16_t color)
{
	uint8_t hi, lo;

	// Check bounds
	if ((x >= _widthTFT) || (y >= _heightTFT)) return;
	if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
	if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

	// Colour to bytes
	hi = color >> 8;
	lo = color & 0xFF;

	// Create bitmap buffer for a row
	uint8_t* rowBuffer = (uint8_t*)malloc(w*sizeof(uint16_t));
	if (rowBuffer == NULL) return; // check malloc
	TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);

	for(uint16_t row = 0; row<h*sizeof(uint16_t);row++)
	{
		for(uint16_t col = 0; col<w*sizeof(uint16_t);) {
			rowBuffer[col++] = hi;
			rowBuffer[col++] = lo;
		}
	TFTspiWriteBuffer(rowBuffer, w*sizeof(uint16_t));

	}
	free(rowBuffer);
}

// Desc: Draws an 16 or 24 bit color bitmap to screen from a data Array
// Param 1,2  X,Y screen co-ord
// Param 3,4 possible values width and height of bitmap in pixels
// Param 5 pointer to the databuffer containing data
// Parma 6 enum ST7735_BMPType_e Pixel  bit resolution 16 or 24 , 565 or RGB.
// Note uses buffered data , sends data row by row in a buffer 256 bytes
// for 128 pixel wide screen
// COnverts 24 bit images to 16 bit internally
void TFTdrawBitmap1624Buffer(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *pBmp, ST7735_BMPType_e bmpType) {
	uint8_t col, row;
	uint16_t ptr=0;
	uint16_t color;
	uint8_t red, green, blue=0;

	// Check bounds
	if ((x >= _widthTFT) || (y >= _heightTFT)) return;
	if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
	if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

	// Create bitmap buffer
	uint8_t* rowBuffer= (uint8_t*)malloc(w *2);
	if (rowBuffer == NULL) return;
	TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);

	for(row = 0; row < h; row++)
	{
		for(col = 0; col < w; col ++)
		{
			if (bmpType == TFT_BMP_16)
			{
				rowBuffer[ptr++] = (*pBmp++);
				rowBuffer[ptr++] = (*pBmp++);
			} else if (bmpType == TFT_BMP_24){
				// RRRR RRRR GGGG GGGG BBBB BBBB => 565 => RRRRR GGGGGG BBBBB
				red = *pBmp++;
				green = *pBmp++;
				blue = *pBmp++;
				color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
				rowBuffer[ptr++] = color >> 8; // upper byte
				rowBuffer[ptr++] = color & 0x00FF; // lower byte
			}
		}
		TFTspiWriteBuffer(rowBuffer, w*sizeof(uint16_t));
		ptr = 0;
	}
	free(rowBuffer);
}
//**************** EOF *****************
