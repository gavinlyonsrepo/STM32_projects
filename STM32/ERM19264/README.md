![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Overview
--------------------
* Name : STM32 ERM19264_UC1609C library

* Description : 	
1. STM32 library C++.   
2. Inverse, rotate and contrast control. 
3. ASCII text strings and character text display 
4. Graphics library included.
5. Sleep mode.
6. Bitmaps supported.
7. Hardware
9. 12 fonts.
10. Print class which can handle multiple data types 

* Author: Gavin Lyons

* IDE used: Stm32cubeIDE Version: 1.4.0 , C++

* IC used: STM32F303k8T6 Nucleo-64

Note
------------------

This library is a port of the arduino library at [LINK](https://github.com/gavinlyonsrepo/ERM19264_UC1609)
Full details there. The  examples files in examples folder correspond somewhat to the example files at the arduino repo examples folder. Simply replace the main.cpp with this file for the model user wants.
The default main.cpp is "MULTISCREEN_main.cpp"

Connections
------------------------------

Connections Hardware SPI

1. CLk PB3
2. Data MOSI PA7
3. CD PA8
4. CS PA10
5. Reset PA9

No other changes are required to the peripheral default setup of the Nucleo board by the IDE, besides configure the SPI and  GPIO. SPI settings are in main.cpp.
