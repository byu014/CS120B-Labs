/*
 * byu014_lnguy089_lab9_part1.c
 *
 * Created: 2/7/2019 12:57:25 PM
 * Author : Sky
 */ 
/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 9 Exercise 2
 *I acknowledge all content contained herein, excluding template
 *or example code, is my own original work.
 */

#include <avr/io.h>
// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
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
enum States{start, up, down, wait, on, off, ready}state;
unsigned char i = 0;
const double tones[8] = {261.63, 293.66, 329.63, 349.23, 329, 440, 493.88, 523.25};
unsigned char isOn = 0;
void Tick()
{
	switch(state)
	{
		case start:
			state = off;
			break;
		
		case off:
			state = wait;
			break;
		
		case on:
			state = wait;
			break;
		case ready:
			if((~PINA & 0x07) == 0x01)
			{
				state = up;
				break;
			}
			else if((~PINA & 0x07) == 0x02)
			{
				if(isOn == 0)
				{
					state = on;
					isOn = 1;
					break;
				}
				else
				{
					isOn = 0;
					state = off;
					break;
				}
			}
			else if((~PINA & 0x07) == 0x04)
			{
				state = down;
				break;
			}
			else
			{
				state = ready;
				break;
			}
		case up:
			state = wait;
			break;
		
		case down:
			state = wait;
			break;
		case wait:
			if((~PINA & 0x7) != 0x00)
			{
				state = wait;
				break;
			}
			else
			{
				state = ready;
				break;	
			}
		default:
			break;
	}
	switch(state)
	{
		case start:
			break;
		case wait:
			break;
		case up:
			if(i < 7 && isOn)
			{
				i += 1;
				set_PWM(tones[i]);
			}
			break;
		case down:
			if(i > 0 && isOn)
			{
				i -= 1;
				set_PWM(tones[i]);
			}
			break;
		case off:
			set_PWM(0);
			break;
		case on:
			set_PWM(tones[i]);
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x08; PORTB = 0x00;
	PWM_on();
	state = start;
    while (1) 
    {
		Tick();
    }
}

