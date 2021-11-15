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

#define watchdogReset() (IWDG->KR = 0xAAAA)

typedef enum 
{
	watchdogTimeout5MS   = 0x00,
	watchdogTimeout10MS  = 0x01,
	watchdogTimeout15MS  = 0x02,
	watchdogTimeout30MS  = 0x03,
	watchdogTimeout60MS  = 0x04,
	watchdogTimeout120MS = 0x05,
	watchdogTimeout250MS = 0x06,
	watchdogTimeout500MS = 0x07,
	watchdogTimeout1S    = 0x08,
	watchdogTimeout2S    = 0x09,
	watchdogTimeout4S    = 0x0A,
	watchdogTimeout8S    = 0x0B,
	watchdogTimeout16S   = 0x0C,
	watchdogTimeout32S   = 0x0D,
} watchdogTimeout;

int init_watchDog(watchdogTimeout wdTimeout);

unsigned int TIM2_elapsed_ms(unsigned int startTime);
unsigned int TIM3_elapsed_us(unsigned int startTime);
void TIM3_wait_us(int delay_us);
void TIM3_wait_ms(int delay_ms);
void init_TIMER(void);
void Init_Timer1_BuzzerPWM(unsigned int time_period_us);
void Init_Timer2_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);
void Init_Timer3_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);
void Init_Timer4_RedFlash(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);
void Init_Timer6_DAC(void);
void Init_Timer7_ADC(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable);

#endif 