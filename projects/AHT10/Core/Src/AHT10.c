/*
 * Project Name: library for Aosong ASAIR AHT10, AHT15 Digital Humidity & Temperature Sensor
 * File: AHT_10.c
 * Description: library Source file
 * Author: Gavin Lyons.
 * IC: STM32F303
 * IDE:  STM32cubeIDE
 * Created DEC 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */

#include "AHT10.h"
#include "main.h"

I2C_HandleTypeDef  * _hi2c1;
uint8_t          _address;
ASAIR_I2C_SENSOR _sensorName;
uint8_t          _rawDataBuffer[6] = {AHT10_ERROR, 0, 0, 0, 0, 0};
HAL_StatusTypeDef ret;

// Function Desc init the sensor data types call beofre begin()
// Param  1 I2C 8 bit address converted to 7 bit before fucntion call
// Param 2 Enum with Sensor types
// Param 3 Pointer to  Handle to HAL I2C handletypedef

void AHT10_Init(uint8_t address, ASAIR_I2C_SENSOR sensorName, I2C_HandleTypeDef * hi2c1) {
	_address = address;
	_sensorName = sensorName;
	_hi2c1 = hi2c1 ;
}

 // Function desc: Initialize I2C & configure the sensor, call this function before
 // Returns bool true init success False failure

bool AHT10_begin() {
	HAL_Delay(AHT10_POWER_ON_DELAY);    //wait for sensor to initialize
	AHT10_setNormalMode();                //one measurement+sleep mode
	return AHT10_enableFactoryCalCoeff(); //load factory calibration coeff
}


//  Function Desc :: readRawData()  Read raw measurment data from sensor over I2C
//  Returns AHT10_ERROR for failure, true for sucess with data in the buffer

uint8_t AHT10_readRawData() {

	uint8_t bufTX[3];
	bufTX[0] = AHT10_START_MEASURMENT_CMD;
	bufTX[1] = AHT10_DATA_MEASURMENT_CMD;
	bufTX[2] = AHT10_DATA_NOP;
	// 	Send measurement command
	ret = HAL_I2C_Master_Transmit(_hi2c1, _address, bufTX, 3,
			AHT10_MY_I2C_DELAY );
	if (ret != HAL_OK)
		return AHT10_ERROR; //error handler, collision on I2C bus

	if (AHT10_getCalibrationBit(AHT10_FORCE_READ_DATA) != 0x01)
		return AHT10_ERROR;  // error handler, calibration coefficient turned off
	if (AHT10_getBusyBit(AHT10_USE_READ_DATA) != 0x00)
		HAL_Delay(AHT10_MEASURMENT_DELAY); // measurement delay

	// Read 6-bytes from sensor
	ret = HAL_I2C_Master_Receive(_hi2c1, _address, _rawDataBuffer, 6,
			AHT10_MY_I2C_DELAY );

	if (ret != HAL_OK) {
		_rawDataBuffer[0] = AHT10_ERROR;
		return AHT10_ERROR;
	}

	return true;
}

// Function Desc:  readTemperature() Read temperature, °C
// Param1 readI2C  use last data or new
// Returns: failure  AHT10_ERROR ,  success temperature as floating point
// NOTES:
// temperature range      -40°C..+80°C
// temperature resolution 0.01°C
// temperature accuracy   ±0.3°C

float AHT10_readTemperature(bool readI2C) {
	if (readI2C == AHT10_FORCE_READ_DATA) {
		if (AHT10_readRawData() == AHT10_ERROR)
			return AHT10_ERROR; //force to read data to _rawDataBuffer & error handler
	}

	if (_rawDataBuffer[0] == AHT10_ERROR)
		return AHT10_ERROR; //error handler, collision on I2C bus

	uint32_t temperature = ((uint32_t) (_rawDataBuffer[3] & 0x0F) << 16)
			| ((uint16_t) _rawDataBuffer[4] << 8) | _rawDataBuffer[5]; //20-bit raw temperature data

	return (float) temperature * 0.000191 - 50;
}


// Function Desc :  readHumidity()  Read relative humidity, %
// Param1 readI2C  use last data or new
// Returns: failure  AHT10_ERROR ,  success humidity as floating point
// NOTES
// - relative humidity range      0%..100%
//- relative humidity resolution 0.024%
// - relative humidity accuracy   ±2%

float AHT10_readHumidity(bool readI2C) {
	if (readI2C == AHT10_FORCE_READ_DATA) {
		if (AHT10_readRawData() == AHT10_ERROR)
			return AHT10_ERROR; //force to read data to _rawDataBuffer & error handler
	}

	if (_rawDataBuffer[0] == AHT10_ERROR)
		return AHT10_ERROR; //error handler, collision on I2C bus

	uint32_t rawData = (((uint32_t) _rawDataBuffer[1] << 16)
			| ((uint16_t) _rawDataBuffer[2] << 8) | (_rawDataBuffer[3])) >> 4; //20-bit raw humidity data

	float humidity = (float) rawData * 0.000095;

	if (humidity < 0)
		return 0;
	if (humidity > 100)
		return 100;
	return humidity;
}


// Function Desc:  softReset() Restart sensor, without power off
// Return: failure  AHT10_ERROR ,  success output of  AHT10_enableFactoryCalCoeff()
// NOTE:
// - takes ~20ms
// - all registers restores to default

