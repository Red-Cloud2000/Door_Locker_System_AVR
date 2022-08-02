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


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*t0_callBackPtr)(void) = NULL_PTR;
static volatile void (*t1_callBackPtr)(void) = NULL_PTR;
static volatile void (*t2_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                           ISR Functions                                     *
 *******************************************************************************/

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

/*******************************************************************************
 *                      Functions                                              *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name] : Timer_Init
 *
 * [Description]   : Function responsible for Initialize the Timer
 *
 * [Args in]       : const Timer_ConfigType * Config_Ptr
 * [Args out]      : NONE
 *******************************************************************************/
void Timer_Init(const Timer_ConfigType * Config_Ptr){

	if(Config_Ptr->Timer_number == 0){                        /*timer0*/
		TCNT0 = (uint8)(Config_Ptr->iValue);              /*set the initial value*/
		TCCR0 = (TCCR0 & 0xF8) | ((Config_Ptr->clock));   /*set clock value*/

		if(Config_Ptr->mode == overFlow){                 /*overFlow mode*/
			CLEAR_BIT(TCCR0,WGM01);
			CLEAR_BIT(TCCR0,WGM00);
			TIMSK = (1<<TOIE0);                       /*Enable timer interrupt*/
		}
		else if(Config_Ptr->mode == compare){             /*compare mode*/
			SET_BIT(TCCR0,WGM01);
			CLEAR_BIT(TCCR0,WGM00);
			OCR0   = Config_Ptr->compValue;           /*set the compare value*/
			TIMSK |= (1<<OCIE0);                      /*Enable timer interrupt*/
		}

	}

	if(Config_Ptr->Timer_number == 1){                        /*timer1*/
		TCNT1 = (uint8)(Config_Ptr->iValue);              /*set the initial value*/
		TCCR1B = (TCCR1B & 0xF8) | ((Config_Ptr->clock)); /*set clock value*/

		if(Config_Ptr->mode == overFlow){                 /*overFlow mode*/
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			TIMSK = (1<<TOIE1);                       /*Enable timer interrupt*/
		}
		else if(Config_Ptr->mode == compare){             /*compare mode*/
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			TIMSK |= (1<<OCIE1A);                     /*Enable timer interrupt*/
			OCR1A  = Config_Ptr->compValue;           /*set the compare value*/
		}

	}

	if(Config_Ptr->Timer_number == 2){                        /*timer2*/
		TCNT2 = (uint8)(Config_Ptr->iValue);              /*set the initial value*/
		TCCR2 = (TCCR2 & 0xF8) | ((Config_Ptr->clock));   /*set clock value*/

		if(Config_Ptr->mode == overFlow){                 /*overFlow mode*/
			CLEAR_BIT(TCCR2,WGM21);
			CLEAR_BIT(TCCR2,WGM20);
			SET_BIT(TIFR,TOV2);
			TIMSK = (1<<TOIE2);                       /*Enable timer interrupt*/
		}
		else if(Config_Ptr->mode == compare){             /*compare mode*/
			SET_BIT(TCCR2,WGM21);
			CLEAR_BIT(TCCR2,WGM20);
			OCR2 = Config_Ptr->compValue;             /*set the compare value*/
			TIMSK |= (1<<OCIE2);                      /*Enable timer interrupt*/
		}
	}
}

/*******************************************************************************
 * [Function Name] : Timer_DeInit
 *
 * [Description]   : A function to disable timer
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
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

/*******************************************************************************
 * [Function Name] : Timer0_SetCallBack
 *
 * [Description]   : A function to set the callBack function for timer0
 *
 * [Args in]       : void(*t0_ptr)(void)
 * [Args out]      : NONE
 *******************************************************************************/
void Timer0_SetCallBack(void(*t0_ptr)(void)){
	t0_callBackPtr = t0_ptr;
}

/*******************************************************************************
 * [Function Name] : Timer1_SetCallBack
 *
 * [Description]   : A function to set the callBack function for timer1
 *
 * [Args in]       : void(*t1_ptr)(void)
 * [Args out]      : NONE
 *******************************************************************************/
void Timer1_SetCallBack(void(*t1_ptr)(void)){
	t1_callBackPtr = t1_ptr;
}

/*******************************************************************************
 * [Function Name] : Timer2_SetCallBack
 *
 * [Description]   : A function to set the callBack function for timer2
 *
 * [Args in]       : void(*t2_ptr)(void)
 * [Args out]      : NONE
 *******************************************************************************/
void Timer2_SetCallBack(void(*t2_ptr)(void)){
	t2_callBackPtr = t2_ptr;
}



