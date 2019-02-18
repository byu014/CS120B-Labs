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

task tasks[4];
const double tones[8] = {261.63, 293.66, 329.63, 349.23, 329, 440, 493.88, 523.25};
const unsigned short tasksNum = 4;
unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;
unsigned char buzzer = 0;
unsigned char j = 0;
unsigned char wasOn = 0;

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



void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0 | (1 << WGM00));
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
unsigned char decOrInt = 0;
unsigned char held = 0;
enum Calc_States{Calc_start, Calc_wait, Calc_ready};
int TickFct_Calc(int state)
{
	//static unsigned char isOn = 0;
	switch(state)
	{
		case Calc_start:
		state = Calc_ready;
		break;
		
		case Calc_ready:
			if((~PINA & 0x07) == 0x03)
			{
				decOrInt = 3;
				state = Calc_wait;
				break;
			}
			else if((~PINA & 0x07) == 0x01)
			{
				decOrInt = 1;
				state = Calc_wait;
				break;
			}
			else if((~PINA & 0x07) == 0x02)
			{
				decOrInt = 2;
				state = Calc_wait;
				break;
			}
			else
			{
				state = Calc_ready;
			}
			break;
		
		case Calc_wait:
		if((~PINA & 0x7) != 0x00)
		{
			held = 1;
			state = Calc_wait;
			break;
		}
		else
		{
			held = 0;
			decOrInt = 0;
			state = Calc_ready;
			break;
		}
		
		default:
		break;
	}
	return state;
}

unsigned int timer = 0;
unsigned int interval = 0;
unsigned char go = 0;
enum Counter_States{Counter_Start, Counter_check};
int TickFct_Counter(int state)
{
	static unsigned int counterTicks = 0;
	switch(state)
	{
		case Counter_Start:
			counterTicks = 0;
			timer = 0;
			interval = 1000;
			go = 1;
			state = Counter_check;
			break;
		
		case Counter_check:
			if(held){
				timer += 1;
			state = Counter_check;
			counterTicks += 1;
				if(counterTicks >= 3000)
				{
					counterTicks = 3000;
					interval = 400;
				}
				if(timer >= interval)
				{
					go = 1;
					timer = 0;
					break;
				}
			}
			else
			{
				state = Counter_Start;
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
		
		default:
			break;
	}
	return state;
}
enum Combine_States{Combine_Start};
int TickFct_Combine(int state)
{
	static unsigned char tempB = 0;
	switch(state)
	{
		case Combine_Start:
		if(held && go)
		{
			go = 0;
				if(decOrInt == 3)
				{
					tempB = 0;
					PORTB = tempB;
					break;
				}
				if(decOrInt == 1)
				{
					if(tempB < 9)
					{
						tempB += 1;
						PORTB = tempB;
					}
				}
				else if(decOrInt == 2)
				{
					if(tempB > 0)
					{
						tempB -= 1;
						PORTB = tempB;
					}
				}
			
		}
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
	DDRA = 0x00; PORTA = 0xFF;

	unsigned char i = 0;
	tasks[i].state = Counter_Start;
	tasks[i].period = 1;
	tasks[i].TickFct = &TickFct_Counter;
	i++;
	tasks[i].state = BLEDS_Start;
	tasks[i].period = 1000;
	tasks[i].TickFct = &TickFct_BLEDS;
	i++;
	tasks[i].state = Calc_start;
	tasks[i].period = 1;
	tasks[i].TickFct = &TickFct_Calc;
	i++;
	tasks[i].state = Combine_Start;
	tasks[i].period = 10;
	tasks[i].TickFct = &TickFct_Combine;
	TimerSet(1);
	TimerOn();
    while (1) 
    {
    }
	return 0;
}

