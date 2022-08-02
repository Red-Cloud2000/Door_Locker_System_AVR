
#include "control.h"
#include "uart.h"
#include "external_eeprom.h"
#include "motor.h"
#include "timer.h"
#include "buzzer.h"
#include "util/delay.h"
#include "LCD.h"

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
#define EEPROM_ADDRESS      0x0123

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 matched_flag=0;   /*flag to check if passwords match or not*/
uint8 received_pass[5];  /*array to hold the entered password received from HMI*/
uint8 saved_pass[5];      /*array to hold the  password saved in EEPROM*/
uint8 receivedDataFrom_HMI; /*variable to receive data from HMI (password and re-entered password)*/
uint32 g_tick;                  /*variable to make delays using timer*/
uint8 wrongPass;                /*variable to count number of wrong password entry times*/
uint8 first_pass[5];            /*first password sent from HMI*/
uint8 second_pass[5];           /*re-entered password sent from HMI*/
uint32 backToMain=0;


/*******************************************************************************
 *                      Functions                                              *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name] : SendToHMI
 *
 * [Description]   : A function to send data(byte) to HMI ECU
 *
 * [Args in]       : uint8 g_sentData
 * [Args out]      : NONE
 *******************************************************************************/
void SendToHMI(uint8 g_sentData)
{
	/*NOTE: the same code for sendToControl in the HMI driver */
	UART_sendByte(START_SENDING); /* notation that HMI is ready is ready to send */
	while(UART_recieveByte()!= START_RECEIVEING); /*wait until the control ecu is ready to receive*/
	UART_sendByte(g_sentData);                     /*data(byte)to send to HMI*/
	while(UART_recieveByte()!= COMPLETE_RECEIVEING); /*wait until the control ecu received the whole data*/

}

/*******************************************************************************
 * [Function Name] : ReceiveFromHMI
 *
 * [Description]   : A function to receive data(byte) from HMI ECU
 *
 * [Args in]       : NONE
 * [Args out]      : uint8 receivedDataFrom_HMI
 *******************************************************************************/
uint8 ReceiveFromHMI(void)
{
	while(UART_recieveByte()!=START_SENDING); /*wait until the HMI ecu ready to send */
	UART_sendByte(START_RECEIVEING); /*CONTROL is sending its notation to start receiving*/
	receivedDataFrom_HMI=UART_recieveByte();  /*Receive the data*/
	UART_sendByte(COMPLETE_RECEIVEING); /*CONTROL is ensuring that the whole data has been received*/
	return receivedDataFrom_HMI;
}

/*******************************************************************************
 * [Function Name] : get_pass_from_HMI
 *
 * [Description]   : A function to get password sent by HMI ECU
 *
 * [Args in]       : uint8 *received_pass
 * [Args out]      : NONE
 *******************************************************************************/
void get_pass_from_HMI(uint8 *received_pass){
	uint8 i;
	for(i=0;i<5;i++){
		received_pass[i]=UART_recieveByte(); /*receive the 5 char password from HMI*/
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
		get_pass_from_HMI(first_pass);  /*get first password from HMI*/
		get_pass_from_HMI(second_pass);  /*get second password from HMI*/
		matched_flag = compare_pass(first_pass,second_pass);    /*compare between the 2 passwords*/
		if(1==matched_flag){
			SendToHMI(SUCCESS);                 /*if matched, tell HMI that password matches so you can progress*/
			save_to_EEPROM(second_pass);        /*save the matched password to the EEPROM*/

			/*the added peripheral for code debugging to reassure that password is saved
			 * successfully in EEPROM or not by displaying it on an LCD*/
			read_from_EEPROM(saved_pass); /*read password from EEPROM and save it in an array*/

			LCD_displayStringRowColumn(0,0,"Pass in EEPROM:");
			uint8 i;
			for(i=0;i<5;i++){
				LCD_moveCursor(1,i);
				LCD_intgerToString(saved_pass[i]);    /*this function is used as password can't be anything but integers*/
			}
		}
		else{
			SendToHMI(FAILURE);       /*if not matched, tell HMI that password mis-matches so try again(you can try again forever)*/
		}
	}
}

/*******************************************************************************
 * [Function Name] : save_to_EEPROM
 *
 * [Description]   : A function to save password sent by HMI in EEPROM
 *
 * [Args in]       : uint8 password_to_be_saved[]
 * [Args out]      : NONE
 *******************************************************************************/
void save_to_EEPROM(uint8 password_to_be_saved[]){
	uint8 i;
	for(i=0;i<5;i++){
		EEPROM_writeByte(EEPROM_ADDRESS+i,password_to_be_saved[i]);  /*save the password in the passed array in a given address in the EEPROM*/
		_delay_ms(10);                     /*small delay to confirm saving*/
	}
}

/*******************************************************************************
 * [Function Name] : read_from_EEPROM
 *
 * [Description]   : A function to read the password sent by HMI and saved in EEPROM
 * 					 to save it in a given array
 *
 * [Args in]       : uint8 saved_password[]
 * [Args out]      : NONE
 *******************************************************************************/
