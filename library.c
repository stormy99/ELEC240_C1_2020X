/* Initialisation of peripheral libraries */
#include "library.h"

struct _SWITCH_DATA switchData;

void initialiseAll(void)
{
	__disable_irq();
	
	// Initialise libraries
	init_LED();
	init_userButton();
	
	// Initialise Timers
	Init_Timer2_Timer(45000, 0xFFFFFFFF, DISABLE_ROUTINE);
	Init_Timer3_Timer(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ROUTINE);
	Init_Timer4_RedFlash(PSC_100ms, (2*ARR_100ms), ENABLE_ROUTINE); // 4.8Hz interrupt: onboard Red LED
	
	__enable_irq();	
	
	// Set initial switch-states
	switchData.BLUE = 0;
	switchData.BLUE_SHORT_PRESS = 0;
	switchData.BLUE_LONG_PRESS = 0;
	switchData.BLUE_DOUBLE_PRESS = 0;
}
