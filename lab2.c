/*******************************************************************************
  * File Name          : lab2.c
  * Description        : Takes pin number and state as an input and sets up the corresponding pin
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              : 22-09-2021				 
  ******************************************************************************
  */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
#include "main.h"


ParserReturnVal_t gpioinit(int mode)                                                                                                               //intializes the gpio pins
{ __HAL_RCC_GPIOA_CLK_ENABLE();
GPIO_InitTypeDef  GPIO_InitStruct;
GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15);
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
GPIO_InitStruct.Alternate = 0;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
printf("gpio pins have been intialized\n");
return CmdReturnOk;
}

ADD_CMD("gpioInit",gpioinit,"Initialize GPIO Pins of port A")

ParserReturnVal_t gpio(int mode)  
{

  	int rc;
  	int32_t state;
  	int32_t myGpio;
  
rc = fetch_int32_arg(&myGpio);                                                                                                                    //takes the gpio pin number as the input

if (rc) 
{
	printf("Must specify GPIO pin\n");
    	return CmdReturnBadParameter1;
}

rc = fetch_int32_arg(&state);                                                                                                                     //takes the state that is 1 for on and 0 for off

if (rc) 
{
	printf("Must specify state\n");
    	return CmdReturnBadParameter1;
}

if (myGpio>15 || myGpio<0)
{
	printf("Please enter a gpio pin number between 0 to 15\n");
	return CmdReturnBadParameter2;
}

if (state>1 || state<0)
{
	printf("Please enter a state either 0 or 1\n");
	return CmdReturnBadParameter3;
}

if(myGpio==0){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, state);                              
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);}
else if(myGpio==1){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);}
else if(myGpio==4){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);} 
else if(myGpio==5){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);} 
else if(myGpio==6){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);}  
else if(myGpio==7){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);} 
else if(myGpio==8){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);} 
else if(myGpio==9){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);}  
else if(myGpio==10){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);}
else if(myGpio==11){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);} 
else if(myGpio==12){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);}
else if(myGpio==14){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14,state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, 0);}
else if(myGpio==15){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,state);
HAL_Delay(4000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);} 
else{printf("please enter the coorect gpio number\n");}
return CmdReturnOk;
}

ADD_CMD("gpio",gpio,"<pin> <state> Setting pin state to high or low")


