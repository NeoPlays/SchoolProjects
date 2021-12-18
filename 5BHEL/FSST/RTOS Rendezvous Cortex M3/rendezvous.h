/*
================================================================
Name: rendezvous.h
Author: Maximilian Roll & Martin Platajs
Project: RTOS Rendezvous

Description:
Header file for initialization code, include and defines

Version: 1.0
================================================================
*/


#ifndef RENDEZVOUS_H_INCLUDED
#define RENDEZVOUS_H_INCLUDED


/* ----------- Includes -----------*/
#include <stdio.h>
#include "stm32f10x.h"
#include <cmsis_os2.h>						 
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stm32f10x_usart.h>
#include "stm32f10x_dma.h"
#include <stm32f10x_adc.h>


/* ----------------- Defines -----------------*/
#define ADC1_DR_ADDRESS	((uint32_t)0x4001244C)
#define delay 500


/* ----------- Prototypes -----------*/
static void DMA_Config(void);
static void ADC1_Init(void);
void init_uart(void);


/* ----------------------------- Varibales -------------------------------*/
static __IO uint16_t ADC1ConvertedValues[2];        //array for ADC values


/*--------------------------
  interrupt service routine
 *-------------------------*/
void DMA1_Channel1_IRQHandler(void)
{
	
	DMA_ClearFlag(DMA1_FLAG_TC1);       //Clears the corresponding TCIF flag (Transmision Complete)
}


/*----------------------------------------------
  initializes DMA controller to copy ADC values
 *---------------------------------------------*/
static void DMA_Config(void)
{
DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef nvic;

	/* initialize the DMA1 Channel1 according to the DMA_InitStructure members */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		// Enable the DMA1 clock						
	DMA_DeInit(DMA1_Channel1);								// Deinitalize DMA Channel 1		  	          		
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;          //Source Address for copy process 
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&ADC1ConvertedValues;	//Destination for copy process
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;                           // copy two 16bit ADC Values to Memory
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;      // 16 bit values
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //better for SCAN and CONTINOUS ADC modes
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA1_Channel1->CCR |= DMA1_IT_TC1;	    //transmition-complete --> interrupt
  DMA_ClearITPendingBit(DMA1_FLAG_TC1);     //delete interrupt bit
	DMA_Cmd(DMA1_Channel1, ENABLE);         //enable DMA1_Channel1
	
	// Init NVIC for DMA1 Transmition-complete --> Interrupt (Serves ADC1) 
	nvic.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);
}


/*-----------------
  initializes ADC1
 *----------------*/
static void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	// devide clock for ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// ADC input pin:  PB1=ADC12_IN9 (channel9)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// analog input mode for ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ADC input pin: PC4=ADC12_IN14 (channel14)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// analog input mode for ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	  
	// configure ADC for variable resistors
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_NbrOfChannel = 2;       // 2 channels: ADC12_IN9,	ADC12_IN14
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;  //enable scan Mode -> multiple channel Input possible
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);	// ADC1 for PB1 (Channel 9)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_239Cycles5);	// ADC1 for PC4 (Channel 14)
	
	ADC_DMACmd(ADC1, ENABLE); // Enable ADC1 DMA transfer 
	ADC_Cmd(ADC1, ENABLE);	// ADC aktivieren

	ADC_ResetCalibration(ADC1);					//ADC Calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		// triggers first conversion of ADC

}


/*----------------------------------------------------------------
  Sends a string, character by character, over the specified UART
 *---------------------------------------------------------------*/
void USART_SendString(USART_TypeDef *USARTx, char *str)
{
	while (*str) {
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *str++);
	}
}

/*-----------------
  Initializes UART
 *----------------*/
void init_uart(void)
{
	GPIO_InitTypeDef gpio;	
	USART_ClockInitTypeDef usartclock;
	USART_InitTypeDef usart;
	
	// Enable all GPIO and USART clocks needed for USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// Create gpio struct and fill it with defaults
	GPIO_StructInit(&gpio);
	
	// Set PA9 to alternate function push pull (Tx)
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &gpio);
	
	// Set PA10 to input floating (Rx)
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &gpio);

	// Create usart struct and init USART1 to 9600 baud
	USART_StructInit(&usart);
	usart.USART_BaudRate = 9600;
  usart.USART_WordLength = USART_WordLength_8b;
  usart.USART_StopBits = USART_StopBits_1;
  usart.USART_Parity = USART_Parity_No ;
  usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_Init(USART1, &usart);

	// Init USART1 clock
	USART_ClockStructInit(&usartclock);
	USART_ClockInit(USART1, &usartclock);	

    // Enable USART1
	USART_Cmd(USART1, ENABLE);
}
#endif //RENDEZVOUS_H_INCLUDED
