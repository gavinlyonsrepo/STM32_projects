/* USER CODE BEGIN Header */
/*
  Project Name: TM1638
  File: TM1638plus_TEST_Model1.ino
  Description: ST32 demo file library for  TM1638 module(LED & KEY). Model 1
  Carries out series of tests demonstrating STM32 library TM1638plus.

  TESTS:
  TEST 0 Reset
  TEST 1 Brightness
  TEST 2 ASCII display
  TEST 3 Set a single segment
  TEST 4 Hex digits
  TEST 5 Text String with Decimal point
  TEST 6 TEXT + ASCII combo
  TEST 7 Integer Decimal number
  TEST 8 Text String + Float
  TEST 9 Text String + decimal number
  TEST 10 Multiple dots
  TEST 11 Display Overflow
  TEST 12 Scrolling text
  TEST 13 setLED and setLEDs method
  TEST 14 Buttons + LEDS

  Author: Gavin Lyons.
  Created April  2021
  IDE: STM32CubeIDE C++
  URL: https://github.com/gavinlyonsrepo/STM32_projects
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "TM1638plus.h" //include the library
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
void doLEDs(uint8_t value);
void Test0(void);
void Test1(void);
void Test2(void);
void Test3(void);
void Test4(void);
void Test5(void);
void Test6(void);
void Test7(void);
void Test8(void);
void Test9(void);
void Test10(void);
void Test11(void);
void Test12(void); 
void Test13(void);
void Test14(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TM1638plus tm;
// Some vars and defines for the tests.
#define myTestDelay  5000
#define myTestDelay1 1000
uint8_t  testcount = 0;
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
  tm.displayBegin();
  //Test 0 reset
    Test0();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    switch (testcount)
	  {
	    case 1: Test1(); break; // Brightness
	    case 2: Test2(); break; // ASCII display
	    case 3: Test3(); break; // Set a single segment
	    case 4: Test4(); break; // Hex digits
	    case 5: Test5(); break; // Text String with Decimal point
	    case 6: Test6(); break; // TEXT + ASCII combo
	    case 7: Test7(); break; // Integer Decimal number
	    case 8: Test8(); break; // Text String + Float hack
	    case 9: Test9(); break; // Text String + decimal number
	    case 10: Test10(); break; // Multiple Decimal points
	    case 11: Test11(); break; // Display Overflow
	    case 12: Test12(); break; // Scrolling text
	    case 13: Test13(); break; // setLED and setLEDs
	    case 14: Test14(); break; // Buttons + LEDS
	  }
	  testcount++;
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
  HAL_GPIO_WritePin(GPIOB, TMCLK_Pin|TMSTB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TMDATA_GPIO_Port, TMDATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : TMCLK_Pin TMSTB_Pin */
  GPIO_InitStruct.Pin = TMCLK_Pin|TMSTB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TMDATA_Pin */
  GPIO_InitStruct.Pin = TMDATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TMDATA_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Test0()
{
  // Test 0 reset test
  tm.setLED(0, 1);
  HAL_Delay(myTestDelay);
  tm.reset();
}

void Test1() {
  // Test 1  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.displayText("00000000");
    HAL_Delay(myTestDelay1);
  }
  tm.reset();
  // restore default brightness
  tm.brightness(0x02);
}

void Test2() {
  //Test 2 ASCII , display 2.341

  tm.displayASCIIwDot(0, '2');
  tm.displayASCII(1, '3');
  tm.displayASCII(2, '4');
  tm.displayASCII(3, '1');
  HAL_Delay(myTestDelay);
  tm.reset();
}

void Test3() {
  //TEST 3 single segment (pos, (dp)gfedcba)
  //In this case  segment g (middle dash) of digit position 7
  tm.display7Seg(7, 0b01000000); // Displays "       -"
  HAL_Delay(myTestDelay);
}

void Test4() {
  // Test 4 Hex digits.
  tm.displayHex(0, 1);
  tm.displayHex(1, 2);
  tm.displayHex(2, 3);
  tm.displayHex(3, 4);
  tm.displayHex(4, 5);
  tm.displayHex(5, 6);
  tm.displayHex(6, 7);
  tm.displayHex(7, 8);
  HAL_Delay(myTestDelay); // display 12345678

  tm.displayHex(0, 8);
  tm.displayHex(1, 9);
  tm.displayHex(2, 10);
  tm.displayHex(3, 11);
  tm.displayHex(4, 12);
  tm.displayHex(5, 13);
  tm.displayHex(6, 14);
  tm.displayHex(7, 15);
  HAL_Delay(myTestDelay); // display 89ABCDEF
}

void Test5() {
  // Test 5 TEXT  with dec point
  // abcdefgh with decimal point for c and d
  tm.displayText("abc.d.efgh");
  HAL_Delay(myTestDelay);
}

void Test6() {
  // Test6  TEXT + ASCII combo
  // ADC=.2.948
  char text1[] = "ADC=.";
  tm.displayText(text1);
  tm.displayASCIIwDot(4, '2');
  tm.displayASCII(5, '9');
  tm.displayASCII(6, '4');
  tm.displayASCII(7, '8');
  HAL_Delay(myTestDelay);
  tm.reset();
}

