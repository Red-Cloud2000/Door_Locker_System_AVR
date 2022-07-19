/*
 ================================================================================================
 Name        : hmi.c

 Author      : Abdelrahman Hesham

 Description : Human interface functions header file for door locker application.

 Created on  : Jul 16, 2022
 ================================================================================================
 */
#include "hmi.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"

uint8 password_arr[7];
uint8 Re_entered_password_arr[7];
void Enter_Password(void)
{
	uint8 i =0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"enter password ");
	while(KEYPAD_getPressedKey() != enter)
	{
		for(i=0;i<=4;i++)
		{
			password_arr[i]=KEYPAD_getPressedKey();
			LCD_displayStringRowColumn(1,i,"*");
			if(KEYPAD_getPressedKey() == backspace){
				LCD_displayStringRowColumn(1,i," ");
				i--;
			}
		}
		password_arr[5]='\0';
		password_arr[6]='#';
	}
}

void Re_enter_Password(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Re_enter password ");

	uint8 i =0;
	while(KEYPAD_getPressedKey() != enter)
	{
		for(i=0;i<=4;i++)
		{

			Re_entered_password_arr[i]=KEYPAD_getPressedKey();
			LCD_displayStringRowColumn(1,i,"*");
			if(KEYPAD_getPressedKey() == backspace){
				LCD_displayStringRowColumn(1,i," ");
				i--;
			}
		}
		Re_entered_password_arr[5]='\0';
		Re_entered_password_arr[6]='#';
	}
}

void Change_Password(void)
{
	Enter_Password();
	while(UART_recieveByte() != CHECK_PASS){}
	UART_sendString(password_arr);
}

void Main_Options(void)
{
	LCD_displayStringRowColumn(0,0,"+:open door");
	LCD_displayStringRowColumn(1,0,"-:change password");

	switch(KEYPAD_getPressedKey()){

	case open_door:
		Enter_Password();
		while(UART_recieveByte() != CHECK_PASS){}
		UART_sendString(password_arr);
		while(UART_recieveByte() != DOOR_IS_OPENING){}
		LCD_displayStringRowColumn(0,0,"Door is opening");
		while(UART_recieveByte() != DOOR_IS_CLOSING){}
		LCD_displayStringRowColumn(0,0,"Door is closing");
		break;

	case change_password:
		Enter_Password();
		while(UART_recieveByte() != CHECK_PASS){}
		UART_sendString(password_arr);

		break;
	}
}

void First_Time_Working(void)
{
	Enter_Password();
	while(UART_recieveByte() != CHECK_PASS){}
	UART_sendString(password_arr);
	Re_enter_Password();
	while(UART_recieveByte() != CHECK_PASS2){}
	UART_sendString(Re_entered_password_arr);
}
void Warning(void)
{

	LCD_displayStringRowColumn(0,0,"ALERT !!!!");
}


