/*
* Project Name: ERM19264_UC1609
* File: ERM19264_UC1609.cpp
* Description: ERM19264 LCD driven by UC1609C controller source file
* Author: Gavin Lyons.
*/

#include "ERM19264_UC1609.h"
#include "main.h"

// Class Constructors // 2 off
// No buffer or buffer, bool for SPI type true for hw , false for sw

#ifdef NO_BUFFER

ERM19264_UC1609  :: ERM19264_UC1609(bool SPIHardware)
{
  _SPI_Hardware = SPIHardware;
}

#else

ERM19264_UC1609  :: ERM19264_UC1609(bool SPIHardware) :custom_graphics(LCD_WIDTH, LCD_HEIGHT)
{
	_SPI_Hardware = SPIHardware;

}

#endif


// Desc: begin Method initialise LCD 
// SPI setup
// Param1: VBiasPOT default = 0x49 , range 0x00 to 0xFE
// Param2: pointer to SPIhandle for SPI port one
void ERM19264_UC1609::LCDbegin (uint8_t VbiasPOT,SPI_HandleTypeDef  * hspi1)
{
  if(isHardwareSPI()) _hspi1 = hspi1 ;
 _VbiasPOT  = VbiasPOT;
  LCDinit();
}


// Desc: Called from LCDbegin carries out Power on sequence and register init
// Can be used to reset LCD to default values.
void ERM19264_UC1609::LCDinit()
 {
  HAL_Delay(UC1609_INIT_DELAY2); //3mS delay, datasheet
  UC1609_CD_SetHigh;
  UC1609_CS_SetHigh;
  LCDReset();

  UC1609_CS_SetLow;

  send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET); 
  send_command(UC1609_ADDRESS_CONTROL, UC1609_ADDRESS_SET); 
  send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
  send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);  
  send_command(UC1609_POWER_CONTROL,  UC1609_PC_SET); 
  HAL_Delay(UC1609_INIT_DELAY);
  
  send_command(UC1609_GN_PM, 0);
  send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user
  
  send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
  send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal 
  
  UC1609_CS_SetHigh;
}


// Desc: Sends a command to the display
// Param1: the command
// Param2: the values to change
void ERM19264_UC1609::send_command (uint8_t command, uint8_t value) 
{
  UC1609_CD_SetLow; 
  send_data(command | value);
  UC1609_CD_SetHigh;
}

// Desc: Resets LCD in a four wire setup called at start 
// and  should also be called in a controlled power down setting
void ERM19264_UC1609::LCDReset () 
{
  UC1609_RST_SetLow;
  HAL_Delay(UC1609_RESET_DELAY);
  UC1609_RST_SetHigh;
  HAL_Delay(UC1609_RESET_DELAY2);
}

// Desc: Powerdown procedure for LCD see datasheet P40
void ERM19264_UC1609::LCDPowerDown(void)
{
  LCDReset ();
  LCDEnable(0);
}


// Desc: turns on display
// Param1: bits 1  on , 0 off
void ERM19264_UC1609::LCDEnable (uint8_t bits) 
{
 UC1609_CS_SetLow;
  send_command(UC1609_DISPLAY_ON, bits);
 UC1609_CS_SetHigh;
}


// Desc: Scroll the displayed image up by SL rows. 
//The valid SL value is between 0 (for no
//scrolling) and (64). 
//Setting SL outside of this range causes undefined effect on the displayed
//image.
// Param1: bits 0-64 line number y-axis
void ERM19264_UC1609::LCDscroll (uint8_t bits) 
{
 UC1609_CS_SetLow;
  send_command(UC1609_SCROLL, bits);
 UC1609_CS_SetHigh;
}

// Desc: Rotates the display 
// Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
// Param1: 4 possible values 000 010 100 110 (defined)
// If Mx is changed the buffer must BE updated
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue) 
{
 UC1609_CS_SetLow;
  switch (rotatevalue)
  {
      case 0: rotatevalue = 0; break;
      case 0x02: rotatevalue = UC1609_ROTATION_FLIP_ONE; break;
      case 0x04: rotatevalue = UC1609_ROTATION_NORMAL; break;
      case 0x06: rotatevalue = UC1609_ROTATION_FLIP_TWO; break;
      default: rotatevalue = UC1609_ROTATION_NORMAL; break;
  }
  send_command(UC1609_LCD_CONTROL, rotatevalue);
 UC1609_CS_SetHigh;
}

