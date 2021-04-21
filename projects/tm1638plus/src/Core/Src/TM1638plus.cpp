/*
* Project Name: TM1638 
* File: TM1638plus.cpp
* Description: source file arduino  library for TM1638 module(LED & KEY). Model 1 & Model 3
* Author: Gavin Lyons.
* Created May 2019
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include "TM1638plus.h"


TM1638plus::TM1638plus() {
}

void TM1638plus::displayBegin() {
  
  sendCommand(TM_ACTIVATE);
  brightness(TM_DEFAULT_BRIGHTNESS);
  reset();
}


void TM1638plus::sendCommand(uint8_t value)
{
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(value);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}

void TM1638plus::sendData(uint8_t data)
{
    TM_common.HighFreqshiftOut(data);
}

void TM1638plus::reset() {
  sendCommand(TM_WRITE_INC); // set auto increment mode
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(TM_SEG_ADR);  // set starting address to
  for (uint8_t i = 0; i < 16; i++)
  {
    sendData(0x00);
  }
   HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}

void TM1638plus::setLED(uint8_t position, uint8_t value)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = TMDATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TMDATA_GPIO_Port, &GPIO_InitStruct);
  sendCommand(TM_WRITE_LOC);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(TM_LEDS_ADR + (position << 1));
  sendData(value);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}

void TM1638plus::setLEDs(uint16_t ledvalues)
{
  for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
    uint8_t colour = 0;

    if ((ledvalues & (1 << LEDposition)) != 0) {
      colour |= TM_RED_LED; //scan lower byte, set Red if one
    } 

    if ((ledvalues & (1 << (LEDposition + 8))) != 0) {
      colour |= TM_GREEN_LED; //scan upper byte, set green if one
    }

    setLED(LEDposition, colour);
  }
}


void TM1638plus::displayIntNum(unsigned long number, bool leadingZeros)
{
  char values[TM_DISPLAY_SIZE + 1];
  snprintf(values, TM_DISPLAY_SIZE + 1, leadingZeros ? "%08ld" : "%ld", number); 
  displayText(values);
}


void TM1638plus::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, bool leadingZeros)
{
  char valuesUpper[TM_DISPLAY_SIZE + 1];
  char valuesLower[TM_DISPLAY_SIZE/2 + 1];
  snprintf(valuesUpper, TM_DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : "%d", numberUpper);
  snprintf(valuesLower, TM_DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : "%d", numberLower); 
  strcat(valuesUpper ,valuesLower);
  displayText(valuesUpper);
}

void TM1638plus::displayText(const char *text) {
  char c, pos;
  pos = 0;
	  while ((c = (*text++)) && pos < TM_DISPLAY_SIZE)  {
		if (*text == '.' && c != '.') {
		  displayASCIIwDot(pos++, c);

		  text++;
		}  else {
		  displayASCII(pos++, c);
		}
	  }
}


void TM1638plus::displayASCIIwDot(uint8_t position, uint8_t ascii) { 
    // add 128 or 0x080 0b1000000 to turn on decimal point/dot in seven seg
  display7Seg(position, SevenSeg[ascii- TM_ASCII_OFFSET] + TM_DOT_MASK_DEC);
}

void TM1638plus::display7Seg(uint8_t position, uint8_t value) { // call 7-segment
  sendCommand(TM_WRITE_LOC);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(TM_SEG_ADR + (position << 1));
  sendData(value);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
}


void TM1638plus::displayASCII(uint8_t position, uint8_t ascii) {
  display7Seg(position, SevenSeg[ascii - TM_ASCII_OFFSET]);
}
 
void TM1638plus::displayHex(uint8_t position, uint8_t hex) 
{
    uint8_t offset = 0;
    if (hex <= 9)
    {
        display7Seg(position, SevenSeg[hex + TM_HEX_OFFSET]);
        // 16 is offset in reduced ASCII table for 0 
    }else if ((hex >= 10) && (hex <=15))
    {
        // Calculate offset in reduced ASCII table for AbCDeF
        switch(hex) 
        {
         case 10: offset = 'A'; break;
         case 11: offset = 'b'; break;
         case 12: offset = 'C'; break;
         case 13: offset = 'd'; break;
         case 14: offset = 'E'; break;
         case 15: offset = 'F'; break;
        }
        display7Seg(position, SevenSeg[offset-TM_ASCII_OFFSET]);
    }
    
}


uint8_t TM1638plus::readButtons()
{
  uint8_t buttons = 0;
  uint8_t v =0;
  
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_RESET);
  sendData(TM_BUTTONS_MODE);
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = TMDATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TMDATA_GPIO_Port, &GPIO_InitStruct); 

  for (uint8_t i = 0; i < 4; i++)
  {
    v = TM_common.HighFreqshiftin() << i;
    buttons |= v;
  }

  GPIO_InitStruct.Pin = TMDATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TMDATA_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB,TMSTB_Pin,GPIO_PIN_SET);
  return buttons;
}


void TM1638plus::brightness(uint8_t brightness)
{
    uint8_t  value = 0;
    value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
    sendCommand(value);
}
