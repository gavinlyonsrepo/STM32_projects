/*!
	@file ERM19264_UC1609.cpp
	@brief ERM19264 LCD driven by UC1609C controller, src file.
	@author Gavin Lyons.
*/

#include <ERM19264_UC1609.hpp>
#include "main.h"


/*!
	@brief init the screen/sharedBuffer object
	@param mybuffer Pointer to buffer array data(arrays decay to  pointers)
	@param w width of buffer
	@param h height of buffer
	@param x offset of buffer
	@param y offset of buffer
 */
ERM19264_UC1609_Screen::ERM19264_UC1609_Screen(uint8_t* mybuffer, uint8_t w,  uint8_t h, int16_t  x, int16_t y)
{
	screenBuffer = mybuffer; // pointer to buffer
	width = w;   // bitmap x size
	height = h; // bitmap y size
	xoffset = x; // x offset
	yoffset = y; // y offset
}


/*!
	@brief init the LCD class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
 */
ERM19264_UC1609::ERM19264_UC1609(int16_t LCDwidth, int16_t LCDheight ):custom_graphics(LCDwidth, LCDheight)
{
	_LCD_HEIGHT = LCDheight;
	_LCD_WIDTH = LCDwidth;
}

/*!
 @brief begin Method initialise LCD
 @param AddressCtrl AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
 @param VBiasPOT contast default = 0x49 , range 0x00 to 0xFE
 @param hspi1 pointer to SPIhandle struct for SPI port
*/
void ERM19264_UC1609::LCDbegin(uint8_t AddressCtrl, uint8_t VbiasPOT,
		SPI_HandleTypeDef* hspi1) {
	_hspi1 = hspi1;
	_VbiasPOT = VbiasPOT;
	if (AddressCtrl > 7)
		AddressCtrl = 0x02;
	_AddressCtrl = AddressCtrl;
	LCDinit();
}

/*!
	@brief getter for Library version number
	@return the version number 101 is 1.0.1
*/
uint16_t ERM19264_UC1609::LCDLibVerNumGet() {
	return _LibVerNum;
}

/*!
	@brief begin Method initialise LCD
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
 */
void ERM19264_UC1609::LCDinit() {
	HAL_Delay(UC1609_INIT_DELAY2); //3mS delay, datasheet
	UC1609_CD_SetHigh;
	UC1609_CS_SetHigh;
	LCDReset();

	UC1609_CS_SetLow;

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	send_command(UC1609_ADDRESS_CONTROL, _AddressCtrl);
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	send_command(UC1609_POWER_CONTROL, UC1609_PC_SET);
	HAL_Delay(UC1609_INIT_DELAY);

	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user

	send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal

	UC1609_CS_SetHigh;
}

/*!
	 @brief Sends a command to the display
	 @param command Command to send
	 @param value the values to change
	 @note command and value  will be combined with OR
*/
void ERM19264_UC1609::send_command(uint8_t command, uint8_t value) {
	UC1609_CD_SetLow;
	send_data(command | value);
	UC1609_CD_SetHigh;
}

/*!
	@brief Resets LCD in a four wire setup called at start
	and  should also be called in a controlled power down setting
*/
void ERM19264_UC1609::LCDReset() {
	UC1609_RST_SetLow;
	HAL_Delay(UC1609_RESET_DELAY);
	UC1609_RST_SetHigh;
	HAL_Delay(UC1609_RESET_DELAY2);
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void ERM19264_UC1609::LCDPowerDown(void) {
	LCDReset();
	LCDEnable(0);
}

/*!
	 @brief Turns On Display
	 @param bits  1  display on , 0 display off
*/
void ERM19264_UC1609::LCDEnable(uint8_t bits) {
	UC1609_CS_SetLow;
	send_command(UC1609_DISPLAY_ON, bits);
	UC1609_CS_SetHigh;
}


/*!
	@brief Rotates the display using LCD built-in rotate commands
	@details Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
		Param1: 4 possible values 000 010 100 110 (defined)
	@note If Mx is changed the buffer must BE updated see examples.
*/
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue) {
	UC1609_CS_SetLow;
	switch (rotatevalue) {
	case 0:
		rotatevalue = 0;
		break;
	case 0x02:
		rotatevalue = UC1609_ROTATION_FLIP_ONE;
		break;
	case 0x04:
		rotatevalue = UC1609_ROTATION_NORMAL;
		break;
	case 0x06:
		rotatevalue = UC1609_ROTATION_FLIP_TWO;
		break;
	default:
		rotatevalue = UC1609_ROTATION_NORMAL;
		break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);
	UC1609_CS_SetHigh;
}

/*!
	@brief Scroll the displayed image up by SL rows.
	@details The valid SL value is between 0 (for no
		scrolling) and (64).
		Setting SL outside of this range causes undefined effect on the displayed image.
	@param bits 0-64 line number y-axis
*/
void ERM19264_UC1609::LCDscroll(uint8_t bits) {
	UC1609_CS_SetLow;
	send_command(UC1609_SCROLL, bits);
	UC1609_CS_SetHigh;
}