// Desc: invert the display
// Param1: bits, 1 invert , 0 normal
void ERM19264_UC1609::invertDisplay (uint8_t bits) 
{
 UC1609_CS_SetLow;
  send_command(UC1609_INVERSE_DISPLAY, bits);
 UC1609_CS_SetHigh;
}

// Desc: turns on all Pixels
// Param1: bits Set DC[1] to force all SEG drivers to output ON signals
// 1 all on ,  0 all off
void ERM19264_UC1609::LCD_allpixelsOn(uint8_t bits) 
{
 UC1609_CS_SetLow;
  send_command(UC1609_ALL_PIXEL_ON, bits);
 UC1609_CS_SetHigh;
}

// Desc: Fill the screen NOT the buffer with a datapattern 
// Param1: datapattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
// Param2: optional delay in microseconds can be set to zero normally.
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern=0, uint8_t delay=0) 
{
 UC1609_CS_SetLow;
  uint16_t numofbytes = LCD_WIDTH * (LCD_HEIGHT /8); // width * height
  for (uint16_t i = 0; i < numofbytes; i++) 
  {
    send_data(dataPattern);
    //delayMicroseconds(delay); //TODO
  }
UC1609_CS_SetHigh;
}

// Desc: Fill the chosen page(1-8)  with a datapattern 
// Param1: datapattern can be set to 0 to FF (not buffer)
void ERM19264_UC1609::LCDFillPage(uint8_t dataPattern=0) 
{
 UC1609_CS_SetLow;
  uint16_t numofbytes = ((LCD_WIDTH * (LCD_HEIGHT /8))/8); // (width * height/8)/8 = 192 bytes
  for (uint16_t i = 0; i < numofbytes; i++) 
  {
      send_data(dataPattern);
  }
 UC1609_CS_SetHigh;
}

//Desc: Draw a bitmap in PROGMEM to the screen
//Param1: x offset 0-192
//Param2: y offset 0-64
//Param3: width 0-192
//Param4 height 0-64
//Param5 the bitmap
void ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data) 
{
 UC1609_CS_SetLow;

  uint8_t tx, ty; 
  uint16_t offset = 0; 
  uint8_t column = (x < 0) ? 0 : x;
  uint8_t page = (y < 0) ? 0 : y >>3;

  for (ty = 0; ty < h; ty = ty + 8) 
  {
        if (y + ty < 0 || y + ty >= LCD_HEIGHT) {continue;}
        send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
        send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
        send_command(UC1609_SET_PAGEADD, page++); 

        for (tx = 0; tx < w; tx++) 
        {
              if (x + tx < 0 || x + tx >= LCD_WIDTH) {continue;}
              offset = (w * (ty >> 3)) + tx; 
              send_data(data[offset]);
        }
  }
UC1609_CS_SetHigh;
}

// Desc: Checks if software SPI is on
// Returns: true 1 if hardware SPi on , false 0 for software spi
bool ERM19264_UC1609::isHardwareSPI() {
  return (_SPI_Hardware);
}

// Desc: used in software SPI mode to shift out data
// Param1: the byte to go out
void ERM19264_UC1609::CustomshiftOut(uint8_t value)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
    	if (!!(value & (1 << (7 - i))))
			HAL_GPIO_WritePin(GPIOA, SWMOSI_Pin,GPIO_PIN_SET);
    	else
    		HAL_GPIO_WritePin(GPIOA, SWMOSI_Pin,GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOA, SWSPICLK_Pin,GPIO_PIN_SET);
        //delayMicroseconds(UC1609_HIGHFREQ_DELAY);
    	HAL_GPIO_WritePin(GPIOA, SWSPICLK_Pin,GPIO_PIN_RESET);
        //delayMicroseconds(UC1609_HIGHFREQ_DELAY);
    }
}

//Desc: Send data byte with SPI to UC1609C
//Param1: the data byte
void ERM19264_UC1609::send_data(uint8_t data)
{
  if (isHardwareSPI())
  {
	HAL_SPI_Transmit(_hspi1, &data, 1, 100);
  }else
  {
    CustomshiftOut(data); //Software SPI
  }
} 
 
// ******************************************************
// Functions below not & needed for no_buffer mode 
#ifndef NO_BUFFER

//Desc: updates the buffer i.e. writes it to the screen
void ERM19264_UC1609::LCDupdate() 
{

#ifdef MULTI_BUFFER
      LCDBuffer( this->ActiveBuffer->xoffset, this->ActiveBuffer->yoffset, this->ActiveBuffer->width, this->ActiveBuffer->height, (uint8_t*) this->ActiveBuffer->screenbitmap); 
      return;
#endif

#ifdef SINGLE_BUFFER
  uint8_t x = 0; uint8_t y = 0; uint8_t w = this->bufferWidth; uint8_t h = this->bufferHeight;
  LCDBuffer( x,  y,  w,  h, (uint8_t*) this->buffer);
#endif
}

