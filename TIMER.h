#ifndef _TIMER_H
#define _TIMER_H

#include <stm32f4xx.h> //INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 //this file contains the definitions for register addresses and values etc...
#include "LED.h"

#define ENABLE_ROUTINE  1
#define DISABLE_ROUTINE 0

//Clock frequency set to 180MHz
#define PSC_1s			9227
#define PSC_100ms 	9212
#define PSC_1ms 		8385
#define PSC_1us 		  46
#define PSC_Var_Delay	92

#define ARR_1s 			10001
#define ARR_100ms 	 1001
#define ARR_1ms 		   11
#define ARR_1us 		    2
#define ARR_Var_Delay	  0xFFFF	//allows overflow

unsigned int TIM2_elapsed_ms(unsigned int startTime);
unsigned int TIM3_elapsed_us(unsigned int startTime);
void TIM3_wait_us(int delay_us);
void TIM3_wait_ms(int delay_ms);

void Init_Timer2_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);
void Init_Timer3_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);
void Init_Timer4_RedFlash(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

#endif 