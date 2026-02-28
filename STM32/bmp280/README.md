# BMP280 + HD44780 LCD — STM32 README

## Table of Contents

- [Overview](#overview)
- [Hardware](#hardware)
  - [BMP280 I2C Connections](#bmp280-i2c-connections)
  - [HD44780 LCD I2C Connections](#hd44780-lcd-i2c-connections)
- [Software](#software)
  - [Dependencies](#dependencies)
  - [BMP280 Settings](#bmp280-settings)
  - [BMP280 Power Modes](#bmp280-power-modes)
  - [LCD Notes](#lcd-notes)
- [Output](#output)

---

## Overview

**Name:** BMP280_HD44780_STM32

**Description:**  
STM32 embedded C project that reads temperature and pressure from a Bosch BMP280 sensor over I2C and displays the results on an HD44780 character LCD via a PCF8574 I2C backpack.

**Features:**

1. Reads temperature (°C) and pressure (hPa) from a BMP280 sensor
2. Displays formatted readings on a 16x02 or 20x04 HD44780 LCD
3. I2C only — no SPI
4. Oversampling, standby time, filter, and power mode configurable via API
5. Normal mode, sleep mode, and forced mode supported
6. Chip ID verification on startup (valid range 0x56–0x58 for BMP280)
7. UART debug output (chip ID, sensor read counter, error messages)
8. BMP280 chip ID should be 0x56–0x58; 0x60 indicates a BME280 (not supported)

**Toolchain:**

- STM32CubeIDE v1.10.1
- HAL, C, GCC GNU 11
- IC: STM32F303K8T6 Nucleo-32

**Authors:**  
BMP280 driver ported from [gavinlyonsrepo/BMP280_LTSM](https://github.com/gavinlyonsrepo/BMP280_LTSM) (Arduino/C++).  
LCD library from [gavinlyonsrepo/HD44780_LCD_PCF8574](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574).

**Datasheet:** [Bosch BMP280 Datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp280-ds001.pdf)

---

## Hardware

Both the BMP280 sensor and the HD44780 LCD backpack share the same I2C bus.  
All devices are 3.3 V. The STM32F303K8T6 Nucleo-32 operates at 3.3 V — no level shifting required.

**I2C pins used:**

| STM32 Pin | Function    |
|-----------|-------------|
| PB7       | I2C1 SDA    |
| PB6       | I2C1 SCL    |

### BMP280 I2C Connections

The BMP280 CSB pin must be tied to 3.3 V to select I2C mode (do not leave floating).  
The SDO pin selects the I2C address:

| BMP280 Pin | Function     | STM32       | Notes                                      |
|------------|--------------|-------------|--------------------------------------------|
| SDA        | Data         | PB7         | I2C DATA                                   |
| SCL        | Clock        | PB6         | I2C Clock                                  |
| CSB        | Chip Select  | 3.3 V       | Must be high to enable I2C mode            |
| SDO        | Addr select  | GND / 3.3 V | GND = address 0x76 · 3.3 V = address 0x77 |
| VCC        | Power        | 3.3 V       |                                            |
| GND        | Ground       | GND         |                                            |

### HD44780 LCD I2C Connections

The LCD uses a PCF8574 I2C backpack. Connect the backpack to the same I2C bus.

| PCF8574 Backpack Pin | Function | STM32 | Notes             |
|----------------------|----------|-------|-------------------|
| SDA                  | Data     | PB7   | I2C DATA          |
| SCL                  | Clock    | PB6   | I2C Clock         |
| VCC                  | Power    | 5 V*  | Check your module, work at 3.3 V but backlight dimmer and may have contrast issues |
| GND                  | Ground   | GND   |                   |

> *Many PCF8574 LCD backpack modules require 5 V for the LCD backlight but have 3.3 V-tolerant I2C lines. Check your specific module.

Tested display sizes: **16×02** and **20×04**.

---

## Software

### Dependencies

Add both library pairs to your STM32CubeIDE project under `Core/Src/` and `Core/Inc/`:

| Files                          | Purpose                        |
|--------------------------------|--------------------------------|
| `BMP280_STM32.c` / `.h`        | BMP280 sensor driver (this port) |
| `PCF8574_LCD.c` / `.h`         | HD44780 LCD via PCF8574 driver |

Enable I2C1 in CubeMX with standard mode (100 kHz). The HAL handle `hi2c1` is passed to both drivers at initialisation.

### BMP280 Settings

Default configuration applied by `BMP280_DefaultConfig()`:

| Setting               | Default             | Enum value |
|-----------------------|---------------------|------------|
| Temp oversampling     | Sampling X16        | 5          |
| Pressure oversampling | Sampling X2         | 2          |
| Standby duration      | 0.5 ms              | 0          |
| IIR Filter            | Off                 | 0          |
| Power mode            | Normal              | 3          |

These can be changed by modifying the handle fields before calling `BMP280_Init()`, or by calling `BMP280_SetMode()` afterwards.

### BMP280 Power Modes

| Mode   | Behaviour                                                                 |
|--------|---------------------------------------------------------------------------|
| Sleep  | No measurements performed. Default state after power-on.                 |
| Normal | Automated perpetual cycling between measurement and standby periods.      |
| Forced | Single measurement taken; sensor returns to sleep when complete.         |

Use **Normal** mode for continuous LCD display updates.  
Use **Forced** mode to trigger a single reading on demand (lower power).

### LCD Notes

`PCF8574_LCDPrintf()` uses `vsprintf` internally. To print `float` values you must add the following flag in STM32CubeIDE:

**Project Properties → C/C++ Build → Settings → MCU GCC Linker → Miscellaneous → Other flags:**
```
-u _printf_float
```

However this project as it stands avoids floats entirely by using integer arithmetic.


## Output

Expected serial (UART2, 115200 baud) output on startup:

```
BMP280 : Connected
BMP280 : Chip ID = 0x58
Sensor Read 1
Sensor Read 2
...
```

Expected LCD output:

```
Temp:  23.45 C
Press: 1013 hPa
```
