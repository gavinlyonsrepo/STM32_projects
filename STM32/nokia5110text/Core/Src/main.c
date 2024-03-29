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
#include "NOKIA5110_TEXT.h"


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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void NokiaSetup(void);
void TestPrint(void);
void TestGeneral(void); // General tests, default font , clear and fill , draw, custom char
void TestFont1to6(void); // For testing fonts 1-6  Comment in  defines at top of  NOKIA5110_TEXT.h if using non default
void TestFont7to9(void); // for testing fonts 7-9
void TestBitmap(void); // bitmap test
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define myinverse  false
#define mycontrast 0xB1 // default is 0xBF set in LCDinit, Try 0xB1 to 0xBF if your display is too dark
#define mybias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

#define TEST_DELAY 2000

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
  /* USER CODE BEGIN 2 */
  NokiaSetup();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(TEST_DELAY);

	  TestPrint();
	  TestGeneral();
	  TestFont1to6(); // Comment in to test , read notes in function header
      TestFont7to9(); //Comment in to test , read notes in function header
      TestBitmap(); //Comment in to test , read notes in function header

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SWCLKB3_Pin|SWDATA_Pin|DC_Pin|CE_Pin
                          |Reset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SWCLKB3_Pin SWDATA_Pin DC_Pin CE_Pin
                           Reset_Pin */
  GPIO_InitStruct.Pin = SWCLKB3_Pin|SWDATA_Pin|DC_Pin|CE_Pin
                          |Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void NokiaSetup(void)
{
	LCDInit(myinverse, mycontrast, mybias); // init the lCD
	LCDClear(0x00); // Clear screen
}

//****************************** Test Print function ****************************

void TestPrint(void)
{
	uint16_t myPosInt = 765;
	int16_t  myNegInt = -112;

	LCDgotoXY(0, 0);
    LCDPrintf("pos num %u",myPosInt );

	LCDgotoXY(0, 1);
    LCDPrintf("neg num %d",myNegInt);

    HAL_Delay(5000);
    LCDClear(0x00);
}

// **********************************  Text and  function tests *****************

