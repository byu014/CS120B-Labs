/*
 * byu014_lnguy089_lab2_part4.c
 *
 * Created: 1/13/2019 8:41:26 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 2 Exercise 4
 *I acknoledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0x00; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char totalWeight = 0x00;
	unsigned char tempA = 0;
	unsigned char tempB = 0;
	unsigned char tempC = 0;
	unsigned char tempD = 0;
	unsigned char shiftedWeight = 0;
	MCUCR = 0x80;
	//MCUCR = 0x80;
    while (1)
    {
		tempA = PINA;
		tempB = PINB;
		tempC = PINC;
		tempD = 0;
		
		totalWeight = tempA + tempB + tempC;
		if(totalWeight > 140)
		{
			tempD = tempD | 0x01;
		}
		if(abs(tempA - tempC) > 80)
		{
			tempD = tempD | 0x02;	
		}
		totalWeight = totalWeight/3;
		shiftedWeight = totalWeight << 2;
		tempD = tempD + shiftedWeight;
		PORTD = tempD;
    }
}

