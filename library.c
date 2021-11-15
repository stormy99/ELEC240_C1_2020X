/* Initialisation of peripheral libraries */
#include "library.h"

struct _SWITCH_DATA switchData;

void initialiseAll(void)
{
	__disable_irq();
	
	/* Initialise Libraries */
	init_TIMER();
	init_ADCDAC();
	init_BUZZER();
	init_LCD();
	init_LED();
	init_SWITCHES();
	init_USART(230400);
	
	__enable_irq();	
	
	/* Set initial states */
	// Switch states
	switchData.BLUE = 0;
	switchData.BLUE_SHORT_PRESS = 0;
	switchData.BLUE_LONG_PRESS = 0;
	switchData.BLUE_DOUBLE_PRESS = 0;
	
	// LCD states
	clsLCD();
	
	// USART states
	resetUSART(); // Clears and resets the terminal (PuTTY)
}
