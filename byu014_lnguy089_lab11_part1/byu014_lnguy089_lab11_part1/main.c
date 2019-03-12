#include <avr/io.h>
#include <keypad.h>
#include <io.c>
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
const unsigned char tasksNum = 3;

unsigned char write;
unsigned char writeBool;
unsigned char cursorIndex;

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

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
enum kpStates{kp_start, kp_ready, kp_wait};
int TickFct_kp(int state)
{
	static unsigned char x;
	switch(state)
	{
		case kp_start:
			writeBool = 0;
			write = '\0';
			state = kp_ready;
			break;
		
		case kp_ready:
			x = GetKeypadKey();
			if(x != '\0')
			{
				write = x;
				state = kp_wait;
				break;
			}	
			break;
		
		case kp_wait:
			x = GetKeypadKey();
			if(x != '\0')
			{
				break;
			}
			writeBool = 1;
			state = kp_start;
			break;
		
		default: break;
			
	}
	return state;
}

enum cursorStates{cursor_start, cursor_count};
int TickFct_cursor(int state)
{
	switch(state)
	{
		case cursor_start:
			cursorIndex = 0;
			state = cursor_count;
			break;
		
		case cursor_count:
			if(writeBool)
			{
				if(cursorIndex > 15)
				{
					cursorIndex = 0;
				}
				cursorIndex += 1;
			}
			break;
			
	}
	return state;
}

enum combineStates{combine_start};
int TickFct_combine(int state)
{
	switch(state)
	{
		case combine_start:
			if(writeBool)
			{
				LCD_Cursor(cursorIndex);
				LCD_WriteData(write);
			}
			break;
	}
	return state;
}

const unsigned char stringSize = 16;
int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	LCD_init();
	LCD_DisplayString(1,"Congratulations!"); // 16 size
	unsigned char i = 0;
	tasks[i].state = kp_start;
	tasks[i].period = 1;
	tasks[i].TickFct = &TickFct_kp;
	i++;
	tasks[i].state = cursor_start;
	tasks[i].period = 1;
	tasks[i].TickFct = &TickFct_cursor;
	++i;
	tasks[i].state = combine_start;
	tasks[i].period = 1;
	tasks[i].TickFct = &TickFct_combine;
	TimerSet(1);
	TimerOn();
	while(1) {
		
	}
}