/*!
	 @brief invert the display
	 @param bits 1 invert , 0 normal
*/
void ERM19264_UC1609::LCDInvertDisplay(uint8_t bits) {
	UC1609_CS_SetLow;
	send_command(UC1609_INVERSE_DISPLAY, bits);
	UC1609_CS_SetHigh;
}

/*!
	@brief turns on all Pixels
	@param bits Set DC[1] to force all SEG drivers to output ON signals
	1 all on ,  0 all off
*/
void ERM19264_UC1609::LCDAllpixelsOn(uint8_t bits) {
	UC1609_CS_SetLow;
	send_command(UC1609_ALL_PIXEL_ON, bits);
	UC1609_CS_SetHigh;
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay optional delay in microseconds can be set to zero normally.
*/
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern = 0) {
	UC1609_CS_SetLow;
	uint16_t numofbytes = _LCD_WIDTH * (_LCD_HEIGHT / 8); // width * height
	for (uint16_t i = 0; i < numofbytes; i++) {
		send_data(dataPattern);
	}
	UC1609_CS_SetHigh;
}

/*!
	 @brief Fill the chosen page at cursor  with a datapattern
	 @param dataPattern can be set to 0 to FF (not buffer)
*/
void ERM19264_UC1609::LCDFillPage(uint8_t dataPattern = 0) {
	UC1609_CS_SetLow;
	uint16_t numofbytes = ((_LCD_WIDTH * (_LCD_HEIGHT / 8)) / 8); // (width * height/8)/8 = 192 bytes
	for (uint16_t i = 0; i < numofbytes; i++) {
		send_data(dataPattern);
	}
	UC1609_CS_SetHigh;
}

/*!
	 @brief Draw a bitmap in direct to screen to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data  pointer to the bitmap  data
	 @note No buffer used. Data drawn onto screen directly
*/
void ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h,
		const uint8_t *data) {
	UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >> 3;

	for (ty = 0; ty < h; ty = ty + 8) {
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {
			continue;
		}
		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++) {
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {
				continue;
			}
			offset = (w * (ty >> 3)) + tx;
			send_data(data[offset]);
		}
	}
	UC1609_CS_SetHigh;
}



/*!
	 @brief Send data byte with SPI to UC1609
	 @param byte the data byte to send
*/
void ERM19264_UC1609::send_data(uint8_t data) {
	HAL_SPI_Transmit(_hspi1, &data, 1, 100);
}


/*!
	 @brief updates the LCD  i.e. writes the  shared buffer to the active screen
		pointed to by ActiveBuffer
*/
void ERM19264_UC1609::LCDupdate() {
	LCDBuffer( this->ActiveBuffer->xoffset, this->ActiveBuffer->yoffset, this->ActiveBuffer->width, this->ActiveBuffer->height, (uint8_t*) this->ActiveBuffer->screenBuffer);
}

/*!
	 @brief clears the buffer of the active screen pointed to by ActiveBuffer
	 @note Does NOT write to the screen
*/
void ERM19264_UC1609::LCDclearBuffer() {
	 memset( this->ActiveBuffer->screenBuffer, 0x00, (this->ActiveBuffer->width * (this->ActiveBuffer->height/ 8))  );
}

/*!
	 @brief Draw a data array  to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data pointer to the data array
	 @note Called by LCDupdate internally to write buffer to screen , can be called standalone 	as well
*/
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h,
		uint8_t *data) {
	UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y / 8;

	for (ty = 0; ty < h; ty = ty + 8) {
		if (y + ty < 0 || y + ty >= _LCD_HEIGHT) {
			continue;
		}

		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++) {
			if (x + tx < 0 || x + tx >= _LCD_WIDTH) {
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			send_data(data[offset++]);
		}
	}

	UC1609_CS_SetHigh;
}

/*!
	@brief Draws a Pixel to the screen , overrides the  graphics library
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
*/
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint8_t colour) {

	if ((x < 0) || (x >= this->ActiveBuffer->width) || (y < 0)
			|| (y >= this->ActiveBuffer->height)) {
		return;
	}
	int16_t temp;
	uint8_t RotateMode = getRotation();
	switch (RotateMode) {
	case 1:
		temp = x;
		x = WIDTH - 1 - y;
		y = temp;
	break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
	break;
	case 3:
		temp = x;
		x = y;
		y = HEIGHT - 1 - temp;
	break;
	}

	uint16_t offset = (this->ActiveBuffer->width * (y / 8)) + x;
	switch (colour) {
	case FOREGROUND: this->ActiveBuffer->screenBuffer[offset] |= (1 << (y & 7)); break;
	case BACKGROUND: this->ActiveBuffer->screenBuffer[offset] &= ~(1 << (y & 7)); break;
	case INVERSE: this->ActiveBuffer->screenBuffer[offset] ^= (1 << (y & 7)); break;
	}
	return;
}

/*!
	 @brief Goes to X Y position
	 @param  column Column 0-192
	 @param page 0-7
*/
void ERM19264_UC1609::LCDGotoXY(uint8_t column, uint8_t page) {
	UC1609_CS_SetLow;
	send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
	send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
	send_command(UC1609_SET_PAGEADD, page++);
	UC1609_CS_SetHigh;
}


//***********************************************
