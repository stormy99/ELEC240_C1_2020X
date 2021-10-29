#include "USART.h"

__asm(".global __use_no_semihosting\n\t"); // #pragma import(__use_no_semihosting) // Updated for ARM Compiler 6
//struct __FILE { int handle; }; // Updated for ARM Compiler 6
FILE __stdin  = {0};
FILE __stdout = {1};
FILE __stderr = {2};
void _sys_exit(int x)
{x=x;}

void init_USART(unsigned int baud)
{
	unsigned char x1 = (USART_TX_pin / 8), x2 = (USART_RX_pin >> 3);
	unsigned int x3 = ((((SystemCoreClock) / 4) << 5) / (16 * baud)), xT = ((x3 >> 1) + (x3 & 0x1));
	
	// ENABLE GPIO(x) Clocks
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN; // GPIOD
	
	// CONFIGURE PORT PIN FUNCTIONS
	USART_PORT->MODER &=~ (
			  (3u<<(2*USART_TX_pin))       // Clear pin function bits
			| (3u<<(2*USART_RX_pin))
			);
	
	USART_PORT->MODER |= (
			  (2u<<(2*USART_TX_pin))       // Reset pin function bits
			| (2u<<(2*USART_RX_pin))
			);
	
	// ALTERNATIVE PIN FUNCTION SELECT
	USART_PORT->AFR[x1] &=~ (0x0f << (4 * (USART_TX_pin - (x1 * 8)))); // Clear pin functions
	USART_PORT->AFR[x2] &=~ (0x0f << (4 * (USART_RX_pin - (x2 * 8))));	

	USART_PORT->AFR[x1]  |= (0x07 << (4 * (USART_TX_pin - (x1 * 8)))); // Set alternate functions
	USART_PORT->AFR[x2]  |= (0x07 << (4 * (USART_RX_pin - (x2 * 8))));
	
	
	// ENABLE USART(x) Clocks
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;
	
	USART_ME->CR1|= (
			  USART_CR1_TE  // Transmit enable
			| USART_CR1_RE  // Receive enable
			| USART_CR1_UE  // USART3 enable
			| USART_CR1_PS  // Parity control enable (Even)
			| 0x0020			  // Enable RXNE interrupt (receive not-empty flag)
			);
	
	USART_ME->BRR = xT;
}

void sendUSART(unsigned char ch)
{
	while (!(USART_ME->SR & USART_SR_TC)); // Wait until Tx buffer empty
	USART_ME->DR = ch;	// Write byte to usart data register
}

char receiveUSART(void)
{
	while (!(USART_ME->SR & USART_SR_RXNE)) { __NOP(); } // Check if receive not-empty flag is set
	return USART_ME->DR; // Return status register contents
}

int fputc(int ch, FILE *f) // For printf send
{
	sendUSART((uint8_t) ch);
	return ch;								
}

int fgetc(FILE *f) // For getting user input
{
	while(!(USART_ME->SR & USART_SR_RXNE)) { __NOP(); } 
	uint8_t ch = USART_ME->DR;
	return ch;
}

void formatUSART(char string[], int colour, int background) // Formats USART with text and background colours
{
	printf("\x1b[%dm\x1b[%dm%s\x1b[%dm\x1b[%dm", colour, background, string, resetColour, blackBackground);
}

void clsUSART(int clear, _Bool clearType) // Clears and resets USART communication
{
	if (clearType == usartLine)
	{
		printf("\x1b[%dK", clear);
	} else if(clearType == usartScreen)
	{
		printf("\x1b[%dJ", clear);
	}
}

// Determines cursor position based on current x/y.
void positionUSART(int x, int y)
{
	if (y>0)
	{
		printf("\x1b[%dA", y);
	}	
	
	else if(y<0)
	{
		y = y -(2*y);
		printf("\x1b[%dB", y);
	}
	
	if (x>0)
	{
		printf("\x1b[%dC", x);
	}
	
	else if(x<0)
	{
		x = x - (2*x);
		printf("\x1b[%dD", x);
	}
}

void resetUSART(void)
{
	printf("\033[2J\033[H\033[3J");
}
