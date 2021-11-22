/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # FinalProject  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "io.h"
#endif

unsigned short MAX = 1004;
unsigned short MIN = 30;
unsigned short deg;
unsigned short vPort = 0;
unsigned short hPort = 0;
unsigned short temp;
char vSpeed, hSpeed;
short vPos, hPos;
unsigned char itr, iPrint;
unsigned const char vSpd[6] = "vSPD:";
unsigned const char hSpd[6] = "hSPD:";
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

enum ReadSpeed_states { ReadSpeed_SMStart, ReadSpeed_Read } ReadSpeed_state;
void tickReadSpeed() {
	switch(ReadSpeed_state)
	{
		case ReadSpeed_SMStart:
			ReadSpeed_state = ReadSpeed_Read;
			break;
		case ReadSpeed_Read:
			ReadSpeed_state = ReadSpeed_Read;
			break;
		default:
			break;
	}

	switch(ReadSpeed_state)
	{
		case ReadSpeed_Read:
			ADMUX = 0x00;
			vPort = ADC;
			ADMUX = 0x01;
			hPort = ADC;
			break;
		default:
			break;
	}
}

enum Speed_states { Speed_SMStart, Speed_Convert } Speed_state;
void tickSpeed() {
	deg = MAX/11;
	switch(Speed_state)
	{
		case Speed_SMStart:
			Speed_state = Speed_Convert;
			break;
		case Speed_Convert:
			Speed_state = Speed_Convert;
			break;
		default:
			Speed_state = Speed_SMStart;
			break;
	}

	switch(Speed_state)
	{
		case Speed_Convert:
			if (vPort >= (deg*10))
			{
				vSpeed = 5;
			}
			else if (vPort >= (deg*9))
			{
				vSpeed = 4;
			}
			else if (vPort >= (deg*8))
			{
				vSpeed = 3;
			}
			else if (vPort >= (deg*7))
			{
				vSpeed = 2;
			}
			else if (vPort >= (deg*6))
			{
				vSpeed = 1;
			}
			else if (vPort >= (deg*5))
			{
				vSpeed = 0;
			}
			else if (vPort >= (deg*4))
			{
				vSpeed = -1;
			}
			else if (vPort >= (deg*3))
			{
				vSpeed = -2;
			}
			else if (vPort >= (deg*2))
			{
				vSpeed = -3;
			}
			else if (vPort >= (deg))
			{
				vSpeed = -4;
			}
			else
			{
				vSpeed = -5;
			}

			if (hPort >= (deg*10))
			{
				hSpeed = 5;
			}
			else if (hPort >= (deg*9))
			{
				hSpeed = 4;
			}
			else if (hPort >= (deg*8))
			{
				hSpeed = 3;
			}
			else if (hPort >= (deg*7))
			{
				hSpeed = 2;
			}
			else if (hPort >= (deg*6))
			{
				hSpeed = 1;
			}
			else if (hPort >= (deg*5))
			{
				hSpeed = 0;
			}
			else if (hPort >= (deg*4))
			{
				hSpeed = -1;
			}
			else if (hPort >= (deg*3))
			{
				hSpeed = -2;
			}
			else if (hPort >= (deg*2))
			{
				hSpeed = -3;
			}
			else if (hPort >= (deg))
			{
				hSpeed = -4;
			}
			else
			{
				hSpeed = -5;
			}

			break;
		default:
			break;
	}
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
		//	LCD_Cursor(1);
			LCD_DisplayString(1, vSPD);
			if (vSpeed < 0) {
				LCD_DisplayString(6, neg);
				LCD_Cursor(7);
				LCD_WriteData((vSpeed * -1) + '0');
			}
			else
			{
				LCD_Cursor(6);
				LCD_WriteData(vSpeed + '0');
			}
			//LCD_Cursor(17);
			LCD_DisplayString(17, hSPD);
			if (hSpeed < 0) {
				LCD_DisplayString(22, neg);
				LCD_Cursor(23);
				LCD_WriteData((hSpeed * -1) + '0');
			}
			else
			{
				LCD_Cursor(22);
				LCD_WriteData(hSpeed + '0');
			}
			break;
		default:
			break;
	}
}

//SM to test features
/*
enum Test_states { Test_SMStart, Test_Test } Test_state;
void tickTest() {
	switch(Test_state)
	{
		case Test_SMStart:
			Test_state = Test_Test;
			break;
		case Test_Test:
			break;
		default:
			Test_state = Test_SMStart;
			break;
	}

	switch(Test_state)
	{
		case Test_Test:
			LCD_ClearScreen();
			LCD_Cursor(1);
			LCD_WriteData(1 + '0');
			break;
		default:
			break;
	}
}*/

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	ADC_init();
	LCD_init();
	TimerSet(1000);
	TimerOn();
	ReadSpeed_state = ReadSpeed_SMStart;
	Display_state = Display_SMStart;
//	Test_state = Test_SMStart;
	while (1) {
		tickReadSpeed();
		tickSpeed();
		tickDisplay();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
