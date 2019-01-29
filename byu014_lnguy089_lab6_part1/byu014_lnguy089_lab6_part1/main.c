/*
 * byu014_lnguy089_lab6_part1.c
 *
 * Created: 1/29/2019 12:48:19 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 6 Exercise 1
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

enum States{start,init, shift}state;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempC = 0x00;

void Tick()
{
	switch(state)
	{
		case start:
			state = init;
			break;
		case init:
			state = shift;
		case shift:
			state = shift;
			break;
		
		default:
			break;
	}
	switch(state)
	{
		case start:
			break;
		
		case init:
			tempC = 0x01;
			PORTC = tempC;
			break;
			
		case shift:
			if(tempC == 0x04)
			{
				tempC = 0x01;
			}
			else
			{
				tempC = tempC << 1;
			}
			PORTC = tempC;
			break;
		
		default:
		break;
	}
}

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff()
{
	TimerFlag = 1;
}



void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


int main(void)
{
    /* Replace with your application code */
	DDRC = 0xFF;
	PORTC = 0x00;
	TimerSet(1000);
	TimerOn();
	
    while (1) 
    {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

