/*
 * byu014_lnguy089_lab3_part1.c
 *
 * Created: 1/15/2019 12:55:13 PM
 * Author : ucrcse
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 2 Exercise 2
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char counter = 0;
	unsigned char i = 0;
    while (1) 
    {
		for(i = 0; i < 8; ++i)
		{
			if(GetBit)	
		}
    }
}

