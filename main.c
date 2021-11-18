/* STM32F427ZI Microcontroller and Module Support Board (Creation of libraries and demonstration code) */
#include "main.h" // Metadata, structures and enum storage		

int main(void) 
{
	// Pre-Initialisation Phase
	PLL_Config();
	SystemCoreClockUpdate();
	
	// Initialisation Phase
	initialiseAll();
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP; // Disables watchdog timer whilst in debug mode	
	
	// Post-Initialisation Phase
	loadingBarLCD();
	Init_Timer5_SecTimer(5*PSC_100ms, (2*ARR_100ms), ENABLE_ROUTINE);
	init_watchDog(watchdogTimeout2S); // Watchdog timer: set timeout
		
	// Main


	while(1)
	{ 
		//checkUser();
		generateWaveform(0);
		watchdogReset();
	}
}
