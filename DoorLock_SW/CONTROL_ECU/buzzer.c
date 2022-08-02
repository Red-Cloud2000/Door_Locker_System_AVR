/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER AVR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"
#include "common_macros.h"
#include "avr/io.h"


/*******************************************************************************
 * [Function Name] : buzzer_Init
 *
 * [Description]   : ➢ The Function responsible for initializing the buzzer by setting up the
 * 					   direction for the buzzer pin through the GPIO driver.
 *                   ➢ Disable the buzzer at the beginning through the GPIO driver.
 *
 * [Args]          : NONE
 *******************************************************************************/
void buzzer_Init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*******************************************************************************
 * [Function Name] : buzzer_ON
 *
 * [Description]   : Enable the buzzer through the GPIO driver.
 *
 * [Args]          : NONE
 *******************************************************************************/
void buzzer_ON(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*******************************************************************************
 * [Function Name] : buzzer_OFF
 *
 * [Description]   : Disable the buzzer through the GPIO driver.
 *
 * [Args]          : NONE
 *******************************************************************************/
void buzzer_OFF(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
