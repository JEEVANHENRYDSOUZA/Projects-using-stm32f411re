/*******************************************************************************
  * File Name          : lab11.c
  * Description        : in this lab we are doing pid control
  *
  * Author:            : Jeevan Dsouza,Dushant Choudhary
  * Date:              :  09-12-2021
  ******************************************************************************
  */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "common.h"
TIM_HandleTypeDef tim1;
TIM_HandleTypeDef tim3;
static int32_t Previous_Reading = 0;
static int32_t Reading, Sensor = 0;
static int32_t Error = 0;
static int32_t Control = 0;
static int32_t integral = 0;
static int32_t SetPoint = 0;

void PWM(void)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	tim1.Instance = TIM1;
	tim1.Init.Prescaler = HAL_RCC_GetPCLK2Freq() / 1000000;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = 1000;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim1);

	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 0;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCNPolarity = TIM_OCNPOLARITY_LOW;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&tim1, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&tim1, &sConfig, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&tim1, &sConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_3);
	printf("PWM initialised Successfully\n");

}

void initEncoder(void)
{
	HAL_StatusTypeDef rc;
	TIM_Encoder_InitTypeDef encoderConfig;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = (GPIO_PIN_6 | GPIO_PIN_7);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_TIM3_CLK_ENABLE();
	tim3.Instance = TIM3;
	tim3.Init.Prescaler = 0;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = 0xffff;
	tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim3.Init.RepetitionCounter = 0;
	rc = HAL_TIM_Base_Init(&tim3);
	if (rc != HAL_OK)
	{
		printf("Failed to initialize Timer 3 Base, ""rc=%u\n", rc);
		return;
	}

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
		printf("Failed to initialize Timer 3 Encoder,""rc=%u\n", rc);
		return;
	}
	rc = HAL_TIM_Encoder_Init(&tim3, &encoderConfig);
	if (rc != HAL_OK)
	{
		printf("Failed to initialize Timer 3 Encoder,""rc=%u\n", rc);
		return;
	}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_1);
	if (rc != HAL_OK)
	{
		printf("Failed to start Timer 3 Encoder, ""rc=%u\n", rc);
		return;
	}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_2);
	if (rc != HAL_OK)
	{
		printf("Failed to start Timer 3 Encoder, ""rc=%u\n", rc);
		return;
	}

	printf("Encoders initialised\n");
}

void InitDC(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = (GPIO_PIN_10 | GPIO_PIN_11);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_11,
		RCC->APB1ENR |= RCC_APB1ENR_TIM4E;
	TIM4->PSC = 72;
	TIM4->ARR = 1000;
	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM4_IRQn);

}

ParserReturnVal_t Init(int mode)
{
	if (mode != CMD_INTERACTIVE) return CmdReturnOk;
	PWM();
	initEncoder();
	initDC();
	return CmdReturnOk;
}
ADD_CMD("Init", PIDInit, "this is going to start the motor to control the speed")

int16_t encoders(void)
{
	int16_t a = 0;
	a = TIM3->CNT;
	return a;
}

void PWM_Set3(int32_t channel, int32_t value)
{

	if (channel == 1) {

		TIM1->CCR1 = value;

	}
	else {

		printf("PLEASE ENTER THE CORRECT CHANNEL NUMBER\n");
	}

	return;
}

void PID1()
{
	Reading = encoders();
	printf("Value is = %ld\n", Reading);
	Sensor = Reading - Previous_Reading;
	printf("Sensor Value is = %ld\n", Sensor);
	Previous_Reading = Reading;
	Error = Set_Point - Sensor;
	printf("Error = %ld\n", Error);
	Control = integral * Error;
	printf("Control = %ld\n", Control);

	if (Control < 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
	}
}

ParserReturnVal_t PID(int mode)
{
	if (mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&SetPoint);
	fetch_int32_arg(&integral);
	PID1();
	return CmdReturnOk;
}

ADD_CMD("PID", PID, " Set Point & integral")
