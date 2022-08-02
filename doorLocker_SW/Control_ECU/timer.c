/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver
 *
 * Author: Abdelrahman Hesham
 *
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "Common_Macros.h"

static volatile void (*t0_callBackPtr)(void) = NULL_PTR;
static volatile void (*t1_callBackPtr)(void) = NULL_PTR;
static volatile void (*t2_callBackPtr)(void) = NULL_PTR;



ISR(TIMER0_OVF_vect){
	if(t0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect){
	if(t0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t0_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect){
	if(t1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect){
	if(t1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t1_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect){
	if(t2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect){
	if(t2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*t2_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



void Timer_Init(const Timer_ConfigType * Config_Ptr){

	if(Config_Ptr->Timer_number == 0){
		TCNT0 = (uint8)(Config_Ptr->iValue);
		TCCR0 = (TCCR0 & 0xF8) | ((Config_Ptr->clock));

		if(Config_Ptr->mode == overFlow){
			CLEAR_BIT(TCCR0,WGM01);
			CLEAR_BIT(TCCR0,WGM00);
			TIMSK = (1<<TOIE0);
		}
		else if(Config_Ptr->mode == compare){
			SET_BIT(TCCR0,WGM01);
			CLEAR_BIT(TCCR0,WGM00);
			OCR0 = Config_Ptr->compValue;
			TIMSK |= (1<<OCIE0);
		}

	}

	if(Config_Ptr->Timer_number == 1){
		TCCR1B = (uint8)(TCCR1B & 0xF8) | ((Config_Ptr->clock));
		TCNT1 = Config_Ptr->iValue;
		if(Config_Ptr->mode == overFlow){
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			TIMSK = (1<<TOIE1);
		}
		else if(Config_Ptr->mode == compare){
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			TIMSK |= (1<<OCIE1A);
			TIMSK |= (1<<OCIE1B);
			OCR1A = Config_Ptr->compValue;
		}

	}

	if(Config_Ptr->Timer_number == 2){
		TCNT2 = (uint8)Config_Ptr->iValue;
		TCCR2 = (TCCR2 & 0xF8) | ((Config_Ptr->clock));

		if(Config_Ptr->mode == overFlow){
			CLEAR_BIT(TCCR2,WGM21);
			CLEAR_BIT(TCCR2,WGM20);
			TIMSK = (1<<TOIE2);
			SET_BIT(TIFR,TOV2);

		}
		else if(Config_Ptr->mode == compare){
			SET_BIT(TCCR2,WGM21);
			CLEAR_BIT(TCCR2,WGM20);
			OCR2 = Config_Ptr->compValue;
			TIMSK |= (1<<OCIE2);
		}
	}
}


void Timer_DeInit(void){
	TCNT0  = 0;
	TCNT1  = 0;
	TCNT2  = 0;
	TCCR0  = 0;
	TCCR2  = 0;
	TIMSK &= 0X20;  //due to ICU bit
	OCR0   = 0;
	OCR2   = 0;
	TCCR1B = 0;
	TCCR1A = 0;
}

void Timer0_SetCallBack(void(*a0_ptr)(void)){
	t0_callBackPtr = a0_ptr;
}

void Timer1_SetCallBack(void(*a1_ptr)(void)){
	t1_callBackPtr = a1_ptr;
}

void Timer2_SetCallBack(void(*a2_ptr)(void)){
	t2_callBackPtr = a2_ptr;
}



