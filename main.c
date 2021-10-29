/* STM32F427ZI Microcontroller and Module Support Board (Creation of libraries and demonstration code) */
#include "main.h" 	 // Metadata, structures and enum storage		

int main(void) 
{
	// Pre-Initialisation Phase
	PLL_Config();
	SystemCoreClockUpdate();
	
	// Initialisation Phase
	initialiseAll(); 
	
	// Post-Initialisation Phase
	// ...

	float longNumber = 1.4587945789;
	printf("3.D.P: %.3f", longNumber);
	
	
	while(1)
	{ 
		checkUser();
		__NOP();
	}
}