void Test7() {
  // TEST 7a Integer
  tm.displayIntNum(45, false); // "45      "
  HAL_Delay(myTestDelay);
  // TEST 7b Integer
  tm.displayIntNum(99991, true); // "00099991"
  HAL_Delay(myTestDelay);
  tm.reset();
  // TEST 7b tm.DisplayDecNumNIbble
  tm.DisplayDecNumNibble(1234, 5678, false); // "12345678"
  HAL_Delay(myTestDelay);
  tm.DisplayDecNumNibble(123, 662, true); // "01230662"
  HAL_Delay(myTestDelay);
}

void Test8() {
  // TEST 8  TEXT STRING + integer SSSSIIII
  char workStr[11];
  uint16_t  data = 234;
  sprintf(workStr, "ADC=.%04d", data); // "ADC=.0234"
  tm.displayText(workStr);
  HAL_Delay(myTestDelay);
}

void Test9() {
  // TEST 9 Text String + Float  SSSSFFFF ,  just one possible method.
  float voltage = 12.45;
  uint16_t temp = 0;
  char workStr[11];
  uint8_t  digit1, digit2, digit3 , digit4;
  voltage =  voltage * 100; // 1245
  temp = (uint16_t)voltage;
  digit1 = (temp / 1000) % 10;
  digit2 = (temp / 100) % 10;
  digit3 = (temp / 10) % 10;
  digit4 =  temp % 10;

  sprintf(workStr, "ADC=.%d%d.%d%d", digit1, digit2, digit3, digit4);
  tm.displayText(workStr); //12.45.VOLT
  HAL_Delay(myTestDelay);
  tm.reset();
}

void Test10()
{
  //TEST 10 Multiple dots test
  tm.displayText("Hello...");
  HAL_Delay(myTestDelay);
  tm.displayText("...---..."); //SOS in morse
  HAL_Delay(myTestDelay);
}

void Test11()
{
  //TEST11 user overflow
  tm.displayText("1234567890abc"); //should display just 12345678
  HAL_Delay(myTestDelay);
  tm.reset();
}

void Test12() {
  //TEST 12 scrolling text, just one possible method.
  char textScroll[17] = " Hello world 123";

  while(1)
  {
  tm.displayText(textScroll);
  HAL_Delay(myTestDelay1);
    if (strlen(textScroll) > 0)
    {
      memmove(textScroll, textScroll+1, strlen(textScroll));
      tm.displayText("        "); //Clear display or last character will drag across screen
    }else
    {
      return;
    }
   }

}


void Test13()
{
  //Test 13 LED display
  uint8_t LEDposition = 0;

  // Test 13A Turn on green leds with setLED
  for (LEDposition = 0; LEDposition < 8; LEDposition++) {
    tm.setLED(LEDposition, TM_GREEN_LED);
    HAL_Delay(500);
    tm.setLED(LEDposition, TM_OFF_LED);
  }

  // Test 13b turn on red LEDs with setLED
  for (LEDposition = 0; LEDposition < 8; LEDposition++) {
    tm.setLED(LEDposition, TM_RED_LED);
    HAL_Delay(500);
    tm.setLED(LEDposition, TM_OFF_LED);
  }

  // TEST 13c
  // test setLEDs function (0xgreenred) (0xGGRR) (LED8-LED1, LED8-LED1)
  // Upper byte switch LED green colour ON, lower byte = switch LED red colour ON
  // NB Note on the unit, LED8 is onthe right hand side so result is mirrored.
  // Example:
  // E0 = green on 07 = red on
  // E0  = 1110 0000 , 07 = 0000 0111 = 11100111 = GGGXXRRR = LED8-LED1
  // Shows on display as  LED1-LED8 turns on RRRXXGGG as LED 8 is on right hand side.

  tm.setLEDs(0xE007); //L1-L8 turns on RRRXXGGG on display
  HAL_Delay(3000);

  tm.setLEDs(0xF00F); // L1-L8 turns on RRRRGGGG on display
  HAL_Delay(3000);
  tm.setLEDs(0xFE01); // L1-L8 turns on RGGGGGGG on display
  HAL_Delay(3000);
  tm.setLEDs(0x00FF); //all red   RRRRRRR
  HAL_Delay(3000);
  tm.setLEDs(0xFF00); //all green GGGGGGG
  HAL_Delay(3000);
  tm.setLEDs(0x0000); //all off
  HAL_Delay(3000);

}



void Test14() {
  //Test 14 buttons and LED test, press switch number S-X to turn on LED-X, where x is 1-8.
  //The HEx value of switch is also sent to Serial port.
  tm.displayText("buttons ");
  while (1) // Loop here forever
  {
    uint8_t buttons = tm.readButtons();
      /* buttons contains a byte with values of button s8s7s6s5s4s3s2s1
       HEX  :  Switch no : Binary
       0x01 : S1 Pressed  0000 0001
       0x02 : S2 Pressed  0000 0010
       0x04 : S3 Pressed  0000 0100
       0x08 : S4 Pressed  0000 1000
       0x10 : S5 Pressed  0001 0000
       0x20 : S6 Pressed  0010 0000
       0x40 : S7 Pressed  0100 0000
       0x80 : S8 Pressed  1000 0000
      */
    doLEDs(buttons);
     tm.displayIntNum(buttons, true);
    HAL_Delay(250);
  }
}

// scans the individual bits of value sets a LED based on which button pressed
void doLEDs(uint8_t value) {
  for (uint8_t LEDposition = 0; LEDposition < 8; LEDposition++) {
    tm.setLED(LEDposition, value & 1);
    value = value >> 1;
  }
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
