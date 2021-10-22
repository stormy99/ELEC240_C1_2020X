#ifndef _SWITCHES_H
#define _SWITCHES_H

#include <stm32f4xx.h> //INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 //this file contains the definitions for register addresses and values etc...
#include "TIMER.h"
#include "LED.h"
											 
/* Onboard user-button */
#define USERBTN_PORT GPIOC
#define USER_BUTTON 13 //PC13

// Button event permutations
struct _SWITCH_DATA // Structure to store metadata
{
	_Bool BLUE;
	_Bool BLUE_SHORT_PRESS;
	_Bool BLUE_LONG_PRESS;
	_Bool BLUE_DOUBLE_PRESS;
};

static const int DEBOUNCE_MS = 20;
static const int SHORT_MS = 100;
static const int LONG_MS = 2000;

void init_userButton(void);
_Bool buttonState(char port, unsigned short pin);

void dirTest(void);
void checkUser(void);

#endif 