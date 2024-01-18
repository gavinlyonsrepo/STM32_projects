/*!
	@file ERM19264_UC1609.hpp
	@brief ERM19264 LCD driven by UC1609C controller, header file.
	@author Gavin Lyons.
*/

#ifndef ERM19264_UC1609_H
#define ERM19264_UC1609_H

#include "main.h"
#include <ERM19264_graphics.hpp>

/*!
	@brief Display Pixel colours  definition
	@details
(1): white on blue,  FG = white, BG = blue
ERM19264SBS-4 LCD Display White on Blue
(2): black on white, FG = black, BG = white
ERM19264FS-4 LCD Display Black on White
(3):white on black,  FG = white, BG = black
ERM19264DNS-4LCD Display White on Black
*/
#define FOREGROUND  0  /**< See Display Pixel colours  definition */
#define BACKGROUND 1   /**< See Display Pixel colours  definition */
#define INVERSE 2      /**< See Display Pixel colours  definition */

// UC1909 Write registers
#define UC1609_SYSTEM_RESET 0xE2 /**< System SW Reset */

#define UC1609_POWER_CONTROL 0x28 /**< Power control Address 0*/
#define UC1609_PC_SET 0x06		  /**< PC[2:0] 110, Internal V LCD (7x charge pump) + 10b: 1.4mA*/

#define UC1609_ADDRESS_CONTROL 0x88 /**< set RAM address control*/
#define UC1609_ADDRESS_SET 0x02		/**< Set AC [2:0] Program registers  for RAM address control.*/

#define UC1609_SET_PAGEADD 0xB0	   /**< Page address Set PA[3:0]*/
#define UC1609_SET_COLADD_LSB 0x00 /**< Column Address Set CA [3:0]*/
#define UC1609_SET_COLADD_MSB 0x10 /**< Column Address Set CA [7:4]*/

#define UC1609_TEMP_COMP_REG 0x27 /**< Temperature Compensation Register */
#define UC1609_TEMP_COMP_SET 0x00 /**< TC[1:0] = 00b= -0.00%/ C */

#define UC1609_FRAMERATE_REG 0xA0 /**< Frame rate */
#define UC1609_FRAMERATE_SET 0x01 /**< Set Frame Rate LC [4:3] 01b: 95 fps */

#define UC1609_BIAS_RATIO 0xE8	   /**< Bias Ratio. The ratio between V-LCD and V-D .*/
#define UC1609_BIAS_RATIO_SET 0x03 /**<  Set BR[1:0] = 11 (set to 9 default)*/

#define UC1609_GN_PM 0x81		  /**< Set V BIAS Potentiometer to fine tune V-D and V-LCD (double-byte command)*/
#define UC1609_DEFAULT_GN_PM 0x49 /**< default only used if user does not specify Vbias*/

#define UC1609_LCD_CONTROL 0xC0		/**< Rotate map control*/
#define UC1609_DISPLAY_ON 0xAE		/**< enables display*/
#define UC1609_ALL_PIXEL_ON 0xA4	/**< sets on all Pixels on*/
#define UC1609_INVERSE_DISPLAY 0xA6 /**< inverts display*/
#define UC1609_SCROLL 0x40			/**< scrolls , Set the scroll line number. 0-64 */

// Rotate
#define UC1609_ROTATION_FLIP_TWO 0x06
#define UC1609_ROTATION_NORMAL 0x04
#define UC1609_ROTATION_FLIP_ONE 0x02
#define UC1609_ROTATION_FLIP_THREE 0x00

// Delays
#define UC1609_RESET_DELAY 3  /**< ms Delay ,datasheet >3uS*/
#define UC1609_RESET_DELAY2 0 /**< mS delay datasheet says > 5mS, does not work?*/
#define UC1609_INIT_DELAY 100 /**<  mS delay ,after init*/
#define UC1609_INIT_DELAY2 3  /**< mS delay,  before reset called datasheet <3mS*/

// GPIO
// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET );
#define UC1609_CS_SetHigh HAL_GPIO_WritePin(GPIOA, CS_Pin,GPIO_PIN_SET);
#define UC1609_CS_SetLow HAL_GPIO_WritePin(GPIOA, CS_Pin,GPIO_PIN_RESET);
#define UC1609_CD_SetHigh HAL_GPIO_WritePin(GPIOA, CD_Pin,GPIO_PIN_SET);
#define UC1609_CD_SetLow HAL_GPIO_WritePin(GPIOA, CD_Pin,GPIO_PIN_RESET);
#define UC1609_RST_SetHigh HAL_GPIO_WritePin(GPIOA, RESET_Pin,GPIO_PIN_SET);
#define UC1609_RST_SetLow HAL_GPIO_WritePin(GPIOA, RESET_Pin,GPIO_PIN_RESET);


/*! @brief class to hold screen data to define buffer , multiple screens can be made for the shared buffer. Buffer must be same size and offsets to if saving Data memory is goal
  */
class  ERM19264_UC1609_Screen
{
	public :
		ERM19264_UC1609_Screen(uint8_t* mybuffer, uint8_t w,  uint8_t h, int16_t  x, int16_t y) ;
		~ERM19264_UC1609_Screen(){};

		uint8_t* screenBuffer; /**<  pointer to shared buffer screen data */
		uint8_t width;   /**< buffer x size in pixels  */
		uint8_t height; /**< buffer y size in pixels */
		int16_t xoffset; /**< x offset in pixels */
		int16_t yoffset; /**< y offset in pixels */
	private:
};

/*!
	@brief class to control LCD UC1609
*/
class ERM19264_UC1609: public custom_graphics {
public:
	ERM19264_UC1609(int16_t w=192, int16_t h=64);
	~ERM19264_UC1609() {};

	virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;

	void LCDupdate(void);
	void LCDclearBuffer(void);
	void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data);
	void LCDbegin(uint8_t adddressCtrl, uint8_t contrast, SPI_HandleTypeDef*);
	void LCDinit(void);

	void LCDEnable(uint8_t on);
	void LCDFillScreen(uint8_t pixels);
	void LCDFillPage(uint8_t pixels);
	void LCDrotate(uint8_t rotatevalue);
	void LCDInvertDisplay(uint8_t on);
	void LCDAllpixelsOn(uint8_t bits);
	void LCDscroll(uint8_t bits);
	void LCDReset(void);
	void LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h,
			const uint8_t *data);
	void LCDPowerDown(void);
	void LCDGotoXY(uint8_t column, uint8_t page);

	uint16_t LCDLibVerNumGet(void);
	uint8_t LCDGetConstrast(void);
	uint8_t LCDGetAddressCtrl(void);

	ERM19264_UC1609_Screen* ActiveBuffer = nullptr; /**< Active buffer pointer , a pointer to which screen object shared buffer will be written to */

private:

	void send_data(uint8_t data);
	void send_command(uint8_t command, uint8_t value);

	SPI_HandleTypeDef* _hspi1;  /**< SPI handler instance */
	uint8_t _VbiasPOT;  /**< Contrast default 0x49 datasheet 00-FE, lower works better on Blue*/
	uint8_t _AddressCtrl; /**< Set AC [2:0] Program registers  for RAM address control. 0x00 to 0x07*/

	const uint16_t _LibVerNum = 180; /**< Library version number 180 = 1.8.0*/

	uint8_t _LCD_WIDTH = 192; /**< Width in pixels  of the LCD*/
	uint8_t _LCD_HEIGHT = 64; /**< Height in pixels  of the LCD */
};

#endif
