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
	
	// Post-Initialisation Phase*
	loadingBarLCD();
	
	TIM3_wait_ms(1000);
	
	if(init_watchDog(watchdogTimeout1S)) {} // Initialise watchdog timer and set timeout to 1 second
	else {}
	
	float longNumber = 1.4587945789; // Printf for USART
	printf("3.D.P: %.3f", longNumber);
		
	char buffer[16]; // Printf (kinda!) for LCD
	sprintf(buffer, "2.D.P: %.2f", longNumber);
	locateLCD(0, 0);
	printLCD(buffer);

	while(1)
	{ 
		//checkUser();
		generateWaveform(0);
		watchdogReset();
		__NOP();
	}
}
