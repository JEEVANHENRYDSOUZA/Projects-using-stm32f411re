/*******************************************************************************
  * File Name          : lab10.c
  * Description        : in this lab we are measuring the rortaion of the shaft of dc motor
  *
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              :  17-11-2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"


static TIM_HandleTypeDef tim3;//hmadl for timer which contains the encoder circuitary
//function name: intitalize()
//function description: used to initilize the encoder
void initialize(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_6 | GPIO_PIN_7);//initializing the encoder
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_TIM3_CLK_ENABLE();

	tim3.Instance = TIM3;
	tim3.Init.Prescaler = 0;			// this will cause the timer to create ms (set to 1000000 - 1 for us)
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = 0xffff;
	tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim3.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim3);

	HAL_StatusTypeDef rc;
	TIM_Encoder_InitTypeDef encoderConfig;

	encoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	encoderConfig.IC1Polarity = 0;
	encoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	encoderConfig.IC1Prescaler = 0;
	encoderConfig.IC1Filter = 3;
	encoderConfig.IC2Polarity = 0;
	encoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	encoderConfig.IC2Prescaler = 0;
	encoderConfig.IC2Filter = 3;

	rc = HAL_TIM_Encoder_Init(&tim3, &encoderConfig);
	if (rc != HAL_OK)
	{
		printf("Failed to start Timer 3 Encoder, " "rc=%u\n", rc);
		return;
	}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_1);
	if (rc != HAL_OK)
	{
		printf("Failed to start Timer 3 Encoder, " "rc=%u\n", rc);
		return;
	}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_2);
	if (rc != HAL_OK)
	{
		printf("Failed to start Timer 3 Encoder, " "rc=%u\n", rc);
		return;
	}


}
//function name:init()
//function description:used to read the encoder values
ParserReturnVal_t init(int mode)
{
	int32_t degree = 0;
	int32_t value = 0;
	initialize();
	value = TIM3->CNT;        //reading the encoder value
	degree = (value * 360) / 794; //converting the value into degrees
	printf("value is %ld\n", value);
	printf("the motor angle is %ld\n", degree);
	return CmdReturnOk;
}
ADD_CMD("encoderinit", init, "gives the direction in degrees")







