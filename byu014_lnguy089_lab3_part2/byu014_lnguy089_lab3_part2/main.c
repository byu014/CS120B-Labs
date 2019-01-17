/*
 * byu014_lnguy089_lab3_part2.c
 *
 * Created: 1/15/2019 1:30:20 PM
 * Author : ucrcse
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 3 Exercise 2
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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempC = 0x00;
    while (1) 
    {
		tempC = 0x00;
		tempA = PINA;
		if(tempA == 0x01 || tempA == 0x02)
		{
			tempC = 0x20;	
		}
		if(tempA == 0x03 || tempA == 0x04)
		{
			tempC = 0x10;	
		}
		if(tempA == 0x05 || tempA == 0x06)
		{
			tempC = 0x08;	
		}
		if(tempA == 0x07 || tempA == 0x08 || tempA == 0x09)
		{
			tempC = 0x04;	
		}
		if(tempA == 0x0A || tempA == 0x0B || tempA == 0x0C)
		{
			tempC = 0x02;
		}
		if(tempA == 0x0D || tempA == 0x0E || tempA == 0x0F)
		{
			tempC = 0x01;	
		}
		
		if(tempA <= 0x04)
		{
			tempC += 0x40;	
		}
		//tempC = 0xFF;
		PORTC = tempC;
		//PORTC = tempA; 
    }
}

