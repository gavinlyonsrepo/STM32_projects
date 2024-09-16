/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7735_TFT.h"
#include "ST7735_TFT_BITMAP_DATA.h"
#include <stdio.h> //printf
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// *********** Defines **************
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
bool bTestFPS = true;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */
void Setup(void);

void Test0(void); // Print out all five fonts, fonts must be enabled in header file for this to work
void Test1A(void); // defined 16-bit Colors, text
void Test1B(void); // print entire ASCII font 32 to 127
void Test1C(void); // inverted text
void Test1D(void); // test TFTprint
void Test2(void); // font sizes + character draw
void Test3(void); // pixels and lines
void Test4(void); // rectangles 
void Test4B(void); // rectangle buffer method
void Test5(void); // Circle
void Test6(void); // Triangles 
void Test7(void); // scroll
void Test8(void); // More shapes, media buttons graphic.
void Test9(void); // Rotate
void Test10(void); // change modes test 
void Test11(void); // custom char/icon  byte tall
void Test12(void); // two color bitmap Method one
void Test14(void); // two color bitmap method2
void Test15(void); // 16 _24 color bitmap
void TestFPS(void); // FPS test using timer16 set to 10Khz tick

void EndTest(void);
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
  MX_SPI1_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	Setup();
    Test0();
    Test1A();
    Test1B();
    Test1C();
    Test1D();
    Test2();
    Test3();
    Test4();
    Test4B();
    Test5();
    Test6();
    Test7();
    Test8();
    Test9();
    Test10();
    Test11();
    Test12();
    Test14();
    Test15();
    if (bTestFPS == true) TestFPS();
    EndTest();
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
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 4800-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, RESET_TFT_PC0_Pin|DC_TFT_PC1_Pin|CS_TFT_PC2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SWMOSI_TFT_PA0_Pin|SWCLK_TFT_PA1_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RESET_TFT_PC0_Pin DC_TFT_PC1_Pin CS_TFT_PC2_Pin */
  GPIO_InitStruct.Pin = RESET_TFT_PC0_Pin|DC_TFT_PC1_Pin|CS_TFT_PC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SWMOSI_TFT_PA0_Pin SWCLK_TFT_PA1_Pin LD2_Pin */
  GPIO_InitStruct.Pin = SWMOSI_TFT_PA0_Pin|SWCLK_TFT_PA1_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// Initialize the device

void Setup(void) 
{
    #ifdef TFT_SPI_HARDWARE
   		TFTSPIHWInitialize(&hspi1);
	#endif
    
 // ****** USER OPTION 1 Screen Setup ******  
	uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0 ; // 3, with manufacture tolerance/defects at edge of screen
	uint16_t TFT_PIXEL_WIDTH = 128;// Screen width in pixels
	uint16_t TFT_PIXEL_HEIGHT = 128; // Screen height in pixels
    
	TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_PIXEL_WIDTH  , TFT_PIXEL_HEIGHT);
    
// ******** USER OPTION 2 PCB_TYPE **************************
// init PCB type , pass enum 4 choices, see readme
	TFTInitPCBType(TFT_ST7735R_Red);
//**********************************************************

    TFTFontNum(TFTFont_Default);
    TFTfillScreen(ST7735_BLACK);
}

void EndTest(void)
{
    char testString[10];
    sprintf(testString, "V: %u", TFTgetLibVer());
	TFTfillScreen(ST7735_BLACK);
    TFTdrawText(5, 50, "Tests over", ST7735_GREEN, ST7735_BLACK, 2);
    TFTdrawText(5, 80, testString, ST7735_YELLOW, ST7735_BLACK, 2);

    while (1);
}

