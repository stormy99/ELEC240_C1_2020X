/* Initialisation of peripheral libraries */
#include "library.h"

struct _ADC_DATA dataADC;
struct _SWITCH_DATA switchData;

void initialiseAll(void)
{
	__disable_irq();
	
	/* Initialise Libraries */
	init_ADCDAC();
	init_LED();
	init_SWITCHES();
	init_TIMER();
	
	__enable_irq();	
	
	/* Set initial states */
	switchData.BLUE = 0;
	switchData.BLUE_SHORT_PRESS = 0;
	switchData.BLUE_LONG_PRESS = 0;
	switchData.BLUE_DOUBLE_PRESS = 0;
}
