/* Configuring the GPIOs and appropriate pins to enable use of the blue user button and D-PAD switches */
#include "SWITCHES.h"

void init_userButton(void)
{
	//ENABLE GPIO(x) Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= 0x4000;	//Enable SYSCFG clock
	
	//CONFIGURE PORT PIN FUNCTIONS
	USERBTN_PORT->MODER &=~ (3u<<(2*USER_BUTTON)); // Clear pin mode to input mode

	SYSCFG->EXTICR[3] &= ~0x00F0;
	SYSCFG->EXTICR[3] |= 0x0020;
	
	EXTI->IMR  |= 0x2000;
	EXTI->RTSR |= 0x2000;
	
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
	extern struct _SWITCH_DATA switchData;
	
	TIM3_wait_ms(10); // Debounces the user button
	switchData.BLUE = 1; // Sets switch activity flag
	
	EXTI-> PR |= EXTI_PR_PR13;
}

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

/*
void dirTest(void)
{
	TIM3_wait_us(1000);

	
		
	unsigned int userButton = (buttonState('C', USER_BUTTON)), lastUserButton = 0;
	enum STATES {NOT_PRESSED = 0, PRESSED};
	unsigned int startTime = 0, timeElapsed = 0, longPressCount = 0, currentState = NOT_PRESSED, nextState = NOT_PRESSED;
	
	timeElapsed = (TIM2->CNT - startTime)&0xFFFF;
	switch(currentState)
	{
		case NOT_PRESSED:
			if(userButton > 1)
			{
				lastUserButton = userButton;
				startTime = TIM2->CNT;
				nextState = PRESSED;
			} else nextState = NOT_PRESSED;
			break;
			
		case PRESSED:
			if((userButton == 0) && (timeElapsed < DEBOUNCE_MS)) { nextState = NOT_PRESSED; }
			
			if(userButton == 0)
			{
				startTime = 0;
				timeElapsed = 0;
				longPressCount = 0;
				switchData.BLUE = 1;
			} else switchData.BLUE = 0;
			
			if((userButton == 1) && (timeElapsed > 0x3E8))
			{
				longPressCount++;
				
				if(longPressCount > LONG_MS) 
				{ 
					switchData.BLUE_LONG_PRESS = 1; 
				} else switchData.BLUE_LONG_PRESS = 0;
				startTime = TIM2->CNT;
			}
			break;
	}
	currentState = nextState;
	
	if(switchData.BLUE == 1)
	{
		INT_PORT->BSRR = (1 << GreenLED);
	} else INT_PORT->BSRR = (1 << (GreenLED+16));
	
	if(switchData.BLUE_LONG_PRESS == 1)
	{
		INT_PORT->BSRR = (1 << BlueLED);
	} else INT_PORT->BSRR = (1 << (BlueLED+16));
}
*/
