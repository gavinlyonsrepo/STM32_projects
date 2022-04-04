Overview
--------------------------------------------
1. Name: ST7735
2. Description:  Library for TFT SPI LCD, ST7735 Driver, 
3. Author: Gavin Lyons
4. STM32: STM32F070RB nucleo-64 
5. IDE:  STM32cubeIDE
6.  Note: this is a fork of PIC project at [link](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

Features
----------------------

**TFT SPI LCD, ST7735 Driver, RED PCB v1.1, 1.44'', 128 x 128 pixels, No SD card**

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)
 
This library was only tested on this TFT variant /size.
This Color TFT LCD display has 128 x 128 resolution.
It uses SPI interface to communicate with controller. Onboard LDO, support 5V/3.3V input voltage, 
the LED backlight, 3.3V input. Size 1.44 inch, visual area 1.1 inch. Version 1.1. No SD Card. 
Backlight control is left to user.

**Screen Size settings**

In the setup() function in USER OPTION 1 Screen Setup. Select your PCB size and offsets. User can adjust pixel height, pixel width and screen offsets in the header file. When calling function TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_PIXEL_WIDTH  , TFT_PIXEL_HEIGHT).

**PCB Version**

There are 4 types of the ST7735 TFT display supported.
Only red tab has been tested here. 
It should work on other TFT displays using the different init functions, but not tested.
User picks the one they want when calling TFTInitPCBType() function
See USER OPTION 2 PCB_TYPE in Setup() function in main.c

| Number | Description | Enum label|
| ---- | ---- | --- | 
| 1 | ST7735B controller| TFT_ST7735B |
| 2 | ST7735R Green Tab | TFT_ST7735R_Green |
| 3 | ST7735R Red Tab   | TFT_ST7735R_Red |
| 4 | ST7735S Black Tab | TFT_ST7735S_Black |
 
**Fonts**

Five fonts are included.
USER OPTION 3 FONT  at start of font file to 
turn off unused fonts to save memory.

Five fonts available : 

| Font num | Font name | Pixel size |  Note |
| ------ | ------ | ------ | ------ |  
| 1 | Default | 5x8 |   ------ |
| 2 | Thick   | 7x8 |  no lowercase letters  |
| 3 | Seven segment | 4x8 | ------ |
| 4 | Wide | 8x8 | no lowercase letters |
| 5 | Tiny | 3x8 |  ------ |

**SPI**

This library supports both Hardware SPI and software SPI. Change the define a top of header file to switch between the two. USER OPTION 4 SPI TYPE. 

 ![s](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/images/spi.jpg)

**Files**

The Main.c contains  tests showing library functions
There is also an TFT library (ST7735_TFT.c and ST7735_TFT.h),
and a font files that contains ASCII pixel fonts.


**Bitmap**

There are two functions to support drawing bitmaps
both are bi-color a background and a foreground.
Full color bitmaps will take up a lot of memory on this device.
Note: The library was developed on a
TFT without built-in SD card feature.

1. Draw small custom icons X by 8 size where X is 0-128
useful to create icons or buttons or fill rows with patterns.
vertically addressed.

2. Draw bitmap, horizontally addressed.
 

**Connections**

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

Output
-----------------------

Output of some of the test routine's Left to right top to bottom

1. Different defined colors at default font size 1. Full 16 bit colour 565 available 
2. Different sizes of default font: 2,3,4 & 5 . Are fonts are scale-able
3. Different Fonts at font size 2, five fonts included .
4. Shapes
5. Shapes
6. Bitmap (bi-color) A background and a foreground. 

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/9.jpg)
