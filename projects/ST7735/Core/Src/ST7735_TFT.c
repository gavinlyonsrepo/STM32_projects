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
#include "main.h"

uint8_t _FontNumber = 1;
uint8_t _CurrentFontWidth = 5;
uint8_t _currentMode = 0;
bool _wrap = true;
uint8_t _colstart = OFFSET_COL, _rowstart = OFFSET_ROW, _tft_type, _rotation = 0, _xstart = OFFSET_COL, _ystart = OFFSET_ROW;

SPI_HandleTypeDef * _hspi1;

// Desc: Write to SPI both Software and hardware SPI supported
// define TFT_SPI_HARDWARE toggles(see top of header file)
// Param1:  byte to send

void spiwrite(uint8_t spidata) {
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

void write_command(uint8_t cmd_) {
    TFT_DC_SetLow();
    TFT_CS_SetLow();
    spiwrite(cmd_);
    TFT_CS_SetHigh();
}

// Desc: Write SPI data
// Param1: data byte to send

void write_data(uint8_t data_) {
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    spiwrite(data_);
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
#ifdef TFT_PCB_GREEN

void TFT_GreenTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    Rcmd1();
    Rcmd2green();
    Rcmd3();
    _colstart = 2;
    _rowstart = 1;
    _tft_type = 0;
}

// Desc: init sub-routine ST7735R Green Tab

void Rcmd2green() {
    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(0x02);
    write_data(0x00);
    write_data(0x7F + 0x02);
    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(0x01);
    write_data(0x00);
    write_data(0x9F + 0x01);
}


#endif

// Desc: ST7735R Red Tab Init Red PCB version
#ifdef TFT_PCB_RED

void TFT_RedTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    Rcmd1();
    Rcmd2red();
    Rcmd3();
    _tft_type = 0;

}
#endif

// Desc: Init Routine ST7735R Black Tab (ST7735S)
#ifdef TFT_PCB_BLACK

void TFT_BlackTab_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    Rcmd1();
    Rcmd2red();
    Rcmd3();
    write_command(ST7735_MADCTL);
    write_data(0xC0);
    _tft_type = 1;
}
#endif

// Desc: init routine for ST7735B controller
#ifdef TFT_PCB_GENERIC

void TFT_ST7735B_Initialize() {
    TFT_ResetPIN();
    TFT_CS_SetHigh();
    TFT_DC_SetLow();
#ifndef TFT_SPI_HARDWARE
    TFT_CLK_SetLow();
    TFT_DIN_SetLow();
#endif
    Bcmd();
    _tft_type = 2;
}


// Desc: init routine for ST7735B controller

void Bcmd() {
    write_command(ST7735_SWRESET);
    HAL_Delay(50);
    write_command(ST7735_SLPOUT);
    HAL_Delay(500);
    write_command(ST7735_COLMOD);
    write_data(0x05);
    HAL_Delay(10);
    write_command(ST7735_FRMCTR1);
    write_data(0x00);
    write_data(0x06);
    write_data(0x03);
    HAL_Delay(10);
    write_command(ST7735_MADCTL);
    write_data(0x08);
    write_command(ST7735_DISSET5);
    write_data(0x15);
    write_data(0x02);
    write_command(ST7735_INVCTR);
    write_data(0x00);
    write_command(ST7735_PWCTR1);
    write_data(0x02);
    write_data(0x70);
    HAL_Delay(10);
    write_command(ST7735_PWCTR2);
    write_data(0x05);
    write_command(ST7735_PWCTR3);
    write_data(0x01);
    write_data(0x02);
    write_command(ST7735_VMCTR1);
    write_data(0x3C);
    write_data(0x38);
    HAL_Delay(10);
    write_command(ST7735_PWCTR6);
    write_data(0x11);
    write_data(0x15);
    write_command(ST7735_GMCTRP1);
    write_data(0x09);
    write_data(0x16);
    write_data(0x09);
    write_data(0x20);
    write_data(0x21);
    write_data(0x1B);
    write_data(0x13);
    write_data(0x19);
    write_data(0x17);
    write_data(0x15);
    write_data(0x1E);
    write_data(0x2B);
    write_data(0x04);
    write_data(0x05);
    write_data(0x02);
    write_data(0x0E);
    write_command(ST7735_GMCTRN1);
    write_data(0x0B);
    write_data(0x14);
    write_data(0x08);
    write_data(0x1E);
    write_data(0x22);
    write_data(0x1D);
    write_data(0x18);
    write_data(0x1E);
    write_data(0x1B);
    write_data(0x1A);
    write_data(0x24);
    write_data(0x2B);
    write_data(0x06);
    write_data(0x06);
    write_data(0x02);
    write_data(0x0F);
    HAL_Delay(10);
    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(0x02);
    write_data(0x08);
    write_data(0x81);
    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(0x01);
    write_data(0x08);
    write_data(0xA0);
    write_command(ST7735_NORON);
    HAL_Delay(10);
    write_command(ST7735_DISPON);
    HAL_Delay(500);
}
#endif

