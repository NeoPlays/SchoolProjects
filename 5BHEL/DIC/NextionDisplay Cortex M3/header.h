/*
================================================================
Name: header.h
Author: Martin Platajs
Project: NEXTION Touchscreen NX4832T035

Description:
Header-File with Initialization-Functions

Version: 1.0
================================================================
*/

#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED


/* --------------------------------- Defines ---------------------------------*/
#define countof(x) (sizeof(x)/sizeof(*(x)))		//get length of array

#define PC7 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)	//Port PC7
#define PC8 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)	//Port PC8
#define PC9 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)	//Port PC9


/* -------------------- Return Values --------------------*/
#define SCHALTER_BUTTON_HOME 1
#define LEDS_BUTTON_HOME 2
#define POTI_BUTTON_HOME 4
#define RE_BUTTON_HOME 5
#define LED1_BUTTON 201
#define LED2_BUTTON 202
#define LED3_BUTTON 203
#define LED4_BUTTON 204
#define LED5_BUTTON 205
#define LED6_BUTTON 206
#define LED7_BUTTON 207
#define LED8_BUTTON 208
#define BACK_BUTTON_SCHALTER 101
#define BACK_BUTTON_LEDS 209
#define BACK_BUTTON_POTI 302
#define BACK_BUTTON_RE 401


/* ----------- Includes -----------*/
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stm32f10x_usart.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_tim.h>


/* ----------- Prototypes -----------*/
static void RCC_Configuration(void);
static void NVIC_Configuration(void);
static void GPIO_Configuration(void);
static void USART_Configuration(void);
static void ADC_Configuration(void);
static void NVIC_init(char position, char priority); 
static void TIM3_Config(void);
static void USART_SendString(USART_TypeDef *USARTx, char *str);
static void USART_putBytes(USART_TypeDef *USARTx, uint8_t *bytes);

void executeFunction(uint8_t *RxCounter);
void wait_mil(void);
void wait_sek(int sek)




/* -------------------- Commands --------------------*/
uint8_t commands[][20] =	//only one command can be expanded
{ 
	{0x70,0x61,0x67,0x65,0x20,0x30,0xFF,0xFF,0xFF}	//"page=0ÿÿÿ" in HEX in an array
};


/* -------------------- Plain Return Values --------------------*/
uint8_t return_values[][8] =
{
	{101,0,1,0,255,255,255},		//Switches button @ Home
	{101,0,2,0,255,255,255},		//LEDs button @ Home 
	{101,0,4,0,255,255,255},		//Poti button @ Home 
	{101,0,5,0,255,255,255},		//RE button @ Home 
	{101,2,1,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,2,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,3,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,4,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,5,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,6,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,7,0,255,255,255},		//LED1 button @ LEDs 
	{101,2,8,0,255,255,255},		//LED1 button @ LEDs 
	{101,1,1,0,255,255,255},		//Back button @ Swtiches
	{101,2,9,0,255,255,255},		//Back button @ LEDs 
	{101,3,2,0,255,255,255},		//Back button @ Poti 
	{101,4,1,0,255,255,255} 		//Back button @ RE 
};


/*--------------------------------------------
  Reset and Clock Control, RCC initialization
 *-------------------------------------------*/
static void RCC_Configuration(void){
  	//enable clock for GPIOA, GPIOB, AFIO, ADC1, USART1 and TIM3
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | 
                        	RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1 |
                        	RCC_APB2Periph_USART1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
}


/*----------------------------------------------------------
  Nested Vectored Interrupt Controller, NVIC initialization
 *---------------------------------------------------------*/
static void NVIC_Configuration(void){
  	//Init NVIC for USART1; RX -> Interrupt (Request handling)
  	NVIC_InitTypeDef NVIC_InitStructure;
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);

	//Init NVIC for ADC1_2; Conversion complete -> Interrupt (save value)
  	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
}


/*--------------------------------------------------
  General Purpose Input Output, GPIO initialization
 *-------------------------------------------------*/