void Test0(void) {

	TFTFontNum(TFTFont_Default);
    TFTdrawText(0, 4, "Default 1", ST7735_WHITE, ST7735_BLACK, 2);
    TFTFontNum(TFTFont_Thick);
    TFTdrawText(0, 22, "THICK 2", ST7735_GREEN, ST7735_BLACK, 2);
    TFTFontNum(TFTFont_Seven_Seg);
    TFTdrawText(0, 42, "Seven 3", ST7735_BLUE, ST7735_BLACK, 2);
    TFTFontNum(TFTFont_Wide);
    TFTdrawText(0, 62, "WIDE 4", ST7735_CYAN, ST7735_BLACK, 2);
    TFTFontNum(TFTFont_Tiny);
    TFTdrawText(0, 82, "Tiny 5", ST7735_TAN, ST7735_BLACK, 2);
    TFTFontNum(TFTFont_Homespun);
    TFTdrawText(0, 102, "Home 6", ST7735_YELLOW, ST7735_BLACK, 2);
    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);

    TFTFontNum(TFTFont_Bignum);
    TFTdrawText2(2, 4, "-12.3", ST7735_WHITE, ST7735_BLACK);
    TFTFontNum(TFTFont_Mednum);
    TFTdrawText2(2, 40, "-14.9", ST7735_BLACK, ST7735_YELLOW);
    TFTFontNum(TFTFont_ArialRound);
    TFTdrawText2(2, 72, "Arial R", ST7735_GREEN, ST7735_BLACK);
    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);

    TFTFontNum(TFTFont_ArialBold);
    TFTdrawText2(2, 4, "Arial B", ST7735_WHITE, ST7735_BLACK);
    TFTFontNum(TFTFont_Mia);
    TFTdrawText2(2, 40, "Mia Font", ST7735_BLACK, ST7735_GREEN);
    TFTFontNum(TFTFont_Dedica);
    TFTdrawText2(2, 72, "Dedica Font", ST7735_GREEN, ST7735_BLACK);
    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);

    TFTFontNum(TFTFont_Default);
}

void Test1A(void) {
    TFTdrawText(0, 5, "WHITE", ST7735_WHITE, ST7735_BLACK, 1);
    TFTdrawText(0, 15, "BLUE", ST7735_BLUE, ST7735_BLACK, 1);
    TFTdrawText(0, 25, "RED", ST7735_RED, ST7735_BLACK, 1);
    TFTdrawText(0, 35, "GREEN", ST7735_GREEN, ST7735_BLACK, 1);
    TFTdrawText(0, 45, "CYAN", ST7735_CYAN, ST7735_BLACK, 1);
    TFTdrawText(0, 55, "MAGENTA", ST7735_MAGENTA, ST7735_BLACK, 1);
    TFTdrawText(0, 65, "YELLOW", ST7735_YELLOW, ST7735_BLACK, 1);
    TFTdrawText(0, 75, "ORANGE", ST7735_ORANGE, ST7735_BLACK, 1);
    TFTdrawText(0, 85, "GREY", ST7735_GREY, ST7735_BLACK, 1);
    TFTdrawText(0, 95, "TAN", ST7735_TAN, ST7735_BLACK, 1);
    TFTdrawText(0, 105, "BROWN", ST7735_BROWN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);
    TFTfillScreen(ST7735_BLACK);
}

void Test1B(void) {
    uint8_t row = 20;
    uint8_t col = 0;
    for (char i = 0x20; i < 126; i++) {
        TFTdrawChar(col, row, i, ST7735_GREEN, ST7735_BLACK, 1);
        col += 10;
        if (col > 100) {
            row += 10;
            col = 0;
        }
    }

    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);
}


void Test1C(void)
{
	// test invert font 1-6
	// Draw a rectangle before text to hide  padding gaps(or fill screen)
    TFTfillRect(0, 4, 100, 17, ST7735_YELLOW); 
    TFTdrawText(0, 5, "Invert 1", ST7735_RED, ST7735_YELLOW, 2);
    // Draw a rectangle to hide padding gaps(or fill screen)
    TFTfillRect(0, 24, 100, 17, ST7735_WHITE); 
    TFTdrawText(0, 25, "Invert 2", ST7735_BLACK, ST7735_WHITE, 2);

    // test invert font 7-12(no need for background)
    TFTFontNum(TFTFont_ArialBold);
    TFTdrawText2(0, 65, "Invert 3", ST7735_BLACK, ST7735_WHITE);
    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);
    TFTFontNum(TFTFont_Default);

}
    

