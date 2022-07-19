/******************************************************************************
 *
 * Module:
 *
 * File Name :
 *
 * Description :
 *
 * Author: Abdlarhman Ayman
 *
 *******************************************************************************/

#include "util/delay.h"
#include "timer.h"
#include "dc_motor.h"
#include "control.h"
#include "uart.h"
#include "buzzer.h"
#include "external_eeprom.h"

uint8 matched_flag=0;      /* variable used to check if passwords are matched */
uint8 Recived_Password1[20];          /* array to save temporary string sent from HMI */
uint8 Recived_Password2[20];
uint8 eeprom[20];

uint8 Compare_Password(uint8 *First_Pass,uint8 *R2_enter_First_Pass)
{
	uint8 i=0;
	for(i=0;i<7;i++)
	{
		if(First_Pass[i] == R2_enter_First_Pass[i])
		{
			continue;
		}
		else
		{
			matched_flag = 0;
			return matched_flag;
		}
	}
	matched_flag = 1;

	return matched_flag;
}


void Open_Door(void)
{
	UART_sendByte(DOOR_IS_OPENING);
	Timer_ConfigType Config_Ptr0 = {Timer0,F_CPU_8,Compare,0,15};
	Timer_Init(&Config_Ptr0);
	DcMotor_Rotate_CW();			//state = stop ;
	Timer0_SetCallBack(DcMotor_Rotate_SOTP);
	Timer_DeInit();
}


void Hold_Door(void)
{
	Timer_ConfigType Config_Ptr1 = {Timer0,F_CPU_8,Compare,0,3};
	Timer_Init(&Config_Ptr1);
	Timer_DeInit();
}

void Close_Door(void)
{
	UART_sendByte(DOOR_IS_CLOSING);
	Timer_ConfigType Config_Ptr2 = {Timer0,F_CPU_8,Compare,0,15};
	Timer_Init(&Config_Ptr2);
	DcMotor_Rotate_A_CW();			//state = stop ;
	Timer0_SetCallBack(DcMotor_Rotate_SOTP);
	Timer_DeInit();
	UART_sendByte(MOTOR_STOP);
}

void Warning(void)
{
	UART_sendByte(WARNING);
	Timer_ConfigType Config_Ptr3 = {Timer0,F_CPU_8,Compare,0,60};
	Timer_Init(&Config_Ptr3);
	Buzzer_ON();
	Timer0_SetCallBack(Buzzer_OFF);
	Timer_DeInit();

}

void Set_Up_EEPROM(void)
{
	uint8 i;
	for( i = 0; i < 7;i++)
	{
		EEPROM_writeByte(0x0311, Recived_Password2[i]);
	}
}
void Main_optionsOpenDoor (void)
{
	uint8 i = 0;
	uint8 j = 1;
	UART_sendByte(CHECK_PASS);
	UART_receiveString(Recived_Password1);
	for(i=0;i<7;i++)
	{
		EEPROM_readByte(0x0311+i, &eeprom[i]);
	}
	while(Compare_Password(Recived_Password1,eeprom)!=1)
	{

		if(j != 3)
		{
			j++;
			UART_sendByte(CHECK_PASS);
			UART_receiveString(Recived_Password1);
			continue;
		}
		else
		{
			UART_sendByte(WARNING);

		}
	}
	Open_Door();
	Hold_Door();
	Close_Door();

}
void Main_optionsChangePass(void)
{
	uint8 i = 0;
	uint8 j = 1;
	UART_sendByte(CHECK_PASS);
	UART_receiveString(Recived_Password2);
	for(i=0;i<7;i++)
	{
		EEPROM_readByte(0x0311, &eeprom[i]);
	}
	while(Compare_Password(Recived_Password2,eeprom)!=1)
	{

		if(j != 3)
		{
			j++;
			UART_sendByte(CHECK_PASS);
			UART_receiveString(Recived_Password2);
			continue;
		}
		else
		{
			UART_sendByte(WARNING);

		}
	}
	Set_Up_EEPROM();
}
void First_timeUsing(void){

	UART_sendByte(CHECK_PASS);
	UART_receiveString(Recived_Password1);
	UART_sendByte(CHECK_PASS2);
	UART_receiveString(Recived_Password2);
	matched_flag =Compare_Password(Recived_Password1,Recived_Password2);
	if (matched_flag == 1)
	{
		Set_Up_EEPROM();
	}
	else
	{
		First_timeUsing();
	}
}
