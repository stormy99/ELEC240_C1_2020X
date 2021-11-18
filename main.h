#ifndef _main_H
#define _main_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "library.h"   // Container for project libraries

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	GPIOB->ODR ^= (1u<<RedLED);	// Toggle red led every ~208ms (4.8Hz)
}

int segCounter = -1;
int trafficCounter = -1;
double voltage = 0;
void TIM5_IRQHandler(void)
{
	TIM5->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	
	// Update 7-seg Counter
	if(segCounter > 58)
	{
		segCounter = 0;
	} else {
		segCounter = segCounter + 1;
	}
	LED_7SEG_PRINT(segCounter);
	
	// Update Traffic Lights
	if(trafficCounter > 6)
	{
		trafficCounter = 0;
	} else {
		trafficCounter = trafficCounter + 1;
	}
	trafficLights(trafficCounter);
	
	// Update Voltage
	voltage = readADC_Voltage();
	
	// Update USART/LCD with new voltage
	printf("3.D.P: %.3fV\r", voltage);
	
	char buffer[16];
	sprintf(buffer, "3.D.P: %.3fV", voltage);
	locateLCD(0, 0);
	printLCD(buffer);
	
	char buffer1[16];
	sprintf(buffer1, "%.3f", voltage);
	
	// Update blue onboard LED with voltage in morse form
	doMorse(buffer1);
	
	LED_RGB_VOLTAGE(voltage);
}

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