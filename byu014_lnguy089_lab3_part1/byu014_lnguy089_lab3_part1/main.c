/*
 * byu014_lnguy089_lab3_part1.c
 *
 * Created: 1/15/2019 12:55:13 PM
 * Author : ucrcse
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 3 Exercise 1
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	//unsigned char counter = 0;
	unsigned char i = 0;
    while (1) 
    {
		tempA = PINA;
		tempB = PINB;
		tempC = 0;
		//counter = 0;
		for(i = 0; i < 8; ++i)
		{
			if(GetBit(tempA, i))
			{
				tempC += 1;
			}	
			if(GetBit(tempB,i))
			{
				tempC += 1;	
			}
			
		}
		//tempC = counter;
		PORTC = tempC;
    }
}

