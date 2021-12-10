
/*
* Project Name: ERM19264_UC1609
* File:custom_graphics.cpp
* Description: ERM19264 LCD driven by UC1609C controller header file for the custom graphics functions based on Adafruit graphics library( see above)
* Author: Gavin Lyons.
*/

#include "custom_graphics.h"
#include "custom_graphics_font.h"


custom_graphics::custom_graphics(int16_t w, int16_t h):
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFF;
  wrap      = true;
}

// Draw a circle outline
void custom_graphics::drawCircle(int16_t x0, int16_t y0, int16_t r,
    uint8_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
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

void custom_graphics::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint8_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
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

void custom_graphics::fillCircle(int16_t x0, int16_t y0, int16_t r,
			      uint8_t color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void custom_graphics::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, uint8_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void custom_graphics::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint8_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
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

// Draw a rectangle
void custom_graphics::drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
			    uint8_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void custom_graphics::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint8_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

void custom_graphics::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint8_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

void custom_graphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    uint8_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void custom_graphics::fillScreen(uint8_t color) {
  fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void custom_graphics::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint8_t color) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x    , y+r  , h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void custom_graphics::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint8_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void custom_graphics::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint8_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void custom_graphics::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint8_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

size_t custom_graphics::write(uint8_t c) {
if (_FontNumber < 5)
	{
		if (c == '\n') 
		{
			cursor_y += textsize*_CurrentFontheight;
			cursor_x  = 0;
		} else if (c == '\r') 
		{
			// Skip 
		} else 
		{
			drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
			cursor_x += textsize*(_CurrentFontWidth+1);
			if (wrap && (cursor_x > (_width - textsize*(_CurrentFontWidth+1)))) 
			{
				cursor_y += textsize*_CurrentFontheight;
				cursor_x = 0;
			}
		}
		
	}else if (_FontNumber == 5 || _FontNumber == 6)
	{
		uint8_t radius = 3;
		if (_FontNumber == 6) radius = 2;
		
		if (c == '\n') 
		{
			cursor_y += _CurrentFontheight;
			cursor_x  = 0;
		} else if (c == '\r') 
		{
			// Skip
		} else if (c == '.')
		{
			// draw a circle for decimal point skip a space.
			
			fillCircle(cursor_x+(_CurrentFontWidth/2), cursor_y + (_CurrentFontheight-6), radius, textcolor);
			cursor_x += (_CurrentFontWidth+1);
			if (wrap && (cursor_x  > (_width - (_CurrentFontWidth+1)))) 
			{
				cursor_y += _CurrentFontheight;
				cursor_x = 0;
			}
		}else 
		{
			drawCharNumFont(cursor_x, cursor_y, c, textcolor, textbgcolor);
			cursor_x += (_CurrentFontWidth+1);
			if (wrap && (cursor_x > (_width - (_CurrentFontWidth+1)))) 
			{
				cursor_y += _CurrentFontheight;
				cursor_x = 0;
			}
		}

	}
  return 1;

}

// Draw a character
void custom_graphics::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint8_t color, uint8_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + (_CurrentFontWidth+1) * size - 1) < 0) || // Clip left
     ((y + _CurrentFontheight * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<(_CurrentFontWidth+1); i++ ) {
    uint8_t line;
    if (i == _CurrentFontWidth)
    { 
      line = 0x0;
    }
    else 
    {
           	switch (_FontNumber) {
#ifdef UC1609_Font_One
				case 1: line = UC_Font_One[((c - _CurrentFontoffset) * _CurrentFontWidth) + i]; break;
#endif 
#ifdef UC1609_Font_Two
				case 2: line = UC_Font_Two[((c - _CurrentFontoffset) * _CurrentFontWidth) + i]; break;
#endif
#ifdef UC1609_Font_Three
				case 3: line = UC_Font_Three[((c - _CurrentFontoffset) * _CurrentFontWidth) + i]; break;
#endif
#ifdef UC1609_Font_Four
				case 4: line = UC_Font_Four[((c - _CurrentFontoffset) * _CurrentFontWidth) + i]; break;
#endif
				default: // wrong font number
						return;
				break;
				}
    }
    for (int8_t j = 0; j<_CurrentFontheight; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}

void custom_graphics::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void custom_graphics::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void custom_graphics::setTextColor(uint8_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void custom_graphics::setTextColor(uint8_t c, uint8_t b) {
  textcolor   = c;
  textbgcolor = b; 
}

void custom_graphics::setTextWrap(bool w) {
  wrap = w;
}

uint8_t custom_graphics::getRotation(void) const {
  return rotation;
}

void custom_graphics::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)
int16_t custom_graphics::width(void) const {
  return _width;
}
 
int16_t custom_graphics::height(void) const {
  return _height;
}

// Desc: writes a char (c) on the TFT
// Param 1 , 2 : coordinates (x, y).
// Param 3: The ASCII character
// Param 4: color 
// Param 5: background color
// Notes for font 5 & font 6 (bignums  + mednums)  only
void custom_graphics::drawCharNumFont(uint8_t x, uint8_t y, uint8_t c, uint8_t color , uint8_t bg) 
{

	uint8_t i, j;
	uint8_t ctemp = 0, y0 = y; 

	for (i = 0; i < (_CurrentFontheight*2); i++) 
	{
		if (_FontNumber == 5){
		#ifdef UC1609_Font_Five
			ctemp = UC_Font_Five[c - _CurrentFontoffset][i];
		#endif
		}
		else if (_FontNumber == 6){
		#ifdef UC1609_Font_Six
			ctemp = UC_Font_Six[c - _CurrentFontoffset][i];
		#endif
		}else{ 
			return;
		}
		
		for (j = 0; j < 8; j++) 
		{
			if (ctemp & 0x80) 
			{
				drawPixel(x, y, color);
			} else {
				drawPixel(x, y, bg);
			}

			ctemp <<= 1;
			y++;
			if ((y - y0) == _CurrentFontheight) {
				y = y0;
				x++;
				break;
			}
	}
	}
}

void custom_graphics::invertDisplay(bool i) {
  // Do nothing, must be subclassed if supported
}

// Desc: Writes text string (*ptext) on the TFT 
// Param 1 , 2 : coordinates (x, y).
// Param 3: pointer to string 
// Param 4: color 
// Param 5: background color
// Notes for font 5 & font 6 (bignums  + mednums)  only
void custom_graphics::drawTextNumFont(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg) 
{

	if (_FontNumber < 5)
	{
		return;
	}

	while (*pText != '\0') 
	{
		if (x > (_width - _CurrentFontWidth )) 
		{
			x = 0;
			y += _CurrentFontheight ;
			if (y > (_height - _CurrentFontheight)) 
			{
					y = x = 0;
			}
		}

		drawCharNumFont(x, y, *pText, color, bg);
		x += _CurrentFontWidth ;
		pText++;
	}
}

// Desc :  Set the font number
// Param1: fontnumber 1-5
// 1=default 2=thick 3=seven segment 4=wide 5=bignums 6 = mednums

void custom_graphics::setFontNum(uint8_t FontNumber)
{
	_FontNumber = FontNumber;
	
	enum LCD_Font_width
	{
		FONT_W_FIVE = 5, FONT_W_SEVEN = 7, FONT_W_FOUR = 4, FONT_W_EIGHT = 8,FONT_W_16= 16
	}; // width of the font in bytes cols.
	
	enum LCD_Font_offset
	{
		FONT_O_EXTEND = ERM19264_ASCII_OFFSET, FONT_O_SP = ERM19264_ASCII_OFFSET_SP, FONT_N_SP = ERM19264_ASCII_OFFSET_NUM
	}; // font offset in the ASCII table
	
	enum LCD_Font_height
	{
		FONT_H_8 = 8, FONT_H_16 = 16, FONT_H_32 = 32
	}; // width of the font in bits
	
	enum LCD_Font_width setfontwidth;
	enum LCD_Font_offset setoffset;
	enum LCD_Font_height setfontheight;
	
	switch (_FontNumber) {
		case 1:  // Norm default 5 by 8
			_CurrentFontWidth = (setfontwidth = FONT_W_FIVE);
			_CurrentFontoffset =  (setoffset = FONT_O_EXTEND);
			_CurrentFontheight = (setfontheight=FONT_H_8);
		break; 
		case 2: // Thick 7 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = (setfontwidth = FONT_W_SEVEN);
			_CurrentFontoffset =  (setoffset = FONT_O_SP);
			_CurrentFontheight = (setfontheight=FONT_H_8);
		break; 
		case 3:  // Seven segment 4 by 8
			_CurrentFontWidth = (setfontwidth = FONT_W_FOUR);
			_CurrentFontoffset =  (setoffset = FONT_O_SP);
			_CurrentFontheight = (setfontheight=FONT_H_8);
		break;
		case 4: // Wide  8 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = (setfontwidth = FONT_W_EIGHT);
			_CurrentFontoffset =  (setoffset = FONT_O_SP);
			_CurrentFontheight = (setfontheight=FONT_H_8);
		break; 
		case 5: // big nums 16 by 32 (NUMBERS + : only)
			_CurrentFontWidth = (setfontwidth = FONT_W_16);
			_CurrentFontoffset =  (setoffset = FONT_N_SP);
			_CurrentFontheight = (setfontheight=FONT_H_32);
		break; 
		case 6: // med nums 16 by 16 (NUMBERS + : only)
			_CurrentFontWidth = (setfontwidth = FONT_W_16);
			_CurrentFontoffset =  (setoffset = FONT_N_SP);
			_CurrentFontheight = (setfontheight=FONT_H_16);
		break; 
		default: // if wrong font num passed in,  set to default
			_CurrentFontWidth = (setfontwidth = FONT_W_FIVE);
			_CurrentFontoffset =  (setoffset = FONT_O_EXTEND);
			_CurrentFontheight = (setfontheight=FONT_H_8);
			_FontNumber = 1;
		break;
	}
	
}
