/* Configuration of TIMERS and IRQs */
#include "TIMER.h"

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

void init_TIMER(void)
{
	// Container for AIO initialisation for library.c
	Init_Timer2_Timer(45000, 0xFFFFFFFF, DISABLE_ROUTINE);						// Measures ticks every half millisecond
	Init_Timer3_Timer(PSC_Var_Delay, ARR_Var_Delay, DISABLE_ROUTINE); // Delays and measures ticks every microsecond
	Init_Timer4_RedFlash(PSC_100ms, (2*ARR_100ms), ENABLE_ROUTINE); 	// 4.8Hz interrupt: onboard Red LED
	//Init_Timer7_ADC(PSC_1us, 21, ENABLE_ROUTINE);											// ADC interrupt
}
