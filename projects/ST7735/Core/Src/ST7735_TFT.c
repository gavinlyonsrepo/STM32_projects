/*
 * Project Name: ST7735, 128 by 128, 1.44", red pcb,  SPI TFT module, library.
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

void TFTspiWrite(uint8_t spidata) {
#ifndef TFT_SPI_HARDWARE
    uint8_t i;
    for (i = 0; i < 8; i++) {
        TFT_DIN_SetLow();
        if (spidata & 0x80)TFT_DIN_SetHigh(); // b1000000 Mask with 0 & all zeros out.
        TFT_CLK_SetHigh();
        spidata <<= 1;
        TFT_CLK_SetLow();
    }
#else
    HAL_SPI_Transmit(_hspi1, &spidata, 1, 100);
#endif
}

// Desc: Write an SPI command
// Param1: command byte to send

void TFTwriteCommand(uint8_t cmd_) {
    TFT_DC_SetLow();
    TFT_CS_SetLow();
    TFTspiWrite(cmd_);
    TFT_CS_SetHigh();
}

// Desc: Write SPI data
// Param1: data byte to send

void TFTwriteData(uint8_t data_) {
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    TFTspiWrite(data_);
    TFT_CS_SetHigh();
}

// Desc: Function for Hardware Reset pin

void TFT_ResetPIN() {
    TFT_RST_SetHigh();
    HAL_Delay(10);
    TFT_RST_SetLow();
    HAL_Delay(10);
    TFT_RST_SetHigh();
    HAL_Delay(10);
}

// Desc: init sub-routine ST7735R Green Tab

void TFT_GreenTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFT_Rcmd1();
    TFT_Rcmd2green();
    TFT_Rcmd3();
    _TFTType = TFT_ST7735R_Green;
}

// Desc: init sub-routine ST7735R Green Tab

void TFT_Rcmd2green() {
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

void TFT_RedTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFT_Rcmd1();
    TFT_Rcmd2red();
    TFT_Rcmd3();
    _TFTType = TFT_ST7735R_Red ;

}

// Desc: Init Routine ST7735R Black Tab (ST7735S)

void TFT_BlackTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFT_Rcmd1();
    TFT_Rcmd2red();
    TFT_Rcmd3();
    TFTwriteCommand(ST7735_MADCTL);
    TFTwriteData(0xC0);
    _TFTType = TFT_ST7735S_Black;
}


// Desc: init routine for ST7735B controller

void TFT_ST7735B_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    TFT_Bcmd();
    _TFTType = TFT_ST7735B;
}


// Desc: init routine for ST7735B controller

void TFT_Bcmd() {
    TFTwriteCommand(ST7735_SWRESET);
    HAL_Delay(50);
    TFTwriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);
    TFTwriteCommand(ST7735_COLMOD);
    TFTwriteData(0x05);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_FRMCTR1);
    TFTwriteData(0x00);
    TFTwriteData(0x06);
    TFTwriteData(0x03);
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
    TFTwriteData(0x09);
    TFTwriteData(0x16);
    TFTwriteData(0x09);
    TFTwriteData(0x20);
    TFTwriteData(0x21);
    TFTwriteData(0x1B);
    TFTwriteData(0x13);
    TFTwriteData(0x19);
    TFTwriteData(0x17);
    TFTwriteData(0x15);
    TFTwriteData(0x1E);
    TFTwriteData(0x2B);
    TFTwriteData(0x04);
    TFTwriteData(0x05);
    TFTwriteData(0x02);
    TFTwriteData(0x0E);
    TFTwriteCommand(ST7735_GMCTRN1);
    TFTwriteData(0x0B);
    TFTwriteData(0x14);
    TFTwriteData(0x08);
    TFTwriteData(0x1E);
    TFTwriteData(0x22);
    TFTwriteData(0x1D);
    TFTwriteData(0x18);
    TFTwriteData(0x1E);
    TFTwriteData(0x1B);
    TFTwriteData(0x1A);
    TFTwriteData(0x24);
    TFTwriteData(0x2B);
    TFTwriteData(0x06);
    TFTwriteData(0x06);
    TFTwriteData(0x02);
    TFTwriteData(0x0F);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_CASET);
    TFTwriteData(0x00);
    TFTwriteData(0x02);
    TFTwriteData(0x08);
    TFTwriteData(0x81);
    TFTwriteCommand(ST7735_RASET);
    TFTwriteData(0x00);
    TFTwriteData(0x01);
    TFTwriteData(0x08);
    TFTwriteData(0xA0);
    TFTwriteCommand(ST7735_NORON);
    HAL_Delay(10);
    TFTwriteCommand(ST7735_DISPON);
    HAL_Delay(500);
}


// Desc: init routine

void TFT_Rcmd1() {
    TFTwriteCommand(ST7735_SWRESET);
    HAL_Delay(150);
    TFTwriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);
    TFTwriteCommand(ST7735_FRMCTR1);
    TFTwriteData(0x01);
    TFTwriteData(0x2C);
    TFTwriteData(0x2D);
    TFTwriteCommand(ST7735_FRMCTR2);
    TFTwriteData(0x01);
    TFTwriteData(0x2C);
    TFTwriteData(0x2D);
    TFTwriteCommand(ST7735_FRMCTR3);
    TFTwriteData(0x01);
    TFTwriteData(0x2C);
    TFTwriteData(0x2D);
    TFTwriteData(0x01);
    TFTwriteData(0x2C);
    TFTwriteData(0x2D);
    TFTwriteCommand(ST7735_INVCTR);
    TFTwriteData(0x07);
    TFTwriteCommand(ST7735_PWCTR1);
    TFTwriteData(0xA2);
    TFTwriteData(0x02);
    TFTwriteData(0x84);
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

void TFT_Rcmd2red() {
    TFTwriteCommand(ST7735_CASET);
    TFTwriteData(0x00);
    TFTwriteData(0x00);
    TFTwriteData(0x00);
    TFTwriteData(0x7F);
    TFTwriteCommand(ST7735_RASET);
    TFTwriteData(0x00);
    TFTwriteData(0x00);
    TFTwriteData(0x00);
    TFTwriteData(0x9F);
}

// Desc: init sub-routine

void TFT_Rcmd3() {
    TFTwriteCommand(ST7735_GMCTRP1);
    TFTwriteData(0x02);
    TFTwriteData(0x1C);
    TFTwriteData(0x07);
    TFTwriteData(0x12);
    TFTwriteData(0x37);
    TFTwriteData(0x32);
    TFTwriteData(0x29);
    TFTwriteData(0x2D);
    TFTwriteData(0x29);
    TFTwriteData(0x25);
    TFTwriteData(0x2B);
    TFTwriteData(0x39);
    TFTwriteData(0x00);
    TFTwriteData(0x01);
    TFTwriteData(0x03);
    TFTwriteData(0x10);
    TFTwriteCommand(ST7735_GMCTRN1);
    TFTwriteData(0x03);
    TFTwriteData(0x1D);
    TFTwriteData(0x07);
    TFTwriteData(0x06);
    TFTwriteData(0x2E);
    TFTwriteData(0x2C);
    TFTwriteData(0x29);
    TFTwriteData(0x2D);
    TFTwriteData(0x2E);
    TFTwriteData(0x2E);
    TFTwriteData(0x37);
    TFTwriteData(0x3F);
    TFTwriteData(0x00);
    TFTwriteData(0x00);
    TFTwriteData(0x02);
    TFTwriteData(0x10);
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
            TFTspiWrite(hi);
            TFTspiWrite(lo);
        }
    }
    TFT_CS_SetHigh();
}

// Desc: Fills the whole screen with a given color.

void TFTfillScreen(uint16_t color) {
    TFTfillRectangle(0, 0, _widthTFT, _heightTFT, color);
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
        TFTspiWrite(hi);
        TFTspiWrite(lo);
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
        TFTspiWrite(hi);
        TFTspiWrite(lo);
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
                line = Font_Default[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 2:
#ifdef TFT_Font_Thick
                line = Font_Thick[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 3:
#ifdef TFT_Font_SevenSeg
                line = Font_SevenSeg[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 4:
#ifdef TFT_Font_Wide
                line = Font_Wide[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
#endif
                break;
            case 5:
#ifdef TFT_Font_Tiny
                line = Font_Tiny[(c - ASCIIOffset) * _TFTCurrentFontWidth + i];
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
    TFTspiWrite(hi);
    TFTspiWrite(lo);
    TFT_CS_SetHigh();
}


//  Desc :  Set the font number
// Param1: fontnumber 1-5 enum ST7735_FontType_e
// 1=default 2=thick 3=seven segment 4=wide 5=tiny
// Fonts must be enabled at top of header file.
void TFTFontNum(ST7735_FontType_e FontNumber) {

    typedef enum 
    {
        TFTFont_width_3 = 3, // tiny
        TFTFont_width_4 = 4, // Seven seg
        TFTFont_width_5 = 5, // default
        TFTFont_width_7 = 7, // thick
        TFTFont_width_8 = 8 // wide
    }ST7735_FontWidth_e; // width of the font in bytes, cols.

    ST7735_FontWidth_e setfontwidth;
    _TFTFontNumber = FontNumber;

    switch (_TFTFontNumber) {
        case 1: _TFTCurrentFontWidth = (setfontwidth = TFTFont_width_5);
            break; // Norm default 5 by 8
        case 2: _TFTCurrentFontWidth = (setfontwidth = TFTFont_width_7);
            break; //Thick 7 by 8 (NO LOWERCASE LETTERS)
        case 3: _TFTCurrentFontWidth = (setfontwidth = TFTFont_width_4);
            break; //Seven segment 4 by 8
        case 4: _TFTCurrentFontWidth = (setfontwidth = TFTFont_width_8);
            break; // Wide  8 by 8 (NO LOWERCASE LETTERS)
        case 5: _TFTCurrentFontWidth = (setfontwidth = TFTFont_width_3);
            break; //Tiny 3 by 8
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

// Desc: Draws an custom Icon of X by 8  size to screen , where X = 0 to 127
// Param 1,2  X,Y screen co-ord
// Param 3 0-127 possible values width of icon in pixels , height is fixed at 8 pixels
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
// Param 6: an array of unsigned chars containing bitmap data vertically addressed.
void TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const unsigned char bitmap[]) {
    int16_t byteWidth = (w + 7) / 8;
    uint8_t byte = 0;
    for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++) {
            if (i & 7)
                byte <<= 1;
            else
                byte = bitmap[j * byteWidth + i / 8];
            TFTdrawPixel(x + i, y, (byte & 0x80) ? color : bgcolor);
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
		case TFT_ST7735R_Red : TFT_RedTab_Initialize(); break;
		case TFT_ST7735R_Green: TFT_GreenTab_Initialize(); break;
		case TFT_ST7735S_Black: TFT_BlackTab_Initialize(); break;
		case TFT_ST7735B : TFT_ST7735B_Initialize(); break;
		default: asm("NOP");; break;
	}
}

void SPIHWInitialize(SPI_HandleTypeDef  * hspi1)
{
	_hspi1 = hspi1 ;
}
//**************** EOF *****************
