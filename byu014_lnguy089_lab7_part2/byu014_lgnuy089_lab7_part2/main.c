/*
 * byu014_lnguy089_lab6_part1.c
 *
 * Created: 1/29/2019 12:48:19 PM
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 7 Exercise 2
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

enum States{start,init, lshift, rshift, pause ,wait}state;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempC = 0x00;
enum States prevState;
unsigned char score = 5;
unsigned char reset = 0;
unsigned char countFlag = 0;
unsigned char winTime = 10;

void Tick()
{
	switch(state)
	{
		case start:
			state = init;
			break;
		
		case init:
			state = lshift;
			score = 5;
			break;
		
		case lshift:
			if(~PINA & 0x01)
			{
				prevState = lshift;
				state = wait;
				break;
			}
			if(tempC == 0x04)
			{
				state = rshift;
			}
			else
			{
				state = lshift;
			}
			break;
		case rshift:
			if(~PINA & 0x01)
			{
				prevState = rshift;
				state = wait;
				break;
			}
			if(tempC == 0x01)
			{
				state = lshift;
			}
			else
			{
				state = rshift;
			}
			break;
		
		case wait:
			if(~PINA & 0x01)
			{
				state = wait;
			}
			else
			{
				state = pause;
			}
			break;
		
		case pause:
			if(reset == 10)
			{
				state = init;
			}
			if(~PINA & 0x01)
			{
				if(tempC == 0x04)
				{
					state = rshift;
				}
				else if(tempC == 0x01)
				{
					state = lshift;
				}
				else
				{
					state = prevState;
				}
			}
			else
			{
				state = pause;
			}
			break;
			
			
		default:
			break;
	}
	switch(state)
	{
		case start:
			break;
		
		case init:
			 score = 5;
			 LCD_Cursor(1);
			 LCD_WriteData(score + '0');
			 tempC = 0x01;
			 PORTD = tempC;
			 break;
			
		case lshift:
			tempC = tempC << 1;
			PORTD = tempC;
			break;
		
		case rshift:
			tempC = tempC >> 1;
			PORTD = tempC;
			break;
		
		case pause:
			if(tempC == 0x02)
			{
				if(score == 9 && winTime > 0)
				{
					//score = 9;
					LCD_Cursor(1);
					LCD_DisplayString(1,"WIN");
					winTime -= 1;
					if(winTime == 0)
					{
						state = start;
						winTime = 10;
						LCD_ClearScreen();
						break;
					}
				}
				else if(countFlag)
				{
					score += 1;
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
					countFlag = 0;
				}
				break;
			}
			else
			{
				if(score > 0 && countFlag)
				{
					score -= 1;
					countFlag = 0;
				}
				LCD_Cursor(1);
				LCD_WriteData(score + '0');
				break;
			}
			break;
		
		case wait:
		countFlag = 1;
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	TimerSet(300);
	TimerOn();
	LCD_init();
	//LCD_DisplayString(1, "5");
    while (1) 
    {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