// Desc: init routine

void Rcmd1() {
    write_command(ST7735_SWRESET);
    HAL_Delay(150);
    write_command(ST7735_SLPOUT);
    HAL_Delay(500);
    write_command(ST7735_FRMCTR1);
    write_data(0x01);
    write_data(0x2C);
    write_data(0x2D);
    write_command(ST7735_FRMCTR2);
    write_data(0x01);
    write_data(0x2C);
    write_data(0x2D);
    write_command(ST7735_FRMCTR3);
    write_data(0x01);
    write_data(0x2C);
    write_data(0x2D);
    write_data(0x01);
    write_data(0x2C);
    write_data(0x2D);
    write_command(ST7735_INVCTR);
    write_data(0x07);
    write_command(ST7735_PWCTR1);
    write_data(0xA2);
    write_data(0x02);
    write_data(0x84);
    write_command(ST7735_PWCTR2);
    write_data(0xC5);
    write_command(ST7735_PWCTR3);
    write_data(0x0A);
    write_data(0x00);
    write_command(ST7735_PWCTR4);
    write_data(0x8A);
    write_data(0x2A);
    write_command(ST7735_PWCTR5);
    write_data(0x8A);
    write_data(0xEE);
    write_command(ST7735_VMCTR1);
    write_data(0x0E);
    write_command(ST7735_INVOFF);
    write_command(ST7735_MADCTL);
    write_data(0xC8);
    write_command(ST7735_COLMOD);
    write_data(0x05);
}

// Desc: init sub-routine

void Rcmd2red() {
    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0x7F);
    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0x9F);
}

// Desc: init sub-routine

void Rcmd3() {
    write_command(ST7735_GMCTRP1);
    write_data(0x02);
    write_data(0x1C);
    write_data(0x07);
    write_data(0x12);
    write_data(0x37);
    write_data(0x32);
    write_data(0x29);
    write_data(0x2D);
    write_data(0x29);
    write_data(0x25);
    write_data(0x2B);
    write_data(0x39);
    write_data(0x00);
    write_data(0x01);
    write_data(0x03);
    write_data(0x10);
    write_command(ST7735_GMCTRN1);
    write_data(0x03);
    write_data(0x1D);
    write_data(0x07);
    write_data(0x06);
    write_data(0x2E);
    write_data(0x2C);
    write_data(0x29);
    write_data(0x2D);
    write_data(0x2E);
    write_data(0x2E);
    write_data(0x37);
    write_data(0x3F);
    write_data(0x00);
    write_data(0x00);
    write_data(0x02);
    write_data(0x10);
    write_command(ST7735_NORON);
    HAL_Delay(10);
    write_command(ST7735_DISPON);
    HAL_Delay(100);
}

void SPIHWInitialize(SPI_HandleTypeDef  * hspi1)
{
	_hspi1 = hspi1 ;
}

/*
  Desc SPI displays set an address window rectangle for blitting pixels
  Param1:  Top left corner x coordinate
  Param2:  y  Top left corner x coordinate
  Param3:  w  Width of window
  Param4:  h  Height of window
 https://en.wikipedia.org/wiki/Bit_blit
 */
void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    write_command(ST7735_CASET);
    write_data(0);
    write_data(x0 + _xstart);
    write_data(0);
    write_data(x1 + _xstart);
    write_command(ST7735_RASET);
    write_data(0);
    write_data(y0 + _ystart);
    write_data(0);
    write_data(y1 + _ystart);
    write_command(ST7735_RAMWR); // Write to RAM
}

//Desc: Draw a pixel to screen
//Param1: X co-ord
//Param2 Y  co-ord
//Param3 color 565 16-bit

