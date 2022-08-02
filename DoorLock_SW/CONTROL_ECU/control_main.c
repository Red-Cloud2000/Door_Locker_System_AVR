
#include "util/delay.h"
#include "timer.h"
#include "LCD.h"
#include "motor.h"
#include "control.h"
#include "uart.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "twi.h"
#include "avr/io.h"
#include <avr/interrupt.h>

/***************************************************************************
 *                    Definitions                                          *
 ***************************************************************************/
#define MY_SLAVE            0b00000001     /*Slave address*/
#define SUCCESS             1
#define FAILURE             0

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 first_pass[5]={'%','%','%','%','%'}; /*string to show that it's first time to use the system*/
uint8 saved_pass[5];     /*Array to save the password in EEPROM in it*/
uint8 first_time_flag=0; /*A flag to see if it's the first time to use this system or not*/


int main (void)
{
/*******************************************************************************
*                           Peripherals Initialization                         *
*******************************************************************************/
	/*I've used LCD here for debugging to see if password is saved in the external EEPROM
	 * or not and to display what is saved in it
	 */
	LCD_init();          /*initializing LCD to use its function*/

	TWI_ConfigType config_Ptr = {NO_PRESCALER,Normal_Mode,MY_SLAVE}; /*configurations of I2C*/
	TWI_init(&config_Ptr);    /*initializing I2C*/
	UART_ConfigType ConfigPtr = {Paraity_Disabled,One_Stop_bit,eight_bit,9600}; /*configurations of UART*/
	UART_init(&ConfigPtr);     /*initializing UART*/
	DcMotor_Init();            /*initializing DC_Motor*/
	buzzer_Init();
	SREG|=(1<<7);              /*Enabling Global interrupt*/


/*******************************************************************************
*                           Setting up the EEPROM                              *
*******************************************************************************/
	/*Only used to reset the system, as by deleting the comment(//) it saves a known string to EEPROM
	which is checked below and if this known string is saved it start the system by asking the user to
	create a new password and if it was another string it goes directly to the main options and if you
	forgot the password you can cheat and see it in the LCD on the down right😁😁😁
	for now I've reset it and the next one to use the same proteus would be asked to enter a new password.

	##steps to Reset system by yourself:
	 -----------------------------------
	 1- Remove the comment (just the 2 slashes(//) not all the line) sign from line 70
	 2- Build project
	 3- Run project on proteus(or the real-world system)(don't do anything or press anything just press the play button of proteus)
	 4- Stop simulation
	 5- Put back the 2 slashes(//) again (make line 70 commented again)
	 6- Build project again
	 7- Run proteus(or the real-world system)
	 8- Congratulations you've reseted the system ;-)
	 */


	//save_to_EEPROM(first_pass);

	read_from_EEPROM(saved_pass);         /*get the password saved in EEPROM*/

	LCD_displayStringRowColumn(0,0,"Pass in EEPROM:");

	uint8 i;
	for(i=0;i<5;i++){
		LCD_moveCursor(1,i);
		if(saved_pass[0]=='%')
			LCD_displayCharacter(saved_pass[i]);           /*display this password on the control LCD*/
		else
			LCD_intgerToString(saved_pass[i]);             /*display this password on the control LCD*/
	}

	first_time_flag=compare_pass(first_pass,saved_pass);  /*compare between the known first password and the one saved in EEPROM*/

	if(1==first_time_flag){
		SendToHMI(SUCCESS);           /*if the password never change set up a new password*/
		new_pasword();
	}
	else{                             /*else go to main options*/
		SendToHMI(FAILURE);
	}
/*******************************************************************************
*                           All time running(App.) code                        *
*******************************************************************************/
	while(1)
	{
		Main_Options();             /*wait for user action(option selection)*/
	}
}
