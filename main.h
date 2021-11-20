#ifndef _main_H
#define _main_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "library.h"   // Container for project libraries

unsigned int optionDAC = 1;
signed int optionADC = 0;
unsigned int ADCDAC = 0;
double v2 = 0;
void mainTest(void)
{
	extern struct _SWITCH_DATA switchData;
	
	// BUTTON A
	if(switchData.A == 1)
	{
		if(ADCDAC == 0)
		{
			optionDAC--;
			optionDAC = optionCheckDAC(optionDAC);
			generateWaveform(optionDAC);
		}
		
		else if(ADCDAC == 1)
		{
			optionADC--;
			optionADC = optionCheckADC(optionADC);
			v2 = readADC_Voltage();
		}
	}
	
	// BUTTON B
	if(switchData.B == 1)
	{
		if(ADCDAC == 0) ADCDAC = 1;
		else if(ADCDAC == 1) ADCDAC = 0;
	}
	
	// BUTTON C
	if(switchData.C == 1)
	{
		if(ADCDAC == 0)
		{
			optionDAC++;
			optionDAC = optionCheckDAC(optionDAC);
			generateWaveform(optionDAC);
		}
		
		else if(ADCDAC == 1)
		{
			optionADC++;
			optionADC = optionCheckADC(optionADC);
			v2 = readADC_Voltage();
		}
	}
	
	// BUTTON D
	if(switchData.D == 1)
	{
		if(ADCDAC == 0) ADCDAC = 1;
		else if(ADCDAC == 1) ADCDAC = 0;
	}
	
	// Read voltage
	v2 = readADC_Voltage();
	// Show voltage on LCD
	displayVoltageLCD(v2, ADCDAC, optionDAC, optionADC);
	// Show voltage on USART
	printf("3.D.P: %.3fV\r", v2);
	// Show voltage in the form of the RGB LED bar
	LED_RGB_VOLTAGE(v2);
	TIM3_wait_ms(200);
}

int segCounter = -1;
int trafficCounter = -1;
int ped = 0;
double voltage = 0;

void TIM5_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	TIM5->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	
	
	if(switchData.BLUE == 1)
	{
		// Set pedestrian flag
		ped = 1;
		
	}
	
	if(ped == 1)
	{
		trafficCounter = -2;
		// Reset traffic
		LED_OD(1, 0, 0);
		LED_PP(1, 0, 0);
		
		// Let Pedestrian Cross
		LED_WT(1);
		
		// Reset flag
		ped = 0;
	}
	
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
}

unsigned int longCounter = 0;
unsigned int holdStatus = 0;
// 15ms cycles
void TIM1_BRK_TIM9_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	
	TIM9->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	
	pollSwitch();
	
	// 2000ms = 2secs (This requires us to go through this cycle approx 131 times at ~15ms per cycle)
	if(longCounter > 130)
	{
		switchData.BLUE_LONG_PRESS = 1;
	} else switchData.BLUE_LONG_PRESS = 0;
	
	if(switchData.BLUE == 1)
	{
		longCounter = longCounter + 1;
	} else longCounter = 0;
	
	if(switchData.BLUE_LONG_PRESS == 1)
	{
		holdStatus = 1;
	}
	
	if(holdStatus == 0) LED_ON1(1);
	
}

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	
	if(holdStatus == 1)
	{
		GPIOB->ODR ^= (1u<<GreenLED);
		LED_ON2(0);
	} 
	
	else
	{
		GPIOB->ODR ^= (1u<<RedLED);	// Toggle red led every ~208ms (4.8Hz) 
	}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	TIM10->SR &= ~TIM_SR_UIF;		// Clear interrupt flag in status register
	
	//mainTest();
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