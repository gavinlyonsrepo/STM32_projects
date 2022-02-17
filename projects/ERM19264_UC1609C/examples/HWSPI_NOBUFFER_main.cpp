/* USER CODE BEGIN Header */
/*
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609_library,
// no buffer mode.
// *****************************
// NOTES :
// (1) This is for hardware SPI. 
// (2) no buffer mode must be selected in llibrary header file
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

ERM19264_UC1609  mylcd(true);
#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. user adjust

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
  uint8_t buf[12]; // Optional for com port
  mylcd.LCDbegin(VbiasPOT,&hspi1); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // clear screen
  HAL_Delay(500);
  /* USER CODE END 2 */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin,GPIO_PIN_SET); // Optional Set onboard LED on
  strcpy((char*)buf,"Start\r\n"); // Optional for com port
  HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY ); // OPtional for com port

  unsigned char test0_1[] = "No buffer mode 216 chars";
  unsigned char test0_2[] = "(192/7 * 64/8)= 216";
  unsigned char test1_1[] = "This is text only version";
  unsigned char test1_2[] = "123456789012345678901234567";
  unsigned char test1_3[] = "This is page 7";
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Test 0 clear screen
	  mylcd.LCDNoBufferGotoXY(0, 0);
	  mylcd.LCDNoBufferString(test0_1); // Write a character string
	  mylcd.LCDNoBufferGotoXY(0, 1);
	  mylcd.LCDNoBufferString(test0_2); // Write a character string
	  HAL_Delay(5000);
	  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
	  HAL_Delay(50);

	  // Test 1 String function and goto function
	  mylcd.LCDNoBufferGotoXY(0, 0); // (Col , page ) Col 0 to 191 , page 0 to 7
	  mylcd.LCDNoBufferString(test1_1); // Write a character string
	  mylcd.LCDNoBufferGotoXY(0, 4);
	  mylcd.LCDNoBufferString(test1_2);
	  mylcd.LCDNoBufferGotoXY(0, 7);
	  mylcd.LCDNoBufferString(test1_3);
	  HAL_Delay(5000);

	  // Test 2 clear page function
	  mylcd.LCDNoBufferGotoXY(0, 7);
	  mylcd.LCDFillPage(0x00); // Clear page
	  mylcd.LCDNoBufferGotoXY(0, 6);
	  mylcd.LCDFillPage(0x7E); // Write pattern (0111 1110) to a page
	  HAL_Delay(5000);
	  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

	  // Test 3 character function
	  mylcd.LCDNoBufferGotoXY(100, 2);
	  mylcd.LCDNoBufferChar('H');  // write single  character
	  mylcd.LCDNoBufferChar('2');
	  HAL_Delay(5000);
	  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

	  // Test 4 print entire font with character function
	  // For characters after 'z{|}' in ASCII table user can comment out UC_FONT_MOD_TWO in font file (NOTE: this will increase program size)
	  mylcd.LCDNoBufferGotoXY(0, 0);
	  uint8_t row = 1;
	  for(char i=1; i < 126 ; i++)
	  {
		if (i % 27 == 0) mylcd.LCDNoBufferGotoXY(0, row++);
		mylcd.LCDNoBufferChar(i);
		HAL_Delay(50);
	  }
	  HAL_Delay(10000);
	  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

	  // TEST 5 print ASCII font 128-255 with character function
	  // For characters after 'z{|}' in ASCII table user can comment in UC_FONT_MOD_TWO in font file
	  // (NOTE: this will increase program size)
	  mylcd.LCDNoBufferGotoXY(0, 0);
	  row = 1;
	  unsigned char k = 1;
	  for (k = 128; k < 255 ; k++)
	  {
	    if (k % 27 == 0) mylcd.LCDNoBufferGotoXY(0, row++);
	    mylcd.LCDNoBufferChar(k);
	    HAL_Delay(50);
	  }
	  HAL_Delay(10000);
	  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
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
