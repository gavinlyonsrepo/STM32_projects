
[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

# ST7735_TFT


![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Overview

1. Name: ST7735
2. Description:  Library for TFT SPI LCD, ST7735 Driver.
3. Author: Gavin Lyons
4. STM32: STM32F070RB nucleo-64 
5. Toolchain:  STM32cubeIDE, HAL, C.
6.  Note: this is a fork of PIC project at [link](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

## Software
 
### Tested
 
These two are the only type of  ST7735 library  tested on,
but should work on other types in theory.

1. TFT SPI LCD, ST7735 Driver, Red PCB v1.1, 1.44 , 128 x 128 pixels, "ST7735R Red Tab" 
2. TFT SPI LCD, ST7735 Driver, Red PCB v1.2, 1.8 , 128 x 160 pixels, "ST7735S Black Tab" 

No support for built-in SD card holder (that some modules have) at present and backlight control is left to user.

### Screen Size settings

In the setup() function in USER OPTION 1 Screen Setup. Select your PCB size and offsets. User can adjust pixel height, pixel width and screen offsets in the header file. When calling function TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_PIXEL_WIDTH  , TFT_PIXEL_HEIGHT).

### PCB Version

There are 4 types of the ST7735 TFT display supported..
User picks the one they want when calling TFTInitPCBType() function
See USER OPTION 2 PCB_TYPE in Setup() function in main.c

| Number | Description | Enum label|
| ---- | ---- | --- | 
| 1 | ST7735B controller| TFT_ST7735B |
| 2 | ST7735R Green Tab | TFT_ST7735R_Green |
| 3 | ST7735R Red Tab   | TFT_ST7735R_Red |
| 4 | ST7735S Black Tab | TFT_ST7735S_Black |
 
### Fonts

12 fonts are included.
Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Size Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0x20-0x7E, |470|Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|
| 11 | $_Mia| 8x16 | 0x20-0x7E |1520|N|
| 12 | $_Dedica | 6x12 |0x20-0x7E |1152|N|

1. $ = TFTFont
2. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
3. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
All  fonts are optional and can be removed from program
by commenting out the relevant TFT_Font define in the  ST7735_TFT_Font.h file
Turn off unused fonts to save memory.

Font Methods:

| Font num | Method | Size parameter | Notes |
| ------ | ------ | ------ |  ------ |
| 1-6 | drawChar|Y| draws single  character |
| 1-6 | drawText |Y| draws character array |
| 7-12 | drawChar2|N| draws single  character |
| 7-12 | drawText2|N| draws character array |
| 1-12 | TFTprint |~| Polymorphic, prints many types of data using vsprint |

The  TFTprint method  included uses vsprint to print numerical  data. In order to use floating point numbers you need to add  -u _printf_float to project properties , C build , settings , MCU Gcc Linker , misc , other flags section. 

These  functions return a number in event of an error, such as wrong font chosen , ASCII character outside chosen fonts range, character out of screen bounds and invalid character array pointer object. 

### SPI

This library supports both Hardware SPI and software SPI. Change the define a top of header file to switch between the two. USER OPTION 4 SPI TYPE. 

 ![s](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/extra/images/spi.jpg)

### Files

The Main.c contains  tests showing library functions
There is an TFT library (ST7735_TFT.c and ST7735_TFT.h),
There are font files that contains ASCII pixel fonts.
There are bitmap data files that contain the bitmap test data used by the main.c to test functions

### fill Rectangles functions

| Num | Function Name |  Note |
| ------ | ------ | ------ | 
| 1 | TFTfillRectangle | --- |
| 2 | TFTfillRect | uses TFTdrawFastVLine |
| 3 | TFTfillRectangleBuffer | Uses buffered writes , ~5 times faster than #1 & #2 |
| 4 | TFTfillScreen |  Wraps #1  |
| 5 | TFTfillScreenBuffer | Wraps #3 |

### Bitmap

There are 4 functions to support drawing bitmaps.
Full color bitmaps will take up a lot of memory on this device.
In the bitmap data file you can switch off the tests in USER OPTION 5.

| Num | Function Name | Colour support | bitmap size Max |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | Icon | bi-colour | (8 x (0-Max_y))   | useful to create icons  or fill rows with patterns ,Data vertically addressed |
| 2 | Bitmap | bi-colour | 2048 bytes  | Data horizontally addressed |
| 3 | BitmapBuffer | bi-colour   | 2048 bytes  |  Data horizontally  addressed  , uses buffered writes , 25 times faster than #2 |
| 4 | Bitmap1624Buffer  | 16 bit or 24 bit color  | 32768 or 49152  | Data from Array  24 bit Converted by software to 16-bit color, uses buffered writes  |

1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 2-4  assumes 128x128 pixel screen.
3. The data array for 1 -3 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 4 is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

 
## Hardware

| TFT PinNum | Pindesc | STM32 SW SPI | STM32 HW SPI |
| --- | --- | --- | --- | 
| 1 | LED | VCC |  VCC |
| 2 | SCLK | PA1 | PB3	SPI1_SCK	 |
| 3 | SDA | PA0 | PA7	SPI1_MOSI	 |
| 4 | A0/DC | PC1| PC1 |
| 5 | RESET | PC0  | PC0 | 
| 6 | SS/CS | PC2 | PC2 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC  |

1. NOTE connect LED backlight pin 1  thru a 150R to 220R ohm resistor to 3.3V or 5V VCC.
2. This is a 3.3V logic device do NOT connect  the I/O logic lines to 5V. 
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.

## Output

Output of some of the test routine's on a 128x128 TFT.

1. Different defined colors at default font size 1. Full 16 bit colour 565 available 
2. Different sizes of default font: 2,3,4 & 5 . fonts 1-6 are scale-able
3. Different Fonts at font size 2, Fonts 1-5 . font 6-12 not shown here.
4. Shapes.
5. MORE shapes!
6. Bitmap (bi-color) A background and a foreground. 
7. 16 bit color bitmap 128x128 image included in test data
8. 24 bit color bitmap 64x64 image included in test data

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/9.jpg)
![p1](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/extra/images/16pic1.bmp)
![p2](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/extra/images/24pic2.bmp)

## Notes

Timer16 and Usart are included just for the Frames per second test, TestFPS.
The FPS(frame per second) test is enabled by a boolean variable flag in main.c
