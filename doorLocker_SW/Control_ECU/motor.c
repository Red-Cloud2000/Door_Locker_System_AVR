/******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: Source file for the MOTOR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/

#include "common_macros.h"
#include "motor.h"
#include "avr/io.h" /* To use the IO Ports Registers */
#include "std_types.h"
#include "gpio.h"
//#include "pwm.h"


/*******************************************************************************
 * [Function Name] : DcMotor_Init
 *
 * [Description]   : ➢ The Function responsible for setup the direction for the two
 *                     motor pins through the GPIO driver.
 *                   ➢ Stop at the DC-Motor at the beginning through the GPIO driver.
 *
 * [Args]          : NONE
 *******************************************************************************/
void DcMotor_Init(void){
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN1_ID, PIN_OUTPUT);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
}

/***
 * [Function Name] : DcMotor_Rotate.
 *
 * [Description]   :  The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor
 *                    based on the state input state value.
 *                    Send the required duty cycle to the PWM driver based on the required speed value.
 *
 * [Args]          :
 *    [IN]            :  DcMotor_State state state: The required DC Motor state, it should be CW or A-CW or stop.
 *                       DcMotor_State data type declared as enum in motor.h
 *
 *    [IN]            :  uint8 speed
 *                       speed: decimal value for the required motor speed, it should be from 0 → 100
 *********************************************************************************/

void DcMotor_Rotate(DcMotor_State state){

/*switch case for determining which state to choose:
 *      Stop      -   Clock Wise     -    Anti-clock wise
 *   IN1 , IN2    -    IN1 , IN2     -     IN1 , IN2
 *    0  ,  0     -     0  ,  1      -      1  ,  0
 * */
switch(state){
case stop:
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
	break;
case CW:
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_HIGH);
	break;
case A_CW:
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_HIGH);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
	break;
}
}
