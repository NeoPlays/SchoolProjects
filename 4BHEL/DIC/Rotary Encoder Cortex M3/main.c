/******************************************************************************/
/*   (C) Copyright HTL - HOLLABRUNN  2020 All rights reserved. AUSTRIA   	  */ 
/*                                                                            */ 
/* File Name:   Inkrementalgeber.c                                         	  */
/* Autor: 		Maximilian Roll / Martin Platajs                              */
/* Version: 	V1.04.4                                                       */
/* Date: 		04/05/2020                                                    */
/* Description: displays clock and position of rotary encoder                 */
/******************************************************************************/
/* History:																	  */ 	
/*V1.00  creation															  */
/*V1.01.0 working clock														  */
/*V1.02.0 working rotary encoder											  */
/*V1.03.0 other version of rotary encoder									  */
/*V1.04.0 final working version of rotary encoder							  */	
/*V1.04.1 bug fixes and optimizations										  */
/*V1.04.1 bug fixes and optimizations										  */
/*V1.04.3 fixed clock prescaler and ARR									 	  */
/*V1.04.4 comments and final optimizations									  */										          										
/******************************************************************************/
#include <armv10_std.h>						 

/*------------------------------ Function Prototypes -------------------------*/
static void NVIC_init(char position, char priority);
static void TIM4_Config(void);
static void TIM3_Config(void);
static void set_clock_32MHz(void);
static void clock(void);
static void changed_pos(void);
static void init_ports(void);
void init_uart(unsigned long);

/*-----------------------------  Bit Banding Alias Adesses -------------------*/
#define GPIOC_IDR GPIOC_BASE + 2*sizeof(uint32_t)    						// Calc peripheral address GPIOC IDR
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))  
#define PC7  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,7)))		//Button of rotary encoder
#define PC8  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,8)))		//A-lane of rotary encoder
#define PC9  *((volatile unsigned long *)(BITBAND_PERI(GPIOC_IDR,9)))		//B-lane of rotary encoder

/*------------------------------ Static Variables-------------------------*/
volatile int position=2147483647;		//position of rotary encoder
volatile int ackn = 2;			//acknowledge for rotary encoder ("2" == ready, "1" == in counter clockwise turn, "0" == in clockwise turn)
volatile int rpos=0;			//position in clockwise turn 		 	("10"->"0"   "00"->"1"   "01"->"2"   "11"->"3")
volatile int lpos=0;			//position in counter clockwise turn 	("01"->"0"   "00"->"1"   "10"->"2"   "11"->"3")
volatile int ds;			//counter for clock 

/******************************************************************************/
/*							 Set Systemclock to 32 MHz		   				  */
/******************************************************************************/
static void set_clock_32MHz(void)		//overclock SystemClock to 32 MHz
{
	RCC->CR |= RCC_CR_HSEON;					//HSE on
	while ((RCC->CR & RCC_CR_HSERDY) == 0);		//waiting for HSE being ready (HSERDY=1)
	
	RCC->CFGR |= RCC_CFGR_PLLMULL4;			//multiply by 4 -> 4 times 8 MHz equals 32 MHz (SYSCLK)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV16;		//PCLK1 (APB1)=SYSCLK/8 -> 4MHz
	RCC->CFGR |= RCC_CFGR_PLLSRC;			
	
	RCC->CR |= RCC_CR_PLLON;				//PLL on
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);	//wating for PLL being ready (PLLRDY=1)
	
	RCC->CFGR |= RCC_CFGR_SW_PLL;					//PLL = Systemclock
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
	/*		waiting till SYSCLK is stabalized		*/
	
	while ((RCC->CFGR & RCC_CFGR_SWS) != ((RCC->CFGR<<2) & RCC_CFGR_SWS));
	
	RCC->BDCR |=RCC_BDCR_LSEON;		//32 kHz for RTC (AN2821 Reference Manual)
}

/******************************************************************************/
/*   Interrupt Service Routine  Timer4 (triggers every 0.1s and counts ds)    */
/******************************************************************************/
void TIM4_IRQHandler(void)	//timer 4, triggers every 0.1s
{
	TIM4->SR &=~0x01;	 // delete interrupt pending bit (prevents another interrupt)
	ds++;
	
}

