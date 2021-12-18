/*
Name: Display.c
Autoren: Maximilian Roll & Martin Platajs
Projekt: Infrared_IO

Beschreibung: Testprogramm fue Library

Version: 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <armv10_std.h>
#include "Infrared_IO.h"

int main(void)
{
	ired_init();
	uart_init(9600);
	
	char data[12], result[6];
	

	//Initiation of LCD and Uart Communication
	lcd_init();
	lcd_clear();
	lcd_set_cursor(0,0);
	lcd_put_string("   -IRSensor-");
	lcd_set_cursor(1,0);
	lcd_put_string("Press x to start");

	uart_clear();
	uart_put_string("-IR_Sensor V1.0-\r\n Press x to start\r\n");
	if(uart_get_char() == 'x')
	{
		uart_put_string("Started sucessfully\r\n");
		uart_put_string("RC5 Initialized!\r\n");
		uart_put_string("Receiving Data\r\n");
		lcd_clear();
		lcd_set_cursor(0,0);
		lcd_put_string("Command:");
		lcd_set_cursor(1,0);
		
		do
		{
				ired_analysis(result,data);
				uart_put_string(result);
				uart_put_string("\t");
				uart_put_string(data);
				uart_put_string("\r\n");
				lcd_clear();
				lcd_set_cursor(0,0);
				lcd_put_string("Command:");
				lcd_set_cursor(1,0);
				lcd_put_string(result);
			
		}
		while(1);
	}
}
