#include "hmi.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "avr/io.h"


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 first_time_flag=0;    /*A flag to see if it's the first time to use this system or not*/


int main(void)
{
/*******************************************************************************
*                           Peripherals Initialization                         *
*******************************************************************************/
	LCD_init();          /*initializing LCD to use its function*/
	UART_ConfigType ConfigPtr = {Paraity_Disabled,One_Stop_bit,eight_bit,9600};   /*configurations of UART*/
	UART_init(&ConfigPtr);     /*initializing UART*/
	SREG|=(1<<7);              /*Enabling Global interrupt*/

	first_time_flag = RecieveFromControl();   /*waiting for control to inform HMI if it's the first time to use the system or not*/

	if(1==first_time_flag){                   /*if the password never change set up a new password*/
	new_pasword();
	}                                         /*else go to main options*/

/*******************************************************************************
*                           All time running(App.) code                        *
*******************************************************************************/
	while(1)
	{
		Main_Options();                       /*Display main options screen and wait for user action(option selection)*/
	}
}
