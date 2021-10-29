#include "LCD.h"

void init_LCD(void)
{
	SystemCoreClockUpdate();
	TIM3_wait_us(40);
	
	// ENABLE GPIO(x) Clocks
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN; // GPIOD (External LCD)
	
	// CONFIGURE PORT PIN FUNCTIONS
	LCD_PORT->MODER &=~ (
			  (3u<<(2*LCD_RS_pin))		  // Register select reset
			| (3u<<(2*LCD_RW_pin))      // Read and write reset
			| (3u<<(2*LCD_E_pin))       // Enable reset
			| (0xffff<<(2*LCD_D0_pin))  // D4 to D7 bus reset
			| (3u<<(2*LCD_BL_pin))      // Reset backlighting
			);
	
	LCD_PORT->MODER |= (
			  (1u<<(2*LCD_RS_pin))      // Register select output set
			| (1u<<(2*LCD_RW_pin))      // Read and write set
			| (1u<<(2*LCD_E_pin))       // Enable set
			| (0x5555<<(2*LCD_D0_pin))  // D4 to D7 bus output set
			| (1u<<(2*LCD_BL_pin))      // Set backlighting
			);
	
	// LCD INIT COMMANDS
	clr_LCD_RS();	    // By default all lines are low
	clr_LCD_RW(); 
	clr_LCD_E();
	set_LCD_BL();     // Enables backlighting capabilities (if available)
	
	TIM3_wait_ms(40); // Power on (wait for approx. 40ms while VDD rises to ~4.5V)
	
	LCD_PORT->BSRR = 0x00FF0030; // Clears 8-bits, ready for write
	strobeLCD();
	TIM3_wait_us(40);
	
	LCD_PORT->BSRR = 0x00FF0028; // First 4-bit nibble
	strobeLCD();
	TIM3_wait_us(40);
	
	LCD_PORT->BSRR = 0x00FF0028; // Second 4-bit nibble
	strobeLCD();
	TIM3_wait_ms(1);             // Busy unavailable, normal functionality afterwards

	cmdLCD(0x28);	// 2 Line, 4-bit, 5x7 dots
	cmdLCD(0x01);	// Clear LCD
	cmdLCD(0x0C);	// Display on, no cursor (with cursor = 0x0F)
	TIM3_wait_ms(2);
	
	cmdLCD(0x06);	// Auto increment with no shift
	cmdLCD(0x80); // Position set (1,1)
	
	// Load any custom characters from this point forwards during initialisation
	loadCustomCharactersLCD();
}

void strobeLCD(void)
{
	TIM3_wait_us(10); // 10 microsecond high-pulse on LCD enable-line
	set_LCD_E();
	
	TIM3_wait_us(10);
	clr_LCD_E();
}

void cmdLCD(unsigned char cmd) // Send data to the display's control register
{
	waitBusyLCD();   // Wait until the LCD isn't busy
	clr_LCD_RS();		 // Control
	clr_LCD_RW();		 // Write
	
	setDataLCD(cmd); // Sets data on the data-bus
}

void putLCD(unsigned char put) // Send a character to the display
{
	waitBusyLCD();	 // Wait until the LCD isn't busy
	set_LCD_RS();		 // Control
	clr_LCD_RW();		 // Write
	
	setDataLCD(put); // Sets data on the data-bus
}

void waitBusyLCD(void) //Wait function while LCD Busy flag is set
{
	int port;
	set_LCD_bus_input();
	set_LCD_RW();
	clr_LCD_RS();
	
	do
	{
		set_LCD_E();
		TIM3_wait_us(20);
		port = LCD_PORT->IDR & (1u << (LCD_D0_pin + 7));
		TIM3_wait_us(20);
		clr_LCD_E();
		strobeLCD();
	} while (port != 0);
	TIM3_wait_us(20);
	set_LCD_bus_output();
	TIM3_wait_us(20);
}

void setDataLCD(unsigned char data)
{
	LCD_PORT->BSRR = 0x00FF0000;
	LCD_PORT->BSRR = (data & 0xF0);
	strobeLCD();
	
	LCD_PORT->BSRR = 0x00FF0000;
	LCD_PORT->BSRR = ((data << 0x4) & 0xF0);
	strobeLCD();
}

void printLCD(char *strChars) // Prints a string of characters to the LCD for ease of use
{
	unsigned int n = 0;
	while(* (strChars + n) > 0)
	{
    putLCD(* (strChars + n));
    n++;
	}
}

void locateLCD(unsigned int column, unsigned int row) // Select position of LCD cursor
{
	switch (row)
	{
		case 0:
			cmdLCD(0x80 + column);
			break;
		
		case 1:
			cmdLCD(0xc0 + column);
			break;
	}
}

void clsLCD(void) // Clears the LCD screen
{
	cmdLCD(0x01);
	cmdLCD(0x80);
	locateLCD(0, 0);
	TIM3_wait_us(20);
}

void loadCustomCharactersLCD(void)
{
	int customCharacter[2][8] =
	{
		// Loading bar character (DDRAM Addr. 0x00)
		0b00000,
		0b00100,
		0b01110,
		0b11111,
		0b11111,
		0b01110,
		0b00100,
		0b00000,
		
		// Waving stick-man character (DDRAM Addr. 0x01)
		0b01110,
		0b01010,
		0b01110,
		0b10101,
		0b01110,
		0b00100,
		0b00100,
		0b01010
	};
	
	for(int x = 0; x < 2; x++)
	{
		for(int i = 0; i < 8; i++)
		{
			cmdLCD((0x40 | (x<<3) |i));    // Set CGRRAM Addr. to store
			putLCD(customCharacter[x][i]); // Write to CGRAM Addr.
		}
	}
}
