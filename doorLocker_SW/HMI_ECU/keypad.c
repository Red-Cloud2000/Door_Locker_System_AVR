/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/
#include "keypad.h"
#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

#if (KEYPAD_NUM_COLS == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif (KEYPAD_NUM_COLS == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x4 keypad
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif

uint8 KEYPAD_getPressedKey(void){
	uint8 row,col;
	uint8 Keypad_port_value=0;
	while(1){
		for(col=0;col<KEYPAD_NUM_COLS;col++){

			GPIO_setupPortDirection(KEYPAD_PORT_ID,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT_ID,col+KEYPAD_FIRST_COLUMN_PIN_ID,PIN_OUTPUT);
#if(KEYPAD_BUTTON_PRESSED == LOGIC_LOW)
			Keypad_port_value = ~(1<<(col+KEYPAD_FIRST_COLUMN_PIN_ID));
#else
			Keypad_port_value = (1<<(col+KEYPAD_FIRST_COLUMN_PIN_ID));
#endif
			GPIO_writePort(KEYPAD_PORT_ID,Keypad_port_value);
			for(row=0;row<KEYPAD_NUM_ROWS;row++){
				if(GPIO_readPin(KEYPAD_PORT_ID,row+KEYPAD_FIRST_ROW_PIN_ID)==KEYPAD_BUTTON_PRESSED){
#if (KEYPAD_NUM_COLS == 3)
					return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
#elif (KEYPAD_NUM_COLS == 4)
					return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
#endif
				}
			}

		}
	}
}
#if (KEYPAD_NUM_COLS == 3)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button = 0;
	switch(button_number)
	{
	case 10: keypad_button = '*'; // ASCII Code of *
	break;
	case 11: keypad_button = 0;
	break;
	case 12: keypad_button = '#'; // ASCII Code of #
	break;
	default: keypad_button = button_number;
	break;
	}
	return keypad_button;
}

#elif (KEYPAD_NUM_COLS == 4)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number){
	uint8 keypad_button = 0;
	switch(button_number)
	{
	case 1: keypad_button = 7;
	break;
	case 2: keypad_button = 8;
	break;
	case 3: keypad_button = 9;
	break;
	case 4: keypad_button = '%'; // ASCII Code of %
	break;
	case 5: keypad_button = 4;
	break;
	case 6: keypad_button = 5;
	break;
	case 7: keypad_button = 6;
	break;
	case 8: keypad_button = '*'; /* ASCII Code of '*' */
	break;
	case 9: keypad_button = 1;
	break;
	case 10: keypad_button = 2;
	break;
	case 11: keypad_button = 3;
	break;
	case 12: keypad_button = '-'; /* ASCII Code of '-' */
	break;
	case 13: keypad_button = 13;  /* ASCII of Enter */
	break;
	case 14: keypad_button = 0;
	break;
	case 15: keypad_button = '='; /* ASCII Code of '=' */
	break;
	case 16: keypad_button = '+'; /* ASCII Code of '+' */
	break;
	default: keypad_button = button_number;
	break;
	}
	return keypad_button;
}

#endif
