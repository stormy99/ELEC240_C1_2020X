#ifndef _LED_H
#define _LED_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "TIMER.h"
#include "ADCDAC.h"

/* GPIO ports */
#define INT_PORT GPIOB // Onboard MCU LEDs
#define LED_PORT GPIOC // External push-pull traffic-light LED
#define SEG_PORT GPIOE // 7-Segment LEDs
#define WHT_PORT GPIOF // White open-drain LED

/* Onboard */
#define GreenLED 0 // PB0
#define BlueLED  7 // PB7
#define RedLED  14 // PB14

#define RedLED1 2  // PC2
#define YelLED1 3  // PC3
#define GrnLED1 6  // PC6

#define RedLED2 7  // PC7
#define YelLED2 8  // PC8
#define GrnLED2 9  // PC9
#define WhtLED2 10 // PF10

/*7-Seg, 2-digit Display
		AA   			AA
	F		 B		F		 B
	F		 B  	F		 B
	  GG				GG
	E		 C		E		 C
	E		 C		E		 C
		DD				DD 		
				 DP					DP 
*/
#define RGBO 0 // PE0 RGBOutput
#define SegA 6 // PE6
#define SegB 7 // PE7
#define SegC 8 // PE8
#define SegD 5 // PE5
#define SegE 3 // PE3
#define SegF 2 // PE2
#define SegG 4 // PE4
#define _DP  9 // PE9
#define LE1 10 // PE10
#define LE2 11 // PE10
#define RLE 12 // PE12 R
#define GLE 13 // PE13 G 
#define BLE 14 // PE14 B
#define _OE 15 // PE15

void init_LED(void);
void LED_ON(int B);
void LED_PP(int R1, int Y1, int G1);
void LED_OD(int R2, int Y2, int G2);
void LED_WT(int W2);
void TIM4_IRQHandler(void);
void LED_7SEG_PRINT(unsigned int n);
void SEG_COUNTER(void);
void trafficLights(unsigned int trafficCount);
void LED_RGB_VOLTAGE(double m);
void LED_ON1(int G);
void LED_ON2(int R);

#endif 