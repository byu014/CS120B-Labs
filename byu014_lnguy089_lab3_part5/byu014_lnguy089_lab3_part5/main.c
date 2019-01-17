/*
 * byu014_lnguy089_lab3_part5.c
 *
 * Created: 1/17/2019 1:28:01 PM
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
#include <math.h>


int main(void)
{
    /* Replace with your application code */
	DDRB = 0x6; PORTB = 0x0;
	DDRD = 0x00; PORTD = 0x00;
	unsigned short weight = 0;
	unsigned char tempB = 0;
	unsigned char tempD = 0;
    while (1) 
    {
		weight = 0;
		tempB = PINB;
		tempD = PIND;
		weight += tempB & 0x1;
		for(int i = 0; i < 8; ++i)
		{
			if(GetBit(tempD, i) == 1)
			{
				weight += pow(2,i+1);
			}
		}
		tempB = 0;
		if(weight >= 0x46)
		{
			tempB = 0x2;
		}
		else if(weight < 0x46 && weight > 0x05)
		{
			tempB = 0x4;
		}
		else if(weight < 0x05)
		{
			tempB = 0;
		}
		PORTB = tempB;
    }
}

