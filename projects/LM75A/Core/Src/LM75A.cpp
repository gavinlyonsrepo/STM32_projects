/*
 * Project Name: Library for the LM75A temperature sensor by NXP and Texas Instruments.
 * File: LM75A.cpp
 * Description: library Source file
 * Author: Gavin Lyons.
 * IC: STM32F303
 * IDE:  STM32cubeIDE C++
 * Created DEC 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */

#include "LM75A.h"

// Constructor
// Param 1 pointer I2C HAL handler
// Param 2 I2C address (7-bit)
LIB_LM75A::LIB_LM75A(I2C_HandleTypeDef * hi2c1 ,uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
	_hi2c1 = hi2c1 ;
}

// Power management

// Func Desc : enter Shutdown mode : 4 μA (Typical) current draw.
void LIB_LM75A::shutdown()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111110) | 0b1);
}

// Func Desc : exit Shutdown mode for  Operating  mode: 280 μA (Typical)
void LIB_LM75A::wakeup()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, config & 0b11111110);
}
// Fucn Desc get power mode
// Return bool
// 1 = shutdown
// 0 operating mode
bool LIB_LM75A::isShutdown()
{
	return (read8bitRegister(LM75A_REGISTER_CONFIG) & 0b1) == 1;	
}


// Temperature functions

// Get temperature Celsius
// returns float
float LIB_LM75A::getTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_TEMP, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

// Get temperature Farenheit(
// returns float
float LIB_LM75A::getTemperatureInFarenheit()
{
	return getTemperature() * 1.8f + 32.0f;
}

// Configuration functions

float LIB_LM75A::getHysterisisTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_THYST, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

FaultQueueValue LIB_LM75A::getFaultQueueValue()
{
	return (FaultQueueValue)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b00011000);
}

float LIB_LM75A::getOSTripTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_TOS, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

OsPolarity LIB_LM75A::getOsPolarity()
{
	return (OsPolarity)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b100);
}

DeviceMode LIB_LM75A::getDeviceMode()
{
	return (DeviceMode)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b010);
}

void LIB_LM75A::setHysterisisTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_THYST, temperature * 256);
}

void LIB_LM75A::setOsTripTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_TOS, temperature * 256);
}

void LIB_LM75A::setFaultQueueValue(FaultQueueValue value)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11100111) | value);
}

void LIB_LM75A::setOsPolarity(OsPolarity polarity)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111011) | polarity);
}

void LIB_LM75A::setDeviceMode(DeviceMode deviceMode)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111101) | deviceMode);
}


// Function Desc Is the Device connected
// Writes Reads and writes  the Config register
// Returns 1 for success 0 if not.
bool LIB_LM75A::isConnected()
{
	uint8_t oldValue = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, 0x0f);
	uint8_t newValue = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, oldValue);
	return newValue == 0x0f;
}

// Func Desc :  reads config register
// Returns Byte with values of config register
uint8_t LIB_LM75A::getConfig()
{
	return read8bitRegister(LM75A_REGISTER_CONFIG);
}

// Func Desc :  reads value of product ID register
float LIB_LM75A::getProdId()
{
	uint8_t value = read8bitRegister(LM75A_REGISTER_PRODID);
	return (float)(value >> 4) + (value & 0x0F) / 10.0f;
} 

// *************** Private function  ************************

uint8_t LIB_LM75A::read8bitRegister(const uint8_t reg)
{
	uint8_t result;
	uint8_t BufTx[1];

	BufTx[0] = reg;

	ret = HAL_I2C_Master_Transmit(_hi2c1, _i2cAddress,
			BufTx, 1,  LM75A_TO_I2C_DELAY  );
	if (ret != HAL_OK)
	{
		return 0xFF;
	}
	ret = HAL_I2C_Master_Receive(_hi2c1, _i2cAddress, &result, 1,
			 LM75A_TO_I2C_DELAY  );

	if (ret != HAL_OK)
	{
		return 0xFF;
	}
	return result;
}

bool LIB_LM75A::read16bitRegister(const uint8_t reg, uint16_t& response)
{
	uint8_t bufRX[3];
	uint8_t bufTX[1];
	bufTX[0] = reg;
	ret = HAL_I2C_Master_Transmit(_hi2c1, _i2cAddress,
			bufTX, 1,  LM75A_TO_I2C_DELAY  );

	if (ret != HAL_OK)
	{
		return false;
	}

	ret = HAL_I2C_Master_Receive(_hi2c1, _i2cAddress, bufRX, 2,
			LM75A_TO_I2C_DELAY );

	if (ret != HAL_OK)
	{
		return false;
	}
	
	//response = (Wire.read() << 8) | Wire.read();
	//uint16_t temp = part1 << 8 | part2;
	response = bufRX[0] << 8 | bufRX[1];
	return true;
}

bool LIB_LM75A::write16bitRegister(const uint8_t reg, const uint16_t value)
{
	uint8_t bufTX[3];
		bufTX[0] = reg;
		bufTX[1] = value >> 8;
		bufTX[2] = value;

		ret = HAL_I2C_Master_Transmit(_hi2c1, _i2cAddress,
					bufTX, 3,  LM75A_TO_I2C_DELAY  );
		if (ret != HAL_OK)
		{
			return false;
		}

	return true;
}

bool LIB_LM75A::write8bitRegister(const uint8_t reg, const uint8_t value)
{
	uint8_t bufTX[2];
	bufTX[0] = reg;
	bufTX[1] = value;

	ret = HAL_I2C_Master_Transmit(_hi2c1, _i2cAddress,
				bufTX, 2,  LM75A_TO_I2C_DELAY  );
	if (ret != HAL_OK)
	{
		return false;
	}
	return true;
}

