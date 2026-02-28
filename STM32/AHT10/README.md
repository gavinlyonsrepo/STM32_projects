Overview
--------------------------------------------
* Name: AHT10 
* Description: 
Library for AHT10 Integrated temperature and humidity sensor
* Author: Gavin Lyons
* STM32: STM32F303K8T6 Nucleo-32 
* IDE:  STM32cubeIDE C.

Features
----------------------

 ![o](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/extra/images/aht10.jpg)
 

This is an STM32 library for Aosong ASAIR AHT10, AHT15 & AHT20 Digital Humidity & Temperature Sensor I2C.

1. Supply voltage:               1.8v - 3.6v for AHT10, AHT15 & 2.0v - 5.5v for AHT20
2. Temperature range:            -40°C..+85°C
3. Temperature resolution:       0.01°C
4. Temperature accuracy:         ±0.3°C
5. Relative humidity range:      0%..100%
6. Relative humidity resolution: 0.024%
7. Relative humidity accuracy:   ±2%**
8. I²C bus speed:                0Hz - 400KHz
9. Recommended polling frequency: 8sec - 30sec

Supports sensors features:

1. Read humidity
2.  Read temperature
3. Soft reset with sensor initialisation
4. The library returns 255 if a communication error occurs or if the calibration coefficient is off.

**Connections**

The Sensor uses I2C for communication's. Data is outputted (eg to a PC) via a USART .
pins  used are in file AHT10_project.txt.

**Files**

The Main.c contains tests showing library functions
There is also an  library (AHT10.c and AHT10.h),

**Output**

Data is outputted (eg to a PC) via a USART .
 ![o1](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/extra/images/aht10output.jpg)

