
![ lcd ](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/LCDPCF.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Example files](#example-files)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Output](#output)
  
Overview
--------------------
* Name : HD44780_LCD_PCF8574_PICO

* Description :

0. Library to support the HD44780 LCD , (I2C PCF8574 "backpack" interface) 
1. STM32 library C. 
2. Backlight, scroll, cursor and entry-mode control.
3. Custom character support + print class for numerical data.
4. Hardware I2C.
5. Tested on size 16x02 + 20x04 (but may work on other sizes  eg 16X4 but not tested).
6. Port of my Arduino library at [link](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574).

* Toolchain
	1. STM32CubeIDE Version: 1.10.1
	2. HAL, C, compiler gcc GNU 11
	3. IC used: STM32F303K8T6 Nucleo-32  
	
* Author: Gavin Lyons

Example files
-------------------------

The example files are in example folder. BY default the main is set to 16x02. 
Swap out the file for 20x04 LCD.

1. examples/16X02main.cpp  Test sequence for 16x02 LCD.
2. examples/20X04main.cpp Test sequence for 20x04 LCD.

Hardware
--------------------

I2C pins used are 

1. I2C_1_SDA = PB7
2. I2C_1_SCL = PB6


Software
-------------------
A print method is included using vsprint to print numerical  data. In order to use floating point numbers you need to add 
-u _printf_float to project properties , C build , settings , MCU Gcc Linker , misc , other flags section. 

For detailed graphical description of entry modes , cursor types, custom characters 
and more see here [link](http://dinceraydin.com/lcd/commands.htm)

Output
---------------------

Output  of custom character test in testrun example  file on 16x02 display.

![ pic ](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/main/extras/image/custom_output.jpg)

20x04 display. 

![ pic2 ](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574/blob/main/extras/image/2004.jpg)


