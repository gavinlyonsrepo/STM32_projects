/**
 * @file    BMP280_STM32.c
 * @brief   BMP280 pressure/temperature sensor driver for STM32 (I2C, HAL)
 * @target  STM32F303K8T6 (Nucleo-32), STM32CubeIDE
 *
 * Ported from gavinlyonsrepo/BMP280_LTSM (Arduino/C++) to plain C with STM32 HAL.
 * SPI support removed – I2C only.
 *
 * Compensation formulas taken directly from the official
 * Bosch BMP280 datasheet (BST-BMP280-DS001, Section 4.2.3).
 */

#include "BMP280_STM32.h"
#include <string.h>   /* memset */

/* ------------------------------------------------------------------ */
/*  PRIVATE HELPERS – I2C READ / WRITE                                  */
/* ------------------------------------------------------------------ */

/** HAL I2C timeout in milliseconds */
#define BMP280_I2C_TIMEOUT  100U

/**
 * @brief  Write a single byte to a register.
 */
static BMP280_Status_e prv_WriteReg(BMP280_Handle_t *dev, uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = { reg, value };
    if (HAL_I2C_Master_Transmit(dev->hi2c, dev->i2cAddr, buf, 2, BMP280_I2C_TIMEOUT) != HAL_OK)
        return BMP280_ERR_I2C;
    return BMP280_OK;
}

/**
 * @brief  Read one or more bytes starting from a register address.
 */
static BMP280_Status_e prv_ReadRegs(BMP280_Handle_t *dev, uint8_t reg,
                                     uint8_t *data, uint16_t len)
{
    /* Send register address */
    if (HAL_I2C_Master_Transmit(dev->hi2c, dev->i2cAddr, &reg, 1, BMP280_I2C_TIMEOUT) != HAL_OK)
        return BMP280_ERR_I2C;
    /* Receive data */
    if (HAL_I2C_Master_Receive(dev->hi2c, dev->i2cAddr, data, len, BMP280_I2C_TIMEOUT) != HAL_OK)
        return BMP280_ERR_I2C;
    return BMP280_OK;
}

/* ------------------------------------------------------------------ */
/*  PRIVATE HELPER – CALIBRATION                                        */
/* ------------------------------------------------------------------ */

/**
 * @brief  Read 26 bytes of factory calibration data stored in NVM 0x88-0x9F.
 *         Values are stored little-endian, some signed, some unsigned.
 */
static BMP280_Status_e prv_ReadCalibration(BMP280_Handle_t *dev)
{
    uint8_t raw[24];
    BMP280_Status_e st = prv_ReadRegs(dev, BMP280_REG_CALIB_START, raw, 24);
    if (st != BMP280_OK) return st;

    BMP280_CalibData_t *c = &dev->calib;

    c->dig_T1 = (uint16_t)(raw[1]  << 8 | raw[0]);
    c->dig_T2 = (int16_t) (raw[3]  << 8 | raw[2]);
    c->dig_T3 = (int16_t) (raw[5]  << 8 | raw[4]);

    c->dig_P1 = (uint16_t)(raw[7]  << 8 | raw[6]);
    c->dig_P2 = (int16_t) (raw[9]  << 8 | raw[8]);
    c->dig_P3 = (int16_t) (raw[11] << 8 | raw[10]);
    c->dig_P4 = (int16_t) (raw[13] << 8 | raw[12]);
    c->dig_P5 = (int16_t) (raw[15] << 8 | raw[14]);
    c->dig_P6 = (int16_t) (raw[17] << 8 | raw[16]);
    c->dig_P7 = (int16_t) (raw[19] << 8 | raw[18]);
    c->dig_P8 = (int16_t) (raw[21] << 8 | raw[20]);
    c->dig_P9 = (int16_t) (raw[23] << 8 | raw[22]);

    return BMP280_OK;
}

/* ------------------------------------------------------------------ */
/*  PRIVATE HELPER – RAW ADC READS                                      */
/* ------------------------------------------------------------------ */

/**
 * @brief  Read the 20-bit raw temperature ADC value.
 */
