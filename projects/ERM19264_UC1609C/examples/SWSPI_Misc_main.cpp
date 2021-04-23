/* USER CODE BEGIN Header */
/*
// Example file name : main.cpp
// Description:
// Test file for ERM19264_UC1609_library,
// multi buffer mode. Misc functions
// *****************************
// NOTES :
// (1) This is for software SPI 
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
ERM19264_UC1609  mylcd(false);
uint16_t count  = 0;
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
  HAL_GPIO_WritePin(GPIOA, LD2_Pin,GPIO_PIN_SET);
  strcpy((char*)buf,"Start\r\n");
  HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY ); //baud 38400


  while (1)
  {
	  // Define a full screen buffer and struct
	   uint8_t  screenBuffer[1537];

	   MultiBuffer whole_screen;
	   whole_screen.screenbitmap = (uint8_t*) &screenBuffer;
	   whole_screen.width = 192;
	   whole_screen.height = 64;
	   whole_screen.xoffset = 0;
	   whole_screen.yoffset = 0;

	   mylcd.ActiveBuffer =  &whole_screen; // set buffer object
	   mylcd.LCDclearBuffer(); // clear the buffer

	   // Set text parameters
	   mylcd.setTextColor(FOREGROUND);
	   mylcd.setTextSize(2);

	   // Test 1 LCD all pixels on
	     mylcd.setCursor(20, 30);
	     mylcd.print("All Pixels on");
	     mylcd.LCDupdate();
	     HAL_Delay(4000);
	     mylcd.LCDclearBuffer();
	     mylcd.LCDupdate();
	     mylcd.LCD_allpixelsOn(1);
	     HAL_Delay(2000);
	     mylcd.LCD_allpixelsOn(0);
	     HAL_Delay(2000);

	     // Test 2 inverse
	     mylcd.setCursor(20, 30);
	     mylcd.print("inverse test  ");
	     mylcd.LCDupdate();
	     mylcd.invertDisplay(0); // Normal
	     HAL_Delay(2000);
	     mylcd.invertDisplay(1); // Inverted
	     HAL_Delay(4000);
	     mylcd.invertDisplay(0);


	     // Test3 LCD rotate
	     mylcd.LCDclearBuffer();
	     mylcd.setCursor(20, 30);
	     mylcd.print("rotate test");
	     mylcd.LCDupdate();
	     HAL_Delay(2000);
	     mylcd.LCDrotate(UC1609_ROTATION_FLIP_ONE);
	     mylcd.LCDupdate();
	     HAL_Delay(5000);
	     mylcd.LCDrotate(UC1609_ROTATION_FLIP_TWO);
	     mylcd.LCDupdate();
	     HAL_Delay(5000);
	     mylcd.LCDrotate(UC1609_ROTATION_FLIP_THREE);
	     mylcd.LCDupdate();
	     HAL_Delay(5000);
	     mylcd.LCDrotate(UC1609_ROTATION_NORMAL);
	     mylcd.LCDupdate();
	     HAL_Delay(5000);


	   // Test4 LCD scroll
	   mylcd.LCDclearBuffer();
	   //mylcd.LCDupdate();
	   mylcd.setCursor(0, 40);
	   mylcd.print("scroll test");
	   for (uint8_t i = 0 ; i < 62 ; i ++)
	   {
	     mylcd.LCDscroll(i);
	     mylcd.LCDupdate();
	     HAL_Delay(50);
	   }
	   mylcd.LCDscroll(0);

	   //Test5 LCD enable and disable
	   mylcd.LCDclearBuffer();
	   mylcd.setCursor(0, 30);
	   mylcd.print("LCD Disable test");
	   mylcd.LCDupdate();
	   HAL_Delay(5000);
	   mylcd.LCDEnable(0);
	   HAL_Delay(5000);
	   mylcd.LCDEnable(1);
	   mylcd.LCDclearBuffer();
	   mylcd.setCursor(20, 30);
	   mylcd.print("End");
	   mylcd.LCDupdate();

	   while (1)
	   {
	     HAL_Delay(1); // tests over, loop here forever
	   }
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
// Function to display left hand side buffer

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
