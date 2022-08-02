#include "hmi.h"
#include "LCD.h"
#include "keypad.h"
#include "uart.h"
#include "util/delay.h"
#include "timer.h"

/***************************************************************************
 *                    Definitions                                          *
 ***************************************************************************/
#define START_SENDING       0x50
#define START_RECEIVEING    0x51
#define COMPLETE_RECEIVEING 0x52
#define SUCCESS             1
#define FAILURE             0
#define openDoor            '+'
#define changePass          '-'
#define WASTE               200
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 matched_flag=0;  /*flag to check if passwords match or not*/
uint8 password[5];         /*array to hold the entered password to send to control*/
uint8 receivedDataFrom_CONTROL; /*variable to receive data from control*/
uint32 g_tick;                  /*variable to make delays using timer*/
uint32 wrongPass;                /*variable to count number of wrong password entry times*/
uint32 backToMain=0;

/*******************************************************************************
 *                      Functions                                              *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name] : SendToControl
 *
 * [Description]   : A function to send data(byte) to CONTROL ECU
 *
 * [Args in]       : uint8 g_sentData
 * [Args out]      : NONE
 *******************************************************************************/
void SendToControl(uint8 sentDataTo_CONTROL ){

	UART_sendByte(START_SENDING); /* notation that HMI is ready is ready to send */
	while(UART_recieveByte()!= START_RECEIVEING); /*wait until the control ecu is ready to receive*/
	UART_sendByte(sentDataTo_CONTROL);
	while(UART_recieveByte()!= COMPLETE_RECEIVEING); /*wait until the control ecu received the whole data*/
}

/*******************************************************************************
 * [Function Name] : RecieveFromControl
 *
 * [Description]   : A function to receive data(byte) from CONTROL ECU
 *
 * [Args in]       : NONE
 * [Args out]      : uint8 receivedDataFrom_CONTROL
 *******************************************************************************/
uint8 RecieveFromControl(void){

	while(UART_recieveByte()!=START_SENDING); /*wait until the control ecu ready to send */
	UART_sendByte(START_RECEIVEING); /*HMI is sending its notation to start receiving*/
	receivedDataFrom_CONTROL=UART_recieveByte();  /*Receive the data*/
	UART_sendByte(COMPLETE_RECEIVEING); /*HMI is ensuring that the whole data has been received*/
	return receivedDataFrom_CONTROL;
}


/*******************************************************************************
 * [Function Name] : Enter_Password
 *
 * [Description]   : A function to enter user password
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void Enter_Password(void){

	uint8 i;
	uint8 key=0;                        /*variable to hold keypad pressed buttons value*/
	for(i=0;i<5;i++){                   /*loop 5 times to enter the 5 integer number password*/
		key = KEYPAD_getPressedKey();   /*get the value of the pressed button*/
		if((key <= 9) && (key >= 0)){   /*don't take any thing but integer numbers*/
			password[i]=key;            /*save each char in password array*/
			LCD_displayCharacter('*');  /*display an * instead of the number for security*/
		}
		else{
			i--;                        /*when entering anything but integers don't count that an entry*/
		}
		_delay_ms(500);
	}
}

/*******************************************************************************
 * [Function Name] : new_pasword
 *
 * [Description]   : A function to create new password
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void new_pasword(void){
	matched_flag=0;   /*make the system ready(Reset system) to receive a new password*/

	while(0==matched_flag){      /*never make any progress until password matching*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter new Pass:");
		LCD_moveCursor(1,0);
		Enter_Password();               /*ask the user to enter password*/
		save_pass_to_control(password); /*send the first password to control for matching checking*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Re-Enter password:");
		LCD_moveCursor(1,0);
		Enter_Password();               /*ask the user to enter password*/
		save_pass_to_control(password); /*send the first password to control for matching checking*/

		matched_flag = RecieveFromControl(); /*receive the matching value after compare in CONTROL*/
		LCD_clearScreen();

		if(0==matched_flag){                 /*if miss-matching*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,2,"MissMatching!!");
			LCD_displayStringRowColumn(1,2," Try Again!!!");
			_delay_ms(1500);
		}
		/*if matched main options screen will be displayed on the HMI LCD*/
	}
}

/*******************************************************************************
 * [Function Name] : save_pass_to_control
 *
 * [Description]   : A function to send password to CONTROL ECU
 *
 * [Args in]       : uint8 *sent_data
 * [Args out]      : NONE
 *******************************************************************************/
void save_pass_to_control(uint8 *sent_data){
	uint8 i;
	for(i=0;i<5;i++){
		UART_sendByte(*(sent_data+i));  /*send the 5 char password to the CONTROL ECU*/
	}
}

/*******************************************************************************
 * [Function Name] : g_tick_Incrementor
 *
 * [Description]   : A function to increment g_tick variable called by timer callback function
 *  				 to control time duration
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void g_tick_Incrementor(void){
	g_tick++;                      /*increment g_tick for time handling*/
}

/*******************************************************************************
 * [Function Name] : set_UP_HMI_Timer
 *
 * [Description]   : A function to configure and initialize the timer
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void set_UP_HMI_Timer(void){
	Timer_ConfigType Config_Ptr = {Timer0,compare,0,250,F_CPU_1024};   /*configurations of Timer*/
	Timer_Init(&Config_Ptr);            /*initializing Timer*/
	Timer0_SetCallBack(g_tick_Incrementor);   /*passing the callBack Function to timer to set it*/
}

