/*
* Project Name: TM1638plus_Model2 
* File: TM1638plus_Model2.h
* Description: TM1638plus_Model2.h header file STM32 library for Model 2 TM1638 module(16 KEY 16 pushbuttons).
* Author: Gavin Lyons.
* Created April 2021
* URL: https://github.com/gavinlyonsrepo/STM32_projects
*/


#ifndef TM1638PLUS_MODEL2_H
#define TM1638PLUS_MODEL2_H


#include "TM1638plus_common.h"


class TM1638plus_Model2  {

public:
    // Constructor Init the module
    // parameters 
    // 1. swap_nibbles = boolean default false, if true swaps nibbles on display byte
    TM1638plus_Model2(bool swap_nibbles= false);
    
    // Methods
    
    // Begin method , Call in setup
    void displayBegin(void);

    void reset(void);      // Reset / Clear module 

    // Sets the brightness level on a scale of brightness = 0 to 7.
    // 0 is not turned off, it's just the lowest brightness.
    // If user wishes to change the default brightness at start-up change.
    // The DEFAULT_BRIGHTNESS define in header file.
    void brightness(uint8_t brightness);

    // Function Desc:Read push buttons function 
    // Note there are two different read button functions See Also ReadKeyTwo
    // Returns: zero if no button pressed.  
    // Returns: a byte with decimal value of buttons 1-16 (1 for S1 2 for S2 ... 16 for S16 ).
   uint8_t ReadKey16(void);

    // Function Desc: Reads the push  buttons:  alternate function to ReadKeys16
    // Can detect multiple key presses , See notes section in readme regarding
    // Problems with seven segment display when pressing certain keys in combination
    // Returns: A 16 bit integer where each bit correpsond to a switch
    // Eg:
    // S1 = 0x0001
    // S16 = 0x8000
    // S16 + S1 together = 0x8001
    uint16_t ReadKey16Two(void);
    
    // Send seven segment value module
    // pass segment  0-7, byte of data corresponding to segments abcdefg(dp) 01234567.
    // a is 0 , dp is 7 , segmentValue is which segments are off or on for each digit. d7d6d5d54d3d2d1 
    // Have to call it 8 times in row to set all segments. 
    // to set all "a" on send (0x00,0xFF)
    // to set all segment "g" off (0x06,0X00)
    void DisplaySegments(uint8_t segment, uint8_t segmentValue);
    
    // Display a Hexadecimal number ,
    // Param 1 :: upper nibble integer 2^16
    // Param 2 :: lower nibble integer 2^16
    // Param 3 :: byte dots turn on decimal points
    // Param 4 :: bool leading zeros , true on , false off
    // Param 5 :: enum text alignment , left or right alignment
    // converts to string internally
    void DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower,  uint8_t dots, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
    
    // Display a decimal number ,
    // Param 1 :: 2^32 unsigned long
    // Param 2 :: byte for decimal point display switch's on decimal point for those positions
    // Param 3 :: leading zeros , true on , false off
    // Param 4 :: enum text alignment ,  left or right
    // converts to string internally
    void DisplayDecNum(unsigned long number,  uint8_t dots, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);

    // Display a string , takes a string and byte for decimal point display
    // Takes in string , converts it to ASCII using the font and masks for the decimal point
    // Then passes array of eight ASCII bytes to DisplayValues function
    void DisplayStr(const char* string, const  uint16_t dots = 0);
   
    // Takes in Array of 8 ASCII bytes , Called from DisplayStr 
    // Scans each ASCII byte converts to array of 8 segment bytes where each byte represents a segment
    // i.e. byte 0 represents a in segment and then each bit represents the a segment in each digit.
    // So for "00000005" is converted by DisplayStr to hex "3F 3F 3F 3F 3F 3F 3F 6D" where left is first digit.
    // this is converted to hex "FF FE FF FF FE FF 01 00"  Where left is first segment
    // So "a" segment is turned on for all bytes and "b" is on for all except last digit.
    // The bits are  mapping below abcdefg(dp) = 01234567 ! 
    // see for mapping of seven segment to digit https://en.wikipedia.org/wiki/Seven-segment_display
    void ASCIItoSegment(const uint8_t values[]);
    
    //Divides the display into two nibbles and displays a Decimal number in each.
    // Param 1 :: upper nibble integer 2^16
    // Param 2 :: lower nibble integer 2^16
    // Param 3 :: byte dots turn on decimal points
    // Param 4 :: bool leading zeros , true on , false off
    // Param 5 :: enum text alignment , left or right alignment
    void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
    
    
private:
       
        void sendCommand(uint8_t value);
        void sendData(uint8_t  data);
        bool _SWAP_NIBBLES= false;     
        TM1638plus_common  TM_common;
};
 
#endif
