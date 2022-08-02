
#ifndef CONTROL_H_
#define CONTROL_H_
#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : A function to send data(byte) to HMI ECU
 *
 */
void SendToHMI(uint8 g_sentData);

/*
 * Description : A function to receive data(byte) from HMI ECU
 *
 */
uint8 ReceiveFromHMI(void);

/*
 * Description : A function to get password sent by HMI ECU
 *
 */
void get_pass_from_HMI(uint8 *received_pass);

/*
 * Description : A function to create new password
 *
 */
void new_pasword(void);

/*
 * Description : A function to save password sent by HMI in EEPROM
 *
 */
void save_to_EEPROM(uint8 *password_to_be_saved);

/*
 * Description : A function to read the password sent by HMI and saved in EEPROM
 *
 */
void read_from_EEPROM(uint8 *saved_password);

/*
 * Description : A function to compare between two given arrays
 *
 */
uint8 compare_pass(uint8 *saved_password , uint8 *entered_password);

/*
 * Description : A function to increment g_tick variable called by timer callback function
 *  			 to control time duration
 *
 */
void g_tick_Incrementor(void);

/*
 * Description : A function to configure and initialize the timer
 *
 */
void set_UP_control_Timer(void);

/*
 * Description : A function to control door movement after unlocking the door lock
 *
 */
void DoorMovement(void);

/*
 * Description : A function to count number of wrong password entry and activate
 * 				 a warning buzzer when more than 2 wrong entry
 *
 */
void Wrong_Pass(void);

/*
 * Description : A function to wait for user choice in HMI ECU and work according to it
 *				 whether it was opening door or changing password
 *
 */
void Main_Options(void);



#endif /* CONTROL_H_ */
