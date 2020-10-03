//******************* HEADER **************
/*
  Name : TEA5767 module 
  Title : custom library for Fm ardio project used by TEA5767 fm radio module , header file
  Version: 1.0
  IDE: Arduino 1.8.10
  Author: Gavin Lyons
  URL: https://github.com/gavinlyonsrepo/STM32_projects
*/
 
#ifndef _TEA5767CUSTOM_H_
#define _TEA5767CUSTOM_H_

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <stdio.h>
#include <Wire.h>

// ************  Write mode register values ************
// First register 
#define TEA5767_MUTE        0x80    // Mutes output 
#define TEA5767_SEARCH        0x40    // Activates station search 
// Bits 0-5 for divider MSB 
// Second register 
// Bits 0-7 for divider LSB 
// Third register 
// Station search from botton to up 
#define TEA5767_SEARCH_UP    0x80
// Searches with ADC output = 10 
#define TEA5767_SRCH_HIGH_LVL    0x60
// Searches with ADC output = 10 
#define TEA5767_SRCH_MID_LVL    0x40
// Searches with ADC output = 5 
#define TEA5767_SRCH_LOW_LVL    0x20
// if on, div=4*(Frf+Fif)/Fref otherwise, div=4*(Frf-Fif)/Freq) 
#define TEA5767_HIGH_LO_INJECT    0x10
// Disable stereo 
#define TEA5767_MONO        0x08
// Disable right channel and turns to mono 
#define TEA5767_MUTE_RIGHT    0x04
// Disable left channel and turns to mono 
#define TEA5767_MUTE_LEFT    0x02
#define TEA5767_PORT1_HIGH    0x01
// Fourth register 
#define TEA5767_PORT2_HIGH    0x80
// Chips stops working. Only I2C bus remains on 
#define TEA5767_STDBY        0x40
// Japan freq (76-108 MHz. If disabled, 87.5-108 MHz 
#define TEA5767_JAPAN_BAND    0x20
// Unselected means 32.768 KHz freq as reference. Otherwise Xtal at 13 MHz 
#define TEA5767_XTAL_32768    0x10
// Cuts weak signals 
#define TEA5767_SOFT_MUTE    0x08
// Activates high cut control 
#define TEA5767_HIGH_CUT_CTRL    0x04
// Activates stereo noise control 
#define TEA5767_ST_NOISE_CTL    0x02
// If activate PORT 1 indicates SEARCH or else it is used as PORT1 
#define TEA5767_SRCH_IND    0x01
// Fifth register 
// By activating, it will use Xtal at 13 MHz as reference for divider 
#define TEA5767_PLLREF_ENABLE    0x80
// By activating, deemphasis=50, or else, deemphasis of 50us 
#define TEA5767_DEEMPH_75    0X40

// ************* Read mode register values **********

// First register 
#define TEA5767_READY_FLAG_MASK    0x80
#define TEA5767_BAND_LIMIT_MASK    0X40
// Bits 0-5 for divider MSB after search or preset 
// Second register 
// Bits 0-7 for divider LSB after search or preset 
// Third register 
#define TEA5767_STEREO_MASK    0x80
#define TEA5767_IF_CNTR_MASK    0x7F
// Fourth register 
#define TEA5767_ADC_LEVEL_MASK    0xF0
// should be 0 
#define TEA5767_CHIP_ID_MASK    0x0F
// Fifth register 
// Reserved for future extensions 
#define TEA5767_RESERVED_MASK    0xFF

// **********  Misc Constants ****************8
#define TEA5767_SEARCH_DIR_UP 1
#define TEA5767_SEARCH_DIR_DOWN 2
#define TEA5767_I2C 0x60 // I2C bus address
#define TEA5767_FREQ_MIN 87.5
#define TEA5767_FREQ_MAX 108

struct tea5767_ctrl
{
  unsigned int port1:1;
  unsigned int port2:1;
  unsigned int high_cut:1;
  unsigned int st_noise:1;
  unsigned int soft_mute:1;
  unsigned int japan_band:1;
  unsigned int deemph_75:1;
  unsigned int pllref:1;
  unsigned int xtal_freq;
};

class TEA5767 {
	private:
		tea5767_ctrl ctrl_data;
		uint8_t HILO; 
    
	protected:
		int hilo_optimal (unsigned long freq);
		void set_frequency (int hilo, double freq);
		int ready (unsigned char *buf);
		int bl_reached (unsigned char *buf);
    void TransmitBuffer(unsigned char *buf);
    
	public:
		TEA5767(void);
    void init(void);
		void set_frequency (double freq);
		int read_status (unsigned char *buf);
		int signal_level (unsigned char *buf);
		int stereo (unsigned char *buf);
		double frequency_available (unsigned char *buf);
		void search_stations (unsigned char *buf, bool upDown);
		int process_search (unsigned char *buf, int search_dir);	
};

#endif  // _TEA5767CUSTOM_H_