void Test1D(void)
{
	uint16_t myPosInt = 765;
	int16_t  myNegInt = -12;
	float myFloat = 3.14422;

    TFTPrintf(5, 5, ST7735_WHITE, ST7735_BLACK, 1, "Pos= %u",myPosInt);
    TFTPrintf(5, 45, ST7735_WHITE, ST7735_BLACK, 2, "Neg= %d",myNegInt);
    TFTPrintf(5, 85, ST7735_WHITE, ST7735_BLACK, 1, "float %.2f" ,myFloat);

    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);

    TFTFontNum(TFTFont_Dedica);
    TFTPrintf(5, 5, ST7735_WHITE, ST7735_BLACK, 0, "Pos num %u",myPosInt);
    TFTPrintf(5, 45, ST7735_WHITE, ST7735_BLACK, 0, "Neg num %d",myNegInt);
    TFTPrintf(5, 85, ST7735_WHITE, ST7735_BLACK, 0, "float %.2f" ,myFloat);
    HAL_Delay(TEST_DELAY5);
    TFTfillScreen(ST7735_BLACK);

    TFTFontNum(TFTFont_Default);
}

void Test2(void) {
    char *txttwo = "TEST";
    TFTdrawText(0, 5, txttwo, ST7735_WHITE, ST7735_BLACK, 2);
    TFTdrawText(0, 25, txttwo, ST7735_WHITE, ST7735_BLACK, 3);
    TFTdrawText(0, 55, txttwo, ST7735_WHITE, ST7735_BLACK, 4);
    TFTdrawChar(0, 85, 'G', ST7735_WHITE, ST7735_BLACK, 5);
    TFTdrawChar(45, 85, 'L', ST7735_WHITE, ST7735_BLACK, 5);

    HAL_Delay(TEST_DELAY2);
    TFTfillScreen(ST7735_BLACK);
}

void Test3(void)  
{
    TFTdrawPixel(85, 5, ST7735_WHITE);
    TFTdrawPixel(87, 7, ST7735_WHITE);
    TFTdrawPixel(89, 9, ST7735_WHITE);
    TFTdrawLine(10, 10, 30, 30, ST7735_RED);
    TFTdrawFastVLine(40, 40, 40, ST7735_GREEN);
    TFTdrawFastHLine(60, 60, 40, ST7735_YELLOW);

    HAL_Delay(TEST_DELAY2);
    TFTfillScreen(ST7735_BLACK);
}

void Test4(void) {
    TFTdrawRectWH(5, 5, 20, 20, ST7735_RED);
    TFTfillRectangle(45, 5, 20, 20, ST7735_YELLOW);
    TFTfillRect(85, 5, 20, 20, ST7735_GREEN);
    TFTdrawRoundRect(15, 60, 50, 50, 5, ST7735_CYAN);
    TFTfillRoundRect(70, 60, 50, 50, 10, ST7735_WHITE);

    HAL_Delay(TEST_DELAY2);
    TFTfillScreen(ST7735_BLACK);
}

void Test4B(void)
{
	TFTfillRectangleBuffer(0, 0, 128, 128, ST7735_RED);
	HAL_Delay(TEST_DELAY1);
	TFTfillRectangleBuffer(0, 0, 128, 128, ST7735_GREEN);
	HAL_Delay(TEST_DELAY1);
	TFTfillRectangleBuffer(0, 0, 128, 128, ST7735_BLUE);
	HAL_Delay(TEST_DELAY1);
	TFTfillRectangleBuffer(20, 20, 20, 40, ST7735_BLACK);
	TFTfillRectangleBuffer(80, 20, 20, 20, ST7735_BLACK);
	HAL_Delay(TEST_DELAY5);
	TFTfillScreenBuffer(ST7735_BLACK);
}

void Test5(void) {
    TFTdrawCircle(40, 20, 15, ST7735_GREEN);
    TFTfillCircle(80, 20, 15, ST7735_YELLOW);
}

