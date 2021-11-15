#include "BUZZER.h"

void init_BUZZER(void) 
{
	Init_Timer1_BuzzerPWM(0);
	
	// ENABLE GPIO(x) Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Buzzer on GPIO Port B
	
	// CONFIGURE PORT PIN FUNCTIONS
	BZR_PORT->AFR[1] |= (1u << (4*(BZR_PIN-8)));
	BZR_PORT->MODER &=~ (3u<<(2*BZR_PIN));
	BZR_PORT->MODER  |= (1u<<(2*BZR_PIN));
}

void playBuzzer(int time_period)
{
	TIM1->ARR = time_period;
	TIM1->CCR1 = (time_period / 2);
}

void playSong(void)
{
	unsigned int periods_1us[81] = {300, 0, 401, 0, 601, 0, 536, 0, 401, 536, 4290, 3214, 4290, 601, 0, 401, 0, 401, 268, 0, 401, 268, 0, 5102, 0, 601, 401, 0, 401, 0, 536, 0, 268, 0, 3822, 0, 2551, 0, 2551, 0, 3822, 0, 3822, 3822, 3822, 0, 637, 851, 955, 1275, 0, 955, 0, 955, 0, 637, 0, 506, 0, 1136, 0, 1702, 0, 1275, 0, 1136, 0, 955, 0, 1012, 1351, 1803, 0, 1803, 1351, 1012, 0, 1072, 715, 637, 0};
	unsigned int durations_100us[81] = {2550, 216, 2458, 208, 2425, 208, 3984, 4026, 4099, 1016, 874, 883, 941, 4969, 391, 1197, 111, 1394, 8101, 7933, 1058, 883, 13966, 4100, 11900, 1058, 883, 13966, 1358, 116, 1183, 108, 1225, 12008, 1958, 166, 1816, 12058, 1958, 166, 1816, 12058, 1058, 883, 1816, 12058, 1058, 883, 883, 925, 12113, 1992, 166, 5941, 7933, 8066, 7933, 1958, 14171, 1992, 14041, 1358, 116, 1183, 108, 1224, 12008, 1999, 13999, 1050, 883, 891, 13000, 1050, 883, 891, 13000, 1050, 883, 891, 0};
	
	playBuzzer(QUIET);
	for(unsigned int n=0; n<81; ++n)
	{
		if(periods_1us[n]==0)
		{
			playBuzzer(QUIET);
		} else 
		{
			playBuzzer(periods_1us[n]);
			playBuzzer(QUIET);
		}
		TIM3_wait_us(durations_100us[n]*100);
	}
}


