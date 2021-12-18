/*
Name: Infrared_IO.h
Autoren: Maximilian Roll & Martin Platajs
Projekt: Infrared_IO

Beschreibung: Header/Declarations

Version: 1.0
*/

#ifndef INFRARED_H_INCLUDED
#define INFRARED_H_INCLUDED

/*Includes*/
/*==============================================================================================================*/
#include <stm32f10x.h>
#include <string.h>
/*==============================================================================================================*/

/*Defines*/
/*==============================================================================================================*/
// Calc Bit Band Adress: a = peripheral address b = Bit number
#define BITBAND_PERI(a,b) ((PERIPH_BB_BASE + (a-PERIPH_BASE)*32 + (b*4)))
#define GPIOA_IDR GPIOA_BASE + 2*sizeof(uint32_t)
// Calc peripheral				address GPIOA IDR				
#define RXD *((volatile unsigned long *)(BITBAND_PERI(GPIOA_IDR,5)))
// PA5

#define Power "001100"		//Power Button Command
#define noFct "100110"		//no Function Button Command
#define Mute "001101"			//Mute Button Command
#define Display "001111"	//Display Button Command
#define ChUp "100000"			//Channel UP Button Command
#define ChDown "100001"		//Channel DOWN Button Command
#define VolUp "010000"		//Volume UP Button Command
#define VolDown "010001"	//Volume DOWN Button Command
/*==============================================================================================================*/

/*Function Prototypes*/
/*==============================================================================================================*/
void ired_init(void);
void wait_10us_ired(void);
void wait_for_bit(void);
void ired_RXD (char data[]);
void ired_analysis (char result[], char data[]);
/*==============================================================================================================*/

#endif //INFRARED_H_INCLUDED


