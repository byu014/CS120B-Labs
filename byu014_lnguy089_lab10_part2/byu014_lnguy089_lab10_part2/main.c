/*
 * byu014_lnguy089_lab10_part1.c
 *
 * Created: 2/12/2019 4:15:14 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

typedef struct task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int(*TickFct)(int);
} task;

task tasks[3];
const unsigned short tasksNum = 3;
unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;

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
	unsigned char i;
	for(i = 0; i < tasksNum; ++i)
	{
		if(tasks[i].elapsedTime >= tasks[i].period)
		{
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += 1;
	}
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


enum ThreeLEDS_States{ThreeLEDS_Start, threeLEDs_shift};
int TickFct_ThreeLEDS(int state)
{
	switch(state)
	{
		case ThreeLEDS_Start:
			threeLEDs = 0x01;
			state = threeLEDs_shift;
			break;
		
		case threeLEDs_shift:
			if(threeLEDs == 0x04)
			{
				threeLEDs = 0x01;
			}
			else
			{
				threeLEDs = threeLEDs << 1;
			}
			break;
		
		default:
		break;
			
	}
	return state;
}
enum BLEDS_States{BLEDS_Start, BLEDS_flip};
int TickFct_BLEDS(int state)
{
	switch(state)
	{
		case BLEDS_Start:
			blinkingLED = 0x08;
			state = BLEDS_flip;
			break;
		
		case BLEDS_flip:
			blinkingLED = (!(blinkingLED >> 3) ) << 3;
			break;
		
		default:
			break;
	}
	return state;
}
enum Combine_States{Combine_Start};
int TickFct_Combine(int state)
{
	switch(state)
	{
		case Combine_Start:
			PORTB = threeLEDs + blinkingLED;
			break;
		default:
			break;
	}
	return state;
}



int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char i = 0;
	tasks[i].state = ThreeLEDS_Start;
	tasks[i].period = 300;
	tasks[i].TickFct = &TickFct_ThreeLEDS;
	i++;
	tasks[i].state = BLEDS_Start;
	tasks[i].period = 1000;
	tasks[i].TickFct = &TickFct_BLEDS;
	i++;
	tasks[i].state = Combine_Start;
	tasks[i].period = 100;
	tasks[i].TickFct = &TickFct_Combine;
	
	TimerSet(1);
	TimerOn();
    while (1) 
    {
    }
	return 0;
}

