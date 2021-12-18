/*
Name: Infrared_IO.c
Autoren: Maximilian Roll & Martin Platajs
Projekt: Infrared_IO

Beschreibung: Funktionen der zu erstellenden Library

Version: 1.0
*/


#include "Infrared_IO.h"


/*Functions*/
/*==============================================================================================================
Name: ired_init
Beschreibung: initializes RC5-Sensor
�bergibt: ---
Parameter: ---
==============================================================================================================*/ 
void ired_init(void) {
int temp;

RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;       // enable clock for GPIOA (APB2 Peripheral clock enable register)

// Datenleitung des RC5-Sensors
temp = GPIOA->CRL;
temp |= 0x00400000;	// PA5 als Input floating definieren
GPIOA->CRL = temp;
}

/*==============================================================================================================
Name: ired_wait_10us
Beschreibung: waits for 10us
�bergibt: ---
Parameter: ---
==============================================================================================================*/ 
void ired_wait_10us(){
int i;
	for(i = 0; i < 16; i++) {
	   }
}

/*==============================================================================================================
Name: ired_wait_for_bit
Beschreibung: waits 1.75 ms to capture the next bit
�bergibt: ---
Parameter: ---
==============================================================================================================*/
void ired_wait_for_bit() {
unsigned char i;
for (i = 0; i<175; i++) {
    ired_wait_10us();
	}
}

/*==============================================================================================================
Name: ired_RXD
Beschreibung: recieves data and converts it into a string
�bergibt: ---
Parameter: string for the recieved data
==============================================================================================================*/
void ired_RXD (char data[]) {
	
do{}while(RXD == 1);			//waiting for incoming bit
	
	
// The datalane of this sensor is activ-low and is reversed into activ-high in the following code.
if(RXD == 0)							//first start bit
	{
			ired_wait_for_bit();
					if(RXD == 1)				//second start bit (start requirements)
						{
								memset(data,0,strlen(data));		//delete data buffer
								for(int i=0; i<12; i++)					//read the next 12 bits after the 2 start bits
									{
										ired_wait_for_bit();
										if(RXD == 1)								// is bit 1?
											{
												strcat(data,"1");				// shift high bit
											}
										else
											{
												strcat(data,"0");				// else shift low bit
											}	
									}	
						}	 
	}
}

/*==============================================================================================================
Name: ired_analysis
Beschreibung: analysises the recieved data and returns a string with the captured command
�bergibt: ---
Parameter: string for the resulting command, string for the recieved data
==============================================================================================================*/
void ired_analysis (char result[], char data[]) {
	
	int i;
	char temp[6];			//string for command bits only
	
	ired_RXD(data);		//get data
	
	for(i=6; i<12; i++)		//extract command bits from data
		{
				temp[i-6] = data[i];
		}
		
		//Find and select the correct command
		if(strcmp(temp,Power) == 0)		//Power button
		{
			strcpy(result,"power");
		}
		else if(strcmp(temp,noFct) == 0)		//Button with no function
		{
			strcpy(result,"nofct");
		}
		else if(strcmp(temp,Display) == 0)		//Display button
		{
			strcpy(result,"display");
		}
		else if(strcmp(temp,Mute) == 0)		//Mute button
		{
			strcpy(result,"mute");
		}
		else if(strcmp(temp,ChUp) == 0)		//Channel + button
		{
			strcpy(result,"chup");
		}
		else if(strcmp(temp,ChDown) == 0)		//Channel - button
		{
			strcpy(result,"chdown");
		}
		else if(strcmp(temp,VolUp) == 0)		//Volume + button
		{
			strcpy(result,"volup");
		}
		else if(strcmp(temp,VolDown) == 0)		//Volume - button
		{
			strcpy(result,"voldown");
		}
		else
		{strcpy(result,"error");}		//if the data wasn't correctly transmitted it will show up as an error
}



