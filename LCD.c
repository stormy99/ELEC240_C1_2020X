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
			| (0xFFFF<<(2*LCD_D0_pin))  // D4 to D7 bus reset
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


void waitBusyLCD(void)
{
	unsigned int port = 1;
	
	set_LCD_bus_input();
	set_LCD_RW();
	clr_LCD_RS();
	
	while(port != 0)
	{
		set_LCD_E();
		TIM3_wait_us(20);
		port = LCD_PORT->IDR & (1u << LCD_BUSY_FLAG);
		clr_LCD_E();
		TIM3_wait_us(20);
	}

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

void locateLCD(unsigned int column, unsigned int row)
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
