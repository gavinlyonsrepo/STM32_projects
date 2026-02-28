/* USER CODE BEGIN Header */
/*
 * File: main.c
 * Description: Shows BMP280 sensor data on HD44780 LCD
 * Author: Gavin Lyons.
 * IC: STM32F303
 * IDE:  STM32cubeIDE C++
 * Created Jan 2026
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HD44780_I2C_lcd.h" // user custom
#include "BMP280_STM32.h"    // user custom
#include <stdint.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INIT_DELAY 100
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void PrintSensorData(void);
void Setup(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char uart_buf[50];
int uart_buf_len;
BMP280_Handle_t bmp;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  Setup();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  PrintSensorData();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void Setup(void) {
    HAL_Delay(INIT_DELAY);
    uint8_t  chipId      = 0;

    // check LCD IS ON BUS connection
	// PCF8574_LCDInit(Cursor type , rows number , col number , I2C address, I2C handle)
    while (PCF8574_LCDInit(LCDCursorTypeOn, 2, 16, 0x27<<1, &hi2c1) != true) // Connection error loop until comes up
    {
    	// Print bad connection message to UART
    	uart_buf_len = sprintf(uart_buf,"LCD : Not connected? \r\n");
    	HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
    	HAL_Delay(5000);
    }

    // Print Good connection message to UART
    uart_buf_len = sprintf(uart_buf,"LCD : Connected \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
    HAL_Delay(50);

    PCF8574_LCDClearScreen();
    PCF8574_LCDBackLightSet(true);

    BMP280_DefaultConfig(&bmp, &hi2c1, BMP280_I2C_ADDR_SDO_LOW);
    while (BMP280_Init(&bmp) != BMP280_OK) {
    	// Print bad connection message to UART
    	uart_buf_len = sprintf(uart_buf,"BMP280 : Not connected? \r\n");
    	HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
    	HAL_Delay(5000);
        //Error_Handler();
    }
    BMP280_Status_e status = BMP280_ReadChipID(&bmp, &chipId);

    if (status != BMP280_OK)
    {
        uart_buf_len = sprintf((char*)uart_buf, "BMP280 : Chip ID read failure\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
        HAL_Delay(5000);
        Error_Handler();
    }
    else
    {
        uart_buf_len = sprintf((char*)uart_buf, "BMP280 : Connected\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
        HAL_Delay(50);

        uart_buf_len = sprintf((char*)uart_buf, "BMP280 : Chip ID = 0x%02X\r\n", chipId);
        HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
        HAL_Delay(50);
    }
}

void PrintSensorData(void) {

    PCF8574_LCDClearScreen();
    static uint8_t SensorCountRead = 0;
    int32_t  tempC   = 0;
    uint32_t pressPa = 0;
    BMP280_ReadAll(&bmp, &tempC, &pressPa);
    // Determine sign before splitting
    uint8_t  isNegative = (tempC < 0);
    int32_t  absTemp    = isNegative ? -tempC : tempC;

    int32_t  tempWhole  = absTemp / 100;
    int32_t  tempFrac   = absTemp % 100;

    // Convert: pressPa is in Pa → divide by 100 to get hPa
    uint32_t pressHpa = pressPa / 100;

    PCF8574_LCDGOTO(LCDLineNumberOne, 0);
    if (isNegative)
        PCF8574_LCDPrintf("Temp: -%ld.%02ld C", tempWhole, tempFrac);
    else
        PCF8574_LCDPrintf("Temp:  %ld.%02ld C", tempWhole, tempFrac);


    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDPrintf("Press: %lu hPa", pressHpa);

    HAL_Delay(DISPLAY_DELAY);
    uart_buf_len = sprintf((char*)uart_buf, "Sensor Read %u\r\n", SensorCountRead++);
    HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
    HAL_Delay(50);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