/******************************************************************************/
/*   Interrupt Service Routine  Timer3 (checks for rotary encoder movement)   */
/******************************************************************************/
void TIM3_IRQHandler(void)	//timer 3, refreshes and checks for status of rotary encoder
{
	TIM3->SR &=~0x01;	 // delete interrupt pending bit (prevents another interrupt)
	
	if(PC8 == 1 & PC9 == 0 & rpos == 0 & ackn == 2){rpos=1;ackn=0;}		//set clockwise ackn(0), step 1 (10)
	else if(PC8 == 0 & PC9 == 0 & rpos == 1 & ackn == 0){rpos=2;}		//step 2 (00)
	else if(PC8 == 0 & PC9 == 1 & rpos == 2 & ackn == 0){rpos=3;}		//step 2 (01)
	else if(PC8 == 1 & PC9 == 1 & rpos == 3 & ackn == 0){rpos=0;ackn=2;position--;}		//set ready ackn(2), count position down, step 4 (11)
	
	if(PC8 == 0 & PC9 == 1 & lpos == 0 & ackn == 2){lpos=1;ackn=1;}		//set counter clockwise ackn(1), step 1 (01)
	else if(PC8 == 0 & PC9 == 0 & lpos == 1 & ackn == 1){lpos=2;}		//step 2 (00)
	else if(PC8 == 1 & PC9 == 0 & lpos == 2 & ackn == 1){lpos=3;}		//step 3 (10)
	else if(PC8 == 1 & PC9 == 1 & lpos == 3 & ackn == 1){lpos=0;ackn=2;position++;}		//set ready ackn(2), count position up, 4. step 4 (11)
	
	if(PC7 == 0){position =0;}		//reset position if butten pressed
}

/******************************************************************************/
/*              Initialization Timer4 (triggers every 0.1s)      		      */
/******************************************************************************/
static void TIM4_Config(void)
{	
	/*---------------------- config timer 4 -----------------------*/
	RCC->APB1ENR |= 0x0004;	//timer 4 clock enable
	TIM4->SMCR = 0x0000;	//timer 4 clock selection: CK_INT 
	TIM4->CR1  = 0x0000;	//choose timer mode: upcounter

	/*Timer 4 -> 4MHz (look 32MHz config)
	Prescaler = 390 & auto reload value = 1023 equals in 0.1s refresh*/
	TIM4->PSC = 390;		//prescaler value
	TIM4->ARR = 1023;		//auto reload value
	TIM4->RCR = 0;			//deactivate repetition counter 

	/*--------------------- config interrupt timer 4 ---------------------*/
	TIM4->DIER = 0x01;	   		//enable interrupt on overflow/underflow
	NVIC_init(TIM4_IRQn,1);	    //enable timer 4 update interrupt, priority 1

	/*-------------------------- start Timer 4 -------------------------------*/
    TIM4->CR1 |= 0x0001;   //set counter-enable bit
}

/******************************************************************************/
/*              Initialization Timer3 (triggers every 256us)      		      */
/******************************************************************************/ 
static void TIM3_Config(void)
{
	/*---------------------- config timer 3 -----------------------*/
	RCC->APB1ENR |= 0x0002;	//timer 3 clock enable
	TIM3->SMCR = 0x0000;	//timer 3 clock selection: CK_INT 
	TIM3->CR1  = 0x0000;	//choose timer mode: upcounter

	/*Timer 3 -> 4MHz (look 32MHz config)
	no prescaler & auto reload value = 1024 equals in 256us refresh*/
	TIM3->ARR = 1024;		//auto reload value
	TIM3->RCR = 0;			//deactivate repetition counter 

	/*--------------------- config interrupt timer 3 ---------------------*/
	TIM3->DIER = 0x01;	   		//enable interrupt on overflow/underflow
	NVIC_init(TIM3_IRQn,2);	    //enable timer 3 update interrupt, priority 1

	/*-------------------------- start Timer 3 -------------------------------*/
  TIM3->CR1 |= 0x0001;   //set counter-enable bit
}

/******************************************************************************/
/*                   NVIC_init(char position, char priority)    			  */ 
/* Function:                                                                  */    
/*   completley initializes an interrupt in the nested    					  */
/*   vectored interrupt controller (sets priority, prevents triggering,   	  */
/*   enables interrupt)                									  	  */
/* Übergabeparameter: "position" = 0-67 (interrupt number) 		              */
/*                    "priority": 0-15 (priority of the interrupt)		      */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);								//Interrupt priority register: Setzen der Interrupt Priorität
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));	//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));	//Interrupt Set Enable Register: Enable interrupt
}

