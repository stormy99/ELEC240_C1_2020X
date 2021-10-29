#ifndef _LCD_H
#define _LCD_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...					 
#include "TIMER.h" 		 // Include timer delays

/* GPIO ports */
#define LCD_PORT GPIOD  // LCD GPIO Port D

/* GPIO pins */
#define LCD_D0_pin	 0
#define LCD_D1_pin	 1
#define LCD_D2_pin	 2
#define LCD_D3_pin	 3
#define LCD_D4_pin	 4
#define LCD_D5_pin	 5
#define LCD_D6_pin	 6
#define LCD_D7_pin	 7
#define LCD_RS_pin	11
#define LCD_RW_pin	12
#define LCD_E_pin		13
#define LCD_BL_pin  14 // Backlighting
#define LCD_MODEpin 15

/* Bus pins */
#define PORTB (4, 5, 6, 7)

/* LCD Configuration */
#define set_LCD_RS()	LCD_PORT->BSRR=(1u<<LCD_RS_pin)
#define clr_LCD_RS()	LCD_PORT->BSRR=(1u<<(LCD_RS_pin+16))
#define set_LCD_RW()	LCD_PORT->BSRR=(1u<<LCD_RW_pin)
#define clr_LCD_RW()	LCD_PORT->BSRR=(1u<<(LCD_RW_pin+16))
#define set_LCD_E()		LCD_PORT->BSRR=(1u<<LCD_E_pin)
#define clr_LCD_E()		LCD_PORT->BSRR=(1u<<(LCD_E_pin+16))
#define set_LCD_BL()  LCD_PORT->BSRR=(1u<<(LCD_BL_pin)) // Set Backlighting - Custom Pin for select LCDs
#define clr_LCD_BL()  LCD_PORT->BSRR=(1u<<(LCD_BL_pin+16)) // Reset Backlighting

#define set_LCD_bus_input()  LCD_PORT->MODER&=~(0xFFFF<<(2*LCD_D0_pin))
#define set_LCD_bus_output() LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin))

#define LCD_BUSY_FLAG (LCD_D0_pin + 7) // Monitoring busy flag

void init_LCD(void);
void strobeLCD(void);
void cmdLCD(unsigned char cmd);
void putLCD(unsigned char put);
void waitBusyLCD(void);
void setDataLCD(unsigned char data);
void printLCD(char *strChars);
void clsLCD(void);
void locateLCD(unsigned int column, unsigned int row);

#endif 