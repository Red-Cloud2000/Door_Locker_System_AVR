 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER AVR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID                 PORTA_ID
#define BUZZER_PIN_ID                  PIN2_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Buzzer:
 * 1. Setup the Buzzer pin directions by use the GPIO driver (output).
 * 2. Put logic low on it to disable buzzer at beginning.
 */
void buzzer_Init(void);


/*
 * Description :
 * Turn on the Buzzer:
 * 1. Put logic high on it to enable buzzer.
 */
void buzzer_ON(void);


/*
 * Description :
 * Turn off the Buzzer:
 * 1. Put logic low on it to disable buzzer.
 */
void buzzer_OFF(void);


#endif /* BUZZER_H_ */
