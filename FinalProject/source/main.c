/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # FinalProject  Exercise # 3
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
#include "LCD_Functions.h"
#endif

unsigned short MAX = 1004;
unsigned short MIN = 30;
unsigned short deg;
unsigned short vPort = 0;
unsigned short hPort = 0;
unsigned short temp;
char vSpeed, hSpeed;
char vPos, hPos;
unsigned char itr, iPrint;
char vSpd[6] = "vSPD:";
char hSpd[6] = "hSPD:";
char vPosi[6] = "vPOS:";
char hPosi[6] = "hPOS:";
char neg = '-';
char* vSPD = vSpd;
char* hSPD = hSpd;
char* vPOS = vPosi;
char* hPOS = hPosi;


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
			ADCSRA |= (1 << ADSC);
			ADMUX = 0x00;
			hPort = ADC;
			delay_ms(1);
			ADCSRA |= (1 << ADSC);
			ADMUX |= (1 << MUX0);
			vPort = ADC;
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

			vSpeed = vSpeed * -1;

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

enum Position_States { Position_SMStart, Position_Track } Position_state;
void tickPosition() {
	static unsigned char even;
	static unsigned char vDist;
	static unsigned char hDist;
	static unsigned char vRem;
	static unsigned char hRem;
	switch(Position_state)
	{
		case Position_SMStart:
			vPos = 0;
			hPos = 0;
			even = 0;
			vDist = 0;
			hDist = 0;
			vRem = 0;
			hRem = 0;
			Position_state = Position_Track;
			break;
		case Position_Track:
			break;
		default:
			Position_state = Position_SMStart;
			break;
	}

	switch(Position_state)
	{
		case Position_Track:
			vDist += vSpeed;
			hDist += hSpeed;

			if(even)
			{
				vPos += (vDist / 2) + vRem;
				hPos += (hDist / 2) + hRem;

				if(vPos > 50)
				{
					vPos = 50;
				}
				else if(vPos < -50)
				{
					vPos = -50;
				}

				if(hPos > 50)
				{
					hPos = 50;
				}
				else if(hPos < -50)
				{
					hPos = -50;
				}

				vRem = vDist % 2;
				hRem = hDist % 2;
				vDist = 0;
				hDist = 0;
				even = 0;
			}
			else
			{
				even = 1;
			}
			break;
		default:
			break;
	}
}
enum Display_states { Display_SMStart, Display_Show } Display_state;
void tickDisplay() {
	static unsigned char tempDisplay;
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
			//clearDisplay(WHITE);
			//updateDisplay();
			if (vSpeed < 0) {
				tempDisplay = vSpeed * -1;
				setChar(neg, 36, 0, BLACK);
				setChar(tempDisplay + '0', 42, 0, BLACK);
			}
			else
			{
				setChar(' ', 36, 0, BLACK);
				setChar(vSpeed + '0', 42, 0, BLACK);
			}

			if (hSpeed < 0) {
				tempDisplay = hSpeed * -1;
				setChar(neg, 36, 8, BLACK);
				setChar(tempDisplay + '0', 42, 8, BLACK);
			}
			else
			{
				setChar(' ', 36, 8, BLACK);
				setChar(hSpeed + '0', 42, 8, BLACK);
			}

			if (vPos < 0) {
				setChar(neg, 36, 32, BLACK);
				tempDisplay = vPos * -1;
				if ((tempDisplay / 10) != 0) {
					setChar((tempDisplay / 10) + '0', 42, 32, BLACK);
					setChar((tempDisplay % 10) + '0', 48, 32, BLACK);
				}
				else {
					setChar((tempDisplay % 10) + '0', 42, 32, BLACK);
				}
			}
			else {
				setChar(' ', 36, 32, BLACK);
				if ((vPos / 10) != 0) {
					setChar((vPos / 10) + '0', 42, 32, BLACK);
					setChar((vPos % 10) + '0', 48, 32, BLACK);
				}
				else {
					setChar(' ', 42, 32, BLACK);
					setChar((vPos % 10) + '0', 48, 32, BLACK);
				}
			}

			if (hPos < 0) {
				setChar(neg, 36, 40, BLACK);
				tempDisplay = hPos * -1;
				if ((tempDisplay / 10) != 0) {
					setChar((tempDisplay / 10) + '0', 42, 40, BLACK);
					setChar((tempDisplay % 10) + '0', 48, 40, BLACK);
				}
				else {
					setChar((tempDisplay % 10) + '0', 42, 40, BLACK);
				}
			}
			else {
				setChar(' ', 36, 40, BLACK);
				if ((hPos / 10) != 0) {
					setChar((hPos / 10) + '0', 42, 40, BLACK);
					setChar((hPos % 10) + '0', 48, 40, BLACK);
				}
				else {
					setChar(' ', 42, 40, BLACK);
					setChar((hPos % 10) + '0', 48, 40, BLACK);
				}
			}
			updateDisplay();
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	ADC_init();
	lcdBegin();
	setStr(vSPD, 0, 0, BLACK);
	setStr(hSPD, 0, 8, BLACK);
	setStr(vPOS, 0, 32, BLACK);
	setStr(hPOS, 0, 40, BLACK);
	TimerSet(500);
	TimerOn();
	ReadSpeed_state = ReadSpeed_SMStart;
	Speed_state = Speed_SMStart;
	Position_state = Position_SMStart;
	Display_state = Display_SMStart;
//	Test_state = Test_SMStart;
	while (1) {
		tickReadSpeed();
		tickSpeed();
		tickPosition();
		tickDisplay();
		//clearDisplay(BLACK);
		//gotoXY(0, 0);
		//setChar('A', 0, 0, BLACK);
		//updateDisplay();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
