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
void Test0(void); // Print out all five fonts, fonts must be enabled in header file for this to work
void Test1A(void); // defined 16-bit Colors, text
void Test1B(void); // print entire ASCII font 32 to 127
void Test2(void); // font sizes + character draw
void Test3(void); // pixels and lines
void Test4(void); // rectangles
void Test5(void); // Circle
void Test6(void); // Triangles
void Test7(void); // scroll
void Test8(void); // More shapes, media buttons graphic.
void Test9(void); // Rotate
void Test10(void); // change modes test
void Test11(void); // custom char/icon  byte tall
void Test12(void); // two color bitmap
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
  /* USER CODE BEGIN 2 */
   HAL_Delay(TEST_DELAY2);
#ifdef TFT_SPI_HARDWARE
   SPIHWInitialize(&hspi1);
#endif

#ifdef TFT_PCB_RED
   TFT_RedTab_Initialize();
#endif
#ifdef TFT_PCB_GREEN
   TFT_GreenTab_Initialize();
#endif
#ifdef TFT_PCB_BLACK
   TFT_BlackTab_Initialize();
#endif
#ifdef  TFT_PCB_GENERIC
   TFT_ST7735B_Initialize();
#endif



   setRotation(0);
   setTextWrap(true);
   TFTFontNum(1);
   HAL_Delay(TEST_DELAY1);
   fillScreen(ST7735_BLACK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      Test0();
      Test1A();
      Test1B();
      Test2();
      Test3();
      Test4();
      Test5();
      Test6();
      Test7();
      Test8();
      Test9();
      Test10();
      Test11();
      Test12();

      fillScreen(ST7735_BLACK);
      drawText(5, 50, "Tests over", ST7735_GREEN, ST7735_BLACK, 2);
      while (1)
      {
    	  HAL_Delay(1000); // sit here forever!
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
void Test0(void) {

    TFTFontNum(1);
    drawText(0, 5, "Default 1", ST7735_WHITE, ST7735_BLACK, 2);
    TFTFontNum(2);
    drawText(0, 25, "THICK 2", ST7735_GREEN, ST7735_BLACK, 2);
    TFTFontNum(3);
    drawText(0, 45, "Seven 3", ST7735_BLUE, ST7735_BLACK, 2);
    TFTFontNum(4);
    drawText(0, 65, "WIDE 4", ST7735_CYAN, ST7735_BLACK, 2);
    TFTFontNum(5);
    drawText(0, 85, "Tiny 5", ST7735_TAN, ST7735_BLACK, 2);
    HAL_Delay(TEST_DELAY5);
    fillScreen(ST7735_BLACK);
     TFTFontNum(1);
}

void Test1A(void) {
    char *txt = "WHITE";
    drawText(0, 5, txt, ST7735_WHITE, ST7735_BLACK, 1);
    drawText(0, 15, "BLUE", ST7735_BLUE, ST7735_BLACK, 1);
    drawText(0, 25, "RED", ST7735_RED, ST7735_BLACK, 1);
    drawText(0, 35, "GREEN", ST7735_GREEN, ST7735_BLACK, 1);
    drawText(0, 45, "CYAN", ST7735_CYAN, ST7735_BLACK, 1);
    drawText(0, 55, "MAGENTA", ST7735_MAGENTA, ST7735_BLACK, 1);
    drawText(0, 65, "YELLOW", ST7735_YELLOW, ST7735_BLACK, 1);
    drawText(0, 75, txt, ST7735_WHITE, ST7735_BLACK, 1);
    drawText(0, 85, "GREY", ST7735_GREY, ST7735_BLACK, 1);
    drawText(0, 95, "TAN", ST7735_TAN, ST7735_BLACK, 1);
    drawText(0, 105, "BROWN", ST7735_BROWN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY2);
    fillScreen(ST7735_BLACK);
}

void Test1B(void) {
    uint8_t row = 5;
    uint8_t col = 0;
    for (char i = TFT_ASCII_OFFSET; i < 126; i++) {
        drawChar(col, row, i, ST7735_GREEN, ST7735_BLACK, 1);
        col += 10;
        if (col > 100) {
            row += 9;
            col = 0;
        }
        HAL_Delay(25);
    }

    HAL_Delay(TEST_DELAY5);
    fillScreen(ST7735_BLACK);
}

void Test2(void) {
    char *txttwo = "TEST";
    drawText(0, 5, txttwo, ST7735_WHITE, ST7735_BLACK, 2);
    drawText(0, 25, txttwo, ST7735_WHITE, ST7735_BLACK, 3);
    drawText(0, 55, txttwo, ST7735_WHITE, ST7735_BLACK, 4);
    drawChar(0, 85, 'G', ST7735_WHITE, ST7735_BLACK, 5);
    drawChar(45, 85, 'L', ST7735_WHITE, ST7735_BLACK, 5);

    HAL_Delay(TEST_DELAY2);
    fillScreen(ST7735_BLACK);
}

void Test3(void)
{
    drawPixel(85, 5, ST7735_WHITE);
    drawPixel(87, 7, ST7735_WHITE);
    drawPixel(89, 9, ST7735_WHITE);
    drawLine(10, 10, 30, 30, ST7735_RED);
    drawFastVLine(40, 40, 40, ST7735_GREEN);
    drawFastHLine(60, 60, 40, ST7735_YELLOW);

    HAL_Delay(TEST_DELAY2);
    fillScreen(ST7735_BLACK);
}

void Test4(void) {
    drawRectWH(5, 5, 20, 20, ST7735_RED);
    fillRectangle(45, 5, 20, 20, ST7735_YELLOW);
    fillRect(85, 5, 20, 20, ST7735_GREEN);
    drawRoundRect(15, 60, 50, 50, 5, ST7735_CYAN);
    fillRoundRect(70, 60, 50, 50, 10, ST7735_WHITE);

    HAL_Delay(TEST_DELAY2);
    fillScreen(ST7735_BLACK);
}

void Test5(void) {
    drawCircle(40, 20, 15, ST7735_GREEN);
    fillCircle(80, 20, 15, ST7735_YELLOW);
}

void Test6(void) {
    drawTriangle(5, 80, 50, 40, 95, 80, ST7735_CYAN);
    fillTriangle(55, 120, 100, 90, 127, 120, ST7735_RED);

    HAL_Delay(TEST_DELAY2);
    fillScreen(ST7735_BLACK);
}


void Test7(void)
{
  const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;

  for (uint8_t i = 0; i < LINES; i++)
  {
    drawText(0, LINE_OFFSET+i*LINE_SIZE,"Scroll test", ST7735_WHITE, ST7735_BLACK, 1);
  }
  setScrollDefinition(TOP_FIXED,BOTTOM_FIXED,1);  // bottom-to-top
  uint8_t pos = LINE_OFFSET;
  for (uint8_t i = 0; i < LINES; i++)
  {
    for (uint8_t j = 0; j < LINE_SIZE; j++)
    {
      VerticalScroll(pos + TOP_FIXED);
      pos++;
      // check pos if necessary: must be < tft_height - TOP_FIXED - BOTTOM_FIXED
      HAL_Delay(100);
    }
    HAL_Delay(TEST_DELAY2);
  }
  changeMode(NORMAL);
  fillScreen(ST7735_BLACK);
}

void Test8()
{
    //Draw play button
    fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
    fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
    drawText(5, 80, "Press Play", ST7735_GREEN, ST7735_BLACK, 2);
    HAL_Delay(TEST_DELAY5);

    // change play color
    fillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
    HAL_Delay(TEST_DELAY1);
    // change play color
    fillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
    HAL_Delay(TEST_DELAY1);
}

void Test9()
{
    fillScreen(ST7735_BLACK);
    setRotation(0);
    drawText(20, 20, "Rotate 0", ST7735_GREEN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY5);

    fillScreen(ST7735_BLACK);
    setRotation(1);
    drawText(20, 20, "Rotate 1", ST7735_GREEN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY5);

    fillScreen(ST7735_BLACK);
    setRotation(2);
    drawText(20, 20, "Rotate 2", ST7735_GREEN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY5);

    fillScreen(ST7735_BLACK);
    setRotation(3);
    drawText(20, 20, "Rotate 3", ST7735_GREEN, ST7735_BLACK, 1);
    HAL_Delay(TEST_DELAY5);

    setRotation(0);
    changeMode(NORMAL);
    fillScreen(ST7735_BLACK);
}

void Test10()
{
    fillRoundRect(25, 10, 78, 60, 8, ST7735_YELLOW);
    drawText(30, 80, "Mode test", ST7735_WHITE, ST7735_RED, 1);
    HAL_Delay(TEST_DELAY2);
     changeMode(NORMAL);
     HAL_Delay(TEST_DELAY2);
     changeMode(INVERT);
     HAL_Delay(TEST_DELAY5);
     changeMode(NORMAL);
     HAL_Delay(TEST_DELAY2);
     changeMode(DISP_OFF);
     HAL_Delay(TEST_DELAY5);
     changeMode(DISP_ON);
     HAL_Delay(TEST_DELAY2);
     changeMode(SLEEP);
     HAL_Delay(TEST_DELAY5);
     changeMode(NORMAL);
}

void Test11(void)
{
    fillScreen(ST7735_BLACK);
    //power icon vertically addressed
    const unsigned char power[12] = {0xff, 0xe7, 0xc3, 0x99, 0xa5, 0xad, 0xad, 0xa5, 0x99, 0xc3, 0xe7, 0xff};
    //lighting symbol vertically addressed
    const unsigned char myspeed[12] = {0xff, 0xff, 0xf7, 0xb3, 0xd1, 0xc0, 0xe0, 0xf4, 0xf6, 0xfe, 0xff, 0xff};

    drawIcon(10, 20, 12, ST7735_WHITE , ST7735_BLACK,power);
    drawIcon(10, 50, 12, ST7735_RED, ST7735_YELLOW, myspeed);
    drawText(0, 90, "Custom chars", ST7735_BLUE, ST7735_BLACK, 1);
      HAL_Delay(TEST_DELAY5);

}

void Test12(void)
{
// 'backupicon', 128x128px , icon from https://github.com/gavinlyonsrepo/backupmenu
// 128 * (128/8) = 2048 // fullscreen bitmap bi-colour horizontally addressed
const unsigned char mybitmap[2048] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0xc0, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

drawBitmap(0, 0, _width , _height, ST7735_WHITE , ST7735_GREEN, mybitmap);

 HAL_Delay(TEST_DELAY5);
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
