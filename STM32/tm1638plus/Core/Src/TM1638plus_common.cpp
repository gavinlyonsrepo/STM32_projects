/*
* Project Name: TM1638plus 
* File: TM1638plus_common
* Description: cpp  file for common data and functions between model 1 and 2 classes
* Arduino library TM1638plus
* Author: Gavin Lyons.
* Created April 2021
* URL: https://github.com/gavinlyonsrepo/STM32_projects
*/

#include "TM1638plus_common.h"


TM1638plus_common::TM1638plus_common()
{
	// Blank constructor
}

uint8_t  TM1638plus_common::HighFreqshiftin() 
{
    uint8_t value = 0;
    uint8_t i = 0;

    for(i = 0; i < 8; ++i) 
    {
        value |= HAL_GPIO_ReadPin(GPIOA, TMDATA_Pin) << i; //  ??
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_SET);
       // delayMicroseconds(TM_READ_DELAY);
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_RESET);
        //delayMicroseconds(TM_READ_DELAY);
    }
    return value;
}

void TM1638plus_common::HighFreqshiftOut(uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
    	if (!!(val & (1 << i)))
			HAL_GPIO_WritePin(GPIOA, TMDATA_Pin,GPIO_PIN_SET) ; /// ????
    	else
    		HAL_GPIO_WritePin(GPIOA, TMDATA_Pin,GPIO_PIN_RESET);

        HAL_GPIO_WritePin(GPIOB, TMCLK_Pin, GPIO_PIN_SET);
        //delayMicroseconds(TM_WRITE_DELAY);
        HAL_GPIO_WritePin(GPIOB,TMCLK_Pin,GPIO_PIN_RESET);
        //delayMicroseconds(TM_WRITE_DELAY);
    }
}
