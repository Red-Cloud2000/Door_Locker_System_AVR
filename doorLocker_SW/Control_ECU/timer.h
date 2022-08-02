/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER AVR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	Timer0 , Timer1 , Timer2
}TIMER_NO;


typedef enum
{
	overFlow , compare
}Timer_Mode;

typedef struct
{
	TIMER_NO Timer_number;
	Timer_Mode  mode;
	uint16 iValue;      //initial value
	uint16 compValue;   //compare value
	Timer_Clock clock;


}Timer_ConfigType;


void Timer_Init(const Timer_ConfigType * Config_Ptr);

void Timer_DeInit(void);

void Timer0_SetCallBack(void(*a0_ptr)(void));

void Timer1_SetCallBack(void(*a1_ptr)(void));

void Timer2_SetCallBack(void(*a2_ptr)(void));



#endif /* TIMER_H_ */
