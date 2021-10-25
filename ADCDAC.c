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
	DAC->CR |= DAC_CR_EN2; 								// Enable DAC 2
}

unsigned short readADC(unsigned int input)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;		// Enable ADC Clock
	ADC1->SQR1	&=~ ADC_SQR1_L;						// Sets Conversions/sequence to 1
	ADC1->SQR3	&=~ ADC_SQR3_SQ1;					// Clear Channel select bits
	
	switch(input)
	{
		case 0:
			ADC1->SQR3 |= ADC_LDRIN_CHANNEL;
			break;
		
		case 1:
			ADC1->SQR3 |= ADC_POTIN_CHANNEL;
			break;
	}
	
	ADC1->CR2		|=  ADC_CR2_ADON;					// Enable ADC Operation
	ADC1->CR2 |= ADC_CR2_SWSTART;					// Begin ADC Operation
	
	// Wait until ADC Operation finialises
	while((ADC1->SR & ADC_SR_EOC) == 0) { __NOP(); }
	return ADC1->DR;
}

unsigned short ADCVoltage(unsigned int volt)
{
	extern struct _ADC_DATA dataADC;
	
	unsigned short data, v = 0;
	
	if(volt == LDR)
	{
		data = dataADC.ldr;
	}
	
	else 
	{
		data = dataADC.pot;
	}
	
	v = (((mVref * data) / ADCres) / 10);
	return v;
}

void TIM7_IRQHandler(void)
{
	extern struct _ADC_DATA dataADC;
	
	dataADC.pot = readADC(POT); // Read input potentiometer value
	dataADC.ldr = readADC(LDR); // Read input light-dependent-resistor value
	
	dataADC.sampleNumber++;
		
	TIM7->SR &=~ TIM_SR_UIF;
}

void init_ADCDAC(void)
{
	// Container for AIO initialisation for library.c
	init_ADC();
	init_DAC();
}
