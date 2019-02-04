/*
 * byu014_lnguy089_lab7_part1.c
 *
 * Created: 2/1/2019 2:20:51 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include "io.c"
//#include "lcd_8bit_task.h"

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "Hello World");
	
	
	while(1) {continue;}
}
