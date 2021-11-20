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
	
	/* Set initial states */
	// LEDs
	LED_7SEG_PRINT(00);
	LED_OD(0, 0, 0);
	LED_ON(0);
	LED_WT(0);
	LED_RGB_VOLTAGE(0);
	
	__enable_irq();	
	
	/* Set initial states */
	// Switch states
	switchData.BLUE = 0;
	switchData.BLUE_LONG_PRESS = 0;
	switchData.BLUE_DOUBLE_PRESS = 0;
	switchData.A = 0;
	switchData.B = 0;
	switchData.C = 0;
	switchData.D = 0;
	
	// LCD states
	
	
	// LED states
	
	// USART states
	resetUSART(); // Clears and resets the terminal (PuTTY)
}
