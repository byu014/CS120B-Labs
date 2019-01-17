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
		if(tempA == 1 || tempA == 2)
		{
			SetBit(tempC, 4, 1);	
		}
		if(tempA == 3 || tempA == 4)
		{
			SetBit(tempC, 4, 1);	
		}
		if(tempA == 5 || tempA == 6)
		{
			SetBit(tempC, 3, 1);	
		}
		if(tempA == 7 || tempA == 8 || tempA == 9)
		{
			SetBit(tempC, 2, 1);	
		}
		if(tempA == 10 || tempA == 11 || tempA == 12)
		{
			SetBit(tempC, 1, 1);
		}
		if(tempA == 13 || tempA == 14 || tempA == 15)
		{
			SetBit(tempC, 0, 1);	
		}
		
		if(tempA <= 4)
		{
			SetBit(tempC, 6,1);	
		}
		//tempC = 0xFF;
		PORTC = tempC;
		//PORTC = tempA; 
    }
}

