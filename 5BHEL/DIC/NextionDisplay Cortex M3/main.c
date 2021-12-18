/*
================================================================
Name: main.c
Author: Martin Platajs
Project: NEXTION Touchscreen NX4832T035

Description:
This File Proviedes Communication via UART with the
Nextion Touchscreen

Version: 1.0
================================================================
*/

/* ----------- Includes and Defines -----------*/
#include "header.h"
#define RxBufferSize   0x20


/* --------------------------- Variables ---------------------------*/
uint8_t RxCounter = 0;
uint8_t RxBuffer[RxBufferSize];
char buffer[20];			//buffor for response messages
char buffer1[20];
static uint16_t value = 0, old_value = 0, input = 0, old_input = 0;

volatile int returnValue = 0, page=0, recieved = 0;

volatile int old_position=0;	//last position
volatile int position=0;		//position of rotary encoder
volatile int ackn = 2;			//acknowledge for rotary encoder ("2" == ready, "1" == in counter clockwise turn, "0" == in clockwise turn)
volatile int rpos=0;			//position in clockwise turn 		 	("10"->"0"   "00"->"1"   "01"->"2"   "11"->"3")
volatile int lpos=0;			//position in counter clockwise turn 	("01"->"0"   "00"->"1"   "10"->"2"   "11"->"3")




/* --------------------------- TIM3 ISR ---------------------------*/
//timer 3, refreshes and checks for status of rotary encoder
void TIM3_IRQHandler(void)	
{
TIM3->SR &=~0x01;		// delete interrupt pending bit (prevents another interrupt)
	
	if(PC8 == 1 & PC9 == 0 & rpos == 0 & ackn == 2){rpos=1;ackn=0;}		//set clockwise ackn(0), step 1 (10)
	else if(PC8 == 0 & PC9 == 0 & rpos == 1 & ackn == 0){rpos=2;}		//step 2 (00)
	else if(PC8 == 0 & PC9 == 1 & rpos == 2 & ackn == 0){rpos=3;}		//step 2 (01)
	else if(PC8 == 1 & PC9 == 1 & rpos == 3 & ackn == 0){rpos=0;ackn=2;position=position-10;}		//set ready ackn(2), count position down, step 4 (11)
	
	if(PC8 == 0 & PC9 == 1 & lpos == 0 & ackn == 2){lpos=1;ackn=1;}		//set counter clockwise ackn(1), step 1 (01)
	else if(PC8 == 0 & PC9 == 0 & lpos == 1 & ackn == 1){lpos=2;}		//step 2 (00)
	else if(PC8 == 1 & PC9 == 0 & lpos == 2 & ackn == 1){lpos=3;}		//step 3 (10)
	else if(PC8 == 1 & PC9 == 1 & lpos == 3 & ackn == 1){lpos=0;ackn=2;position=position+10;}		//set ready ackn(2), count position up, 4. step 4 (11)
	
	if(PC7 == 0){position =0;}		//reset position if button pressed
}


