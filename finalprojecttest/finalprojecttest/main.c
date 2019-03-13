/*
 * finalprojecttest.c
 *
 * Created: 3/5/2019 1:55:19 PM
 * Author : Sky
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <keypad.h>
#include <stdlib.h> // for abs
#include <io.c>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char coordinates[8][8];
enum sm2_states{sm2_display};
int sm2_Tick(int state)
{
	static unsigned char i = 0;
	static unsigned char j = 0;
	unsigned char column_val2 = 0x00;
	unsigned char column_sel2 = 0xFF;
	switch(state)
	{
		case sm2_display:
		for(i = 0; i < 8; ++i)
		{
			if(coordinates[j][i] == 1)
			{
				column_val2 = SetBit(column_val2, j, 1);
				column_sel2 = SetBit(column_sel2, i, 0);
				
			}
		}
		PORTC = column_val2;
		PORTB = column_sel2;
		j+= 1;
		if(j == 8)
		{
			j = 0;
		}
		break;
	}
	return state;
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
	sm2_Tick(sm2_display);
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void adc_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void set_adc_pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	
	static unsigned char i = 0;
	for(i = 0; i < 15; ++i)
	{
		asm("nop");
	}
}

unsigned char EEMEM x = 0;
unsigned char pattern[8] = {0b00000,
							0b00000,
							0b01010,
							0b10101,
							0b10001,
							0b01110,
							0b00100,
							0b00000};
							
unsigned char pattern2[8] = {0b00000,
								0b00000,
								0b01010,
								0b11111,
								0b11111,
								0b01110,
								0b00100,
							0b00000};
	

enum SM1_States {sm1_display};
int SM1_Tick(int state) {
	// === Local Variables ===
	static unsigned char column_val = 0x01; // sets the pattern displayed on columns
	static unsigned char column_sel = 0x7F; // grounds column to display pattern
	// === Transitions ===
	switch (state) {

		case sm1_display: break;
		default: state = sm1_display;
		break;
	}
	// === Actions ===
	switch (state) {
		case sm1_display: // If illuminated LED in bottom right corner
		if (column_sel == 0xFE && column_val == 0x80) {
			column_sel = 0x7F; // display far left column
			column_val = 0x01; // pattern illuminates top row

		}

		// else if far right column was last to display (grounded)
		else if (column_sel == 0xFE) {

			column_sel = 0x7F; // resets display column to far left column
			column_val = column_val << 1; // shift down illuminated LED one row
		}

		// else Shift displayed column one to the right

		else {
			column_sel = (column_sel >> 1) | 0x80;
		}
		break;
		default: break;
	}
	PORTC = column_val; // PORTA displays column pattern
	PORTB = column_sel; // PORTB selects column to display pattern
	return state;
};
						
int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00; // x axis of led
	DDRC = 0xFF; PORTC = 0x00; // y axis of led
	DDRD = 0xFF; PORTD = 0x00; // lcd display
	DDRA = 0x07; PORTA = 0x08; //0x04 is lr, 0x05 is ud
	LCD_init();
	LCD_CreateCustomChar(1, pattern);
	LCD_CreateCustomChar(2, pattern2);
	unsigned char colval = 0x01;
	unsigned char colsel = 0x7F;
	//GetKeypadKey();
	adc_init();
	unsigned short ud = 0;
	unsigned short lr = 0;
	unsigned char string[32];
	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char flip = 0;
	for(row = 0; row < 8; ++row)
	{
		for(col = 0; col < 8; ++col)
		{
			coordinates[row][col] = 1;
		}	
	}
	//TimerSet(1000);
	//TimerOn();
    while (1) 
    {
		//if(GetKeypadKey() != '\0')
		//{
			//j = GetKeypadKey();
			//eeprom_write_byte(&x,j);
		//}
		
		
			//j = eeprom_read_byte(&x);
			//LCD_Cursor(1);
			//LCD_WriteData(j);
			//LCD_WriteData(0x01);
			//delay_ms(200);
			//LCD_Cursor(2);
			//LCD_WriteData(0x02);
			//delay_ms(200);
		
		//LCD_WriteData(j);
		unsigned char string[32];
		set_adc_pin(0x04);
		lr = ADC;
		set_adc_pin(0x05);
		ud =ADC;
		sprintf(string, "x: %d, y: %d", lr, ud);
		LCD_DisplayString(1,string);
		
		//set_adc_pin(0x06);
		//unsigned short pot = ADC;
		//sprintf(string, "%d", pot);
		//LCD_DisplayString(1, string);
		
		//if(lr < (512 - 100) && abs(ud - 512) < 256) // logic for going left
		//{
			//LCD_DisplayString(1, "left");
		//}
		//else if(lr > (512 + 100) && abs(ud - 512) < 256)
		//{
			//LCD_DisplayString(1, "right");
		//}
		//else
		//{
			//LCD_DisplayString(1, "oops");
		//}
		//sm2_Tick(sm2_display);
		//sprintf(string, "%d %d", coordinates[0][0], coordinates[1][1]);
		//LCD_DisplayString( 1, string);
		//if((~PINA & 0x08) == 0x08)
		//{
			//sprintf(string, "value of pin is: %d", (~PINA & 0x08));
			//LCD_DisplayString(1, "pressed");
		//}
		//else
		//{
			//LCD_DisplayString(1,"unpressed");
		//}
		
		delay_ms(300);
		
    }
}

