/* USER CODE BEGIN Header */

/*
  Project Name: TM1638
  File: TM1638plus_TEST_Model2.ino
  Description: demo file library for "model 2" TM1638 module(16 KEY 16 pushbuutons).
  Carries out series of tests demonstrating STm32 cubeide library TM1638plus.
  The tests will increment automatically with exception of test9, to enter press S16 during test8

  TESTS

  TEST0 = reset function test
  TEST1 =  decimal numbers
  TEST2 =  Hexadecimal number
  TEST3 = manually set segments
  TEST4 = Display  strings
  TEST5  =  ASCII to segments ( no reference to font table)
  TEST6 = Brightness control
  TEST7 = Scroll text example
  TEST8 = Push buttons ReadKey16() buttons function , press 16 to goto test9
  TEST9 = Push buttons ReadKeys16Two() alternate  buttons function

  Created: April  2021
  IDE: STM32CubeIDE C++
  URL: https://github.com/gavinlyonsrepo/STM32_projects
*/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "TM1638plus_Model2.h" //include the library
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buf[12];
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL
TM1638plus_Model2 tm(swap_nibbles);
// Some vars and defines for the tests.
// For test setup
#define  myTestDelay 5000
#define  myTestDelay1 1000
#define  myTestDelay2 250
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
  //strcpy((char*)buf,"hello\r\n");
  //HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY ); //baud 38400
  tm.displayBegin(); // Init the module
  HAL_Delay(1000);
  // Test 0 reset test
  tm.reset();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  testcount++;

	  switch (testcount)
	  {
	    case 1: Test1(); break; // Test 1 decimal numbers
	    case 2: Test2(); break; // Test 2 Hexadecimal number
	    case 3: Test3(); break; // Test 3a 3b & 3C using DisplaySegments method
	    case 4: Test4(); break; // Test 4 strings
	    case 5: Test5(); break; // Test 5 ASCIItoSegment method
	    case 6: Test6(); break; // Test 6  Brightness and reset
	    case 7: Test7(); break; // Test 7 scroll text
	    case 8: Test8(); break; // Test 8 Buttons , ReadKey16() returns byte 1-16 decimal, press S16 to goto test9
	    case 9: Test9(); break; // Test 9 Buttons , Readkey16Two() alternate buttons function.
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
void Test1(void)
{
  // Test 1 decimal numbers
  tm.DisplayDecNum(250, 1 << 2, true); // 000002.50
  HAL_Delay(myTestDelay);
  tm.DisplayDecNum(99991111, 1 << 4, true); // 9999.1111
  HAL_Delay(myTestDelay);
  tm.DisplayDecNum(2888, 0 , true);  // 00002888
  HAL_Delay(myTestDelay);
  tm.DisplayDecNum(331285, 1 <<4 ,false); // "3312.85   "
  HAL_Delay(myTestDelay);
  tm.DisplayDecNum(-33, 0 , false); // "-33        "
  HAL_Delay(myTestDelay);
  // Test 1b  decimal numbers with the DisplayDecNumNibble function divides display into two nibbles.
  tm.DisplayDecNumNibble(2134 , 78, 1<<4 , true); // "2134.0078"
  HAL_Delay(myTestDelay);
}

void Test2(void)
{
  // Test 2 Hexadecimal number
  tm.DisplayHexNum(0x0000, 0x456E, 0x00, true); // 0000456E
  HAL_Delay(myTestDelay);
  tm.DisplayHexNum(0xABCD, 0xEF23, 0x00, true); // ABCDEF23
  HAL_Delay(myTestDelay);
  tm.DisplayHexNum(0x0000, 0x00FF, 1 << 4); // 0000.00FF
  HAL_Delay(myTestDelay);
}

void Test3(void)
{
  // Test 3 manually set segments abcdefg(dp) = 01234567
  // display a one in position one "       1"
  tm.DisplaySegments(0, 0x00); //a
  tm.DisplaySegments(1, 0x01); //b, for b turn on digit one only
  tm.DisplaySegments(2, 0x01); //c, for c turn on digit one only
  tm.DisplaySegments(3, 0x00); //d
  tm.DisplaySegments(4, 0x00); //e
  tm.DisplaySegments(5, 0x00); //f
  tm.DisplaySegments(6, 0x00); //g
  tm.DisplaySegments(7, 0x00); //DP
  HAL_Delay(myTestDelay);

  // Test 3b manually set segments
  // Display "00000005"
  tm.DisplaySegments(0, 0xFF); //a, turn a on for all digits
  tm.DisplaySegments(1, 0xFE); //b
  tm.DisplaySegments(2, 0xFF); //c
  tm.DisplaySegments(3, 0xFF); //d
  tm.DisplaySegments(4, 0xFE); //e
  tm.DisplaySegments(5, 0xFF); //f
  tm.DisplaySegments(6, 0x01); //g , for g middle segment, digit one only on
  tm.DisplaySegments(7, 0x00); //decimal point, turn off all decmial points
  HAL_Delay(myTestDelay);

  // Test 3c manually set segments scroll g
  // Display "-" countup to "--------"
  tm.reset();
  uint8_t dashvalue = 1;
  for (uint8_t j = 0; j < 8;  j++)
  {

    tm.DisplaySegments(6, dashvalue); // g scrolling
    dashvalue = (dashvalue*2)+1; // 1 to 256
    HAL_Delay(myTestDelay1);
  }
  HAL_Delay(myTestDelay);

}

void Test4(void)
{
  // Test 4 strings
  tm.DisplayStr("helloYOU", 1); // "helloYOU."
  HAL_Delay(myTestDelay);
  tm.DisplayStr("      Hi", 0x08); // "     . Hi"
  HAL_Delay(myTestDelay);
  tm.DisplayStr("   ---   ", 0XE7); // ". . .---. . ."
  HAL_Delay(myTestDelay);
  tm.DisplayStr(" helloU2", 0); // " helloU2"
  HAL_Delay(myTestDelay);
  tm.DisplayStr("hello", 0);  // "hello   "
  HAL_Delay(myTestDelay);
}

void Test5(void)
{
  // Test 5 ASCII to segments takes an array of bytes and displays them
  // without ref to the ASCII font table direct data to digits to displays 3F 3F 3F 6D 3F 3F 3F 6D = 00050005
  // gfedcba = 3F for zero https://en.wikipedia.org/wiki/Seven-segment_display

  const uint8_t values[8] = {0x3F, 0x3F, 0x3F, 0x6D, 0x3F, 0x3F, 0x3F, 0x6D}; // for ascii to segment test 00050005

  tm.ASCIItoSegment(values);

  HAL_Delay(myTestDelay);
}

void Test6(void)
{
  // Test 6  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.DisplayStr("brightnes", 1);
    HAL_Delay(myTestDelay1);
  }
  tm.brightness(2);
  tm.reset();
}


