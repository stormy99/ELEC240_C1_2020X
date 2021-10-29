#include "ADCDAC.h"

void init_ADC(void)
{
	//ENABLE GPIO(x) Clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // GPIOC
	
	//CONFIGURE PORT PIN FUNCTIONS
	ADC_POTIN_PORT->MODER |= (3u<<(2*ADC_POTIN_PIN)); // ADC Potentiometer input set to analogue operation
	ADC_LDRIN_PORT->MODER |= (3u<<(2*ADC_LDRIN_PIN)); // ADC Light-dependent-resistor input set to analogue operation
	
	//CONFIGURE ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		// Enable ADC Clock
	ADC1->SQR1	&=~ ADC_SQR1_L;						// Sets Conversions/sequence to 1
	ADC1->SQR3	&=~ ADC_SQR3_SQ1;					// Clear Channel select bits
	ADC1->SQR3	|=  ADC_POTIN_CHANNEL;		// Set Channel
	ADC1->CR2		|=  ADC_CR2_ADON;					// Enable ADC Operation
}

void init_DAC(void)
{
	//ENABLE GPIO(x) Clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA
	
	//CONFIGURE PORT PIN FUNCTIONS
	DAC_PORT->MODER |= (3u<<(2*DAC_PIN)); // DAC output set to analogue operation
	
	//CONFIGURE DAC
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;		// Enable DAC Clock
	DAC->CR |= DAC_CR_EN1; 								// Enable DAC 1
}

unsigned short readADC(unsigned int input)
{
	ADC1->CR2 |= ADC_CR2_SWSTART;					// Begin ADC Conversion
	
	switch(input)
	{
		case 0:
			ADC1->SQR3 |= ADC_LDRIN_CHANNEL;
			break;
		
		case 1:
			ADC1->SQR3 |= ADC_POTIN_CHANNEL;
			break;
	}
	
	// Wait until ADC Operation finialises
	while((ADC1->SR & ADC_SR_EOC) == 0) { __NOP(); }
	return ADC1->DR;
}

void outputDAC(unsigned short data)
{
	DAC->DHR12R2 = (data & 0x0FFF); // Write 12-bit data to DAC2
}

void TIM7_IRQHandler(void)
{
	extern struct _ADC_DATA dataADC;
	
	dataADC.pot = readADC(POT); // Read input potentiometer value
	dataADC.ldr = readADC(LDR); // Read input light-dependent-resistor value
	
	if(dataADC.sampleNumber > 40000) // 40k Samples per second
	{
		dataADC.sampleNumber = 0;
	}
	
	// Sine wave @ 2000Hz
	// (40000 samples / 2000Hz) = 20 samples per wave
	for(unsigned int sineSamples; sineSamples > 20; sineSamples++)
	{
		// ...
	} 
	
	TIM7->SR &=~ TIM_SR_UIF;
}

void init_ADCDAC(void)
{
	// Container for AIO initialisation for library.c
	init_ADC();
	init_DAC();
}
