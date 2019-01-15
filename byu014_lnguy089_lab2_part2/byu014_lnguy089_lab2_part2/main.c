/*
 * byu014_lnguy089_lab2_part2.c
 *
 * Created: 1/13/2019 8:15:01 PM
 * Author : Sky
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
	DDRA = 0x0; PORTA = 0x0;
	DDRC = 0xF; PORTC = 0x0;
	unsigned char tempA = 0x0;
	unsigned char tempC = 0x0;
	unsigned char cntavail = 0;
	int i = 0;
    while (1) 
    {
		tempA = PINA;
		cntavail = 4;
		for(i = 0; i < 4; ++i)
		{
			if((tempA & 0x1) == 0x1)
			{
				cntavail -= 1;
			}	
			tempA = tempA >> 1;
		}
		tempC = cntavail;
		PORTC = tempC;
    }
}

