#include "ADCDAC.h"

unsigned short ADC_DATA;
unsigned int Channel = 0;
int const samplesSize = 40; // 40k/sec
int maxSample;
int Samples[samplesSize];
int nextSample;

void init_ADC(void)
{
	//ENABLE GPIO(x) Clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // GPIOC
	
	//CONFIGURE PORT PIN FUNCTIONS
	ADC_POTIN_PORT->MODER |= (3u<<(2*ADC_POTIN_PIN)); // ADC Potentiometer input set to analogue operation
	ADC_LDRIN_PORT->MODER |= (3u<<(2*ADC_LDRIN_PIN)); // ADC Light-dependent-resistor input set to analogue operation
	ADC_LDRIN_PORT->MODER |= (3u<<(2*ADC_MICIN_PIN)); // ADC Microphone input set to analogue operation
	
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

int optionCheckADC(signed int ADCOption)
{
	if (ADCOption == +4)
	{
		ADCOption = 0;
	}
	
	else if (ADCOption <= 0)
	{
		ADCOption = +3;
	}
	
	Channel = ADCOption;
	return ADCOption;
}

int optionCheckDAC(int DACOption)
{
	if (DACOption == 4)
	{
		DACOption = 1;
	}
	
	else if (DACOption == 0)
	{
		DACOption = 3;
	}
	
	return DACOption;
}

unsigned short readADC_Channel(void)
{
	ADC1->SQR3 &=~ ADC_SQR3_SQ1; // Channel select bits
	
	switch(Channel)
	{
		case 0: // LDR ADC Input
			ADC1->SQR3 |= ADC_LDRIN_CHANNEL;
			break;
		
		case 1: // Potentiometer ADC Input
			ADC1->SQR3 |= ADC_POTIN_CHANNEL;
			break;
		
		case 2: // Microphone ADC Input
			ADC1->SQR3 |= ADC_MICIN_CHANNEL;
			break;
		
		case 3: // Temp (AudioIN) ADC Input
			ADC1->SQR3 |= ADC_POTIN_CHANNEL;
			break;
	}
	
	// Wait until ADC Operation finialises
	ADC1->CR2 |= ADC_CR2_SWSTART; // Begin ADC Conversion
	while((ADC1->SR & ADC_SR_EOC) == 0) { __NOP(); }
	return ADC1->DR;
}

double readADC_Voltage(void)
{
	double v = 0;
	v = ((3 * (float)ADC_DATA)/4095);	
	return v;
}

void generateWaveform(int wave)
{
	int currentSample = 0;
	signed int change = +341;
	
	switch(wave)
	{
		case 0: // Sets all sampling at zero
			maxSample = samplesSize;
		
			for(int i = 0; i < maxSample; i++)
			{
				Samples[i] = 0;
			}
			break;
		
		case 1: // Square wave generation
			maxSample = (samplesSize / 6);
		
			for(int i = 0; i < maxSample; i++) // Approx: 8kHz Square
			{	
				if (i == maxSample / 2)
				{
					currentSample = 2047;
				}
				Samples[i] = currentSample;
			}
			break;
			
		case 2: // Triangular wave generation
			maxSample = (samplesSize / 3);
			
			for (int i = 0; i < maxSample; i++) // Approx: 4kHz Triangle
			{
				if (currentSample >= 2040)
				{
						currentSample = 2046;
						change = -170;				
				}

				else if (currentSample <= 0 )
				{
					currentSample = 0;
					change = +170;
				}
				Samples[i] = currentSample;
				currentSample = currentSample + change;
			}
			break;
			
		case 3: // Sine wave generation
			maxSample = (samplesSize/2);
		
		  for(int i = 0; i < maxSample; i++) // Approx: 2kHz Sine
			{
				Samples[i] = (2048 + (2047 * sin((RadPerDeg * 360) / maxSample * i))); 
			}
			break;
	}
}

void TIM6_DAC_IRQHandler(void)
{
	TIM6->SR &= ~TIM_SR_UIF; // Clear interrupt flag in status register
	
	DAC->DHR12R2 = (Samples[nextSample] & 0x0FFF);
	nextSample++;
	if (nextSample>=maxSample){ nextSample = 0; } // Resets samples after max threshold surpassed
}

void TIM7_IRQHandler(void)
{
	ADC_DATA = readADC_Channel();
	TIM7->SR &= ~TIM_SR_UIF; // Clear interrupt flag in status register
}

void init_ADCDAC(void)
{
	// Container for AIO initialisation for library.c
	init_ADC();
	init_DAC();
}
