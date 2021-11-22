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

unsigned short vPort = 0;
unsigned short hPort = 0;
unsigned short temp;
char vSpeed, hSpeed;
short vPos, hPos;
unsigned char itr, iPrint;
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
			vSpeed = vPort;
			hSpeed = hPort;
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
			/*LCD_DisplayString(1, vSPD);
			if (vSpeed < 0) {
				LCD_DisplayString(7, neg);
				LCD_Cursor(8);
				LCD_WriteData((vSpeed * -1) + '0');
			}
			else
			{
				//LCD_Cursor(7);
				itr = 7;
				while(vPort != 0)
				{
					iPrint = vPort % 10;
					vPort = vPort / 10;
					LCD_Cursor(itr);
					LCD_WriteData(iPrint + '0');
					itr++;
				}
			}*/
			//LCD_Cursor(17);
			LCD_DisplayString(17, hSPD);
			if (hSpeed < 0) {
				LCD_DisplayString(23, neg);
				LCD_Cursor(24);
				LCD_WriteData((hSpeed * -1) + '0');
			}
			else
			{
				//LCD_Cursor(23);
				itr = 23;
				while(!hPort)
				{
					iPrint = hPort % 10;
					hPort = hPort / 10;
					LCD_Cursor(itr);
					LCD_WriteData(iPrint + '0');
					itr++;
				}
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
//	LCD_DisplayString(1, vSPD);
	while (1) {
		tickReadSpeed();
		tickDisplay();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
