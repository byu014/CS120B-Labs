/*
 * finalproject.c
 *
 * Created: 2/28/2019 12:38:21 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <io.c>
#include <keypad.h>
#include <avr/interrupt.h>
#include <stdlib.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

typedef struct task{
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int(*TickFct)(int);
} task;

task tasks[1];
const unsigned char tasksNum = 1;
unsigned char score = 0;
unsigned char EEMEM highScore = 0;

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


enum eeprom{eeprom_start, eeprom_loop};
int TickFct_eeprom(int state)
{
	static char current = 0;
	switch(state)
	{

	}
	return state;
}

int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00; // x axis of led
	DDRC = 0xFF; PORTC = 0x00; // y axis of led
	DDRD = 0xFF; PORTD = 0x00; // lcd display
	DDRA = 0x0F; PORTA = 0x00; //0x04 is lr, 0x05 is ud
	unsigned char i = 0;
	tasks[i].state = eeprom_start;
	tasks[i].period = 500;
	tasks[i].TickFct = &TickFct_eeprom;
	
	
	TimerSet(1);
	TimerOn();
    while (1) 
    {

    }
}

