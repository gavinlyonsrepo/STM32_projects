/*!
	@file     TM1638plus.h
	@author   Gavin Lyons
	@brief    STm32 library Tm1638plus_PICO, Header file for TM1638 module(LED & KEY). Model 1 & Model 3.
*/

#ifndef TM1638PLUS_H
#define TM1638PLUS_H


#include "TM1638plus_common.h"

/*!
	@brief Class for Model 1 and Model 3
*/
class TM1638plus : public TM1638plus_common  {

public:
	// Constructor 
	TM1638plus();
	
	uint8_t readButtons(void);

	void displayText(const char *text);
	void displayASCII(uint8_t position, uint8_t ascii);
	void displayASCIIwDot(uint8_t position, uint8_t ascii) ;
	void displayHex(uint8_t position, uint8_t hex);
	void display7Seg(uint8_t position, uint8_t value);
	void displayIntNum(unsigned long number, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	
    void setLEDs(uint16_t greenred);
	void setLED(uint8_t position, uint8_t value);
	
private:

};


#endif
