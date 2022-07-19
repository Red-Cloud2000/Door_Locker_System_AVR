/*
 ================================================================================================
 Name        : hmi_main.c

 Author      : Abdelrahman Hesham

 Description : Human interface main file for door locker application.

 Created on  : Jul 17, 2022
 ================================================================================================
 */
#include "hmi.h"
#include "LCD.h"
#include "uart.h"


#define FIRST_TIME_WORKING 0x10


int main(void){


	/* initializing drivers*/

	LCD_init();
	//UART_init(const UART_Config * UART_Config_Ptr);
	//Timer_init();

	while(1){
		while(UART_recieveByte() != FIRST_TIME_WORKING){
			options();
		}
		change_pass();
		options();

		/* sending a string to the control ECU that is used to check if
		 * this is the first entered password*/

		/* Receiving the check result in 0 or 1 from the control ECU */

		first_pass_flag=UART_recieveByte();

		/*if(first_pass_flag == 1){
			change_pass();
		}
		else{

			options();

		}*/
	}


	return 0 ;
}