static void GPIO_Configuration(void){
    GPIO_InitTypeDef GPIO_InitStructure;
     
    // Set PA9 to alternate function push pull (Tx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Set PA10 to input floating (Rx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //Mode Push Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_11 |
                                  GPIO_Pin_12 | GPIO_Pin_13 |
                                  GPIO_Pin_14 | GPIO_Pin_15;
                                  
    GPIO_Init(GPIOB, &GPIO_InitStructure); //Initialization

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //Mode Input Pull Up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                  GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_6 | GPIO_Pin_7;
                                  
    GPIO_Init(GPIOA, &GPIO_InitStructure); //Initialization

    // Set PC4 to analog input (ADC1 channel 14)
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
		
}


/*------------------------------------------------------------------------------
  Universal Synchronous/Asynchronous Receiver Transmitter, USART initialization
 *-----------------------------------------------------------------------------*/
static void USART_Configuration(void){
	USART_InitTypeDef USART_InitStructure;
  //Init USART1 to 9600 baud
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  	USART_ClockInitTypeDef Usart_ClockInitStructure;
  	USART_ClockInit(USART1, &Usart_ClockInitStructure);
    
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
	 
}


/*----------------------------------------
  Analog-Digital-Converter, ADC initialization
 *---------------------------------------*/
static void ADC_Configuration(void){
   	ADC_InitTypeDef ADC_InitStructure;
   	//Init ADC1 with Continous Conversion Mode and external Trigger
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
   	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //Continous Conversion Mode
   	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //external trigger line 11
   	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   	ADC_InitStructure.ADC_NbrOfChannel = 1;
   	ADC_Init(ADC1, &ADC_InitStructure);

   	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_28Cycles5);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
   	ADC_Cmd(ADC1, ENABLE);

  	ADC_ResetCalibration(ADC1);					//ADC Calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/*----------------------------------------
  NVIC Initialization
 *---------------------------------------*/
static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);								//Interrupt priority register: Setzen der Interrupt Priorit�t
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));	//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt ausl�st sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));	//Interrupt Set Enable Register: Enable interrupt
}


/*----------------------------------------
  			TIMER 3 Config
 *---------------------------------------*/
static void TIM3_Config(void)
{

int temp;

RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       // enable clock for GPIOC (APB2 Peripheral clock enable register)

/*---------------------- config Port PC7 -----------------------*/
temp = GPIOC->CRL;
temp &= 0x0FFFFFFF;	// delete PC7 config bit
temp |= 0x40000000;	// define PC7 as Input floating
GPIOC->CRL = temp;

/*---------------------- config Port PC8 and PC9 -----------------------*/
temp = GPIOC->CRH;
temp &= 0xFFFFFF00;	// delete PC8,PC9 config bits
temp |= 0x00000044;	// define PC8,PC9 as input floating
GPIOC->CRH = temp;

	/*---------------------- config timer 3 -----------------------*/
	RCC->APB1ENR |= 0x0002;	//timer 3 clock enable
	TIM3->SMCR = 0x0000;	//timer 3 clock selection: CK_INT 
	TIM3->CR1  = 0x0000;	//choose timer mode: upcounter

	/*Timer 3 -> 4MHz (look 32MHz config)
	no prescaler & auto reload value = 1024 equals in 256us refresh*/
	TIM3->ARR = 10000;		//auto reload value
	TIM3->RCR = 0;			//deactivate repetition counter 

	/*--------------------- config interrupt timer 3 ---------------------*/
	TIM3->DIER = 0x01;	   		//enable interrupt on overflow/underflow
	NVIC_init(TIM3_IRQn,2);	    //enable timer 3 update interrupt, priority 1

	/*-------------------------- start Timer 3 -------------------------------*/
  TIM3->CR1 |= 0x0001;   //set counter-enable bit
}



/*----------------------------------------------------------------
  Sends a string, character by character, over the specified UART
 *---------------------------------------------------------------*/
static void USART_SendString(USART_TypeDef *USARTx, char *str)
{
	while (*str) {
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *str++);
	}
}


/*----------------------------------------------------------------
  Sends an array, byte by byte, over the specified UART
 *---------------------------------------------------------------*/
static void USART_putBytes(USART_TypeDef *USARTx, uint8_t *bytes)
{
	while (*bytes) {
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		USART_SendData(USARTx, *bytes++);
}
}

#endif