/******************************************************************************/
/*        		    initializes ports of rotary encoder     			      */
/******************************************************************************/
static void init_ports(void)
{
int temp;

RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       // enable clock for GPIOC (APB2 Peripheral clock enable register)

temp = GPIOC->CRL;
temp &= 0x0FFFFFFF;	// delete PC7 config bit
temp |= 0x40000000;	// define PC7 as Input floating
GPIOC->CRL = temp;
	
temp = GPIOC->CRH;
temp &= 0xFFFFFF00;	// delete PC8,PC9 config bits
temp |= 0x00000044;	// define PC8,PC9 as input floating
GPIOC->CRH = temp;
}

/******************************************************************************/
/*        		    			  clock(void)			    			      */
/*	Function:													     		  */
/*		formats and prints clock for lcd output							 	  */
/******************************************************************************/
static void clock(void)
{
	//variables
	int lcd_ds;	
	int sec;
	int lcd_sec;
	int min;
	int lcd_min;
	int h;
	int lcd_h;
	char buffer[30];
	
	//form clock
	ds--;
	lcd_ds=ds%10;
	sec=ds/10;
	lcd_sec=sec%60;
	min=sec/60;
	lcd_min=min%60;
	h=min/60;
	lcd_h=h%60;
	
	//lcd output
	lcd_set_cursor(0,0);	// reset cursor
	sprintf(&buffer[0],"%02d:%02d:%02d:%d", lcd_h, lcd_min, lcd_sec, lcd_ds); // refresh LCD
  lcd_put_string(&buffer[0]);
	
	if(lcd_h == 23 & lcd_min == 59 & lcd_sec == 59)	
	{
		ds=0;
	}
}

/******************************************************************************/
/*        		    			initializes uart  			   			      */
/******************************************************************************/
void init_uart(unsigned long baudrate)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 	//supply GPIOA with clock
  
  GPIOA->CRH &= ~(0x00F0); 				//delete PA9 config
  GPIOA->CRH   |= (0x0BUL  << 4); 		//define PA9-Tx as alternate function output push pull

  GPIOA->CRH &= ~(0x0F00); 				//delete PA10 config
  GPIOA->CRH   |= (0x04UL  << 8); 		//define PA10-Rx as input floating
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //supply USART1 with clock

  USART1->BRR  = 32000000L/baudrate; 	//baudrate needs now 32000000 because of the changed systemclock
  
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE);   //activate RX, TX
  USART1->CR1 |= USART_CR1_UE;                    //activate USART
}

/******************************************************************************/
/*        		 updates uart and lcd if position has changed  			   	  */
/******************************************************************************/
static void changed_pos(void)
{
	char buffer[30];	
	lcd_set_cursor(1,10);
	lcd_put_string("       ");
	lcd_set_cursor(1,10);
	
	sprintf(&buffer[0],"%d", position); 	//update counter on lcd
  lcd_put_string(&buffer[0]);
	sprintf(&buffer[0],"Position: %d\r\n", position);		//updtae counter on uart
	uart_put_string(&buffer[0]);
}

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
	int ds_test=0;
	int position_test=0;

	set_clock_32MHz();		// set systemclock to 32MHz

	init_uart(9600);    	// 9600,8,n,1
	uart_clear();			
	USART1->CR1|=0x0020;	//USART1 RxNE - Interrupt Enable    

	lcd_init();    			//init lcd display
	lcd_clear();
	ds=0;

	init_ports();			//init Rotary Encoder Ports PC7, PC8 and PC9

	uart_put_string("\r\nRotary Encoder V1.4.4-\r\n---press x to start---\r\n");
	lcd_set_cursor(0,0);
	lcd_put_string("00:00:00:0");
	lcd_set_cursor(1,0);
	lcd_put_string("Position: 0");
	
	if(PC8 == 1 & PC9 == 1)		//check Signal of Rotary Encoder and UART connection
	{
		TIM4_Config();	    	//init Timer 4 Config
		TIM3_Config();			//init Timer 3 Config
		uart_put_string("\r\nRotary Encoder connected and ready to use\r\n");
		do
  		{
			if (ds_test != ds)		//has ds changed? (every 0.1 sec)
	 			{
					clock();		//update information
					ds_test=ds;		//reset test variable
				}
			if (position_test != position)		//has position changed? (if rotary encoder gets turned or pressed)
				{
					changed_pos();				//update information
					position_test=position;		//reset test variable
				}	
  		} while (1);							//endless loop
	}
}