static BMP280_Status_e prv_ReadRawTemp(BMP280_Handle_t *dev, int32_t *rawTemp)
{
    uint8_t raw[3];
    BMP280_Status_e st = prv_ReadRegs(dev, BMP280_REG_TEMP_MSB, raw, 3);
    if (st != BMP280_OK) return st;

    *rawTemp = (int32_t)(((uint32_t)raw[0] << 12) |
                         ((uint32_t)raw[1] <<  4) |
                         ((uint32_t)raw[2] >>  4));
    return BMP280_OK;
}

/**
 * @brief  Read the 20-bit raw pressure ADC value.
 */
static BMP280_Status_e prv_ReadRawPress(BMP280_Handle_t *dev, int32_t *rawPress)
{
    uint8_t raw[3];
    BMP280_Status_e st = prv_ReadRegs(dev, BMP280_REG_PRESS_MSB, raw, 3);
    if (st != BMP280_OK) return st;

    *rawPress = (int32_t)(((uint32_t)raw[0] << 12) |
                          ((uint32_t)raw[1] <<  4) |
                          ((uint32_t)raw[2] >>  4));
    return BMP280_OK;
}

/* ------------------------------------------------------------------ */
/*  PRIVATE HELPERS – BOSCH COMPENSATION FORMULAS (integer, 32-bit)    */
/*  Source: BMP280 datasheet, section 4.2.3                             */
/* ------------------------------------------------------------------ */

/**
 * @brief  Compensate raw temperature using calibration data.
 *         Updates dev->t_fine (needed for pressure compensation).
 * @return Temperature in hundredths of a degree Celsius (e.g. 2345 = 23.45 °C).
 */
static int32_t prv_CompensateTemp(BMP280_Handle_t *dev, int32_t rawTemp)
{
    int32_t var1, var2, T;

    var1 = ((((rawTemp >> 3) - ((int32_t)dev->calib.dig_T1 << 1))) *
            ((int32_t)dev->calib.dig_T2)) >> 11;

    var2 = (((((rawTemp >> 4) - ((int32_t)dev->calib.dig_T1)) *
              ((rawTemp >> 4) - ((int32_t)dev->calib.dig_T1))) >> 12) *
            ((int32_t)dev->calib.dig_T3)) >> 14;

    dev->t_fine = var1 + var2;
    T = (dev->t_fine * 5 + 128) >> 8;   /* result in 0.01 °C units */
    return T;
}

/**
 * @brief  Compensate raw pressure using calibration data and t_fine.
 *         prv_CompensateTemp() must have been called first.
 * @return Pressure in Pa in Q24.8 fixed-point format.
 *         Divide by 256 to get whole Pascals.
 */
static uint32_t prv_CompensatePress(BMP280_Handle_t *dev, int32_t rawPress)
{
    int64_t var1, var2, p;

    var1 = ((int64_t)dev->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dev->calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)dev->calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)dev->calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dev->calib.dig_P3) >> 8) +
           ((var1 * (int64_t)dev->calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) *
           ((int64_t)dev->calib.dig_P1) >> 33;

    if (var1 == 0)
        return 0;   /* Avoid division by zero */

    p    = 1048576 - rawPress;
    p    = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dev->calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dev->calib.dig_P8) * p) >> 19;
    p    = ((p + var1 + var2) >> 8) + (((int64_t)dev->calib.dig_P7) << 4);

    return (uint32_t)p;
}

/* ------------------------------------------------------------------ */
/*  PUBLIC API IMPLEMENTATION                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief  Initialise the BMP280 device handle with default settings.
 *
 * Call this before BMP280_Init(). It fills the handle with safe defaults:
 *   Temp oversampling  : X16
 *   Press oversampling : X2
 *   Mode               : Normal
 *   Filter             : Off
 *   Standby            : 0.5 ms
 *
 * @param  dev      Pointer to a BMP280_Handle_t variable.
 * @param  hi2c     Pointer to the HAL I2C handle (must already be initialised by CubeIDE).
 * @param  i2cAddr  8-bit address: use BMP280_I2C_ADDR_SDO_LOW or BMP280_I2C_ADDR_SDO_HIGH.
 */
