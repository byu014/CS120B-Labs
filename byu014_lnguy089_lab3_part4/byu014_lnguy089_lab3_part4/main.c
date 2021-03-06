/*
 * byu014_lnguy089_lab3_part4.c
 *
 * Created: 1/17/2019 1:16:59 PM
 * Author : ucrcse
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 3 Exercise 4
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	unsigned char getA = 0x00;
    while (1) 
    {
		tempA = PINA;
		tempB = 0x00;
		tempC = 0x00;
		getA = 0x00;
		unsigned char i = 0;
		for(i = 0; i < 4; ++i)
		{
			getA = GetBit(tempA, i + 4);
			tempB = SetBit(tempB,i, getA);
		}
		for(i = 0; i < 4; ++i)
		{
			getA = GetBit(tempA, i);	
			tempC = SetBit(tempC, i + 4, getA);
		}
		PORTB = tempB;
		PORTC = tempC;
    }
}

