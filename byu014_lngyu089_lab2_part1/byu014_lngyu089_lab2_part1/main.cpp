/*
 * byu014_lngyu089_lab2_part1.cpp
 *
 * Created: 1/10/2019 1:15:59 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x0; PORTA = 0xF; //sets PORTA as input
	DDRB = 0xF; PORTB = 0x0;//sets PORTB as output
	
	unsigned char tempA = 0x0;// initialize tempA
	unsigned char tempB = 0x0;//initialize tempB
    while (1) 
	{
		tempA = PINA;
		
		if(tempA== 0x1)
		{
			tempB = 0x1;
		}
		else
		{
			tempB = 0x0;	
		}
		PORTB = tempB;
    }
	return 0;
}

