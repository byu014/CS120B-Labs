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
enum States{Start,init, s0, s1, wait} state;
unsigned char tempA;
unsigned char tempB = 0x0;
unsigned char flag = 1;

void Tick()
{
	tempA = PINA;
	switch(state)
	{
		case Start:
			state = init;
			break;
		
		case init:
			state = wait;
		
		case s0:
			state = wait;
			break;
		
		case s1:
			state = wait;
			break;
			
		case wait:
			if(tempA == 0x0)
			{
				flag = 0;
			}
			if(flag)
			{
				break;
			}
			if(tempA == 0x1)
			{
				if(tempB == 0x2)
				{
					state = s0;
				}
				else if(tempB == 0x1)
				{
					state = s1;
				}
			}
			else
			{
				state = wait;
			}
			break;
		default:
			break;
	}
	switch(state)
	{
		case Start:
			break;
			
		case init:
			tempB = 0x1;
			PORTB = tempB;
			flag = 1;
			break;
			
		case wait:
			break;
		
		case s0:
			tempB = 0x1;
			PORTB = tempB;
			flag = 1;
			break;
			
		case s1:
			tempB  = 0x2;
			PORTB = tempB;
			flag = 1;
			break;
		
		default:
			break;
		
	}
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

