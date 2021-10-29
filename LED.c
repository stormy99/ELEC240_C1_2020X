/* Configuring the GPIOs and appropriate pins to enable use of LEDs */
#include "LED.h"

void init_LED(void)
{
	// ENABLE GPIO(x) Clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // GPIOB (Onboard LEDs)
	
	// CONFIGURE PORT PIN FUNCTIONS
	/*Onboard*/
	INT_PORT->MODER &=~ ((3u<<(2*RedLED)) | (3u<<(2*BlueLED)) | (3u<<(2*GreenLED))); //Set
	INT_PORT->MODER  |= ((1u<<(2*RedLED)) | (1u<<(2*BlueLED)) | (1u<<(2*GreenLED))); //Reset
}

// On-board MCU LEDs
void LED_ON(int R, int G, int B) // i.e. RGB: 0,0,1 will only turn on the blue LED
{
	INT_PORT->BSRR = ((1 << (RedLED+16)) | (1 << (GreenLED+16)) | (1 << (BlueLED+16))); //Reset
	INT_PORT->BSRR = ((R << RedLED) | (G << GreenLED) | (B << BlueLED)); //Set
}

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;		//clear interrupt flag in status register
	GPIOB->ODR ^= (1u<<RedLED);	//toggle red led every ~208ms (4.8Hz)
}