/*******************************************************************************
 * [Function Name] : DoorMovement
 *
 * [Description]   : A function to control door movement after unlocking the door lock
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void DoorMovement(void){
	/*timer tick = (1/(1000000/1024))*250 = 0.256 */
	set_UP_HMI_Timer();    /*Timer is on to calculate how much the door will be opened*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"DoorIsOpening");

	while(g_tick<59){      /*The door will be closing for 15 seconds (15/0.256=59)*/
		LCD_displayStringRowColumn(0,13,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,14,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,15,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,15," ");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,14," ");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,13," ");
		_delay_ms(WASTE);
	}
	g_tick=0;              /*reset to start a new duration*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"HELLO Master, :)");
	while(g_tick<12); 	   /*The door will be holding for 3 seconds (3/0.256=12)*/
	g_tick=0;              /*reset to start a new duration*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"DoorIsClosing");
	while(g_tick<59){      /*The door will be closing for 15 seconds (15/0.256=59)*/
		LCD_displayStringRowColumn(0,13,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,14,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,15,".");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,15," ");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,14," ");
		_delay_ms(WASTE);
		LCD_displayStringRowColumn(0,13," ");
		_delay_ms(WASTE);
	}
	g_tick=0;              /*reset to start a new duration*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Security ON ;)");
	while(g_tick<8);       /*The door will be holding for 2 seconds (2/0.256=8)*/
	g_tick=0;              /*reset to start a new duration*/

	Timer_DeInit();        /*disable timer*/
}

/*******************************************************************************
 * [Function Name] : Wrong_Pass
 *
 * [Description]   : A function to count number of wrong password entry and activate
 * 					 a warning buzzer when more than 2 wrong entry
 *
 * [Args in]          : NONE
 * [Args out]         : NONE
 *******************************************************************************/
void Wrong_Pass(void){

	wrongPass++; /*increment wrongPass value each time the user miss-matches the saved password*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"INCORRECT PASS!!");
	_delay_ms(1500);
	if(wrongPass==3)  /*when reaching 3 connected wrong entries enable a warning buzzer for 1min*/
	{
		set_UP_HMI_Timer(); /*Timer is on to calculate how much time to enable the buzzer*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"            |I|");
		LCD_displayStringRowColumn(1,0,"7araamiii!!|_0_|");

		while(g_tick<235);     /*same as door movement time calculation we calculate here a 1min warning message (60/0.256=235)*/
		LCD_clearScreen();
		g_tick=0;              /*reset to start a new duration*/
		wrongPass=0;           /*reset the wrong trials*/
		backToMain=1;

		Timer_DeInit();        /*disable timer*/
	}
}

/*******************************************************************************
 * [Function Name] : Display_Main_Options
 *
 * [Description]   : A function to display main option screen on the LCD
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void Display_Main_Options(void){

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+:Open Door");
	LCD_displayStringRowColumn(1,0,"-:ChangePassword");
}

/*******************************************************************************
 * [Function Name] : Main_Options
 *
 * [Description]   : A function for user to choose the option to do by CONTROL ECU
 *  				 whether it was opening door or changing password
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void Main_Options(void)
{

	Display_Main_Options();          /*display main options screen on the LCD*/
	uint8 keyx=0;                    /*variable to hold keypad pressed buttons value*/
	keyx = KEYPAD_getPressedKey();   /*get the value of the pressed button*/
	matched_flag=0;
	switch(keyx)
	{
	case openDoor:
		SendToControl('+');                  /*send open door(+) option to CONTROL ECU*/
		while(0==matched_flag){              /*not to back to main until taking the 3 password entry chances*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Enter Pass:");
			LCD_moveCursor(1,0);
			Enter_Password();                    /*ask the user to enter password*/
			save_pass_to_control(password);      /*send the first password to control for matching checking*/
			matched_flag = RecieveFromControl(); /*wait for CONTROL ECU to compare between entered and saved password*/
			if(0==matched_flag){
				Wrong_Pass();                    /*if mis-match increment wrongPass value*/
				if(backToMain==1){               /*if taken the 3 password entry chances*/
					backToMain=0;                /*reset back to main value*/
					break;                       /*break to get back to main after the warning message*/
				}
			}
			else{                                /*if matching start door movement screen display and reset wrongPass value*/
				wrongPass=0;
				DoorMovement();
			}
		}
		break;

	case changePass:
		SendToControl('-');                  /*send change password(-) option to CONTROL ECU*/
		while(0==matched_flag){              /*not to back to main until taking the 3 password entry chances*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Enter Old Pass:");
			LCD_moveCursor(1,0);
			Enter_Password();                    /*ask the user to enter password*/
			save_pass_to_control(password);      /*send the first password to control for matching checking*/
			matched_flag = RecieveFromControl(); /*wait for CONTROL ECU to compare between entered and saved password*/
			if(0==matched_flag){
				Wrong_Pass();                    /*if mis-match increment wrongPass value*/
				if(backToMain==1){               /*if taken the 3 password entry chances*/
					backToMain=0;                /*reset back to main value*/
					break;                       /*break to get back to main after the warning message*/
				}
			}
			else{                                /*if matching start setting up new password routine and reset wrongPass value*/
				wrongPass=0;
				new_pasword();
			}
		}
		break;
	}
}

