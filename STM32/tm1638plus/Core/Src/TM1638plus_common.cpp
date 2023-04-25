/*!
	@file     TM1638plus_common.cpp
	@author   Gavin Lyons
	@brief     cpp  file for common data and functions between model 1 and 2 classes
*/

#include "TM1638plus_common.h"

/*!
	@brief Constructor for class TM1638plus_common
*/
TM1638plus_common::TM1638plus_common()
{ // blank constructor
}

/*!
	@brief Begin method , sets pin modes and activate display.
*/
void TM1638plus_common::displayBegin() {

  sendCommand(TM_ACTIVATE);
  brightness(TM_DEFAULT_BRIGHTNESS);
  reset();
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(value);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}

/*!
	@brief Send Data to display
	@param data  Data byte to send
*/
void TM1638plus_common::sendData(uint8_t data)
{
    HighFreqshiftOut(data);
}

/*!
	@brief Reset / clear  the  display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset() {
  sendCommand(TM_WRITE_INC); // set auto increment mode
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(TM_SEG_ADR);  // set starting address to
  for (uint8_t i = 0; i < 16; i++)
  {
    sendData(0x00);
  }
   HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}

/*!
	@brief  Sets the brightness level of segments in display on a scale of brightness
	@param brightness byte with value 0 to 7 The DEFAULT_BRIGHTNESS = 0x02
*/
void TM1638plus_common::brightness(uint8_t brightness)
{
    uint8_t  value = 0;
    value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
    sendCommand(value);
}

/*!
	@brief    Shifts in a byte of data from the Tm1638 SPI-like bus
	@return  Data byte
*/
uint8_t  TM1638plus_common::HighFreqshiftin() 
{
    uint8_t value = 0;
    uint8_t i = 0;

    for(i = 0; i < 8; ++i) 
    {
        value |= HAL_GPIO_ReadPin(GPIOA, TMDATA_Pin) << i;
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_RESET);
    }
    return value;
}


/*!
	@brief   Shifts out a byte of data on to the Tm1638 SPI-like bus
	@param val The byte of data to shift out
*/
void TM1638plus_common::HighFreqshiftOut(uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
    	if (!!(val & (1 << i)))
			HAL_GPIO_WritePin(GPIOA, TMDATA_Pin,GPIO_PIN_SET) ;
    	else
    		HAL_GPIO_WritePin(GPIOA, TMDATA_Pin,GPIO_PIN_RESET);

        HAL_GPIO_WritePin(GPIOB, TMCLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_RESET);
    }
}

// font , map of ASCII values/table to 7-segment, offset to position 32.
const  unsigned char SevenSeg[] = {
  0x00, /* (space) */
  0x86, /* ! */
  0x22, /* " */
  0x7E, /* # */
  0x6D, /* $ */
  0xD2, /* % */
  0x46, /* & */
  0x20, /* ' */
  0x29, /* ( */
  0x0B, /* ) */
  0x21, /* * */
  0x70, /* + */
  0x10, /* , */
  0x40, /* - */
  0x80, /* . */
  0x52, /* / */
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x09, /* : */
  0x0D, /* ; */
  0x61, /* < */
  0x48, /* = */
  0x43, /* > */
  0xD3, /* ? */
  0x5F, /* @ */
  0x77, /* A */
  0x7C, /* B */
  0x39, /* C */
  0x5E, /* D */
  0x79, /* E */
  0x71, /* F */
  0x3D, /* G */
  0x76, /* H */
  0x30, /* I */
  0x1E, /* J */
  0x75, /* K */
  0x38, /* L */
  0x15, /* M */
  0x37, /* N */
  0x3F, /* O */
  0x73, /* P */
  0x6B, /* Q */
  0x33, /* R */
  0x6D, /* S */
  0x78, /* T */
  0x3E, /* U */
  0x3E, /* V */
  0x2A, /* W */
  0x76, /* X */
  0x6E, /* Y */
  0x5B, /* Z */
  0x39, /* [ */
  0x64, /* \ */
  0x0F, /* ] */
  0x23, /* ^ */
  0x08, /* _ */
  0x02, /* ` */
  0x5F, /* a */
  0x7C, /* b */
  0x58, /* c */
  0x5E, /* d */
  0x7B, /* e */
  0x71, /* f */
  0x6F, /* g */
  0x74, /* h */
  0x10, /* i */
  0x0C, /* j */
  0x75, /* k */
  0x30, /* l */
  0x14, /* m */
  0x54, /* n */
  0x5C, /* o */
  0x73, /* p */
  0x67, /* q */
  0x50, /* r */
  0x6D, /* s */
  0x78, /* t */
  0x1C, /* u */
  0x1C, /* v */
  0x14, /* w */
  0x76, /* x */
  0x6E, /* y */
  0x5B, /* z */
};

const uint8_t * pFontSevenSegptr = SevenSeg; /**<  Pointer to the seven segment font data table */
