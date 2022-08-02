/*
 ================================================================================================
 Name        : hmi.c

 Author      : Abdelrahman Hesham

 Description : Human interface functions header file for door locker application.

 Created on  : Jul 16, 2022
 ================================================================================================
 */

#ifndef HMI_H_
#define HMI_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : A function to send data(byte) to CONTROL ECU
 *
 */
void SendToControl(uint8 sentDataTo_CONTROL );

/*
 * Description : A function to receive data(byte) from CONTROL ECU
 *
 */
uint8 RecieveFromControl(void);

/*
 * Description : A function to enter user password
 *
 */
void Enter_Password(void);

/*
 * Description : A function to create new password
 *
 */
void new_pasword(void);

/*
 * Description : A function to send password to CONTROL ECU
 *
 */
void save_pass_to_control(uint8 *sent_data);

/*
 * Description : A function to increment g_tick variable called by timer callback function
 *
 */
void g_tick_Incrementor(void);

/*
 * Description : A function to configure and initialize the timer
 *
 */
void set_UP_HMI_Timer(void);

/*
 * Description : A function to control door movement after unlocking the door lock
 *
 */
void DoorMovement(void);

/*
 * Description : A function to count number of wrong password entry and activate
 * 			     a warning buzzer when more than 2 wrong entry
 *
 */
void Wrong_Pass(void);

/*
 * Description : A function to display main option screen on the LCD
 *
 */
void Display_Main_Options(void);

/*
 * Description : A function for user to choose the option to do by CONTROL ECU
 *  			 whether it was opening door or changing password
 *
 */
void Main_Options(void);


#endif /* HMI_H_ */
