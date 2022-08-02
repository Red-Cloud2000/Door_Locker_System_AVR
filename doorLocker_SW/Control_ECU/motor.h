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

#define MOTOR_PORT_ID      PORTA_ID
#define MOTOR_PIN0_ID      PIN0_ID
#define MOTOR_PIN1_ID      PIN1_ID

//enum of values of the motor state
typedef enum{
	stop,CW,A_CW
}DcMotor_State;

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
