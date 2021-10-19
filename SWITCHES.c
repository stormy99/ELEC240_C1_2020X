/* Configuring the GPIOs and appropriate pins to enable use of the blue user button and D-PAD switches */
#include "SWITCHES.h"

void init_userButton(void)
{
	__disable_irq();
	
	//ENABLE GPIO(x) Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= 0x4000;	//Enable SYSCFG clock
	
	//CONFIGURE PORT PIN FUNCTIONS
	USERBTN_PORT->MODER &=~ (3u<<(2*USER_BUTTON));
	
	SYSCFG->EXTICR[3] &= ~0x00F0;
	SYSCFG->EXTICR[3] |= 0x0020;
	EXTI->IMR 	|= 0x000F;
	EXTI->RTSR 	|= 0x000F;
	
	__enable_irq();
}

/*
void EXTI15_10_IRQHandler(void)
{
	unsigned int currentTime = TIM2->CNT;
		
		if ((currentTime - startTime >= SHORT_MS) && !(currentTime - startTime >= LONG_MS)) 
		{
			// short press detected 
			//switchData.BLUE = 1;
			//LED_ON(0,1,0);
		}
		
		if ((currentTime - startTime >= LONG_MS)) 
		{
			// long press detected
			//switchData.BLUE_LONG_PRESS = 1;
			//LED_ON(0,0,1);
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

void dirTest(void)
{
	TIM3_wait_us(1000);

	extern struct _SWITCH_DATA switchData;
		
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