/* --------------------------- Code Evaluation and Execution ---------------------------*/
void executeFunction(uint8_t *RxCounter)
{
	for(int i=0; i < countof(return_values); i++){		//run through all known return-values to find matching one
		if(RxCounter[1] == return_values[i][1]){		//only second 
			if(RxCounter[2] == return_values[i][2])		//and third bytes are important (rest is identical to all others)
			{
				returnValue = ((return_values[i][1] * 100) + (return_values[i][2]));	//save recieved Value <secondByte>0<thirdByte> example : 202
			}
		}
	}
	//check if return-value was recieved
	if(recieved){
			//evaluate which Command has to be executed
			switch(returnValue){
				case SCHALTER_BUTTON_HOME:					//Button for Page "Schalter"
					page = 1;
						sprintf(buffer,"page 1ÿÿÿ");	  //Create Buffer for UART Transmision
						USART_SendString(USART1,buffer);	//Send via UART		
				break;

				case LEDS_BUTTON_HOME:					//Button for Page "LEDs"
					page = 2;
					sprintf(buffer,"page 2ÿÿÿ");	  //Create Buffer for UART Transmision
					USART_SendString(USART1,buffer);	//Send via UART
				break;
				
				case POTI_BUTTON_HOME:					//Button for Page "Poti"
					page = 3;
					sprintf(buffer,"page 3ÿÿÿ");	  //Create Buffer for UART Transmision
					USART_SendString(USART1,buffer);	//Send via UART
				break;
				
				case RE_BUTTON_HOME:					//Button for Page "RE(Rotary Encoder)"
					page = 4;
					sprintf(buffer,"page 4ÿÿÿ");	  //Create Buffer for UART Transmision
					USART_SendString(USART1,buffer);	//Send via UART
				break;
				
				case LED1_BUTTON:		//LED1 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15)));		//Toggle LED1 Status
				break;
				
				case LED2_BUTTON:		//LED2 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14)));		//Toggle LED2 Status
				break;
				
				case LED3_BUTTON:		//LED3 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_13)));		//Toggle LED3 Status
				break;
				
				case LED4_BUTTON:		//LED4 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)));		//Toggle LED4 Status
				break;
				
				case LED5_BUTTON:		//LED5 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11)));		//Toggle LED5 Status
				break;
				
				case LED6_BUTTON:		//LED6 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10)));		//Toggle LED6 Status
				break;
				
				case LED7_BUTTON:		//LED7 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9)));		//Toggle LED7 Status
				break;
				
				case LED8_BUTTON:		//LED8 Button
				GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));		//Toggle LED8 Status
				break;
				
				case BACK_BUTTON_SCHALTER:		//Back-Button to home-page
					page = 0;
				USART_putBytes(USART1,commands[0]);		//Send via UART byte per byte (hex)
				break;
				
				case BACK_BUTTON_LEDS:			//Back-Button to home-page
					page = 0;
				USART_putBytes(USART1,commands[0]);		//Send via UART byte per byte (hex)
				break;
				
				case BACK_BUTTON_POTI:			//Back-Button to home-page
					page = 0;
				USART_putBytes(USART1,commands[0]);		//Send via UART byte per byte (hex)
				break;
				
				case BACK_BUTTON_RE:			//Back-Button to home-page
					page = 0;
				USART_putBytes(USART1,commands[0]);		//Send via UART byte per byte (hex)
				break;
			}
			recieved = 0;	//reset recieved variable
		}
}


/* --------------------- ADC1_2 ISR ---------------------*/
void ADC1_2_IRQHandler()	//interrupt service routine for ADC1 (and ADC2)
{
	value = ADC_GetConversionValue(ADC1);	//save converted value
}


/* --------------------- ADC1_2 ISR ---------------------*/
void USART1_IRQHandler()	
{
	
   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//Check UART RX line status
   {
     /* Read one byte from the receive data register */
     RxBuffer[RxCounter++] = USART_ReceiveData(USART1);
     if(RxCounter == 7)		//7 bytes == one return Value 
     {
			 executeFunction(RxBuffer);
			 RxCounter = 0;
			 recieved = 1;
     }
  }

}


/* --------------- wait_mil ---------------*/
void wait_mil(void)	//waits one millisecond
{
    
    for(int i=0;i<7780;i++);
}

/* --------------- wait_sek ---------------*/
void wait_sek(int sek) //waits sek*100ms
{
    
    for(int i=0;i<100*sek;i++)
    {
        wait_mil();
    }
}

int main(){
RCC_Configuration();	//load RCC Configuration
NVIC_Configuration();	//load NVIC Configuration
GPIO_Configuration();	//laod GPUI Configuration
USART_Configuration();	//load USART Configuration
ADC_Configuration();	//laod ADC Configuration
TIM3_Config();			//load TIM3 Configuration

while(1){

	//update Poti value		
if(value != old_value && page == 3){
		sprintf(buffer,"j0.val=%dÿÿÿ",value/41);
		USART_SendString(USART1,buffer);
	old_value = value;
}

	//update Rotary Encoder position
if(position != old_position && page == 4){
		sprintf(buffer,"n1.val=%dÿÿÿ",position);
		USART_SendString(USART1,buffer);
	old_position = position;
}

	//Update Switches Values
input = GPIO_ReadInputData(GPIOA);
input = input & 0xFF;
if(input != old_input){
	sprintf(buffer,"n0.val=%dÿÿÿ",input);
	USART_SendString(USART1,buffer);
	old_input = input;
}

wait_sek(2);	//wait 200ms
}
}
