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

#ifndef CONTROL_H_
#define CONTROL_H_

#include "std_types.h"

#define FIRST_TIME_WORKING 0X10
#define FIRST_PASS_IN_EEPROM 0X54
#define SAVE_FIRST_PASS  0X23
#define CHECK_PASS       0X53
#define DOOR_IS_OPENING  0X13
#define DOOR_IS_CLOSING  0X12
#define MOTOR_STOP       0X14
#define OPEN_DOOR        0X15
#define CHANGE_PASS      0X16
#define WARNING          0X17
#define CHECK_PASS2		 0X27

uint8 Compare_Password(uint8 *First_Pass,uint8 *R2_enter_First_Pass);


void Hold_Door(void);
void Open_Door(void);
void Close_Door(void);

void Warning(void);

void Set_Up_EEPROM(void);
void First_timeUsing(void);
void Main_optionsChangePass(void);
void Main_optionsOpenDoor (void);

#endif /* CONTROL_H_ */
