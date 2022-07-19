 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.h
 *
 * Description: Header file for the MOTOR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/


#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

#define MOTOR_PORT_ID      PORTD_ID
#define MOTOR_PIN0_ID      PIN6_ID
#define MOTOR_PIN1_ID      PIN7_ID



void DcMotor_Init(void);


void DcMotor_Rotate_SOTP(void);
void DcMotor_Rotate_CW(void);
void DcMotor_Rotate_A_CW(void);

#endif /* MOTOR_H_ */
