/*
 * byu014_lnguy089_lab4_part3.c
 *
 * Created: 1/19/2019 5:24:00 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 4 Exercise 3
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
enum States{Start,locked1, locked2, unlocked, wait }state;
unsigned char tempA;
unsigned char tempB;
unsigned char tempC;
enum States next;

void Tick()
{
	tempA = PINA;
	switch(state)
	{
		case Start:
			state = locked1;
			break;
		
		case wait:
		if(tempA == 0x00)
		{
			state = next;
		}
		else
		{
			state = wait;
		}
		break;
		
		case locked1:
		if(tempA == 0x04)
		{
			state = wait;
			next = locked2;
		}
		else if(tempA == 0x00)
		{
			state = locked1;
		}
		else
		{
			state = locked1;
		}
		break;
		
		case locked2:
		if(tempA == 0x02)
		{
			state = unlocked;
		}
		else if(tempA == 0x00)
		{
			state = locked2;
		}
		else
		{
			state = wait;
			next = locked1;
		}
		break;
		
		case unlocked:
			if(tempA == 0x80)
			{
				state = locked1;
			}
			else
			{
				state = unlocked;
			}
			break;
		default:
			break;
	}
	
	switch(state)
	{
		case Start:
			break;
			
		case wait:
		tempC = state;
		PORTC = tempC;
		
		case locked1:
			tempB = 0x0;
			PORTB = tempB;
			tempC = state;
			PORTC = tempC;
			break;
		
		case locked2:
			tempB = 0x0;
			PORTB = tempB;
			tempC = state;
			PORTC = tempC;
			break;
		
		case unlocked:
			tempB = 0x1;
			PORTB = tempB;
			tempC = state;
			PORTC = tempC;
			break;
		
		
			
	}
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xF; PORTB = 0x0;
	DDRC = 0xF; PORTC = 0x0;
    while (1) 
    {
		Tick();
    }
}

