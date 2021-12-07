Overview
--------------------------------------------
* Name: LM75A
* Description: 
Library for LM75A  temperature sensor
* Author: Gavin Lyons
* STM32: STM32F303K8T6 Nucleo-32 
* IDE:  STM32cubeIDE C ++.
* Credits: ported from an [Arduino library](https://github.com/m2m-solutions/M2M_LM75A) to STM32. 

Features
----------------------
 ![mod](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/images/lm75a.jpg)
 
This is an STM32 library for TI LM75A Digital Temperature Sensor and Thermal Watchdog With Two-Wire Interface Temperature Sensor I2C.

1. Shutdown Mode to Minimize Power Consumption
2. Up to Eight LM75As can be Connected to a Single Bus
3. Power up Defaults Permit Stand-Alone Operation
4. Supply Voltage
	/* LM75A: 2.7 V to 5.5 V
5. Supply Current
	* Operating: 280 μA (Typical)
	* Shutdown: 4 μA (Typical)
6. Temperature Accuracy
	*  25°C to 100°C: ±2°C (Max)
	*  55°C to 125°C: ±3°C (Max)
7. I2C 2 wire 400Khz

The LM75A provides 9-bit digital temperature
readings . The LM75A has a dedicated over-temperature output
(O.S.) with programmable limit and hysteresis. This
output has programmable fault tolerance, which lets
the user to define the number of consecutive error
conditions that must occur before O.S. is activated.
Shutdown low current mode included. 

Power up mode:
 - Comparator mode
 - Tos = 80 °C
 - Thyst = 75 °C
 - OS active low

Pin 3 is the OS (overtemperature shutdown). It can be used to to trigger a pin interrupt.
Set the functional mode, Tos and Thyst: if that functionally desired.

**Connections**

The Sensor uses I2C for communication's, Data is outputted (eg to a PC) via a USART .
pins  used are in file LM75A_project.txt.

**Files**

The Main.c contains tests showing library functions
There is also an  library (LM75A.c and LM75A.h),

**Output**

Data is outputted (eg to a PC) via a USART
Switch between by changing define statement in main.c 
Can also be toggled between Celsius and Fahrenheit. 
 
1. Basic Usage
2. Library test  routine

![mod1](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/images/lm75aoutput.jpg)

**Datasheet**

LM75A data sheets.
 - [Texas Instruments data sheet](http://www.ti.com/lit/ds/symlink/lm75a.pdf)
