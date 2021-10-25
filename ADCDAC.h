#ifndef _ADCDAC_H
#define _ADCDAC_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...

// #include <math.h>			 // Sawtooth, triangular, sine and cosine functionality

/* GPIO ports */
#define ADC_POTIN_PORT GPIOA // Potentiometer GPIO port
#define ADC_LDRIN_PORT GPIOC // Light-dependent-resistor GPIO port
#define DAC_PORT GPIOA // Digital-to-analogue-converter GPIO port

/* GPIO pins */
#define ADC_POTIN_PIN 0 // Potentiometer GPIO pin
#define ADC_LDRIN_PIN 0 // Light-dependent-resistor GPIO pin
#define DAC_PIN 5 // Digital-to-analogue-converter GPIO pin

/* ADC channels */
#define ADC_POTIN_CHANNEL 0	 // Potentiometer ADC channel
#define ADC_LDRIN_CHANNEL 10 // Light-dependent-resistor ADC channel

/* ADC select */
#define POT 1
#define LDR 0

/* ADC STM32 C Programming - Mazidi
-------------------------------
| n-bit | # steps | step size |
|-------|---------|-----------|
|   8   |    256  | 5V / 256  |
|  10   |   1024  | 5V / 1024 |
|  12   |   4096  | 5V / 4096 |
|  16   |  65536  | 5V / 65536|
-------------------------------
(N.B: 5V = VRef in this example)
*/
#define mVref 3300 // 3.3V (Voltage Reference)
#define ADCres 4096 // Number of steps for 12-bit res (ADC resolution)
#define stepSize (mVref / ADCres) // Calculates voltage step size

void init_ADC(void);
void init_DAC(void);
void init_ADCDAC(void);

unsigned short readADC(unsigned int input);

// ADC event value storage
struct _ADC_DATA
{
	unsigned short pot, ldr, sampleNumber;
};

#endif 