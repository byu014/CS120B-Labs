/*
 * lcdtest.c
 *
 * Created: 2/4/2019 7:44:57 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char tempC = 0x00;
//enum States prevState;
unsigned char score = 5;
unsigned char reset = 0;

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
	DDRA = 0x00; PORTA = 0xFF;
    /* Replace with your application code */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0xFF;
	unsigned char oof = 0x00;
	LCD_init();
	TimerSet(1000);
	TimerOn();
	//LCD_DisplayString(1, "Hello World!");
    while (1) 
    {
		LCD_Cursor(1);
		LCD_WriteData(~PINA + '0');
		//LCD_DisplayString(1,"Fuck this lab");
		unsigned char tempA = ~PINA;
		if(tempA == 0x01)
		{
			oof = ~oof;
			PORTD = oof;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