void drawPixel(uint8_t x, uint8_t y, uint16_t color) {
    if ((x >= _width) || (y >= _height))
        return;
    setAddrWindow(x, y, x + 1, y + 1);
    write_data(color >> 8);
    write_data(color & 0xFF);
}

// Desc: fills a rectangle starting from coordinates (x,y) with width of w and height of h.

void fillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _width) || (y >= _height))
        return;
    if ((x + w - 1) >= _width)
        w = _width - x;
    if ((y + h - 1) >= _height)
        h = _height - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    hi = color >> 8;
    lo = color;
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            spiwrite(hi);
            spiwrite(lo);
        }
    }
    TFT_CS_SetHigh();
}

// Desc: Fills the whole screen with a given color.

void fillScreen(uint16_t color) {
    fillRectangle(0, 0, _width, _height, color);
}

// Desc: Draws a vertical line starting at (x,y) with height h.

void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _width) || (y >= _height))
        return;
    if ((y + h - 1) >= _height)
        h = _height - y;
    hi = color >> 8;
    lo = color;
    setAddrWindow(x, y, x, y + h - 1);
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    while (h--) {
        spiwrite(hi);
        spiwrite(lo);
    }
    TFT_CS_SetHigh();
}

// Desc: Draws a horizontal line starting at (x,y) with width w.

void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
    uint8_t hi, lo;
    if ((x >= _width) || (y >= _height))
        return;
    if ((x + w - 1) >= _width)
        w = _width - x;
    hi = color >> 8;
    lo = color;
    setAddrWindow(x, y, x + w - 1, y);
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    while (w--) {
        spiwrite(hi);
        spiwrite(lo);
    }
    TFT_CS_SetHigh();
}

// Desc: draws a circle where (x0,y0) are center coordinates an r is circle radius.

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

// Desc : used internally by drawRoundRect

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
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
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

// Desc : used internally by fill circle fillRoundRect

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
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
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Desc: draws rectangle at (x,y) where h is height and w is width of the rectangle.

void drawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

// Desc : draws a line from (x0,y0) to (x1,y1).

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep, dx, dy, err, ystep;
    steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap(x0, y0);
        _swap(x1, y1);
    }
    if (x0 > x1) {
        _swap(x0, x1);
        _swap(y0, y1);
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
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

// Desc : fills a rectangle starting from coordinates (x,y) with width of w and height of h.

void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    int16_t i;
    for (i = x; i < x + w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

// Desc: draws a rectangle with rounded edges. h: height, w:width, r: radius of the rounded edges.

void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color) {
    drawFastHLine(x + r, y, w - 2 * r, color);
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color);
    drawFastVLine(x, y + r, h - 2 * r, color);
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color);
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Desc: draws a filled rectangle with rounded edges. h: height, w:width, r: radius of the rounded edges.

void fillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color) {
    fillRect(x + r, y, w - 2 * r, h, color);
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}


// Desc: draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// Desc: draws a filled triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12, sa, sb;
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap(y0, y1);
        _swap(x0, x1);
    }
    if (y1 > y2) {
        _swap(y2, y1);
        _swap(x2, x1);
    }
    if (y0 > y1) {
        _swap(y0, y1);
        _swap(x0, x1);
    }
    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        drawFastHLine(a, y0, b - a + 1, color);
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
        if (a > b) _swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) _swap(a, b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}

// Desc: writes a char (c) on the TFT
// Param 1 , 2 : coordinates (x, y).
// Param 3: The ASCII character
// Param 4: color 565 16-bit
// Param 5: background color
// Param 6: size.

void drawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size) {
    int8_t i, j;

    if ((x >= _width) || (y >= _height))
        return;
    if (size < 1) size = 1;
    if ((c < ' ') || (c > '~'))
        c = '?';
    for (i = 0; i < _CurrentFontWidth; i++) {
        uint8_t line;
        switch (_FontNumber) {
            case 1:
#ifdef TFT_Font_One
                line = FontOne[(c - TFT_ASCII_OFFSET) * _CurrentFontWidth + i];
#endif
                break;
            case 2:
#ifdef TFT_Font_Two
                line = FontTwo[(c - TFT_ASCII_OFFSET) * _CurrentFontWidth + i];
#endif
                break;
            case 3:
#ifdef TFT_Font_Three
                line = FontThree[(c - TFT_ASCII_OFFSET) * _CurrentFontWidth + i];
#endif
                break;
            case 4:
#ifdef TFT_Font_Four
                line = FontFour[(c - TFT_ASCII_OFFSET) * _CurrentFontWidth + i];
#endif
                break;
            case 5:
#ifdef TFT_Font_Five
                line = FontFive[(c - TFT_ASCII_OFFSET) * _CurrentFontWidth + i];
#endif
                break;
        }
        for (j = 0; j < 7; j++, line >>= 1) {
            if (line & 0x01) {
                if (size == 1) drawPixel(x + i, y + j, color);
                else fillRect(x + (i * size), y + (j * size), size, size, color);
            } else if (bg != color) {
                if (size == 1) drawPixel(x + i, y + j, bg);
                else fillRect(x + i*size, y + j*size, size, size, bg);
            }
        }
    }
}

