#ifndef _main_H
#define _main_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "library.h"   // Container for project libraries

void loadingBarLCD(void)
{
	locateLCD(0, 0);
	printLCD("    Loading    ");
	TIM3_wait_ms(1);
	
	locateLCD(0, 1);
	for(unsigned int x = 0; x < 16; x++)
	{
		locateLCD(x, 1);
		TIM3_wait_ms(145);
		putLCD(0);
	}
	
	locateLCD(0, 0);
	TIM3_wait_ms(1);
	printLCD(" Ready to Start ");
	locateLCD(0, 1);
	printLCD("                ");
	
	locateLCD(6, 1);
	TIM3_wait_ms(1);
	putLCD(1);
	
	locateLCD(9, 1);
	TIM3_wait_ms(1);
	putLCD(1);
	
	locateLCD(0, 0);
	TIM3_wait_ms(1500);
	printLCD("                ");
	
	locateLCD(0, 1);
	TIM3_wait_ms(1);
	printLCD("                ");
}

#endif 