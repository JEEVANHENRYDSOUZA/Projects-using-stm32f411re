/*******************************************************************************
  * File Name          : lab3.c
  * Description        : in this program we are using two adc channels and displaying the raw adc values 
                         of both the channels and converting them to voltage 
  * 
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              :  25-09-2021
  ******************************************************************************
  */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"
ADC_HandleTypeDef hadc1;
 __IO uint32_t value=0;
  float adc_volt=0;
  float adc_volt1=0;
  uint32_t array[2]={0}; 
 ADC_ChannelConfTypeDef sConfig = {0};

 // FUNCTION      : adcInit
// DESCRIPTION    : used to initialze the adc as well as gpio port c which is used in the analog mode  
// RETURNS        : CmdReturnOk, when the command runs properly

ParserReturnVal_t  adcInit(int mode)                                                                                                              
{  __HAL_RCC_GPIOC_CLK_ENABLE();
   GPIO_InitTypeDef  GPIO_InitStruct={0};
 __HAL_RCC_ADC1_CLK_ENABLE();
 hadc1.Instance = ADC1;
 hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
 hadc1.Init.Resolution = ADC_RESOLUTION_12B;
 hadc1.Init.ScanConvMode = DISABLE;
 hadc1.Init.ContinuousConvMode = DISABLE;
 hadc1.Init.DiscontinuousConvMode = DISABLE;
 hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
 hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
 hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
 hadc1.Init.NbrOfConversion= 1;
 hadc1.Init.DMAContinuousRequests = DISABLE;
 hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)

  {

    Error_Handler();

  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.

  */


 GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_0;
 GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
printf("ADC and Gpio have been intialized\n");
return CmdReturnOk;
}
ADD_CMD("adcInit",adcInit,"Initialize adc ")

// FUNCTION      : adcVal
// DESCRIPTION    : used to give the adc values and convert them into voltages
// RETURNS        : CmdReturnOk, when the command runs properly


ParserReturnVal_t adcVal(int mode)    
{
  sConfig.Channel = ADC_CHANNEL_10;
sConfig.Rank = 1;
sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

 if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)

 {

    Error_Handler();
}


if (HAL_ADC_Start(&hadc1) != HAL_OK)
 {
               
/* Start Conversation Error */

printf("HAL_ADC_Start Error\r\n");
 }

                      
 if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) 
 {

printf("HAL_ADC_PollForConversion Error/n");

 }
else
 {

/* ADC conversion completed */

/*##-5- Get the converted value of regular channel  ########################*/

value  = HAL_ADC_GetValue(&hadc1);
adc_volt1 = (double)value*3.3/4095;
array[0]=value;
}

sConfig.Channel = ADC_CHANNEL_11;
sConfig.Rank = 1;
sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

 if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)

 {

    Error_Handler();
}

if (HAL_ADC_Start(&hadc1) != HAL_OK)
 {
               
/* Start Conversation Error */

printf("HAL_ADC_Start Error\r\n");
 }

                      
 if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) 
 {

printf("HAL_ADC_PollForConversion Error/n");

 }
else
 {

/* ADC conversion completed */

/*##-5- Get the converted value of regular channel  ########################*/

value  = HAL_ADC_GetValue(&hadc1);
adc_volt = (double)value*3.3/4095;
array[1]=value;
}

printf("value in volts is : %.3f\n" ,adc_volt);  
printf("value in volts is : %.3f\n" ,adc_volt1);                                     
printf("ADC 12bit Value : %li %li \n",array[0],array[1]);                      
HAL_Delay(1000);
return CmdReturnOk;
 }
  
ADD_CMD("adcVal",adcVal,"adc values ")