void BMP280_DefaultConfig(BMP280_Handle_t *dev, I2C_HandleTypeDef *hi2c, uint8_t i2cAddr)
{
    memset(dev, 0, sizeof(BMP280_Handle_t));
    dev->hi2c         = hi2c;
    dev->i2cAddr      = i2cAddr;
    dev->tempSampling = BMP280_SAMPLING_X16;
    dev->pressSampling= BMP280_SAMPLING_X2;
    dev->mode         = BMP280_MODE_NORMAL;
    dev->filter       = BMP280_FILTER_OFF;
    dev->standbyTime  = BMP280_STANDBY_MS_0_5;
}

/**
 * @brief  Verify chip ID, read calibration data, and write configuration registers.
 * @param  dev  Pointer to configured BMP280_Handle_t.
 * @return BMP280_OK on success, error code otherwise.
 */
BMP280_Status_e BMP280_Init(BMP280_Handle_t *dev)
{
    if (dev == NULL || dev->hi2c == NULL)
        return BMP280_ERR_NULL_PTR;

    /* 1. Verify chip ID */
    uint8_t chipId = 0;
    BMP280_Status_e st = BMP280_ReadChipID(dev, &chipId);
    if (st != BMP280_OK) return st;

    if (chipId < 0x56 || chipId > 0x58)
    {
#if BMP280_DEBUG
        /* If you have printf() routed, uncomment: */
        /* printf("BMP280: bad chip ID 0x%02X\r\n", chipId); */
#endif
        return BMP280_ERR_CHIP_ID;
    }

    /* 2. Read factory calibration data */
    st = prv_ReadCalibration(dev);
    if (st != BMP280_OK) return st;

    /* 3. Write ctrl_meas register (osrs_t[7:5], osrs_p[4:2], mode[1:0]) */
    uint8_t ctrlMeas = (uint8_t)((dev->tempSampling  << 5) |
                                  (dev->pressSampling << 2) |
                                   dev->mode);
    st = prv_WriteReg(dev, BMP280_REG_CTRL_MEAS, ctrlMeas);
    if (st != BMP280_OK) return st;

    /* 4. Write config register (t_sb[7:5], filter[4:2], spi3w_en[0]) */
    uint8_t config = (uint8_t)((dev->standbyTime << 5) |
                                (dev->filter      << 2));
    st = prv_WriteReg(dev, BMP280_REG_CONFIG, config);
    if (st != BMP280_OK) return st;

    return BMP280_OK;
}

/**
 * @brief  Soft-reset the device (register 0xE0 = 0xB6).
 * @param  dev  Pointer to BMP280_Handle_t.
 * @return BMP280_OK or BMP280_ERR_I2C.
 */
BMP280_Status_e BMP280_SoftReset(BMP280_Handle_t *dev)
{
    if (dev == NULL) return BMP280_ERR_NULL_PTR;
    return prv_WriteReg(dev, BMP280_REG_RESET, BMP280_RESET_VALUE);
}

/**
 * @brief  Read chip ID register (should be 0x56-0x58 for BMP280).
 * @param  dev    Pointer to BMP280_Handle_t.
 * @param  chipId Output: chip ID byte.
 * @return BMP280_OK or BMP280_ERR_I2C.
 */
BMP280_Status_e BMP280_ReadChipID(BMP280_Handle_t *dev, uint8_t *chipId)
{
    if (dev == NULL || chipId == NULL) return BMP280_ERR_NULL_PTR;
    return prv_ReadRegs(dev, BMP280_REG_CHIP_ID, chipId, 1);
}


/**
 * @brief  Read compensated temperature in hundredths of a degree Celsius.
 *         Divide by 100 to get degrees C as a float.
 * @param  dev      Pointer to BMP280_Handle_t.
 * @param  tempC    Output: temperature × 100  (e.g. 2345 = 23.45 °C).
 * @return BMP280_OK or error code.
 */