void read_from_EEPROM(uint8 saved_password[]){
	uint8 i;
	for(i=0;i<5;i++){
		EEPROM_readByte(EEPROM_ADDRESS+i,&saved_password[i]); /*Read the password saved in a given address in the EEPROM in an array (pass thae array address only)*/
		_delay_ms(10);                     /*small delay to confirm reading*/
	}
}

/*******************************************************************************
 * [Function Name] : compare_pass
 *
 * [Description]   : A function to compare between two given arrays
 *
 * [Args in]       : uint8 *saved_password , uint8 *entered_password
 * [Args out]      : 1 or 0
 *******************************************************************************/
uint8 compare_pass(uint8 *saved_password , uint8 *entered_password){
	matched_flag = 0;
	uint8 i=0;
	for(i=0;i<5;i++)
	{
		if(saved_password[i] == entered_password[i]) /*compare between the 2 arrays char by char, if matches then continue*/
		{
			continue;
		}
		else
		{
			return 0;                          /*else return 0 that they miss-match*/
		}
	}
	return 1;
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
 * [Function Name] : set_UP_control_Timer
 *
 * [Description]   : A function to configure and initialize the timer
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void set_UP_control_Timer(void){
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
	set_UP_control_Timer();/*Timer is on to calculate how much time for each door state(movement)*/
	DcMotor_Rotate(A_CW);
	while(g_tick<59);      /*The door will be closing for 15 seconds (15/0.256=59)*/
	g_tick=0;              /*reset to start a new duration*/
	DcMotor_Rotate(stop);
	while(g_tick<12); 	   /*The door will be holding for 3 seconds (3/0.256=12)*/
	g_tick=0;         	   /*reset to start a new duration*/
	DcMotor_Rotate(CW);
	while(g_tick<59);      /*The door will be closing for 15 seconds (15/0.256=59)*/
	g_tick=0;              /*reset to start a new duration*/
	DcMotor_Rotate(stop);
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
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void Wrong_Pass(void){
	wrongPass++; /*increment wrongPass value each time the user miss-matches the saved password*/

	if(wrongPass==3)  /*when reaching 3 connected wrong entries enable a warning buzzer for 1min*/
	{
		set_UP_control_Timer(); /*Timer is on to calculate how much time to enable the buzzer*/
		buzzer_ON();            /*enable buzzer*/
		while(g_tick<235){      /*same as door movement time calculation we calculate here a 1min warning message (60/0.256=235)*/
			_delay_ms(300);
			buzzer_OFF();       /*disable buzzer*/
			_delay_ms(200);
			buzzer_ON();        /*enable buzzer*/
		}
		buzzer_OFF();           /*disable buzzer*/
		g_tick=0;               /*reset to start a new duration*/
		wrongPass=0;            /*reset the wrong password counter*/
		backToMain=1;

		Timer_DeInit();         /*disable timer*/
	}
}

/*******************************************************************************
 * [Function Name] : Main_Options
 *
 * [Description]   : A function to wait for user choice in HMI ECU and work according to it
 *					 whether it was opening door or changing password
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void Main_Options(void){

	uint8 received_option;                /*a variable to hold the chosen option sent from HMI*/
	received_option = ReceiveFromHMI();   /*receive option from HMI ECU*/
	matched_flag=0;

	switch(received_option){
	case openDoor:                               /*if HMI chose '+' (open door) */
		while(0==matched_flag){                  /*not to back to main until taking the 3 password entry chances*/
			read_from_EEPROM(saved_pass);        /*get the password saved in EEPROM*/
			get_pass_from_HMI(received_pass);    /*get the password from HMI*/
			matched_flag = compare_pass(saved_pass,received_pass); /*check for matching*/
			if(0==matched_flag){                 /*if mis-match send failure to HMI and increment wrongPass value*/
				SendToHMI(FAILURE);
				Wrong_Pass();
				if(backToMain==1){               /*if taken the 3 password entry chances*/
					backToMain=0;                /*reset back to main value*/
					break;                       /*break to get back to main after the warning buzzer*/
				}
			}
			else{                                /*if matching send success to HMI, start door movement and reset wrongPass value*/
				SendToHMI(SUCCESS);
				wrongPass=0;
				DoorMovement();
			}
		}
		break;

	case changePass:                             /*if HMI chose '-' (change password) */
		while(0==matched_flag){                  /*not to back to main until taking the 3 password entry chances*/
			read_from_EEPROM(saved_pass);        /*get the password saved in EEPROM*/
			get_pass_from_HMI(received_pass);    /*get the password from HMI*/
			matched_flag = compare_pass(saved_pass,received_pass);
			if(0==matched_flag){                 /*if mis-match send failure to HMI and increment wrongPass value*/
				SendToHMI(FAILURE);
				Wrong_Pass();
				if(backToMain==1){               /*if taken the 3 password entry chances*/
					backToMain=0;                /*reset back to main value*/
					break;                       /*break to get back to main after the warning buzzer*/
				}

			}
			else{                                /*if matching send success to HMI, set up a new password and reset wrongPass value*/
				SendToHMI(SUCCESS);
				wrongPass=0;
				new_pasword();
			}
		}
		break;

	}
}

