![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

# STM32 TM1638plus 

Table of contents
---------------------------

  * [Overview](#overview)
  * [Examples](#examples)
  * [Hardware](#hardware)
  * [See Also](#see-also)

Overview
--------------------
* Name : STM32 TM1638plus 

* Description : 	

An STM32 library to display data on a 8-digit TM1638 seven segment module.

* Author: Gavin Lyons

* IDE used: Stm32cubeIDE Version: 1.4.0 , C++

* IC used: STM32F303k8T6 Nucleo-32

Examples
--------------------------------

The three examples files in examples folder correspond to  the model number ino files at the main arduino repo. Simply replace the main.cpp with this file for the model user wants.

|  List No | example file name  | Model Num | Desc|
| ------ | ------ |  ------ | ------ |
| 1 | examples/main_model1.cpp | 1 | test routines for model 1 |
| 2 | examples/main_model2.cpp | 2 | test routines for model 2 |
| 3 | examples//main_model3.cpp | 3 | test routines for model 3 |

Hardware
----------------------

1. PA12 - DATA (DIO)  -- pinname = TMDATA
2. PB0 - Clock (CLK)  -- pinname  = TMCLK
3. PB7 - Strobe (STB)  -- pinname = TMSTB

No other changes are required to the peripheral default setup of the Nucleo board by the IDE

This library supports three variants of the TM1638,
which for purposes of this documentation,
will be named Model 1 ,Model 2 and Model 3. 
Pictured at top of readme from left to right.

| Model No | Module Name | LEDS | Push buttons |
| ------ | ------ |  ------ | ------ |
| 1 | TM1638 LED & KEY | 8 red only | 8 |
| 2 | TM1638 KEYS, QYF  | 0 | 16 |
| 3 | TM1638 V1.3 or LKM1638  | 8 bi color,  red and green  | 8 |

See Also
-------------------------------------

This library is a port of my Arduino Library. There you will find the full documentation
including the API, which  is  near identical to STM32  version except ::
 
	* Extra examples files for Arduino and examples are .ino files.
	* Shift data methods have different parameters(used internally)
	* High freq data member flag is not needed in Stm32 library.
	* 3 GPIO are not passed as arguments into constructor or defined in library but in main.h
	* In the file dependency graphs of API,High level file includes will differ. #include "main.h" will be missing etc. 


[ Arduino github Link ](https://github.com/gavinlyonsrepo/TM1638plus)



 
