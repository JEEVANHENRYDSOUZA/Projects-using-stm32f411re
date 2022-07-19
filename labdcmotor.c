
/*******************************************************************************
  * File Name          : lab10.c
  * Description        : in this lab we are used to change the speed of the dc motor
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
void my_Tick(void);
void TaskingTickHandler(void);
# define PERIOD_VALUE 100       //here i have set the PERIOD_VALUE to 100 so that we can give the input in percentage
static int32_t delay = 0;
TIM_HandleTypeDef htim2;         //timer2 handle 


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);


//function name:intialize()
//function paramters:None
//function description:this is used to initialize the timer2  for pwm 
void intialize(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };



	/* GPIO Ports Clock Enable */

	__HAL_RCC_GPIOC_CLK_ENABLE();

	__HAL_RCC_GPIOH_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */

	/*Configure GPIO pins : PA6 PA7 */

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;    //setting up gpio pins for direction of the motor
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
//function description:used to give value to the dc motor to turn along with directions
//function return type:returns comdReturnOk if the command was executed correctly
ParserReturnVal_t  init(int mode) {
	int32_t channelNumber;
	int32_t value = 0;
	int32_t direction = 0;
	fetch_int32_arg(&direction);
	fetch_int32_arg(&value);
	fetch_int32_arg(&channelNumber);
	fetch_int32_arg(&delay);
	intialize();
	if (value >= 55 && value <= 99) {
		if (channelNumber == 1) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); }          //starting pwm generation on timer2 channel1
		if (channelNumber == 2) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); }
	}
	else { printf("please enter the correct period value\n"); }

	//starting pwm generation on timer 2 channel2
	TIM2->CCR1 = value;
	if (direction == 1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //anticlockwise
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	}
	if (direction < 0) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //anticlockwise
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	}//motion
	return CmdReturnOk;

}
ADD_CMD("pwm", init, " <direction>  <value> <channelnumber> <delay> channelnumber and value and direction") //in this command  we are passing the channel number and period value
//function name :Systick_Handler()
//function description :used to generate the delay and after delay turn the dc motor off
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */
	static int count= 0;//used as a counter

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */
	my_Tick();
	TaskingTickHandler();
	/* USER CODE END SysTick_IRQn 1 */

	count++;
	if (count > delay) {
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
		count = 0; //again re-initialize the count
	}
}