void TestGeneral(void) {
    /* TESTS
     1. TEST 1a writing text to blocks
     2. TEST 1b clearing by  blocks
     3. TEST 1c sleep mode
     4. TEST 1d Draw some pixel
     5. TEST 1e Draw a horizontal line with SetPixel across screen at row 10
     6. TEST 1f Draw a vertical line in block 2 of one byte at column 3
     7. TEST 1g Fill Screen
     8. TEST 1h Fill block
     9. TEST 1i Custom Character
     */

    //Custom Characters for  TEST 1i Custom Character
    const unsigned char power[12] = {0xff, 0xe7, 0xc3, 0x99, 0xa5, 0xad, 0xad, 0xa5, 0x99, 0xc3, 0xe7, 0xff}; //power icon
    const unsigned char myspeed[12] = {0xff, 0xff, 0xf7, 0xb3, 0xd1, 0xc0, 0xe0, 0xf4, 0xf6, 0xfe, 0xff, 0xff}; //lighting symbol

    bool drawtext = true; //enable  TEST 1a
    bool clearline = true; //enable  TEST 1b
    bool sleepmodetest = true; // enable TEST 1c
    bool pixeltest = true; // enable TEST d e f

    //TEST 1a writing text to blocks
    if (drawtext == true) {
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(0, 0); // (go to (X , Y) (0-84 columns, 0-5 blocks)
        LCDString("Block 0"); //print to block 0 (0-5 blocks or row bytes)
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(2, 1);
        LCDString("Block 1"); //print to block 1 indented by two
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(4, 2);
        LCDString("Block 2"); //print to block 2   indented by four
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(6, 3);
        LCDString("Block 3"); //print to block 3  indented by six
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(8, 4);
        LCDString("Block 4"); //print to block 4 indented by 8 col
        HAL_Delay(TEST_DELAY);
        LCDgotoXY(10, 5);
        LCDString("Block 5"); //print to block 5 indented by 10 col
        HAL_Delay(TEST_DELAY);
    }


    // TEST 1b clearing by  blocks
    if (clearline == true) {
        LCDClearBlock(0); //clear line/block 0
        HAL_Delay(TEST_DELAY);
        LCDClearBlock(5); //clear line/block 5
        HAL_Delay(TEST_DELAY);
    }

    //TEST 1c sleep mode
    if (sleepmodetest == true) {
        LCDClear(0x00);
        LCDgotoXY(0, 0);
        LCDString("Sleep 5s");
        HAL_Delay(1000);
        LCDenableSleep(); //go to sleep
        HAL_Delay(5000);
        LCDdisableSleep(); //wake up
        LCDgotoXY(0, 0);
        LCDString("Awake");
        HAL_Delay(TEST_DELAY);
    }

    // Test 1d Pixels
     if (pixeltest == true) {
        LCDClear(0x00);

        LCDgotoXY(0, 5); // (go to (X , Y) (0-84 columns, 0-5 blocks)
        LCDString("Draw Pixels");
        HAL_Delay(TEST_DELAY);

        for (uint8_t Pixels = 11; Pixels < 255; Pixels++) {
            LCDSetPixel(Pixels / 5, Pixels / 10);
            HAL_Delay(15);
        }

        // 1e Draw a horizontal line with SetPixel across screen at row 10
        LCDClear(0x00);
        LCDgotoXY(0, 0); // (go to (X , Y) (0-84 columns, 0-5 blocks)
        LCDString("Draw Lines"); //print to block 0 (0-5 blocks or row bytes)
        HAL_Delay(TEST_DELAY);

        for (uint8_t col = 3; col < 71; col++) {
            LCDSetPixel(col, 23);
            LCDSetPixel(col, 31);
        }
        HAL_Delay(TEST_DELAY);

        // 1f Draw a vertical line in block 2 of one byte at column 3
        LCDgotoXY(3, 3);
        LCDWrite(LCD_DATA, 0xFF);
        LCDgotoXY(70, 3);
        LCDWrite(LCD_DATA, 0xFF);
        HAL_Delay(TEST_DELAY);
    }
    // TEST 1g Fill whole with Screen with pattern
    LCDClear(0x00);
    LCDgotoXY(0, 0);
    LCDString("Fill Screen");
    HAL_Delay(TEST_DELAY);

    LCDClear(0xBA); //  pattern 0b10111010
    HAL_Delay(TEST_DELAY);

    // Test 1h fill block
    LCDClear(0X00);
    LCDgotoXY(0, 0);
    LCDString("Fill block");
    HAL_Delay(TEST_DELAY);
    LCDFillBlock(0xAB, 4); //  0b10101011
    HAL_Delay(TEST_DELAY);


    //Test 1i Custom characters
    LCDClear(0X00);
    LCDgotoXY(0, 0);
    LCDString("Custom char");
    LCDgotoXY(40, 2);
    LCDCustomChar(power, sizeof (power) / sizeof (unsigned char), LCDPadding_Both);
    LCDgotoXY(40, 4);
    LCDCustomChar(myspeed, sizeof (myspeed) / sizeof (unsigned char), LCDPadding_Both);
    HAL_Delay(5000);

    //whole screen clear again
    LCDClear(0x00);
    HAL_Delay(TEST_DELAY);
}

//****************************** TestFont1to6 ****************************
// fonts 1-6 tests,  NOTE fonts  must be  commented in header file

void TestFont1to6() {


    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Default); //font 1
    LCDgotoXY(0, 0); // (go to (X , Y) (0-84 columns, 0-5 blocks)
    LCDString("1234567890AB"); //print to block 0
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Thick); // font 2
    LCDgotoXY(0, 1);
    LCDString("VOLTAGE 2"); //print to block 1
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_HomeSpun); //font 3
    LCDgotoXY(0, 2);
    LCDString("ABCDEFGH"); //print to block 2
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Seven_Seg); //font 4
    LCDgotoXY(0, 3);
    LCDString("VOLTAGE 5.843"); //print to block 3
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Wide); //font 5
    LCDgotoXY(0, 4);
    LCDString("VOLTAGE"); //print to block 4
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Tiny); //font 6
    LCDgotoXY(0, 5); // (go to (X , Y) (0-84 columns, 0-5 blocks)
    LCDString("1234567890123456"); //print to block 5
    HAL_Delay(TEST_DELAY);
    LCDFont(LCDFont_Default);
    LCDClear(0x00);
}

