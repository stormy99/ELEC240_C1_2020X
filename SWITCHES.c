/* Configuring the GPIOs and appropriate pins to enable use of the blue user button and D-PAD switches */
#include "SWITCHES.h"

/*
int buttonAOnCounter;
int buttonAOffCounter;

int buttonBOnCounter;
int buttonBOffCounter;

int buttonCOnCounter;
int buttonCOffCounter;

int buttonDOnCounter;
int buttonDOffCounter;

int buttonBlueOnCounter;
int buttonBlueOffCounter;
*/

void init_userButton(void)
{
	// ENABLE GPIO(x) Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= 0x4000;	//Enable SYSCFG clock
	
	// CONFIGURE PORT PIN FUNCTIONS
	USERBTN_PORT->MODER &=~ (3u<<(2*USER_BUTTON)); // Clear pin mode to input mode

	SYSCFG->EXTICR[3] &=~ 0x00F0;
	SYSCFG->EXTICR[3] |=  0x0020;
	
	EXTI->IMR  |= 0x2000;
	EXTI->RTSR |= 0x2000;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void init_directionalButtons(void)
{
	// ENABLE GPIO(x) Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->APB2ENR |= 0x4000;	//Enable SYSCFG clock
	
	// CONFIGURE PORT PIN FUNCTIONS
	DIRBTN_PORT->PUPDR  |= ((2u<<(2*DIRBTN_A)) | (2u<<(2*DIRBTN_B)) | (2u<<(2*DIRBTN_C)) | (2u<<(2*DIRBTN_D)));
		
	SYSCFG->EXTICR[0] &=~ (0xFFFF);
	SYSCFG->EXTICR[0] |=  (0x6666);
	
	EXTI->IMR 	|= 0x000F;
	EXTI->RTSR 	|= 0x000F;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	
	TIM3_wait_ms(12); // Debounces the user button
	switchData.BLUE = 1; // Sets switch activity flag
	
	EXTI-> PR |= EXTI_PR_PR13;
}

void EXTI0_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	EXTI-> PR |= EXTI_PR_PR0;
	
	TIM3_wait_ms(12); // Debounces the user button
	switchData.A = 1; // Sets switch activity flag
}
void EXTI1_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	EXTI-> PR |= EXTI_PR_PR1;
	
	TIM3_wait_ms(12); // Debounces the user button
	switchData.B = 1; // Sets switch activity flag
}
void EXTI2_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	EXTI-> PR |= EXTI_PR_PR2;
	
	TIM3_wait_ms(12); // Debounces the user button
	switchData.C = 1; // Sets switch activity flag
}
void EXTI3_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	EXTI-> PR |= EXTI_PR_PR3;
	
	TIM3_wait_ms(12); // Debounces the user button
	switchData.D = 1; // Sets switch activity flag
}

void init_SWITCHES(void)
{
	// Container for AIO initialisation for library.c
	init_directionalButtons();
	init_userButton();
}

void pollSwitch(void)
{
	extern struct _SWITCH_DATA switchData;
	
	unsigned int userButton = (buttonState('C', USER_BUTTON));
	unsigned int aButton = (buttonState('G', DIRBTN_A));
	unsigned int bButton = (buttonState('G', DIRBTN_B));
	unsigned int cButton = (buttonState('G', DIRBTN_C));
	unsigned int dButton = (buttonState('G', DIRBTN_D));
	
	if(userButton != 1)
	{
		switchData.BLUE = 0;
	}
	
	if(aButton != 1)
	{
		switchData.A = 0;
	}
	
	if(bButton != 1)
	{
		switchData.B = 0;
	}
	
	if(cButton != 1)
	{
		switchData.C = 0;
	}
	
	if(dButton != 1)
	{
		switchData.D = 0;
	}
}

/*
void checkUser(void)
{
	extern struct _SWITCH_DATA switchData;
	
	unsigned int userButton = (buttonState('C', USER_BUTTON)), startTime = 0, elapsedTime = 0;
	
	if(userButton != 0)
	{
		startTime = TIM2->CNT;
	}
	
	if(userButton != 1) // Resets switch activity flag
	{
		switchData.BLUE = 0;
		
		elapsedTime = TIM2_elapsed_ms(startTime);
		if((elapsedTime >= LONG_MS) && (switchData.BLUE == 0))
		{
			switchData.BLUE_LONG_PRESS = 1;
		}
	}
}

void dirTest(void) 
{
	extern struct _SWITCH_DATA switchData;
	
	if(switchData.BLUE == 1)
	{
		INT_PORT->BSRR = (1 << GreenLED);
	} else { 
		INT_PORT->BSRR = (1 << (GreenLED+16));
	}
}
*/

_Bool buttonState(char port, unsigned short pin)
{
	unsigned short state;
	
	switch(port)
	{
		case 'A':																			
			state = ((GPIOA->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port A
			break;
		case 'B':																			
			state = ((GPIOB->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port B
			break;
		case 'C':																			
			state = ((GPIOC->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port C
			break;
		case 'D':																			
			state = ((GPIOD->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port D
			break;
		case 'E':																			
			state = ((GPIOE->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port E
			break;
		case 'F':																			
			state = ((GPIOF->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port F
			break;
		case 'G':																			
			state = ((GPIOG->IDR & (1u<<(pin)))>>pin);	// Check state of pin on port G
			break;
	}
	return state;																		
}
