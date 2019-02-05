/*
 * test.c
 *
 * Created: 2/3/2019 10:58:14 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include "io.h"

int main(void)
{
	DDRC = 0xFF; PORTC=0x00;
	DDRD = 0xFF; PORTD=0x00;
    /* Replace with your application code */
	LCD_init();
	LCD_ClearScreen();
    while (1) 
    {
		LCD_DisplayString(1,"Hello World");
		
    }
}