//****************************** TestFont7to9 ****************************
// fonts 7-9 tests,  Note  fonts  must be  commented in header file

void TestFont7to9() {
    // ***** TEST 3a font 7 large 12 * 16 *********
    // :NO LOWER CASE LETTERS for  font 7
    LCDClear(0X00); //clear whole screen
    LCDFont(LCDFont_Large); //font 7
    LCDgotoXY(20, 1);
    LCDString("TIME");
    LCDgotoXY(15, 3); // this font takes two blocks
    LCDString("23:04");
    HAL_Delay(5000);


    // ***** TEST 3b font 8 huge 16 * 24 *********
    // NUMBERS  only + : . /
    LCDClear(0x00);
    LCDFont(LCDFont_Huge); // font 8 this font takes 3 blocks
    LCDgotoXY(0, 0);
    LCDString("10:39");
    LCDgotoXY(0, 3);
    LCDString("4.288");
    HAL_Delay(5000);

    // ********** TEST 3c font 9 MEGA font 16 * 32 *********
    //  NUMBERS  only + : . /
    LCDClear(0x00);
    LCDFont(LCDFont_Mega); // font 8 this font takes 4 blocks
    LCDgotoXY(0, 1);
    LCDString("14.23");
    HAL_Delay(5000);

}

//****************************** TestBitmap ****************************
// bitmap tests,  Note  fonts one  must be  commented in font header file

