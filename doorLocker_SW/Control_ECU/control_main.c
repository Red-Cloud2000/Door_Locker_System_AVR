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
uint8 match_flag = 0;


int main (void)
{
	First_timeUsing();
	while(1)
	{
		Main_optionsOpenDoor();
		Main_optionsChangePass();
	}
}
