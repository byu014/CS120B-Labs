/*
 * byu014_lnguy089_lab4_part2.c
 *
 * Created: 1/19/2019 4:36:49 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 4 Exercise 2
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
enum States{Start, init, inc, dec,wait, zero} state;
char unsigned tempA;
char unsigned tempC;
unsigned char flag = 1;

void Tick()
{
	tempA = PINA;
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
			if (tempA == 0x3)
			{
				state = zero;
			}
			if(tempA == 0x00)
			{
				flag = 0;
			}
			if(flag == 1)
			{
				break;
			}
			if(tempA == 0x1)
			{
				state = inc;
			}
			else if(tempA == 0x2)
			{
				state = dec;
			}
			else if(tempA == 0x0)
			{
				state = wait;
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
			tempC = 7;
			PORTC = tempC;
			break;
			
		case inc:
			if(tempC < 9)
			{
				tempC += 1;
				PORTC = tempC;
			}
			flag = 1;
			break;
		
		case dec:
			if(tempC > 0)
			{
				tempC -= 1;
				PORTC = tempC;
			}
			flag = 1;
			break;
		
		case wait:
			break;
		
		case zero:
			flag = 1;
			tempC = 0;
			PORTC = 0;
			break;
		 
		 default: 
			break;
	}
}

int main(void)
{
	DDRA = 0x0; PORTA = 0x0;
	DDRC = 0xF; PORTC = 0x0;
    /* Replace with your application code */
    while (1) 
    {
		Tick();
    }
}

