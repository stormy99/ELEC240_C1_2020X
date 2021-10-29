#ifndef _USART_H
#define _USART_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
											 
#include <stdio.h> 		 // Re-target printf to USART

/* GPIO ports */
#define USART_PORT GPIOD  // USART GPIO Port D
#define USART_ME USART3   // USART module

/* GPIO pins */
#define USART_TX_pin 8
#define USART_RX_pin 9

#define usartLine 		 0
#define usartScreen 	 1

enum colour
{
	resetColour,
	blackText = 30,
	redText,
	greenText,
	yellowText,
	blueText,
	magentaText,
	cyanText,
	whiteText,
	
	blackBackground = 40,
	redBackground,
	greenBackground,
	yellowBackground,
	blueBackground,
	magentaBackground,
	cyanBackground,
	whiteBackground
};

enum clear
{
	cursorEnd = 0,
	cursorBeginning,
	clearAll	
};

// Setup USART
void init_USART(unsigned int baud);
void sendUSART(unsigned char ch);
char receiveUSART(void);

// Redirect to Printf
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

// Escape codes for formatting
void formatUSART(char string[], int colour, int background);
void clsUSART(int clear, _Bool clearType);
void positionUSART(int x, int y);
void resetUSART(void);

#endif 