// Desc :  turn on or off wrap of the text
//Param1 ; TRUE or FALSE

void setTextWrap(bool w) {
    _wrap = w;
}

// Desc: Writes text (*text) on the TFT at coordinates (x, y). size: text size.

void drawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size) {
    uint8_t cursor_x, cursor_y;
    uint16_t textsize, i;
    cursor_x = x, cursor_y = y;
    textsize = strlen(_text);
    for (i = 0; i < textsize; i++) {
        if (_wrap && ((cursor_x + size * _CurrentFontWidth) > _width)) {
            cursor_x = 0;
            cursor_y = cursor_y + size * 7 + 3;
            if (cursor_y > _height) cursor_y = _height;
            if (_text[i] == TFT_ASCII_OFFSET) goto _skip;
        }
        drawChar(cursor_x, cursor_y, _text[i], color, bg, size);
        cursor_x = cursor_x + size * (_CurrentFontWidth + 1);
        if (cursor_x > _width) cursor_x = _width;
_skip:
        ;
    }
}

// Desc: This command defines the Vertical Scrolling Area of the display where:
// Param 1: top_fix_height: describes the Top Fixed Area,
// Param 2: bottom_fix_height: describes the Bottom Fixed Area and
// Param 3: _scroll_direction: is scroll direction (0 for top to bottom and 1 for bottom to top).

void setScrollDefinition(uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction) {
    uint8_t scroll_height;
    scroll_height = _height - top_fix_height - bottom_fix_height;
    write_command(ST7735_VSCRDEF);
    write_data(0x00);
    write_data(top_fix_height);
    write_data(0x00);
    write_data(scroll_height);
    write_data(0x00);
    write_data(bottom_fix_height);
    write_command(ST7735_MADCTL);
    if (_scroll_direction) {
        if (_tft_type == 0) {
            write_data(0xD8);
        }
        if (_tft_type == 1) {
            write_data(0xD0);
        }
        if (_tft_type == 2) {
            write_data(0x18);
        }
    } else {
        if (_tft_type == 0) {
            write_data(0xC8);
        }
        if (_tft_type == 1) {
            write_data(0xC0);
        }
        if (_tft_type == 2) {
            write_data(0x08);
        }
    }
}

// Desc: This command is used together with the setScrollDefinition.
// These two commands describe the scrolling area and the scrolling mode.

void VerticalScroll(uint8_t _vsp) {
    write_command(ST7735_VSCRSADD);
    write_data(0x00);
    write_data(_vsp);
}

/*
Desc: This changes the mode of the display as:
    NORMAL: Normal mode.
    PARTIAL: Enable partial mode to work in portions of display 
    IDLE: IDLE consume less current and shows less color
    SLEEP: Put display driver section in sleep but rest of the logic works.
    INVERT: invert the display
    DISP_ON: Turn on display
    DISP_OFF: Turn off display
 */
void changeMode(const enum ST7735_modes m) {
    switch (m) {
        case NORMAL:
            if (_currentMode == 6) {//was in off display?
                write_command(ST7735_DISPON);
            }
            if (_currentMode == 2) {//was in idle?
                write_command(ST7735_IDLEOF);
            }
            if (_currentMode == 3) {//was in sleep?
                write_command(ST7735_SLPOUT);
                HAL_Delay(120);
            }
            if (_currentMode == 4) {//was inverted?
                _currentMode = 0;
                write_command(ST7735_INVOFF);
            }
            write_command(ST7735_NORON);
            _currentMode = 0;
            break;
        case PARTIAL:
            write_command(ST7735_PTLON);
            _currentMode = 1;
            break;
        case IDLE:
            write_command(ST7735_IDLEON);
            _currentMode = 2;
            break;
        case SLEEP:
            write_command(ST7735_SLPIN);
            _currentMode = 3;
            HAL_Delay(5);
            return;
        case INVERT:
            write_command(ST7735_INVON);
            _currentMode = 4;
            break;
        case DISP_ON:
            write_command(ST7735_DISPON);
            _currentMode = 5;
            break;
        case DISP_OFF:
            write_command(ST7735_DISPOFF);
            _currentMode = 6;
            break;
    }//switch
}


