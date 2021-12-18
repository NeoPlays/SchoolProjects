/*
================================================================
Name: rendezvous.c
Author: Maximilian Roll & Martin Platajs
Project: RTOS Rendezvous

Description:
A semaphore rendezvous which should secure, 
that 2 input values are captured before further execution

Version: 1.0
================================================================
*/
#include "rendezvous.h"



/* ----------------------------- Prototypes -----------------------*/

void ch14_Thread1 (void  *argument);
void ch9_Thread2 (void  *argument);

/* ----------------------------- Varibales -----------------------*/

__IO uint16_t Ch_9 = 0;		//variable for value from ADC1 channel 9
__IO uint16_t Ch_14 = 0;	//variable for value from ADC1 channel 14
char buffer[100];			//string buffer for UART transfer




osThreadId_t T_ch14_Thread1, T_ch9_Thread2;		//thread identifiers
osSemaphoreId_t semArrived1,semArrived2;		//semaphore identifiers

/*-----------------------------------------------------------------------------
  Synchronise inputs before processing input-data using a semaphore rendezvous
 *----------------------------------------------------------------------------*/
 static const osThreadAttr_t ThreadAttr_CH14 = {	//create attributes for thread "ch14_Thread1" (only name attribute)
	.name = "CH14",
};
 
__NO_RETURN void ch14_Thread1 (void *argument) {
	int perc = 0;
	for (;;) {
		Ch_14 = ADC1ConvertedValues[1];						//get data from ADC1 channel14
		osSemaphoreRelease(semArrived2);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived1,osWaitForever);		//before continuing
		sprintf(&buffer[0], "\r\nChannel_14: %d",Ch_14);	//prepare string buffer for UART
		USART_SendString(USART1,buffer);					//send string via UART
		osSemaphoreRelease(semArrived2);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived1,osWaitForever);		//before continuing
		osDelay(delay);										//500 Ticks delay (placeholder because thread2 sends String via UART meanwhile)
		osSemaphoreRelease(semArrived2);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived1,osWaitForever);		//before continuing
		perc = ((Ch_14 + Ch_9)*100)/8190;					//calculate combined percentage of both variable resistors
		sprintf(&buffer[0],"\r\nCombined Percentage: %d\r\n",perc);		//prepare string buffer for UART
		USART_SendString(USART1,buffer);					//send string via UART
		osDelay(delay);
  }
}

/*-----------------------------------------------------------------------------
  Synchronise inputs before processing input-data using a semaphore rendezvous
 *----------------------------------------------------------------------------*/
static const osThreadAttr_t ThreadAttr_CH9 = {		//create attributes for thread "ch14_Thread1" (only name attribute)
	.name = "CH9",
};

__NO_RETURN void ch9_Thread2 (void *argument) {
	
	for (;;) {
		Ch_9 = ADC1ConvertedValues[0];						//get data from ADC1 channel9
		osSemaphoreRelease(semArrived1);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived2,osWaitForever);		//before continuing
		osDelay(delay);										//500 Ticks delay (placeholder because thread1 sends String via UART in meanwhile)
		osSemaphoreRelease(semArrived1);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived2,osWaitForever);		//before continuing
		sprintf(&buffer[0], "\r\nChannel_9:  %d",Ch_9);		//prepare string buffer for UART
		USART_SendString(USART1,buffer);					//send string via UART
		osSemaphoreRelease(semArrived1);					//The semaphores ensure both tasks arrive here
		osSemaphoreAcquire(semArrived2,osWaitForever);		//before continuing
	}
}

/*----------------------------------------------------------------------------
 Create the semaphores and start the threads
 *---------------------------------------------------------------------------*/

static const osSemaphoreAttr_t semAttr_SEM1 = {		//create attributes for thread "ch14_Thread1" (only name attribute)
	.name = "SEM1",
};

static const osSemaphoreAttr_t semAttr_SEM2 = {		//create attributes for thread "ch14_Thread1" (only name attribute)
	.name = "SEM2",
};

static const osThreadAttr_t ThreadAttr_main = {		//create attributes for thread "ch14_Thread1" (only name attribute)
	.name = "main",
};

void app_main (void *argument) 						//main thread
{
	semArrived1 = osSemaphoreNew(1,0,&semAttr_SEM1);				
	semArrived2 = osSemaphoreNew(1,0,&semAttr_SEM2);		
	T_ch14_Thread1 = osThreadNew(ch14_Thread1, NULL, &ThreadAttr_CH14);
	T_ch9_Thread2 =	osThreadNew(ch9_Thread2, NULL, &ThreadAttr_CH9);
}

int main (void) {
	DMA_Config();		//load DMA config
	ADC1_Init();		//initialize ADC1
	init_uart();			//initialize UART
	USART_SendData(USART1,12); //Clear Console
	USART_SendString(USART1,"\r\n*********************\r\n*  RTOS-Rendezvous  *\r\n*********************\r\n");
	SystemCoreClockUpdate();
  osKernelInitialize();                 						// Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, &ThreadAttr_main);    // Create application main thread
  if (osKernelGetState() == osKernelReady) {
    osKernelStart();                    						// Start thread execution
  }
	while(1);
}
