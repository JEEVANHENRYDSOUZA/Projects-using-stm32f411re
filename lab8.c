
/*******************************************************************************
  * File Name          : lab8.c
  * Description        : in this lab we are using pwm to vary the width and see the corresponding led glow
  *
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              :  3-11-2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"

# define PERIOD_VALUE 100       //here i have set the PERIOD_VALUE to 100 so that we can give the input in percentage

TIM_HandleTypeDef htim2;         //timer2 handle 


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);


//function name:intialize()
//function paramters:None
//function description:this is used to initialize the timer2 
void intialize(void)
{



	/* USER CODE END TIM2_Init 0 */
	__HAL_RCC_TIM2_CLK_ENABLE();
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = HAL_RCC_GetPCLK2Freq() / 1000000 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = PERIOD_VALUE + 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)     //we are configuring channel1 timer2
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)    //we are configguring channel2 timer2
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)   //we are configuring channel3 timer2
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

	/* USER CODE END TIM2_Init 2 */

}

//function name :HAL_TIM_MspPostInit()
//function paramters:htim2
//function description:here we are intializing the  gpio pins in pwm mode
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (htim->Instance == TIM2)
	{
		/* USER CODE BEGIN TIM2_MspPostInit 0 */

		/* USER CODE END TIM2_MspPostInit 0 */

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**TIM2 GPIO Configuration
		PA1     ------> TIM2_CH2
		PA5     ------> TIM2_CH1
		PB10     ------> TIM2_CH3
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM2_MspPostInit 1 */

		/* USER CODE END TIM2_MspPostInit 1 */
	}

}
//function name:init()
//function description:used to vary the width of the led
//function return type:returns comdReturnOk if the command was executed correctly
ParserReturnVal_t  init(int mode) {
	int32_t channelNumber;
	int32_t value = 0;
	fetch_int32_arg(&value);
	fetch_int32_arg(&channelNumber);
	intialize();
	if (channelNumber == 1) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); }          //starting pwm generation on timer2 channel1
	if (channelNumber == 2) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); }          //starting pwm generation on timer 2 channel2
	if (channelNumber == 3) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); }
	if (value <= PERIOD_VALUE) {             //bound checking the input
		if (channelNumber == 1) {
			while (value < PERIOD_VALUE)

			{

				TIM2->CCR1 = value;
				HAL_Delay(100);
				value += 5;



			}
			while (value >= 0) {

				TIM2->CCR1 = value;

				HAL_Delay(100);

				value -= 5;


			}
		}


		else if (channelNumber == 2) {
			while (value < PERIOD_VALUE)

			{

				TIM2->CCR2 = value;
				HAL_Delay(100);
				value += 5;




			}
			while (value >= 0) {

				TIM2->CCR2 = value;
				HAL_Delay(100);
				value -= 5;




			}


		}
		else  if (channelNumber == 3) {
			while (value < PERIOD_VALUE) {

				TIM2->CCR3 = value;
				HAL_Delay(100);
				value += 5;
			}
			while (value >= 0) {

				TIM2->CCR3 = value;
				HAL_Delay(100);
				value -= 5;




			}

		}

		else { printf("enter the correct channel numeber\n"); }
	}
	else { printf("enter the correct period vale\n"); }
	return CmdReturnOk;

}
ADD_CMD("pwm", init, "<channelNumber> <value> channelnumber and value") //in this command  we are passing the channel number and period value