// Desc: Convert 24-bit color to 16-bit color

int16_t Color565(int16_t r, int16_t g, int16_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Desc:  16-bit type to 8-bit types.

void pushColor(uint16_t color) {
    uint8_t hi, lo;
    hi = color >> 8;
    lo = color;
    TFT_DC_SetHigh();
    TFT_CS_SetLow();
    spiwrite(hi);
    spiwrite(lo);
    TFT_CS_SetHigh();
}


//  Desc :  Set the font number
// Param1: fontnumber 1-5
// 1=default 2=thick 3=seven segment 4=wide 5=tiny
// Fonts must be enabled at top of header file.
void TFTFontNum(uint8_t FontNumber) {

    enum TFT_Font_width {
        BLANK = 0, FONT_W_FIVE = 5, FONT_W_SEVEN = 7, FONT_W_FOUR = 4, FONT_W_EIGHT = 8, FONT_W_THREE = 3
    };
    enum TFT_Font_width setfontwidth;
    _FontNumber = FontNumber;

    switch (_FontNumber) {
        case 1: _CurrentFontWidth = (setfontwidth = FONT_W_FIVE);
            break; // Norm default 5 by 8
        case 2: _CurrentFontWidth = (setfontwidth = FONT_W_SEVEN);
            break; //Thick 7 by 8 (NO LOWERCASE LETTERS)
        case 3: _CurrentFontWidth = (setfontwidth = FONT_W_FOUR);
            break; //Seven segment 4 by 8
        case 4: _CurrentFontWidth = (setfontwidth = FONT_W_EIGHT);
            break; // Wide  8 by 8 (NO LOWERCASE LETTERS)
        case 5: _CurrentFontWidth = (setfontwidth = FONT_W_THREE);
            break; //Tiny 3 by 8
    }
}

// Desc: change rotation of display.
// Param1 : mode can be 0-3
// 0 = Normal
// 1=  90 rotate
// 2 = 180 rotate
// 3 =  270 rotate

void setRotation(uint8_t mode) {
    uint8_t madctl = 0;

    _rotation = mode % 4;

    switch (_rotation) {
        case 0:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR;
            _xstart = _colstart;
            _ystart = _rowstart;
            break;
        case 1:
            madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
            _ystart = _colstart;
            _xstart = _rowstart;
            break;
        case 2:
            madctl = ST7735_MADCTL_RGB;
            _xstart = _colstart;
            _ystart = _rowstart;
            break;
        case 3:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
            _ystart = _colstart;
            _xstart = _rowstart;
            break;
    }
    write_command(ST7735_MADCTL);
    write_data(madctl);
}

// Desc: Draws an custom Icon of X by 8  size to screen , where X = 0 to 127
// Param 1,2  X,Y screen co-ord
// Param 3 0-127 possible values width of icon in pixels , height is fixed at 8 pixels
// Param 4,5 icon colors ,is bi-color
// Param 6: an array of unsigned chars containing icon data vertically addressed.
void drawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t backcolor, const unsigned char character[]) {
    if ((x >= _width) || (y >= _height))
        return;
    uint8_t value;
    for (uint8_t byte = 0; byte < w; byte++) 
    {
        for (uint8_t mybit = 0; mybit < 8; mybit++)
        {
            value = !!(character[byte]& (1 << mybit));
            if (value) 
            {
                drawPixel(x + byte, y + mybit, backcolor);
            } else 
            {
                drawPixel(x + byte, y + mybit, color);
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
void drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const unsigned char bitmap[]) {
    int16_t byteWidth = (w + 7) / 8;
    uint8_t byte = 0;
    for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++) {
            if (i & 7)
                byte <<= 1;
            else
                byte = bitmap[j * byteWidth + i / 8];
            drawPixel(x + i, y, (byte & 0x80) ? color : bgcolor);
        }
    }
}
//**************** EOF *****************