void Test6(void) {
    TFTdrawTriangle(5, 80, 50, 40, 95, 80, ST7735_CYAN);
    TFTfillTriangle(55, 120, 100, 90, 127, 120, ST7735_RED);

    HAL_Delay(TEST_DELAY2);
    TFTfillScreen(ST7735_BLACK);
}


void Test7(void) 
{
  const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
  
  for (uint8_t i = 0; i < LINES; i++)
  {
    TFTdrawText(0, LINE_OFFSET+i*LINE_SIZE,"Scroll test", ST7735_WHITE, ST7735_BLACK, 1);
  }
  TFTsetScrollDefinition(TOP_FIXED,BOTTOM_FIXED,1);  // bottom-to-top
  uint8_t pos = LINE_OFFSET;
  for (uint8_t i = 0; i < LINES; i++) 
  {
    for (uint8_t j = 0; j < LINE_SIZE; j++) 
    {
      TFTVerticalScroll(pos + TOP_FIXED);
      pos++;
      // check pos if necessary: must be < tft_height - TOP_FIXED - BOTTOM_FIXED
      HAL_Delay(25);
    }
    HAL_Delay(TEST_DELAY1);
  }
  TFTchangeMode(ST7735_modes_Normal); 
  TFTfillScreen(ST7735_BLACK);
}

void Test8() 
{
    //Draw play button 
    TFTfillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
    TFTfillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
    TFTdrawText(5, 80, "Press Play", ST7735_GREEN, ST7735_BLACK, 2);
    HAL_Delay(TEST_DELAY5);
    
    // change play color
    TFTfillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
    HAL_Delay(TEST_DELAY1);
    // change play color
    TFTfillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
    HAL_Delay(TEST_DELAY1);
}

void Test9()
{
    TFTfillScreen(ST7735_BLACK);
    TFTsetRotation(ST7735_Degrees_0);
    TFTdrawText(20, 20, "Rotate 0", ST7735_YELLOW, ST7735_BLACK, 1);
    TFTdrawText(20, 100, "Rotate 0", ST7735_YELLOW, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);

    TFTfillScreen(ST7735_BLACK);
    TFTsetRotation(ST7735_Degrees_90);
    TFTdrawText(20, 20, "Rotate 90", ST7735_YELLOW, ST7735_BLACK, 1);
    TFTdrawText(20, 100, "Rotate 90", ST7735_YELLOW, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);
    
    TFTfillScreen(ST7735_BLACK);
    TFTsetRotation(ST7735_Degrees_180);
    TFTdrawText(20, 20, "Rotate 180", ST7735_YELLOW, ST7735_BLACK, 1);
    TFTdrawText(20, 100, "Rotate 180", ST7735_YELLOW, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);
    
    TFTfillScreen(ST7735_BLACK);
    TFTsetRotation(ST7735_Degrees_270);
    TFTdrawText(20, 20, "Rotate 270", ST7735_YELLOW, ST7735_BLACK, 1);
    TFTdrawText(20, 100, "Rotate 270", ST7735_YELLOW, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);
    
    TFTsetRotation(ST7735_Degrees_0);
    TFTchangeMode(ST7735_modes_Normal);
    TFTfillScreen(ST7735_BLACK);
}

void Test10()
{
     TFTfillRoundRect(25, 10, 78, 60, 8, ST7735_YELLOW);
     TFTdrawText(30, 80, "Mode test", ST7735_WHITE, ST7735_RED, 1);
     HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_Normal);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_Invert);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_Normal);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_DisplayOff);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_DisplayOn);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_Sleep);
	 HAL_Delay(TEST_DELAY2);
	 TFTchangeMode(ST7735_modes_Normal);
}

