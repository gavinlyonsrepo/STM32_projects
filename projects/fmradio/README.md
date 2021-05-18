Overview
--------------------
* Name : STM32_FMradio
* Title : Fm-radio

* Description : 	STM32F103C8T6 "blue pill" based FM radio using TEA5767 FM module.
Radio stations are selected via two push buttons. The Station frequency signal strength and Audio volume 
is displayed on a  NOKIA 5110 LCD. The volume is read from the control on an audio amplifier circuit. 
attached to audio output of the module. 

* Author: Gavin Lyons

Software used
-----------------------------
1. Arduino IDE  v. 1.8.10 , C++


TODO
--------------
1. Settings menu 
2. Store radio stations in EEPROM

Connections
------------------------------

1. PB12 Push button 1
2. PB14 Push Button 2 
3. PB15 Push button 3 (settings) TODO not used in V1.0
4. PA7 ADC volume in from audio amp
5. PA0 RST NOKIA 5110
6. PA1 CE NOKIA 5110
7. PA2 DC NOKIA 5110
8. PA3 DIN NOKIA 5110
9. PA4 CLK NOKIA 5110
10. PB7 I2C SData
11. PB6 I2C SCLK1

The Audio out of the  TEA5767 module is connected to an Audio amplifier and speaker.
 
 ![ Tea5767 ](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/images/tea5767.jpg)
 
 Output
 -----------------------
 
  ![ output ](https://github.com/gavinlyonsrepo/STM32_projects/blob/master/images/fmradio.jpg)
 
 