bool AHT10_softReset(void) {
	//MX_I2C1_Init;
	uint8_t bufRX[1];
	bufRX[0]= AHT10_SOFT_RESET_CMD;

	ret = HAL_I2C_Master_Transmit(_hi2c1, _address,
			bufRX, 1, AHT10_MY_I2C_DELAY );

	if (ret != HAL_OK)
		return false;

	HAL_Delay(AHT10_SOFT_RESET_DELAY);
	AHT10_setNormalMode();                 //reinitialize sensor registers after reset
	return AHT10_enableFactoryCalCoeff();  //reinitialize sensor registers after reset
}


// Function Desc setNormalMode() Set normal measurement mode
// Returns True for success , false for failure

bool AHT10_setNormalMode(void) {
	uint8_t bufTX[3];

	bufTX[0] = AHT10_NORMAL_CMD;
	bufTX[1] = AHT10_DATA_NOP;
	bufTX[2] = AHT10_DATA_NOP;
	ret = HAL_I2C_Master_Transmit(_hi2c1, _address, bufTX, 3,
			AHT10_MY_I2C_DELAY );
	if (ret != HAL_OK)
		return false; //safety check, make sure transmission complete

	HAL_Delay(AHT10_CMD_DELAY);

	return true;
}


// Function Desc  setCycleMode(),  Set cycle measurment, mode continuous measurement
// Returns True for success , false for failure
bool AHT10_setCycleMode(void) {
	uint8_t bufTX[3];

	if (_sensorName != AHT20_SENSOR)
		bufTX[0] = AHT10_INIT_CMD;
	else
		bufTX[0] = AHT20_INIT_CMD;
	bufTX[1] = AHT10_INIT_CYCLE_MODE | AHT10_INIT_CAL_ENABLE;
	bufTX[2] = AHT10_DATA_NOP;

	ret = HAL_I2C_Master_Transmit(_hi2c1, _address, bufTX, 3,
			AHT10_MY_I2C_DELAY );
	if (ret != HAL_OK)
		return false; //safety check, make sure transmission complete


	return true;
}


// Function Desc :  readStatusByte() , Read status byte from sensor over I2C
// Returns Status byte sucess or AHT10_ERROR failure

uint8_t AHT10_readStatusByte() {

	ret = HAL_I2C_Master_Receive(_hi2c1, _address, _rawDataBuffer, 1,
			AHT10_MY_I2C_DELAY );

	if (ret != HAL_OK) {
		_rawDataBuffer[0] = AHT10_ERROR;
		return AHT10_ERROR;
	}

	return _rawDataBuffer[0];

}


// Function Desc :getCalibrationBit(),  Read Calibration bit from status byte
// Param1 readI2C  use last data or force new read
// Returns The calibration bit for success or AHT10_ERROR for error
// NOTES:
// - 0, factory calibration coeff disabled
// - 1, factory calibration coeff loaded

uint8_t AHT10_getCalibrationBit(bool readI2C) {
	uint8_t valueBit;
	if (readI2C == AHT10_FORCE_READ_DATA)
		_rawDataBuffer[0] = AHT10_readStatusByte(); //force to read status byte

	if (_rawDataBuffer[0] != AHT10_ERROR)
	{
		valueBit = (_rawDataBuffer[0] & 0x08);
		return (valueBit>>3); //get 3-rd bit 0001000
	}else{
		return AHT10_ERROR;
	}
}


// Function Desc : enableFactoryCalCoeff() , Load factory calibration coefficients
// returns true for success, false for failure

bool AHT10_enableFactoryCalCoeff() {

	uint8_t bufTX[3];
	if (_sensorName != AHT20_SENSOR)
		bufTX[0] = AHT10_INIT_CMD;
	else
		bufTX[0] = AHT20_INIT_CMD;

	bufTX[1] = AHT10_INIT_CAL_ENABLE;
	bufTX[2] = AHT10_DATA_NOP;

	ret = HAL_I2C_Master_Transmit(_hi2c1, _address, bufTX, 3,
			AHT10_MY_I2C_DELAY );

	if (ret != HAL_OK)
		return false; //safety check, make sure transmission complete

	HAL_Delay(AHT10_CMD_DELAY);

	/*check calibration enable */
	if (AHT10_getCalibrationBit(AHT10_FORCE_READ_DATA) == 0x01)
		return true;
	else
		return false;

}

// Function Desc : getBusyBit(),  Read busy bit from status byte
// Param1 readI2C  use last data or force new read
// Returns busy bit for success or AHT10_ERROR for failure
// NOTES:
// - 0, sensor idle & sleeping
// - 1, sensor busy & in measurement state

uint8_t AHT10_getBusyBit(bool readI2C) {
	uint8_t valueBit;
	if (readI2C == AHT10_FORCE_READ_DATA)
		_rawDataBuffer[0] = AHT10_readStatusByte(); // Read status byte

	if (_rawDataBuffer[0] != AHT10_ERROR)
	{
		valueBit = (_rawDataBuffer[0] & 0x80);
		return (valueBit>>7);; //get 7-th bit 1000 0000  0x80
	}
	else{
		return AHT10_ERROR;
	}
}