void Test11(void)
{
    TFTfillScreen(ST7735_BLACK);
    //power icon
    const unsigned char power[12] = {0xff, 0xe7, 0xc3, 0x99, 0xa5, 0xad, 0xad, 0xa5, 0x99, 0xc3, 0xe7, 0xff};
    //lighting symbol
    const unsigned char myspeed[12] = {0xff, 0xff, 0xf7, 0xb3, 0xd1, 0xc0, 0xe0, 0xf4, 0xf6, 0xfe, 0xff, 0xff}; 
    
    TFTdrawIcon(10, 20, 12, ST7735_WHITE , ST7735_BLACK,power);
    TFTdrawIcon(10, 50, 12, ST7735_RED, ST7735_YELLOW, myspeed);
    TFTdrawText(0, 90, "Custom chars", ST7735_GREEN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY5);
      
}

void Test12(void)
{
#ifdef TFT_BITMAP_BICOLOUR
 extern const uint8_t * pBackUpIconptr; // defined in ST7735_TFT_BITMAPDATA.c

 TFTdrawBitmap(0, 0, 128 , 128, ST7735_WHITE , ST7735_GREEN, pBackUpIconptr, 128*(128/8));
 HAL_Delay(TEST_DELAY5);
 TFTfillScreen(ST7735_BLACK);
#endif
}

void Test14(void)
{
#ifdef TFT_BITMAP_BICOLOUR
extern const uint8_t * pBackUpIconptr; // defined in ST7735_TFT_BITMAPDATA.c

TFTdrawBitmapBuffer(0, 0, 128 , 128, ST7735_WHITE , ST7735_GREEN, pBackUpIconptr,  128*(128/8));
HAL_Delay(TEST_DELAY5);
TFTfillScreen(ST7735_BLACK);
#endif

}

void Test15(void)
{
#ifdef TFT_BITMAP_16COLOUR // 16 bit colour bitmap test
extern const uint8_t * pMotorptr; // defined in ST7735_TFT_BITMAPDATA.c

TFTdrawBitmap1624Buffer(0, 0, 128 , 128, pMotorptr, TFT_BMP_16);
HAL_Delay(TEST_DELAY5);
TFTfillScreen(ST7735_BLACK);
#endif

#ifdef TFT_BITMAP_24COLOUR // 24 bit colour bitmap test
extern const uint8_t * pFruitBowlptr; // defined in ST7735_TFT_BITMAPDATA.c

TFTdrawBitmap1624Buffer(20, 20, 64, 64, pFruitBowlptr, TFT_BMP_24);
HAL_Delay(TEST_DELAY5);
TFTfillScreen(ST7735_BLACK);
#endif
}



void TestFPS(void)
{
	char uart_buf[50];
	int uart_buf_len;
	uint16_t timer_val;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count  = 0;
	uint16_t seconds  = 0;
	uint16_t fps = 0;
	uint16_t shapeColor = 0x2222;

    uart_buf_len = sprintf(uart_buf, "FPS TEST \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
   // Start timer
	HAL_TIM_Base_Start(&htim16);
	// Get current time (microseconds)
	timer_val = __HAL_TIM_GET_COUNTER(&htim16);

	 while(1)
	{
		 // If enough time has passed (1 second), toggle LED and get new timestamp
		  // 10 KHz = .0001seconds .1 mS  * 10000 = 1 second . max 65000 6.5 seconds.
		if (__HAL_TIM_GET_COUNTER(&htim16) - timer_val >= 10000)
		{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // onboard LED toggle
				timer_val = __HAL_TIM_GET_COUNTER(&htim16);
				fps = currentFramerate - lastFramerate;
				lastFramerate = currentFramerate ;
				seconds++;
				shapeColor = rand() % 60000;
				if (seconds == 10)break; // end after 10 seconds
		}
		currentFramerate++;
		count++;
		//  ** Code to test here **
		//TFTdrawBitmapBuffer(0, 0, 128 , 128, ST7735_WHITE , ST7735_GREEN, pBackUpIconptr);
		TFTfillRectangleBuffer(0, 0, 128, 128, shapeColor);
		//TFTfillScreen(shapeColor);
		// ***************************
	}
	uart_buf_len = sprintf(uart_buf, "Seconds :: %u, Count :: %u, FPS :: %u\r\n", seconds,count,fps);
	HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);


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