void TestBitmap() {
    // 'image1', 84x48px snake
    const unsigned char mybitmap[504] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xf8, 0x7c,
        0x7c, 0x20, 0x00, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0x80, 0x00, 0x00, 0xe0, 0xf8, 0xf8, 0xfc, 0x38,
        0x00, 0xe0, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0x80, 0x00, 0x10, 0x78, 0x7c, 0xfc, 0xfc, 0x00, 0x80,
        0xe0, 0xf0, 0xf8, 0xf8, 0xf0, 0xe4, 0x7c, 0x7e, 0xbe, 0x9e, 0x1c, 0x18, 0xf0, 0x30, 0x30, 0x90,
        0x90, 0x18, 0x98, 0xc8, 0xc8, 0xcc, 0x0c, 0xce, 0xce, 0xe6, 0xfe, 0xf0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7f, 0x7f, 0x73,
        0x31, 0x30, 0xb8, 0xf8, 0xf8, 0xf8, 0xf8, 0xa1, 0xd7, 0xff, 0xff, 0x07, 0x1f, 0x7e, 0x78, 0x7f,
        0x7f, 0x3f, 0x8f, 0xe0, 0xfc, 0xff, 0xff, 0x3e, 0x30, 0x1b, 0x7f, 0x7f, 0x7f, 0x6f, 0x0f, 0x77,
        0xf9, 0xff, 0xff, 0x3f, 0x3f, 0x7b, 0xe3, 0xe3, 0x0f, 0x3f, 0xfe, 0xfe, 0xef, 0x77, 0x78, 0x38,
        0x03, 0x07, 0x03, 0x07, 0xff, 0x00, 0x0f, 0xff, 0xff, 0x7f, 0x00, 0x7f, 0x7f, 0x3f, 0x30, 0xf0,
        0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x04, 0x1c, 0x1c, 0x1e, 0x0f, 0x0f, 0x07, 0x07, 0x03, 0x01, 0x80, 0x80, 0x81, 0x41, 0x40, 0x80,
        0x60, 0xa0, 0x60, 0xa0, 0x60, 0xa0, 0xe1, 0x41, 0x61, 0x79, 0x34, 0x34, 0x08, 0x04, 0x24, 0x32,
        0x2b, 0x25, 0x3d, 0x1a, 0x44, 0x45, 0xc5, 0x44, 0x04, 0x08, 0xe0, 0xc0, 0x80, 0x80, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x32, 0x23, 0x31, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x0e, 0x0e,
        0x07, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xd8, 0x6c, 0x06, 0x83, 0xb5, 0xea, 0x11,
        0x09, 0x84, 0x80, 0x86, 0x03, 0x02, 0x01, 0x02, 0x01, 0x01, 0x05, 0x00, 0x01, 0x01, 0x01, 0x78,
        0x99, 0x00, 0x00, 0x1c, 0x0e, 0x86, 0x62, 0x22, 0x12, 0x8f, 0x43, 0x23, 0x13, 0x0f, 0x8b, 0x45,
        0x27, 0x27, 0x27, 0x24, 0x24, 0x24, 0x24, 0x04, 0x80, 0x08, 0x10, 0x20, 0xe0, 0x30, 0xe8, 0xc8,
        0x90, 0x10, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x9c, 0x29,
        0x24, 0x6c, 0x76, 0x67, 0x47, 0x00, 0x00, 0x92, 0x92, 0xd0, 0x9c, 0xd8, 0x70, 0x00, 0xe0, 0x10,
        0x08, 0x04, 0x02, 0xe1, 0x30, 0x18, 0xc4, 0xe6, 0xf1, 0xf8, 0xf0, 0xd0, 0x83, 0x00, 0x00, 0x0c,
        0x02, 0x01, 0x00, 0x1c, 0x36, 0x0a, 0x33, 0x83, 0x05, 0x25, 0x27, 0x07, 0x07, 0x24, 0x04, 0x84,
        0x83, 0x93, 0x67, 0xff, 0x7f, 0xfe, 0xf8, 0x00, 0x01, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x03, 0x07, 0x07, 0x18, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x11, 0x31, 0x10,
        0x30, 0x31, 0x20, 0x28, 0x08, 0x00, 0x11, 0x10, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
        0x22, 0x20, 0x20, 0x22, 0x00, 0x10, 0x10, 0x10, 0x0d, 0x27, 0x30, 0x37, 0x24, 0x2d, 0x29, 0x29,
        0x29, 0x29, 0x20, 0x24, 0x35, 0x13, 0x1b, 0x19, 0x1a, 0x1a, 0x1b, 0x1a, 0x0a, 0x09, 0x04, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    // 'image2', 84x24px
    const unsigned char mybitmap2[252] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x7f, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f, 0x0f, 0x07, 0x87, 0xc3, 0xe3, 0xf9, 0xfd, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f,
        0x0f, 0x07, 0x87, 0xc3, 0xe3, 0xf9, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfb,
        0xf1, 0xf1, 0xf0, 0xf0, 0xe0, 0xe0, 0x60, 0x20, 0x00, 0x00, 0x04, 0x06, 0x07, 0x07, 0x0f, 0x0f,
        0x8f, 0x8f, 0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfb, 0xf1, 0xf1, 0xf0, 0xf0, 0xe0, 0xe0,
        0x60, 0x20, 0x00, 0x00, 0x04, 0x06, 0x07, 0x07, 0x0f, 0x0f, 0x8f, 0x8f, 0xdf, 0xdf, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x9f, 0xc7, 0xc3, 0xe1, 0xe0, 0xf0, 0xf0, 0xf8, 0xfc, 0xfc,
        0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf,
        0x9f, 0xc7, 0xc3, 0xe1, 0xe0, 0xf0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    LCDClear(0x00); // Clear whole screen
    LCDFont(LCDFont_Default); // Set the font
    LCDgotoXY(0, 0); // (go to (X , Y) (0-84 columns, 0-5 blocks) top left corner
    LCDString("BITMAP TEST"); //print
    HAL_Delay(2000);

    // Test half size bitmap  252 bytes
    LCDgotoXY(0, 0);
    LCDCustomChar(mybitmap2, sizeof (mybitmap2) / sizeof (unsigned char), 0x00);
    LCDgotoXY(0, 4);
    LCDString("Error : 1202");
    HAL_Delay(5000);
    LCDClear(0x00);

    //Test full size bitmap 504 bytes
    LCDCustomChar(mybitmap, sizeof (mybitmap) / sizeof (unsigned char), 0x00);
    HAL_Delay(5000);
    LCDClear(0x00);
}
//****************************************
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
