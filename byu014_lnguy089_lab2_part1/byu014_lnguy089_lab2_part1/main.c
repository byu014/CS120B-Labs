/*
 * byu014_lnguy089_lab2_part1.c
 *
 * Created: 2019/1/14 9:09:04
 * Author : Sky
 */ 

/*Bailey Yu, byu014@ucr.edu
 *Liem Nguyen, lnguy089@ucr.edu
 *Lab section: 22
 *Assignment: Lab 2 Exercise 1
 *I acknoledge all content contained herein, excluding template
 *or example code, is my own original work.
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

