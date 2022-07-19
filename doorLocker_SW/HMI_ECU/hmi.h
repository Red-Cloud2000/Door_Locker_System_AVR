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

/*******************************************************************************
 *                               defines                                       *
 *******************************************************************************/
# define enter '='
# define backspace '%'
# define open_door '+'
# define change_password '-'
# define SAVE_FIRST_PASS  0X23
# define CHECK_PASS       0X53
# define CHECK_PASS       0X53
#define DOOR_IS_OPENING  0X13
#define DOOR_IS_CLOSING  0X12
#define MOTOR_STOP       0X14
#define OPEN_DOOR        0X15
#define CHANGE_PASS      0X16


/*******************************************************************************
 *                          functions prototypes                               *
 *******************************************************************************/
/*================================================================================================
 * [function name] : chagnge_pass
 * [description] : this function is responsible for taking password input from user using keypad
 * and displaying proper messages on LCD, then send the password to the control ECU
 * [Args] : void
 * [in] : void
 * [out] : void
 * [return] : void
 * ===============================================================================================*/

void change_pass(void);

/*================================================================================================
 * [function name] : comp_pass
 * [description] : this function is responsible for comparing the password and the re-enterd password
 * [Args] : void
 * [in] : void
 * [out] : void
 * [return] : void
 * ===============================================================================================*/

uint8 comp_pass(uint8 * pass1_ptr,uint8 * pass2_ptr);

void enter_pass(void);
void options(void);

#endif /* HMI_H_ */
