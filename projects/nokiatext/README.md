

Overview
--------------------
* Name : STM32 Library for Nokia 5110 LCD (PCD8544 controller) 

* Description : 	

1. STM32 library.      
2. Inverse, Bias and contrast control. 
3. ASCII strings and character text display.
4. Nine ASCII text fonts.
5. Sleep mode.

* Author: Gavin Lyons

* IDE used: STM32CubeIDE C : 1.4.0 , C 

* IC used: STM32F303K8T6 Nucleo-32  

Ports
------------------

This a port of these libraries, full details at link.

PIC X8 library [LINK](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/tree/master/projects/nokiatext)

Arduino library at [LINK](https://github.com/gavinlyonsrepo/NOKIA5110_TEXT)

Connections
------------------------------

Connections Software SPI 

1.	PB3	GPIO_Output	SWCLK = Nokia CLK
2.	PB4	GPIO_Output	SWDATA = NOKIA DIN
3.	PB5	GPIO_Output	DC = NOKIA DC
4.	PB6	GPIO_Output	CE = NOKIA CE
5.	PB7	GPIO_Output	Reset = NOKIA RST

No other changes are required to the peripheral default setup of the Nucleo board by the IDE
besides configure the GPIO. 

Pinout of a Nokia 5110 LCD.

![ pinout ](https://github.com/gavinlyonsrepo/NOKIA5110_TEXT/blob/master/extras/image/NOKIA_PINOUT.jpg)
 
