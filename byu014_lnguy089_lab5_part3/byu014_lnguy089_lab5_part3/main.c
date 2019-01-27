/*
 * byu014_lnguy089_lab5_part3.c
 *
 * Created: 1/25/2019 3:46:18 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 5 Exercise 3
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
enum States{start, off, init, wait, shift, init2, blink, ready} state;
unsigned char tempA = 0x00;
unsigned char tempC = 0x00;
unsigned char counter = 0;

void Tick()
{
	tempA = ~PINA;
	switch(state)
	{
		case start:
			state = off;
			break;
		
		case off:
			if(tempA & 0x1)
			{
				state = init;
			}
			else
			{
				state = off;
			}
			break;
		
		case init:
			state = wait;
			break;
		
		case wait:
			if(tempA & 0x1)
			{
				state = wait;
			}
			else
			{
				state = ready;
			}	
			break;
		
		case init2:
			state = wait;
			break;
			
		case blink:
			state = wait;
		case shift:
			state = wait;
			break;
		
		case ready:
			if(tempA & 0x1)
			{
				if(counter < 6)
				{
					counter += 1;
					state = shift;
				}
				else if(counter == 6)
				{
					counter += 1;
					state = init2;
				}
				else if(counter < 12)
				{
					counter += 1;
					state = blink;
				}
				else
				{
					counter = 0;
					state = off;
				}
				break;
			}
			else
			{
				state = ready;
			}
			
			default:
				break;
	}
	switch(state)
	{
		case start:
			break;
		
		case off:
			tempC = 0x00;
			PORTC = tempC;
			break;
		
		case init:
			tempC = 0x2A;// 2A
			PORTC = tempC;
			break;
		
		case wait:
			break;
		
		case shift:
			tempC = ~tempC;
			PORTC = tempC;
			break;
		
		case init2:
			tempC = 0xFF;
			PORTC = tempC;
			break;
		
		case blink:
			tempC = ~tempC;
			PORTC = tempC;
			break;
		
		case ready:
			break;
		
		default:
			break; 
		
		
	}
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x0; PORTA = 0xF;
	DDRC = 0xFF; PORTC = 0x00;
    while (1) 
    {
		Tick();
    }
}

