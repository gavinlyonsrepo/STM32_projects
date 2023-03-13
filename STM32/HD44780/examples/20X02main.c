/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HD44780_I2C_lcd.h"            // user custom
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
void Setup(void);
void TestPrintf(void);
void HelloWorld(void);
void CursorMoveTest(void);
void ScrollTest(void);
void GotoTest(void);
void CursorTypeTest(void);
void EntryModeTest(void);
void CustomChar(void);
void BackLightTest(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	  TestPrintf(); // test numerical data
	  EntryModeTest(); // test entry mode commands
	  HelloWorld(); // test string + character data
	  CursorMoveTest(); //test cursor move
	  ScrollTest(); // test screen scroll

	  GotoTest(); // test cursor goto and clear line
	  CursorTypeTest(); // test reset + change cursor types
	  CustomChar(); // test custom character save and print
	  BackLightTest(); //test back light LED, on and off

	  HAL_Delay(DISPLAY_DELAY);
	  PCF8574_LCDClearScreen();
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
    
    // check LCD IS ON BUS connection
    // PCF8574_LCDInit(Cursor type , rows number , col number , I2C address, I2C handle)
    while (PCF8574_LCDInit(LCDCursorTypeOn, 4, 20, 0x27<<1, &hi2c1) != true) // Connection error loop until comes up
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
}

void TestPrintf(void) {
    uint16_t testOne = 62005;
    int16_t testTwo = -4501;
    float testThree = 3.143567;
    float testFour = -14.561;

    PCF8574_LCDGOTO(LCDLineNumberOne, 0);
    PCF8574_LCDPrintf("%u ", testOne);
    PCF8574_LCDPrintf("%d", testTwo);
    PCF8574_LCDGOTO(LCDLineNumberThree, 0);
    PCF8574_LCDPrintf("%.4f ", testThree);
    PCF8574_LCDPrintf("%.2f", testFour);

    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDClearScreen();
}

void EntryModeTest(void) {

    PCF8574_LCDSendCmd(LCDEntryModeOne);
    PCF8574_LCDGOTO(LCDLineNumberOne, 8);
    PCF8574_LCDSendString("1234"); // <-C4321
    HAL_Delay(DISPLAY_DELAY_2);
    PCF8574_LCDClearScreenCmd();

    PCF8574_LCDSendCmd(LCDEntryModeTwo);
    PCF8574_LCDGOTO(LCDLineNumberTwo, 8);
    PCF8574_LCDSendString("1234"); // C4321->
    HAL_Delay(DISPLAY_DELAY_2);
    PCF8574_LCDClearScreenCmd();

    PCF8574_LCDSendCmd(LCDEntryModeFour);
    PCF8574_LCDGOTO(LCDLineNumberTwo, 8);
    PCF8574_LCDSendString("1234"); // <-1234C
    HAL_Delay(DISPLAY_DELAY_2);
    PCF8574_LCDClearScreenCmd();

    PCF8574_LCDSendCmd(LCDEntryModeThree); // Set back to default entry mode
    PCF8574_LCDClearScreenCmd();
}

void HelloWorld(void) {
    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("Hello");
    PCF8574_LCDGOTO(LCDLineNumberThree, 0);
    PCF8574_LCDSendString("World"); // Display a string
    PCF8574_LCDSendChar('!'); // Display a single character
    HAL_Delay(DISPLAY_DELAY);
}

void CursorMoveTest(void) {
    PCF8574_LCDMoveCursor(LCDMoveRight, 2);
    HAL_Delay(DISPLAY_DELAY_2);
    PCF8574_LCDMoveCursor(LCDMoveLeft, 2);
    HAL_Delay(DISPLAY_DELAY_2);
}

void ScrollTest(void) {
    for (uint8_t i = 0; i < 5; i++) {
        PCF8574_LCDScroll(LCDMoveRight, 1);
        HAL_Delay(DISPLAY_DELAY_2);
    }
    PCF8574_LCDScroll(LCDMoveLeft, 5);
    HAL_Delay(DISPLAY_DELAY_2);
}

void GotoTest(void) {
	  PCF8574_LCDClearScreen();

	  char teststr1[] = "Line 1";
	  char teststr2[] = "Line 2";
	  char teststr3[] = "Line 3";
	  char teststr4[] = "Line 4";
	  char testchar = '!';
	  uint8_t columnPos = 0;
	  uint8_t rowPos = 1;

	  // Print a string to each line

	  PCF8574_LCDGOTO(LCDLineNumberOne, 0);
	  PCF8574_LCDSendString(teststr1);
	  PCF8574_LCDGOTO(LCDLineNumberTwo , 0);
	  PCF8574_LCDSendString(teststr2);
	  PCF8574_LCDGOTO(LCDLineNumberThree , 0);
	  PCF8574_LCDSendString(teststr3);
	  PCF8574_LCDGOTO(LCDLineNumberFour , 0);
	  PCF8574_LCDSendString(teststr4);
	  HAL_Delay(DISPLAY_DELAY_2);

	  PCF8574_LCDClearScreen();

	  // Print out ASCII table one character at a time in every position
	  // with a unique goto command
	  for (rowPos = 1; rowPos < 5; rowPos++)
	  {
		for (columnPos = 0 ;  columnPos <20 ; columnPos++)
		{
			PCF8574_LCDGOTO((LCDLineNumber_e)rowPos, columnPos);
			PCF8574_LCDSendChar(testchar++);
		} //Line 1
		HAL_Delay(DISPLAY_DELAY_2);
	  }
	  HAL_Delay(DISPLAY_DELAY_2);
}

void CursorTypeTest(void) {
    PCF8574_LCDResetScreen(LCDCursorTypeOnBlink); //type 4 cursor
    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("Cursor no 4");
    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDClearLine(LCDLineNumberTwo);

    PCF8574_LCDResetScreen(LCDCursorTypeOff); //type 1 cursor
    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("Cursor no 1");
    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDClearLine(LCDLineNumberTwo);

    PCF8574_LCDResetScreen(LCDCursorTypeBlink); //type 2 cursor
    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("Cursor no 2");
    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDClearLine(LCDLineNumberTwo);

    PCF8574_LCDResetScreen(LCDCursorTypeOn); // Back to initial state , type 3
    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("Cursor no 3");
    HAL_Delay(DISPLAY_DELAY_2);
}

void CustomChar(void) {

	uint8_t symbolData[8][8] = {
		    {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4}, // bell
			{0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0}, // note
		    {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0}, // clock
		    {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0}, // heart
		    {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0}, // duck
		    {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0}, //check
		    {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0}, // cross
		    {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4}, // return arrow
			};

	PCF8574_LCDClearScreen();

	// load the data into LCD memory
    for (uint8_t j = 0; j < 8; j++) {
    	PCF8574_LCDCreateCustomChar(j, symbolData[j]);
    }

    PCF8574_LCDGOTO(LCDLineNumberOne, 0);

    // Print out custom characters from LCD memory
    for (uint8_t i = 0; i < 8; i++) {
        PCF8574_LCDPrintCustomChar(i);
        PCF8574_LCDMoveCursor(LCDMoveRight, 1);
    }

    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDClearScreen();
}

void BackLightTest(void) {
    PCF8574_LCDBackLightSet(false);
    PCF8574_LCDGOTO(LCDLineNumberTwo, 1);
    PCF8574_LCDSendString("Backlight test");
    HAL_Delay(DISPLAY_DELAY);
    PCF8574_LCDBackLightSet(true);
    PCF8574_LCDClearScreen();

    PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
    PCF8574_LCDSendString("End of Tests");
    HAL_Delay(DISPLAY_DELAY);
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
