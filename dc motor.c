
/*******************************************************************************
  * File Name          : lab7.c
  * Description        : used to rotate the stepper motor  at given speed by generating delays using interrupts and then stop depending on the input given
  * 
  * Author:            : Dushant Choudhary, Jeevan Dsouza
  * Date:              :  3-11-2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"
 uint32_t speed;
 int16_t state;
 uint32_t ms;


 GPIO_InitTypeDef  GPIO_InitStruct={0};
 TIM_HandleTypeDef htim2;

//FUNCTION NAME : TIM2_IRQHandler()
//FUNCTION DESCRIPTION : Interrupt handler 
 
void TIM2_IRQHandler(void)
{
  
 HAL_TIM_IRQHandler(&htim2);
 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);//interrupt handler
 
}
//fUNCTION NAME: intialize()
//FUNCTION PARAMTERS:None
//FUNCTION RETURNS:NONE

    
  void intialize(void){

  __HAL_RCC_TIM2_CLK_ENABLE();//timer conifguration
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period =speed;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = 0;
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

 
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR1;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_RCC_GPIOC_CLK_ENABLE();

  __HAL_RCC_GPIOH_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();

 

  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);// pins for the motor driver ic

 

  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8, GPIO_PIN_RESET);

 

  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

 

  /*Configure GPIO pin : B1_Pin */

  GPIO_InitStruct.Pin = B1_Pin;

  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;

 GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

 

  /*Configure GPIO pins : LD2_Pin PA11 PA12 */

  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_11|GPIO_PIN_12;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 

  /*Configure GPIO pins : PC5 PC6 PC8 */

  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

 

  /*Configure GPIO pin : PB12 */

  GPIO_InitStruct.Pin = GPIO_PIN_12;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  
}
//FUNCTION NAME :Init()
//FUNCTION DESCRIPTION:Used to Start the timer and the direction of the motor anticlockwise or clockwise
//FUNCTION RETRUN VALUES:Return CmdRetrun oK if the command works properly


ParserReturnVal_t  Init(int mode)                                                                                                              
{ 
fetch_uint32_arg(&speed);
fetch_int16_arg(&direction);
fetch_uint32_arg(&ms);
intialize();
if (direction < 0)
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);//CLOCKWISE ROTATION OF THE MOTOR
}
else if (direction > 0)
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);//ANTICLOCKWISE ROTATION OF THE MOTOR
}
else{printf("error\n");}
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
HAL_TIM_Base_Start_IT(&htim2);
printf("Jeevan and Dushant\n");

HAL_Delay(ms);               
NVIC_DisableIRQ(TIM2_IRQn);//this is used to stop the interrupt generation

return CmdReturnOk;
}

ADD_CMD("speed_state_time",Init,"<speed> <direction> <ms> Initialize timer")  









