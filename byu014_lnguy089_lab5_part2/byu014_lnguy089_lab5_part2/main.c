/*
 * byu014_lnguy089_lab4_part2.c
 *
 * Created: 1/19/2019 4:36:49 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 5 Exercise 2
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
enum States{Start, init, inc, dec,wait, ready, zero} state;
char unsigned tempA;
char unsigned tempC;

void Tick()
{
	tempA = ~PINA & 0x3;
	tempC = PORTC;
	switch(state)
	{
		case Start:
			state = init;
			break;
			
		case init:
			state = wait;
			break;
		
		case inc:
			state = wait;
			break;
		
		case dec:
			state = wait;
			break;
			
		case wait:
			if(tempA == 0x3)
			{
				state = zero;
				break;
			}
			if(tempA != 0x0)
			{
				state = wait;
			}
			else
			{
				state = ready;
			}
			break;
		
		case ready:
			if(tempA == 0x1)
			{
				state = inc;
			}
			else if(tempA == 0x2)
			{
				state = dec;
			}
			else
			{
				state = ready;
			}
			break;
			
		
		case zero:
			state = wait;
			break;
		
		default:
			break;
	}
	
	switch(state)
	{
		case Start:
			break;
			
		case init:
			tempC = 0;
			PORTC = tempC;
			break;
			
		case inc:
			if(tempC < 9)
			{
				tempC += 1;
				PORTC = tempC;
			}
			break;
		
		case dec:
			if(tempC > 0)
			{
				tempC -= 1;
				PORTC = tempC;
			}
			break;
		
		case wait:
			break;
		
		case zero:
			tempC = 0;
			PORTC = 0;
			break;
		 
		 default: 
			break;
	}
}

int main(void)
{
	DDRA = 0x0; PORTA = 0xF;
	DDRC = 0xF; PORTC = 0x0;
	state = Start;
    /* Replace with your application code */
    while (1) 
    {
		Tick();
    }
}

