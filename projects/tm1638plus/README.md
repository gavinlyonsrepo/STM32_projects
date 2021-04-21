![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

Overview
--------------------
* Name : STM32 TM1638plus 

* Description : 	
An STM32 library to display data on a 8-digit TM1638 seven segment module.
This library supports 3 different models, pictured above from left to right.

1. Model 1, The (8 KEY & 8 LED) variant which has 8 LED's and 8 Push buttons.
2. Model 2, The (QYF 16 KEY) variant which has 16 pushbuttons.
3. Model 3, The (LKM1638) variant which has 8 bi-colour LED's and 8 Push buttons.

* Author: Gavin Lyons

* IDE used: Stm32cubeIDE Version: 1.4.0 , C++

* IC used: STM32F303k8T6 Nucleo-32

Note
------------------

This library is a port of the arduino library at [LINK](https://github.com/gavinlyonsrepo/TM1638plus)
Full details there. The three examples files in examples folder correspond to  the model number ino files at the arduino repo. Simply replace the main.cpp with this file for the model user wants.

Connections
------------------------------

1. PA12 - DATA (DIO)  -- pinname = TMDATA
2. PB0 - Clock (CLK)  -- pinname  = TMCLK
3. PB7 - Strobe (STB)  -- pinname = TMSTB

No other changes are required to the peripheral default setup of the Nucleo board by the IDE
 
