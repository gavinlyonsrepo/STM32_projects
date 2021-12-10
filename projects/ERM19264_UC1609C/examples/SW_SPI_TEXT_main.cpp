/* USER CODE BEGIN Header */
/*
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609_library,
// multi buffer mode. Text exmaple
// *****************************
// NOTES :
   (1) Software SPI
   (2) test 7 8 9 require the repsective fonts commented in
    custom_graphics_font.h file FONT DEFINE SECTION
// ******************************
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ERM19264_UC1609.h"
#include <string.h>


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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void DisplayText(MultiBuffer* targetBuffer);
void TestReset(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
ERM19264_UC1609  mylcd(false);
#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. user adjust
#define mylcdheight 64
#define mylcdwidth  192
#define DisplayDelay1 5000
#define DisplayDelay2 0
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  uint8_t buf[12];
  mylcd.LCDbegin(VbiasPOT,&hspi1); // initialize the LCD
  mylcd.LCDFillScreen(0xE5, 0); // Fill the screen with a a pattern , "just for splashscreen effect"
  HAL_Delay(1500);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Optional
  HAL_GPIO_WritePin(GPIOA, LD2_Pin,GPIO_PIN_SET);
  strcpy((char*)buf,"Start\r\n");
  HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY ); //baud 38400


  while (1)
  {
	  uint8_t  textBuffer[(mylcdwidth * (mylcdheight / 8)) + 1]; // 192 X (64/8) + 1 = 1537
	  MultiBuffer window;
	  window.screenbitmap = (uint8_t*) &textBuffer;

	  // Call a function to display text
	  DisplayText(&window);
	  /* USER CODE END WHILE */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|CD_Pin|RESET_Pin|CS_Pin
                          |SWSPICLK_Pin|SWMOSI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin CD_Pin RESET_Pin CS_Pin
                           SWSPICLK_Pin SWMOSI_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|CD_Pin|RESET_Pin|CS_Pin
                          |SWSPICLK_Pin|SWMOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// A series of tests to display the text mode
// Test 1 Font size 3 string
// Test 2 font size 2 string
// Test 3 font size 1 string inverted
// Test 4 draw a single character font size 4
// Test 5 print ASCII  font 0-127
// Test 6 print ASCII font 128-255.
// Test 7 print font 2 3 4
// Test 8 print font 5 big num
// Test 9 print font 6 med num
void  DisplayText(MultiBuffer* targetBuffer)
{
	  char myString[9] = {'1', '3', ':', '2', '6', ':', '1', '8'};
	  mylcd.setTextWrap(true);
	  mylcd.ActiveBuffer =  targetBuffer;
	  mylcd.LCDclearBuffer(); // Clear the buffer

	  while (1)
	  {
	    // Test 1
		mylcd.setFontNum(1);
		mylcd.setTextColor(FOREGROUND);
	    mylcd.setTextSize(3);
	    mylcd.setCursor(0, 0);
	    mylcd.print("345.897");

	    // Test 2
	    mylcd.setTextSize(2);
	    mylcd.setCursor(0, 30);
	    mylcd.print("1234567890");

	    // Test 3
	    mylcd.setTextSize(1);
	    mylcd.setTextColor(BACKGROUND, FOREGROUND);
	    mylcd.setCursor(0, 50);
	    mylcd.print("HelloWorld");

	    // Test 4
	    mylcd.drawChar(150, 25 , 'H', FOREGROUND, BACKGROUND, 4);

	    TestReset();

	    // Test 5
	    mylcd.setCursor(0, 0);
	    mylcd.setTextColor(FOREGROUND);
	    mylcd.setTextSize(1);
	    mylcd.print("ASCII font 0-127");
	    mylcd.setCursor(0, 15);
	    // Print first 127 chars of font
	    for (char i = 0; i < 126; i++)
	    {
	      if (i == '\n' || i == '\r') continue;
	      mylcd.print(i);
	      HAL_Delay(DisplayDelay2);
	    }
	    TestReset();

	    // Test 6,
	    mylcd.setCursor(0, 0);
	    mylcd.setTextColor(FOREGROUND);
	    mylcd.setTextSize(1);
	    mylcd.print("ASCII font 128-255");

	    uint8_t x = 0;
	    uint8_t y = 15;
	    mylcd.setCursor(x, y);

	    for (uint8_t i = 128; i < 255; i++)
	    {
	      if (x > 180)
	      {
	        x = 0;
	        y += 9;
	      }
	      mylcd.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
	      x += 7;
	      HAL_Delay(DisplayDelay2);
	    }

	    TestReset();

	    // Test 7  fonts 2-4
	    mylcd.setTextSize(2);

	    mylcd.setFontNum(2);
		mylcd.setCursor(0, 0);
		mylcd.print("THICK");

	    mylcd.setFontNum(3);
		mylcd.setCursor(0, 20);
		mylcd.print("3354610");

	    mylcd.setFontNum(4);
		mylcd.setCursor(0, 40);
		mylcd.print("WIDE");

		TestReset();

	    // Test 8 font 5
	    mylcd.setFontNum(5);
	    mylcd.setTextColor(FOREGROUND, BACKGROUND);
	    mylcd.setCursor(80,0);
	    mylcd.print(859);
	    mylcd.drawTextNumFont(0, 32, myString , BACKGROUND, FOREGROUND); //7b drawTextNumFont , 13:26:18 inverted
	    mylcd.drawCharNumFont(0, 0, '8', FOREGROUND, BACKGROUND); // 7c drawCharNumFont
	    mylcd.drawCharNumFont(160, 0, '4', BACKGROUND, FOREGROUND); // 7d drawCharNumFont inverted

	    TestReset();

	    // Test 9 font 6
		mylcd.setFontNum(6);
		mylcd.setCursor(80,0);
		mylcd.print(993);
		mylcd.drawTextNumFont(0, 32, myString , BACKGROUND, FOREGROUND); // 8b drawTextNumFont , 13:26:18 inverted
		mylcd.drawCharNumFont(0, 0, '6', FOREGROUND, BACKGROUND); // 8c drawCharNumFont
		mylcd.drawCharNumFont(160, 0, '6', BACKGROUND, FOREGROUND); // 8d drawCharNumFont inverted

		TestReset();


	  } // while
}

void TestReset(void)
{
  mylcd.LCDupdate();  // Write to the buffer
  HAL_Delay(DisplayDelay1);
  mylcd.LCDclearBuffer();
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
