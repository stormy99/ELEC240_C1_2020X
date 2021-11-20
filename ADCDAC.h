#ifndef _ADCDAC_H
#define _ADCDAC_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include <math.h>			 // Sawtooth, triangular, sine and cosine functionality

/* GPIO ports */
#define ADC_POTIN_PORT GPIOA // Potentiometer GPIO port
#define ADC_AUDIN_PORT GPIOB // AudioIN GPIO port
#define ADC_LDRIN_PORT GPIOC // Light-dependent-resistor GPIO port
#define DAC_PORT GPIOA // Digital-to-analogue-converter GPIO port

/* GPIO pins */
#define ADC_POTIN_PIN 0 // Potentiometer GPIO pin
#define ADC_LDRIN_PIN 0 // Light-dependent-resistor GPIO pin
#define ADC_MICIN_PIN 3 // Microphone GPIO pin
#define DAC_PIN 5 // Digital-to-analogue-converter GPIO pin

/* ADC channels */
#define ADC_POTIN_CHANNEL 0	 // Potentiometer ADC channel
#define ADC_MICIN_CHANNEL 3  // Microphone ADC channel
#define ADC_AUINL_CHANNEL 8  // AudioIN ADC Lchannel
#define ADC_AUINR_CHANNEL 9  // AudioIN ADC Rchannel
#define ADC_LDRIN_CHANNEL 10 // Light-dependent-resistor ADC channel

/* ADC select */
#define LDR 0
#define POT 1
#define MIC 2

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
#define reset 0x004C0380
#define RadPerDeg	0.0174532925
#define Vref 3 // 3V (Voltage Reference)
#define ADCres 4095 // Number of steps for 12-bit res (ADC resolution)
#define stepSize (Vref/ADCres) // Calculates voltage step size

void init_ADC(void);
void init_DAC(void);
void init_ADCDAC(void);

int optionCheckADC(signed int ADCOption);
int optionCheckDAC(int DACOption);
unsigned short readADC_Channel(void);
double readADC_Voltage(void);

void generateWaveform(int wave);

typedef enum Waveform 
{
	SQUARE,
  SINE,
  TRIANGLE,
  SAW,
  OFF
} Waveform;

#endif 