void Test7(void)
{
  char textScroll[17] = " Hello world 123";

  while(1)
  {
  tm.DisplayStr(textScroll, 0);
  HAL_Delay(myTestDelay1);
    if (strlen(textScroll) > 0)
    {
      memmove(textScroll, textScroll+1, strlen(textScroll)); // delete first char in array.
    }else
    {
      return;
    }
   }

}

void Test8(void)
{
  unsigned char buttons;
  while(1)
  {
      // Test 8 , buttons readkey16() function, no debounce see notes at URL for example to debounce.
      // returns 0-16 , 0 for nothing pressed.
      // NOTE: pressing  S16 will move to test 9
      buttons = tm.ReadKey16();
      tm.DisplayDecNum(buttons, 0 ,false);
      HAL_Delay( myTestDelay2);
      if (buttons == 16)
      {
           //pressing 16 moves  to test 9
          HAL_Delay( myTestDelay2);
          return;
      }
    }
}

//returns word with binary value of switch. S16 = Bit 15 , S15 = bit 14 etc
void Test9(void)
{
  uint16_t buttons=0;
  while(1)
      {
      // Test 9 buttons Read_key16_two() function
      // returns a uint16_t where each bit represents a switch.
      // S16S15S14S13S12S11S10S9S8S7S6S5S4S3SS2S1.
      // eg S1 =  0x0001
      // eg S16 = 0x8000
      // eg S1 + S16 = 0x8001
      // Can be used to detect multi key presses , see Notes section in readme.
      // For issues related to display when pressing multi keys together.
      buttons = tm.ReadKey16Two();
      tm.DisplayStr("buttons2", 0);
      sprintf(buf, "%hu\r\n", buttons);
      // send buttons value DEC , back to terminal via usart2
      HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
      HAL_Delay( myTestDelay2);
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
