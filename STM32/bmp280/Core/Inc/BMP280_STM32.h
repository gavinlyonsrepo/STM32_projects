/**
 * @file    BMP280_STM32.h
 * @brief   BMP280 pressure/temperature sensor driver for STM32 (I2C, HAL)
 * @target  STM32F303K8T6 (Nucleo-32), STM32CubeIDE
 *
 * Ported from gavinlyonsrepo/BMP280_LTSM (Arduino/C++) to plain C with STM32 HAL.
 * SPI support removed – I2C only.
 *
 * Wiring (3.3 V device):
 *   BMP280 SDA  → STM32 SDA  (e.g. PB7 / I2C1)
 *   BMP280 SCL  → STM32 SCL  (e.g. PB6 / I2C1)
 *   BMP280 CSB  → 3.3 V  (selects I2C mode)
 *   BMP280 SDO  → GND   → I2C address 0x76
 *              → 3.3 V  → I2C address 0x77
 */

#ifndef BMP280_STM32_H
#define BMP280_STM32_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"   /* Change to stm32f1xx_hal.h / stm32f4xx_hal.h etc. for other families */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  USER CONFIGURATION                                                  */
/* ------------------------------------------------------------------ */

/** I2C address – 0x76 (SDO low) or 0x77 (SDO high).
 *  HAL uses 8-bit address (7-bit << 1). */
#define BMP280_I2C_ADDR_SDO_LOW    (0x76 << 1)
#define BMP280_I2C_ADDR_SDO_HIGH   (0x77 << 1)

/** Set to 1 to enable debug output via printf() / ITM / UART as required. */
#define BMP280_DEBUG               0

/* ------------------------------------------------------------------ */
/*  BMP280 REGISTER MAP                                                 */
/* ------------------------------------------------------------------ */

#define BMP280_REG_CALIB_START     0x88   /* Calibration data 0x88-0x9F (26 bytes) */
#define BMP280_REG_CHIP_ID         0xD0
#define BMP280_REG_RESET           0xE0
#define BMP280_REG_STATUS          0xF3
#define BMP280_REG_CTRL_MEAS       0xF4
#define BMP280_REG_CONFIG          0xF5
#define BMP280_REG_PRESS_MSB       0xF7   /* 0xF7, 0xF8, 0xF9 – 20-bit pressure */
#define BMP280_REG_TEMP_MSB        0xFA   /* 0xFA, 0xFB, 0xFC – 20-bit temperature */

#define BMP280_CHIP_ID             0x58   /* Valid range 0x56-0x58 for BMP280 */
#define BMP280_RESET_VALUE         0xB6

/* ------------------------------------------------------------------ */
/*  ENUMERATIONS                                                        */
/* ------------------------------------------------------------------ */

/** Oversampling settings (osrs_t / osrs_p bits) */
typedef enum {
    BMP280_SAMPLING_NONE   = 0x00,  /* skipped */
    BMP280_SAMPLING_X1     = 0x01,
    BMP280_SAMPLING_X2     = 0x02,
    BMP280_SAMPLING_X4     = 0x03,
    BMP280_SAMPLING_X8     = 0x04,
    BMP280_SAMPLING_X16    = 0x05
} BMP280_Sampling_e;

/** Power modes */
typedef enum {
    BMP280_MODE_SLEEP  = 0x00,
    BMP280_MODE_FORCED = 0x01,   /* 0x02 is also forced */
    BMP280_MODE_NORMAL = 0x03
} BMP280_Mode_e;

/** IIR filter coefficients */
typedef enum {
    BMP280_FILTER_OFF  = 0x00,
    BMP280_FILTER_X2   = 0x01,
    BMP280_FILTER_X4   = 0x02,
    BMP280_FILTER_X8   = 0x03,
    BMP280_FILTER_X16  = 0x04
} BMP280_Filter_e;

/** Standby time in normal mode (t_sb bits) */
typedef enum {
    BMP280_STANDBY_MS_0_5   = 0x00,
    BMP280_STANDBY_MS_62_5  = 0x01,
    BMP280_STANDBY_MS_125   = 0x02,
    BMP280_STANDBY_MS_250   = 0x03,
    BMP280_STANDBY_MS_500   = 0x04,
    BMP280_STANDBY_MS_1000  = 0x05,
    BMP280_STANDBY_MS_2000  = 0x06,
    BMP280_STANDBY_MS_4000  = 0x07
} BMP280_StandbyTime_e;

/** Return codes */
typedef enum {
    BMP280_OK             =  0,
    BMP280_ERR_I2C        = -1,   /* HAL I2C error               */
    BMP280_ERR_CHIP_ID    = -2,   /* Wrong chip ID               */
    BMP280_ERR_NULL_PTR   = -3    /* Null handle or output ptr   */
} BMP280_Status_e;

/* ------------------------------------------------------------------ */
/*  CALIBRATION DATA (stored on chip, read once at init)               */
/* ------------------------------------------------------------------ */

typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} BMP280_CalibData_t;

/* ------------------------------------------------------------------ */
/*  DEVICE HANDLE                                                       */
/* ------------------------------------------------------------------ */

typedef struct {
    I2C_HandleTypeDef  *hi2c;        /**< Pointer to HAL I2C handle (set by user) */
    uint8_t             i2cAddr;     /**< 8-bit I2C address (7-bit << 1)           */
    BMP280_CalibData_t  calib;       /**< Calibration coefficients                  */
    int32_t             t_fine;      /**< Fine temperature used by pressure comp.   */

    /* Settings – change before calling BMP280_Init() or use setters */
    BMP280_Sampling_e   tempSampling;
    BMP280_Sampling_e   pressSampling;
    BMP280_Mode_e       mode;
    BMP280_Filter_e     filter;
    BMP280_StandbyTime_e standbyTime;
} BMP280_Handle_t;

/* ------------------------------------------------------------------ */
/*  PUBLIC API                                                          */
/* ------------------------------------------------------------------ */

void BMP280_DefaultConfig(BMP280_Handle_t *dev, I2C_HandleTypeDef *hi2c, uint8_t i2cAddr);
BMP280_Status_e BMP280_Init(BMP280_Handle_t *dev);
BMP280_Status_e BMP280_SoftReset(BMP280_Handle_t *dev);
BMP280_Status_e BMP280_ReadChipID(BMP280_Handle_t *dev, uint8_t *chipId);
BMP280_Status_e BMP280_ReadTemperature(BMP280_Handle_t *dev, int32_t *tempC);
BMP280_Status_e BMP280_ReadPressure(BMP280_Handle_t *dev, uint32_t *pressRaw);
BMP280_Status_e BMP280_ReadAll(BMP280_Handle_t *dev, int32_t *tempC, uint32_t *pressPa);
BMP280_Status_e BMP280_SetMode(BMP280_Handle_t *dev, BMP280_Mode_e mode);
bool BMP280_IsMeasuring(BMP280_Handle_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* BMP280_STM32_H */
