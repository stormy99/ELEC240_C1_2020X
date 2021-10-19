#ifndef _LED_H
#define _LED_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "TIMER.h"

/* GPIO ports */
#define INT_PORT GPIOB // Onboard MCU LEDs

/* Onboard */
#define GreenLED  	0 // PB0
#define BlueLED 		7 // PB7
#define RedLED  	 14 // PB14

void init_LED(void);
void LED_ON(int R, int G, int B);
void TIM4_IRQHandler(void);

#endif 