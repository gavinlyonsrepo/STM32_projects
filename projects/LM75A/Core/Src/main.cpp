/* USER CODE BEGIN Header */
/*
 * Project Name: Library for the LM75A temperature sensor by NXP and Texas Instruments.
 * File: main.c
 * Description: test file to show/test library functions
 * Author: Gavin Lyons.
 * IC: STM32F303
 * IDE:  STM32cubeIDE C++
 * Created DEC 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/STM32_projects
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <LM75A.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
// Only needed for LIB_TEST
void checkTrueValue(const char* caption, const bool value, const bool expected);

const char* getFaultQueueValueString(const FaultQueueValue value);
const char* getOsPolarityString(const OsPolarity value);
const char* getDeviceModeString(DeviceMode value);

void checkFaultQueueValue(const char* caption, const FaultQueueValue value, const FaultQueueValue expected);
void checkOsPolarityValue(const char* caption, const OsPolarity value, const OsPolarity expected);
void checkDeviceModeValue(const char* caption, const DeviceMode value, const DeviceMode expected);

void checkTemperatureResult(const char* caption, const float value);
void checkTemperatureValue(const char* caption, const float value, const float expected);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
LIB_LM75A lm75a(&hi2c1, LM75A_DEFAULT_ADDRESS << 1);
char uart_buf[50];
int uart_buf_len;
bool testResult = true;

#define LIB_BASIC
//#define LIB_TEST

#ifdef  LIB_BASIC
bool Temp_type_Celsius = true; //true for Celsius , False  for Farenheit
bool basic_usage_or_test = true; // true for basic , false for full library test
float temperature = 0.0;
bool is_shutdown = false;
#endif

#ifdef LIB_TEST
float oldHysterisisTemperature;
float oldOsTripTemperature;
FaultQueueValue oldFaultQueueValue;
OsPolarity oldOsPolarity;
DeviceMode oldDeviceMode;
float newHysterisisTemperature = 62.0f;
float newOsTripTemperature = 59.0f;
FaultQueueValue newFaultQueueValue = FaultQueueValue::NUMBER_OF_FAULTS_6;
OsPolarity newOsPolarity = OsPolarity::OS_POLARITY_ACTIVEHIGH;
DeviceMode newDeviceMode = DeviceMode::DEVICE_MODE_INTERRUPT;
#endif

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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

	uart_buf_len = sprintf(uart_buf, "\nLM75A Start\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
#ifdef  LIB_BASIC
		//Celsius
		if (Temp_type_Celsius == true) {
			temperature = lm75a.getTemperature();
			temperature *= 100;
			uart_buf_len = sprintf(uart_buf, "\nTemperature : %u.%u  *C \r\n",
					((unsigned int) temperature / 100),
					((unsigned int) temperature % 100));
			HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
		} else {
			//Farenheit
			temperature = lm75a.getTemperatureInFarenheit();
			temperature *= 1000;
			uart_buf_len = sprintf(uart_buf, "\nTemperature : %u.%u  *F \r\n",
					((unsigned int) temperature / 1000),
					((unsigned int) temperature % 100));
			HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
		}
		// Hysteris
		temperature = lm75a.getHysterisisTemperature();
		temperature *= 100;
		uart_buf_len = sprintf(uart_buf, "Hysteris temperature: %u.%u  *C \r\n",
				((unsigned int) temperature / 100),
				((unsigned int) temperature % 100));
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);

		// OS trip temperature
		temperature = lm75a.getOSTripTemperature();
		temperature *= 100;
		uart_buf_len = sprintf(uart_buf, "OS trip temperature: %u.%u  *C \r\n",
				((unsigned int) temperature / 100),
				((unsigned int) temperature % 100));
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);

		// Shutdown
		uart_buf_len = sprintf(uart_buf, "Shutting Down\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
		lm75a.shutdown();

		is_shutdown = lm75a.isShutdown();
		uart_buf_len = sprintf(uart_buf, "Is shutdown?: %u  \r\n",
				((unsigned int) is_shutdown));
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);

		HAL_Delay(5000);

		///Wake up
		uart_buf_len = sprintf(uart_buf, "Waking up\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
		lm75a.wakeup();

		is_shutdown = lm75a.isShutdown();
		uart_buf_len = sprintf(uart_buf, "Is shutdown?: %u  \r\n",
				((unsigned int) is_shutdown));
		HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);

		HAL_Delay(5000);

#endif

#ifdef  LIB_TEST
		  // Test 1 : Test comms
		  checkTrueValue("isConnected", lm75a.isConnected(), true);
		  HAL_Delay(2000);

		  // Test 2 testing temperature getters (4*)
		  checkTemperatureResult("getTemperature", lm75a.getTemperature());
		  checkTemperatureResult("getTemperatureInFarenheit", lm75a.getTemperatureInFarenheit());
		  checkTemperatureResult("getHysterisisTemperature", lm75a.getHysterisisTemperature());
		  checkTemperatureResult("getOSTripTemperature", lm75a.getOSTripTemperature());
		  HAL_UART_Transmit(&huart2, (uint8_t*)'\n', 1, 100);
		  // Test 3 Testing other getters
		  checkFaultQueueValue("getFaultQueueValue", lm75a.getFaultQueueValue(), FaultQueueValue::NUMBER_OF_FAULTS_1);
		  checkOsPolarityValue("getOsPolarity", lm75a.getOsPolarity(), OsPolarity::OS_POLARITY_ACTIVELOW);
		  checkDeviceModeValue("getDeviceMode", lm75a.getDeviceMode(), DeviceMode::DEVICE_MODE_COMPARATOR);
		  HAL_Delay(2000);

		  // Test 4 : Testing temperature setters
		  oldHysterisisTemperature = lm75a.getHysterisisTemperature();
		  oldOsTripTemperature = lm75a.getOSTripTemperature();

		  lm75a.setHysterisisTemperature(newHysterisisTemperature);
		  lm75a.setOsTripTemperature(newOsTripTemperature);

		  checkTemperatureValue("setHysterisisTemperature", lm75a.getHysterisisTemperature(), newHysterisisTemperature);
		  checkTemperatureValue("setOsTripTemperature", lm75a.getOSTripTemperature(), newOsTripTemperature);

		  lm75a.setHysterisisTemperature(oldHysterisisTemperature);
		  lm75a.setOsTripTemperature(oldOsTripTemperature);
		  HAL_Delay(2000);

		  // Test 5 "Testing other setters");
		  oldFaultQueueValue = lm75a.getFaultQueueValue();
		  oldOsPolarity = lm75a.getOsPolarity();
		  oldDeviceMode = lm75a.getDeviceMode();

		  lm75a.setFaultQueueValue(newFaultQueueValue);
		  lm75a.setOsPolarity(newOsPolarity);
		  lm75a.setDeviceMode(newDeviceMode);
		  checkFaultQueueValue("setFaultQueueValue: ", lm75a.getFaultQueueValue(), newFaultQueueValue);
		  checkOsPolarityValue("setOsPolarity", lm75a.getOsPolarity(), newOsPolarity);
		  checkDeviceModeValue("setDeviceMode", lm75a.getDeviceMode(), newDeviceMode);

		  lm75a.setFaultQueueValue(oldFaultQueueValue);
		  lm75a.setOsPolarity(oldOsPolarity);
		  lm75a.setDeviceMode(oldDeviceMode);
		  HAL_Delay(2000);
		  // Test 6
		  lm75a.shutdown();
		  checkTrueValue("shutdown", lm75a.isShutdown(), true);
		  HAL_Delay(2000);
		  lm75a.wakeup();
		  checkTrueValue("shutdown", lm75a.isShutdown(), false);
		  HAL_Delay(2000);
		  uart_buf_len = sprintf(uart_buf, "\nTest Result %u\r\n",(uint8_t)testResult);
		  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
		  while(1){}; // stay here test over
#endif

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

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
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
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
static void MX_USART2_UART_Init(void) {

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
	if (HAL_UART_Init(&huart2) != HAL_OK) {
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
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void checkTrueValue(const char* caption, const bool value, const bool expected)
{
  bool fail = value != expected;

  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %u FAIL \r\n\n", caption , (uint8_t)value);
  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %u: PASS\r\n\n", caption , (uint8_t)value);
  }

  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}

const char* getFaultQueueValueString(const FaultQueueValue value)
{
  switch (value)
  {
    case FaultQueueValue::NUMBER_OF_FAULTS_1:
      return "NUMBER_OF_FAULTS_1";
    case FaultQueueValue::NUMBER_OF_FAULTS_2:
      return "NUMBER_OF_FAULTS_2";
    case FaultQueueValue::NUMBER_OF_FAULTS_4:
      return "NUMBER_OF_FAULTS_4";
    case FaultQueueValue::NUMBER_OF_FAULTS_6:
      return "NUMBER_OF_FAULTS_4";
    default:
      return "** ILLEGAL VALUE **";
  }
}

const char* getOsPolarityString(const OsPolarity value)
{
  switch (value)
  {
    case OsPolarity::OS_POLARITY_ACTIVEHIGH:
      return "OS_POLARITY_ACTIVEHIGH";
    case OsPolarity::OS_POLARITY_ACTIVELOW:
      return "OS_POLARITY_ACTIVELOW";
    default:
      return "** ILLEGAL VALUE **";
  }
}

const char* getDeviceModeString(DeviceMode value)
{
  switch (value)
  {
    case DeviceMode::DEVICE_MODE_COMPARATOR:
      return "DEVICE_MODE_COMPARATOR";
    case DeviceMode::DEVICE_MODE_INTERRUPT:
      return "DEVICE_MODE_INTERRUPT";
    default:
      return "** ILLEGAL VALUE **";
  }
}

void checkTemperatureValue(const char* caption, const float value, const float expected)
{
  bool fail = value != expected;

  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %u: %u FAIL\r\n", caption , (uint8_t)value, (uint8_t)expected);
  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %u: PASS\r\n", caption , (uint8_t)value);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}

void checkTemperatureResult(const char* caption, const float value)
{
  bool fail = value == LM75A_INVALID_TEMPERATURE;
  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %u: FAIL LM75A_INVALID_TEMPERATURE\r\n", caption , (uint8_t)value);
  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %u: PASS\r\n", caption , (uint8_t)value);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}

void checkOsPolarityValue(const char* caption, const OsPolarity value, const OsPolarity expected)
{
  bool fail = value != expected;
  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %d: FAIL \r\n", caption , (uint8_t)expected);
  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %d: PASS\r\n", caption , (uint8_t)expected);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}

void checkDeviceModeValue(const char* caption, const DeviceMode value, const DeviceMode expected)
{
  bool fail = value != expected;

  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %d: FAIL Expected\r\n", caption , (uint8_t)expected);
  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %d: PASS \r\n\n", caption , (uint8_t)expected);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}

void checkFaultQueueValue(const char* caption, const FaultQueueValue value, const FaultQueueValue expected)
{
  bool fail = value != expected;
  if (fail)
  {
    testResult = false;
    uart_buf_len = sprintf(uart_buf, "%s : %d: FAIL Expected\r\n", caption , (uint8_t)expected);

  }
  else
  {
	  uart_buf_len = sprintf(uart_buf, "%s : %d: PASS \r\n", caption , (uint8_t)expected);
  }
  HAL_UART_Transmit(&huart2, (uint8_t*) uart_buf, uart_buf_len, 100);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
