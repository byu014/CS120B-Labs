/*
 * byu014_lnguy089_lab8_part1.c
 *
 * Created: 2/5/2019 12:46:51 PM
 * Author : Sky
 */ 

#include <avr/io.h>
/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 8 Exercise 1
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;// is potentiometer an input
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempD = 0x00;
	ADC_init();
    while (1) 
    {
		unsigned short x = ~ADC;  // Value of ADC register now stored in variable x.
		tempB = (char)x;
		PORTB = tempB;
		tempD = (char)(x>>8);
		//tempD = tempD >> 8;
		PORTD = tempD;
    }
}