BMP280_Status_e BMP280_ReadTemperature(BMP280_Handle_t *dev, int32_t *tempC)
{
    if (dev == NULL || tempC == NULL) return BMP280_ERR_NULL_PTR;

    int32_t rawTemp = 0;
    BMP280_Status_e st = prv_ReadRawTemp(dev, &rawTemp);
    if (st != BMP280_OK) return st;

    *tempC = prv_CompensateTemp(dev, rawTemp);   /* updates t_fine */
    return BMP280_OK;
}

/**
 * @brief  Read compensated pressure in Pascals × 256 (Q24.8 fixed point).
 *         Divide by 256 to get Pascals; divide by 25600 to get hPa.
 * @param  dev       Pointer to BMP280_Handle_t.
 * @param  pressRaw  Output: pressure in Q24.8 Pa units.
 * @return BMP280_OK or error code.
 *
 * @note   BMP280_ReadTemperature() must be called first in each measurement
 *         cycle to update t_fine, which is used for pressure compensation.
 */
BMP280_Status_e BMP280_ReadPressure(BMP280_Handle_t *dev, uint32_t *pressRaw)
{
    if (dev == NULL || pressRaw == NULL) return BMP280_ERR_NULL_PTR;

    int32_t rawPress = 0;
    BMP280_Status_e st = prv_ReadRawPress(dev, &rawPress);
    if (st != BMP280_OK) return st;

    *pressRaw = prv_CompensatePress(dev, rawPress);
    return BMP280_OK;
}

/**
 * @brief  Convenience: read both temperature and pressure in one call.
 * @param  dev     Pointer to BMP280_Handle_t.
 * @param  tempC   Output: temperature × 100 (integer, °C × 100).
 * @param  pressPa Output: pressure in whole Pascals (rounded).
 * @return BMP280_OK or error code.
 */
BMP280_Status_e BMP280_ReadAll(BMP280_Handle_t *dev, int32_t *tempC, uint32_t *pressPa)
{
    if (dev == NULL || tempC == NULL || pressPa == NULL) return BMP280_ERR_NULL_PTR;

    /* Must read temperature first – sets t_fine for pressure compensation */
    BMP280_Status_e st = BMP280_ReadTemperature(dev, tempC);
    if (st != BMP280_OK) return st;

    uint32_t pressQ24;
    st = BMP280_ReadPressure(dev, &pressQ24);
    if (st != BMP280_OK) return st;

    *pressPa = pressQ24 >> 8;   /* Convert Q24.8 → whole Pascals */
    return BMP280_OK;
}

/**
 * @brief  Set power mode after initialisation (use BMP280_MODE_FORCED for
 *         single-shot measurements, or BMP280_MODE_NORMAL for continuous).
 * @param  dev   Pointer to BMP280_Handle_t.
 * @param  mode  Desired mode.
 * @return BMP280_OK or BMP280_ERR_I2C.
 */
BMP280_Status_e BMP280_SetMode(BMP280_Handle_t *dev, BMP280_Mode_e mode)
{
    if (dev == NULL) return BMP280_ERR_NULL_PTR;

    /* Read current ctrl_meas register to preserve oversampling bits */
    uint8_t ctrlMeas = 0;
    BMP280_Status_e st = prv_ReadRegs(dev, BMP280_REG_CTRL_MEAS, &ctrlMeas, 1);
    if (st != BMP280_OK) return st;

    ctrlMeas = (ctrlMeas & 0xFC) | (uint8_t)mode;   /* keep bits[7:2], overwrite bits[1:0] */
    dev->mode = mode;
    return prv_WriteReg(dev, BMP280_REG_CTRL_MEAS, ctrlMeas);
}


/**
 * @brief  Returns true when a measurement is in progress (status register bit 3).
 * @param  dev  Pointer to BMP280_Handle_t.
 * @return true if measuring, false if idle or on I2C error.
 */
bool BMP280_IsMeasuring(BMP280_Handle_t *dev)
{
    if (dev == NULL) return false;
    uint8_t status = 0;
    if (prv_ReadRegs(dev, BMP280_REG_STATUS, &status, 1) != BMP280_OK)
        return false;
    return (status & 0x08) != 0;   /* bit 3 = measuring */
}


