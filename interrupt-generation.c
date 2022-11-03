
/*******************************************************************************
  * File Name          : lab5.c
  * Description        : in this program we are using timer 2 and generating interrupts
  * 
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              :  4-10-2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"
 GPIO_InitTypeDef  GPIO_InitStruct={0};
 TIM_HandleTypeDef htim2;
 
 //FUNCTION NAME : TIM2_IRQHandler
 //FUNCTION PARAMETER : NONE
 //FUNCTION DESCRIPTION : Handles the ISR
 //PARAMETERS RETURNED : NONE

void TIM2_IRQHandler(void)
{
  
  HAL_TIM_IRQHandler(&htim2);       //this is the interrupt handler
 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


}
//FUNCTION NAME : intialize()
//FUNCTION PARAMETER :None
//FUNCTION DESCRIPTION :Used to initialize the timer and interrupt intr1
//PARAMETERS RETURNED :None
  void intialize(void){
  GPIO_InitTypeDef  GPIO_InitStruct={0};     //this is used to initialize the timer and interrupt refernce taken from cube ide
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8399;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period =9999;
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
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
  //FUNCTION NAME :Init()
  //FUNCTION PARAMETER:NONE
  //FUNCTION DESCRIPTION:THIS IS A COMMAND THAT INITIALIZES THE TIMER AND INTERRUPT 
  //FUNCTION RETURN VALUE:CmdReturnOk if the command runs properly


ParserReturnVal_t  Init(int mode)                                                                                                              
{ 
intialize();
printf("Jeevan and Dushant\n");

return CmdReturnOk;
}

ADD_CMD("timer",Init,"Initialize timer and interrupt") //initializes the interrupt