//Desc: clears the buffer i.e. does NOT write to the screen
void ERM19264_UC1609::LCDclearBuffer()
{
#ifdef MULTI_BUFFER
   memset( this->ActiveBuffer->screenbitmap, 0x00, (this->ActiveBuffer->width * (this->ActiveBuffer->height/ 8))  ); 
   return;
#endif

#ifdef SINGLE_BUFFER
  memset( this->buffer, 0x00, (this->bufferWidth * (this->bufferHeight /8))  ); 
#endif
}

//Desc: Draw a bitmap to the screen
//Param1: x offset 0-192
//Param2: y offset 0-64
//Param3: width 0-192
//Param4 height 0-64
//Param5 the bitmap
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data) 
{
 UC1609_CS_SetLow;

  uint8_t tx, ty; 
  uint16_t offset = 0; 
  uint8_t column = (x < 0) ? 0 : x;
  uint8_t page = (y < 0) ? 0 : y/8;

  for (ty = 0; ty < h; ty = ty + 8) 
  {
    if (y + ty < 0 || y + ty >= LCD_HEIGHT) {continue;}
    
    send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
    send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4); 
    send_command(UC1609_SET_PAGEADD, page++); 
 
    for (tx = 0; tx < w; tx++) 
    {
          if (x + tx < 0 || x + tx >= LCD_WIDTH) {continue;}
          offset = (w * (ty /8)) + tx; 
          send_data(data[offset++]);
    }
  }
  
UC1609_CS_SetHigh;
}

// Desc: Draws a Pixel to the screen overides the custom graphics library
// Passed x and y co-ords and colour of pixel.
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint8_t colour) 
{
    
#ifdef MULTI_BUFFER
  if ((x < 0) || (x >= this->ActiveBuffer->width) || (y < 0) || (y >= this->ActiveBuffer->height)) {
    return;
  }
      uint16_t offset = (this->ActiveBuffer->width * (y/8)) + x; 
      switch (colour)
      {
        case FOREGROUND: this->ActiveBuffer->screenbitmap[offset] |= (1 << (y & 7)); break;
        case BACKGROUND: this->ActiveBuffer->screenbitmap[offset] &= ~(1 << (y & 7)); break;
        case INVERSE: this->ActiveBuffer->screenbitmap[offset] ^= (1 << (y & 7)); break;
      }
    return;
#endif
    
#ifdef SINGLE_BUFFER
  if ((x < 0) || (x >= this->bufferWidth) || (y < 0) || (y >= this->bufferHeight)) {
    return;
  }
      uint16_t tc = (bufferWidth * (y /8)) + x; 
      switch (colour)
      {
        case FOREGROUND:  this->buffer[tc] |= (1 << (y & 7)); break;
        case BACKGROUND:  this->buffer[tc] &= ~(1 << (y & 7)); break;
        case INVERSE: this->buffer[tc] ^= (1 << (y & 7)); break;
      }
#endif
}

// ***************************************************
#else 
//  these functions are not needed by buffers modes
// and only used in "no buffer" mode
// ***********************************************

// Desc: goes to XY position
// Param1 : coloumn 0-192
// Param2  : page 0-7
void ERM19264_UC1609::LCDNoBufferGotoXY(uint8_t column , uint8_t page)
{
        UC1609_CS_SetLow;
        send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
        send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
        send_command(UC1609_SET_PAGEADD, page++); 
        UC1609_CS_SetHigh;
}

// Desc: draws passed character.
// Param1: character 'A' or number in  the ASCII table 1-127(default)
void ERM19264_UC1609::LCDNoBufferChar(unsigned char  character)
{
   UC1609_CS_SetLow;
   UC1609_FONTPADDING;

   for (uint8_t column = 0 ; column <  UC1609_FONTWIDTH ; column++)
    {
	   send_data((uint8_t)(UC_Font_One[(character*UC1609_FONTWIDTH) + column]));
    }
    UC1609_FONTPADDING;
    UC1609_CS_SetHigh;
}

// Desc: draws passed  character array
// Param1: pointer to start of character array
void ERM19264_UC1609::LCDNoBufferString(const unsigned char *characters)
{
    while (*characters)
    LCDNoBufferChar(*characters++);
}
#endif
//***********************************************
