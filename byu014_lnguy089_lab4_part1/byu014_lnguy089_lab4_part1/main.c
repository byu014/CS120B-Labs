/*
 * byu014_lnguy089_lab4_part1.c
 *
 * Created: 1/19/2019 4:08:55 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 4 Exercise 1
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
enum States{Start, s0, s1} state;
unsigned char tempA;
unsigned char tempB;

void Tick()
{
	tempA = PINA;
	tempB = 0x0;
	switch(state)
	{
		case Start:
			state = s0;
			break;
		
		case s0:
			if(tempA == 0x1)
			{
				state = s1;
			}
			else
			{
				state = s0;
			}
			break;
		
		case s1:
			if(tempA == 0x1)
			{
				state = s0;
			}
			else
			{
				state = s1;
			}
			break;
		
		default:
			break;
	}
	switch(state)
	{
		case Start:
			break;
		
		case s0:
			tempB = 0x1;
			break;
			
		case s1:
			tempB  = 0x2;
			break;
		
		default:
			break;
		
	}
		PORTB = tempB;
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x0; PORTA = 0x0;
	DDRB = 0xF; PORTB = 0x0;
	state = Start;
	
    while (1) 
    {
		Tick();
    }
}

