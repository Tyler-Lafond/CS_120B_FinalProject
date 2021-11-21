/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#endif

char vSpeed, hSpeed;
short vPos, hPos;
unsigned const char vSpd[6] = "vSPD: ";
unsigned const char hSpd[6] = "hSPD: ";
unsigned const char negative[1] = "-";
unsigned const char* vSPD = vSpd;
unsigned const char* hSPD = hSpd;
unsigned const char* neg = negative;


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit enables the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// 	in Free-Running Mode, a new conversion will trigger whenever
	// 	the previous conversion completes.
} 

enum Display_states { Display_SMStart, Display_Show } Display_state;
void tickDisplay() {
	switch(Display_state)
	{
		case Display_SMStart:
			Display_state = Display_Show;
			break;
		case Display_Show:
			Display_state = Display_Show;
			break;
		default:
			Display_state = Display_SMStart;
			break;
	}

	switch(Display_state)
	{
		case Display_Show:
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_DisplayString(1, vSPD);
			if (vSpeed < 0) {
				LCD_DisplayString(7, neg);
				LCD_Cursor(8);
				LCD_WriteData((vSpeed * -1) + '0');
			}
			else
			{
				LCD_Cursor(7);
				LCD_WriteData(vSpeed + '0');
			}
			LCD_Cursor(17);
			LCD_DisplayString(1, hSPD);
			if (hSpeed < 0) {
				LCD_DisplayString(23, neg);
				LCD_Cursor(24);
				LCD_WriteData((hSpeed * -1) + '0');
			}
			else
			{
				LCD_Cursor(23);
				LCD_WriteData(hSpeed + '0');
			}
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
    while (1) {

    }
    return 1;
}
