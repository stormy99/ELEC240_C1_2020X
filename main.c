/* STM32F427ZI Microcontroller and Module Support Board (Creation of libraries and demonstration code) */
#include "library.h" // Container for project libraries
#include "main.h" 	 // Metadata, structures and enum storage		

extern struct _SWITCH_DATA switchData;

int main(void) 
{
	// Pre-Initialisation Phase
	PLL_Config();
	SystemCoreClockUpdate();
	
	// Initialisation Phase
	initialiseAll(); 
	
	// Post-Initialisation Phase
	// ...
	
	while(1)
	{ 
		checkUser();
		__NOP();
	}
}
