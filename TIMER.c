/* Configuration of TIMERS and IRQs */
#include "TIMER.h"

void Init_Timer1_BuzzerPWM(unsigned int time_period_us)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	// Timer 1 clock enabled
	
	TIM1->PSC |= 45 - 1;						    // Setting pre-scaler value (APB1 clock divider) 
	TIM1->ARR = time_period_us - 1;			// Counter reload value (Auto Reload Register ARR)	
	TIM1->CNT = 0;											// Initial value for timer counter
	
	TIM1->CCMR1 = 0x0068;               // Enable preloading for PWM
	TIM1->CCER 	=	0x0004;               // Enable PWM on CH1N
	TIM1->CCR1 	=	(time_period_us / 2); // Pulse width 50%
	TIM1->BDTR	=	0x8000;               // Enable output
	
	TIM1->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer2_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Timer 2 clock enabled
	TIM2->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM2->PSC |= PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM2->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM2->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC->ISER[0]|=(1u<<28); } 				// Timer 2 global interrupt enabled
	
	TIM2->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer3_Timer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// Timer 3 clock enabled
	TIM3->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM3->PSC = PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM3->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM3->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC->ISER[0]|=(1u<<29); } 				// Timer 3 global interrupt enabled
	
	TIM3->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer4_RedFlash(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	// Timer 4 clock enabled
	TIM4->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM4->PSC = PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM4->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM4->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC->ISER[0]|=(1u<<30); } 				// Timer 4 global interrupt enabled
	
	TIM4->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer5_SecTimer(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;	// Timer 5 clock enabled
	TIM5->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM5->PSC = PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM5->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM5->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC_EnableIRQ(TIM5_IRQn); } 			// Timer 5 global interrupt enabled
	
	TIM5->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer6_DAC(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	// Timer 6 clock enabled
	TIM6->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM6->PSC = PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM6->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM6->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC_EnableIRQ(TIM6_DAC_IRQn); }  // Timer 6 global interrupt enabled
   			
	
	TIM6->CR1|= TIM_CR1_CEN;						// Start timer counter
}

void Init_Timer7_ADC(unsigned int PSC_val, unsigned int ARR_val, _Bool ISR_Enable)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;	// Timer 7 clock enabled
	TIM7->DIER |= TIM_DIER_UIE;					// Timer update interrupt enabled
	
	TIM7->PSC = PSC_val - 1;						// Setting pre-scaler value (APB1 clock divider) 
	TIM7->ARR = ARR_val - 1;						// Counter reload value (Auto Reload Register ARR)	
	TIM7->CNT = 0;											// Initial value for timer counter
	
	if (ISR_Enable == 1)
	{ NVIC_EnableIRQ(TIM7_IRQn); } 			// Timer 7 global interrupt enabled
	
	TIM7->CR1|= TIM_CR1_CEN;						// Start timer counter
}

unsigned int TIM2_elapsed_ms(unsigned int startTime)
{
	
	unsigned int currentTime = TIM2->CNT;
	unsigned int timeElapsed_ms = (((currentTime - startTime) & 0xFFFFFFFF) / 2);
	return timeElapsed_ms;
}

unsigned int TIM3_elapsed_us(unsigned int startTime)
{
	
	unsigned int currentTime = TIM3->CNT;
	unsigned int timeElapsed_us = (((currentTime - startTime) & 0xFFFF));
	return timeElapsed_us;
}

void TIM3_wait_us(int delay_us) // Calculate time difference in microseconds
{
	int start = TIM3->CNT; // Initial point
	int current_CNT = start;
	
	while(((current_CNT - start) & (0xFFFF)) < delay_us)
	{ current_CNT = TIM3->CNT; }
}

void TIM3_wait_ms(int delay_ms) // Loop microsecond calculation for milliseconds
{
	int count = delay_ms;
	while(count > 0)
	{
		TIM3_wait_us(1000);
		count --;
	}
}

/* Configuration of WATCHDOG_TIMERS and Delays */
int init_watchDog(watchdogTimeout wdTimeout)
{
	unsigned int rel = 0, res = 0;
	
	if(RCC->CSR & RCC_CSR_WDGRSTF) // Check if the MCU resumed from IWDG reset
	{
		res = 1;
		RCC->CSR |= RCC_CSR_RMVF; // Clear reset flag
	}
	
	IWDG->KR = 0x5555; // Write enable for IWDG registers
	
	if(wdTimeout >= watchdogTimeout8S)
	{
		IWDG->PR = 0x07;
	} else
	{
		IWDG->PR = 0x03;
	}
	
	switch(wdTimeout)
	{
		case watchdogTimeout5MS:
			rel = 5;
			break;
		
		case watchdogTimeout10MS:
			rel = 10;
			break;
		
		case watchdogTimeout15MS:
			rel = 15;
			break;
		
		case watchdogTimeout30MS:
			rel = 31;
			break;
		
		case watchdogTimeout60MS:
			rel = 61;
			break;
		
		case watchdogTimeout120MS:
			rel = 123;
			break;
		
		case watchdogTimeout250MS:
			rel = 255;
			break;
		
		case watchdogTimeout500MS:
			rel = 511;
			break;
		
		case watchdogTimeout1S:
			rel = 1023;
			break;
		
		case watchdogTimeout2S:
			rel = 2047;
			break;
		
		case watchdogTimeout4S:
			rel = 4095;
			break;
		
		case watchdogTimeout8S:
			rel = 8191;
			break;
		
		case watchdogTimeout16S:
			rel = 16383;
			break;
		
		case watchdogTimeout32S:
			rel = 32767;
			break;
	}
	
	IWDG->RLR = rel;   // Set reload
	IWDG->KR = 0xAAAA; // Reload counter
	IWDG->KR = 0xCCCC; // Enable IWDG
	
	return res; // Return current status result
}

void init_TIMER(void)
{
	// Container for AIO initialisation for library.c
	Init_Timer2_Timer(45000, 0xFFFFFFFF, DISABLE_ROUTINE);						// Measures ticks every half millisecond
	Init_Timer3_Timer(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ROUTINE); // Delays and measures ticks every microsecond
	Init_Timer4_RedFlash(PSC_100ms, (2*ARR_100ms), ENABLE_ROUTINE); 	// 4.8Hz interrupt: onboard Red LED
	//Init_Timer5_SecTimer(5*PSC_100ms, (2*ARR_100ms), ENABLE_ROUTINE); // Interrupt used for ticking multiple operations each second
	Init_Timer6_DAC(5, 376, ENABLE_ROUTINE);													// DAC interrupt
	Init_Timer7_ADC(PSC_1us, 21, ENABLE_ROUTINE);											// ADC interrupt
}
