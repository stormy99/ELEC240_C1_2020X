/* Configuring the GPIOs and appropriate pins to enable use of LEDs */
#include "LED.h"

void init_LED(void)
{
	// ENABLE GPIO(x) Clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // GPIOB (Onboard LEDs)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // GPIOE (Segment LEDs)
	
	// CONFIGURE PORT PIN FUNCTIONS
	/*Onboard*/
	INT_PORT->MODER &=~ ((3u<<(2*RedLED)) | (3u<<(2*BlueLED)) | (3u<<(2*GreenLED))); //Set
	INT_PORT->MODER  |= ((1u<<(2*RedLED)) | (1u<<(2*BlueLED)) | (1u<<(2*GreenLED))); //Reset
	
	/*Push-pull*/
	LED_PORT->MODER &=~ ((3u<<(2*RedLED1)) | (3u<<(2*YelLED1)) | (3u<<(2*GrnLED1)));
	LED_PORT->MODER  |= ((1u<<(2*RedLED1)) | (1u<<(2*YelLED1)) | (1u<<(2*GrnLED1)));
	
	/*Open-drain*/
	LED_PORT->MODER &=~ ((3u<<(2*RedLED2)) | (3u<<(2*YelLED2)) | (3u<<(2*GrnLED2)));
	LED_PORT->MODER  |= ((1u<<(2*RedLED2)) | (1u<<(2*YelLED2)) | (1u<<(2*GrnLED2)));
	LED_PORT->OTYPER |= ((1u<<RedLED2) | (1u<<YelLED2) | (1u<<GrnLED2)); //Open Drain (GND/float)
	WHT_PORT->MODER &=~ (3u<<(2*WhtLED2)); //WhiteLED on separate GPIO port (F)
	WHT_PORT->MODER  |= (1u<<(2*WhtLED2));
	WHT_PORT->OTYPER |= (1u<<WhtLED2);
	
	/*7-Seg Display*/
	SEG_PORT->MODER &=~ ((3u<<(2*SegA)) | (3u<<(2*SegB)) | (3u<<(2*SegC)) | (3u<<(2*SegD)) | (3u<<(2*SegE)) | (3u<<(2*SegF)) | (3u<<(2*SegG)) | (3u<<(2*LE1)) | (3u<<(2*LE2)) | (3u<<(2*_DP)));
	SEG_PORT->MODER  |= ((1u<<(2*SegA)) | (1u<<(2*SegB)) | (1u<<(2*SegC)) | (1u<<(2*SegD)) | (1u<<(2*SegE)) | (1u<<(2*SegF)) | (1u<<(2*SegG)) | (1u<<(2*LE1)) | (1u<<(2*LE2)) | (1u<<(2*_DP)) | (1u<<(2*_OE))); //Low-active enable _OE

	/*RGB Bar LEDs*/
	SEG_PORT->MODER &=~ ((3u<<(2*RLE)) | (3u<<(2*GLE)) | (3u<<(2*BLE)) | (3u<<(2*RGBO)));
	SEG_PORT->MODER  |= ((1u<<(2*RLE)) | (1u<<(2*GLE)) | (1u<<(2*BLE)) | (1u<<(2*RGBO)));
}

// On-board MCU LEDs
void LED_ON(int R, int G, int B) // i.e. RGB: 0,0,1 will only turn on the blue LED
{
	INT_PORT->BSRR = ((1 << (RedLED+16)) | (1 << (GreenLED+16)) | (1 << (BlueLED+16))); //Reset
	INT_PORT->BSRR = ((R << RedLED) | (G << GreenLED) | (B << BlueLED)); //Set
}

// Push-pull traffic light LEDs
void LED_PP(int R1, int Y1, int G1)
{
	LED_PORT->BSRR = ((1 << (RedLED1+16)) | (1 << (YelLED1+16)) | (1 << (GrnLED1+16))); //Reset
	LED_PORT->BSRR = ((R1 << RedLED1) | (Y1 << YelLED1) | (G1 << GrnLED1)); //Set
}

// Open-drain traffic light LEDs
void LED_OD(int R2, int Y2, int G2, int W2)
{
	LED_PORT->BSRR = ((1 << (RedLED2)) | (1 << (YelLED2)) | (1 << (GrnLED2))); //Reset
	WHT_PORT->BSRR = (1 << (WhtLED2));
	
	LED_PORT->BSRR = ((R2 << (RedLED2 + 16)) | (Y2 << (YelLED2 + 16)) | (G2 << (GrnLED2 + 16))); //Set
	WHT_PORT->BSRR = (W2 << (WhtLED2 + 16));
}

void trafficLights(unsigned int trafficCount) 
{
	switch(trafficCount)
	{
		// Push-pull Traffic Flowing & Open-drain Traffic Waiting
		case 0:
			LED_OD(1, 0, 0, 0); 
			LED_PP(1, 0, 0); //Red
			break;
		
		case 1:
			LED_PP(1, 1, 0); //Red, Amber
			break;
		
		case 2:
			LED_PP(0, 0, 1); // Green
			break;
		
		case 3:
			LED_PP(0, 1, 0); // 'Flashing' Amber
		  break;
		
		// Push-pull Traffic Waiting & Open-drain Traffic Flowing
		case 4:
			LED_OD(1, 0, 0, 0);
			LED_PP(1, 0, 0);
			break;
		
		case 5:
			LED_OD(1, 1, 0, 0);
			break;
		
		case 6:
			LED_OD(0, 0, 1, 0);
			break;
		
		case 7:
			LED_OD(0, 1, 0, 0);
			break;
	}
}

/*7-segment, 2-digit LEDs w/ 74HC573 latch*/
void SEG_DIGIT(int digit, int dp)
{
	//Reset pins before writing
	SEG_PORT->BSRR = ((1 << (SegA+16)) | (1 << (SegB+16)) | (1 << (SegC+16)) | (1 << (SegD+16)) | (1 << (SegE+16)) | (1 << (SegF+16)) | (1 << (SegG+16)) | (1 << (_DP+16)));
	
	switch(digit)
	{
		case 0:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (1 << SegC) | (1 << SegD) | (1 << SegE) | (1 << SegF) | (0 << SegG)); //0
			break;
		
		case 1:
			SEG_PORT->BSRR = ((0 << SegA) | (1 << SegB) | (1 << SegC) | (0 << SegD) | (0 << SegE) | (0 << SegF) | (0 << SegG)); //1
			break;
		
		case 2:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (0 << SegC) | (1 << SegD) | (1 << SegE) | (0 << SegF) | (1 << SegG)); //2
			break;
		
		case 3:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (1 << SegC) | (1 << SegD) | (0 << SegE) | (0 << SegF) | (1 << SegG)); //3
			break;
		
		case 4:
			SEG_PORT->BSRR = ((0 << SegA) | (1 << SegB) | (1 << SegC) | (0 << SegD) | (0 << SegE) | (1 << SegF) | (1 << SegG)); //4
			break;
		
		case 5:
			SEG_PORT->BSRR = ((1 << SegA) | (0 << SegB) | (1 << SegC) | (1 << SegD) | (0 << SegE) | (1 << SegF) | (1 << SegG)); //5
			break;
		
		case 6:
			SEG_PORT->BSRR = ((1 << SegA) | (0 << SegB) | (1 << SegC) | (1 << SegD) | (1 << SegE) | (1 << SegF) | (1 << SegG)); //6
			break;
		
		case 7:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (1 << SegC) | (0 << SegD) | (0 << SegE) | (0 << SegF) | (0 << SegG)); //7
			break;
		
		case 8:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (1 << SegC) | (1 << SegD) | (1 << SegE) | (1 << SegF) | (1 << SegG)); //8
			break;
		
		case 9:
			SEG_PORT->BSRR = ((1 << SegA) | (1 << SegB) | (1 << SegC) | (1 << SegD) | (0 << SegE) | (1 << SegF) | (1 << SegG)); //9
			break;
		
		case 10:
			//Turns all LED segments off
			SEG_PORT->BSRR = ((0 << SegA) | (0 << SegB) | (0 << SegC) | (0 << SegD) | (0 << SegE) | (0 << SegF) | (0 << SegG)); //Null
			break;
	}
	
	switch(dp)
	{
		case 0:
			SEG_PORT->BSRR = (0 << _DP);
			break;
		
		case 1:
			SEG_PORT->BSRR = (1 << _DP);
			break;
	}
}

//7-segment 2-digit LED control
void LED_7SEG_PRINT(unsigned int n) //Writes given digits to the LED digit peripheral
{
	unsigned int units = (n % 10);
	unsigned int tens = ((n / 10) % 10);
	
	SEG_PORT->BSRR = (1u << LE1); //Enable latch 1
	
	SEG_DIGIT(tens, 0); //Writes first digit (Tens)
	SEG_PORT->BSRR = (1u << (LE1 + 16)); //Disable latch 1

	SEG_PORT->BSRR = (1u << LE2);
	SEG_DIGIT(units, 0); //Writes second digit (Units)
	SEG_PORT->BSRR = (1u << (LE2 + 16));
}

void LED_RGB_VOLTAGE(double m)
{
	SEG_PORT->BSRR = 0xFFFF0000; // Reset all
	
	unsigned int r, g, b;
	
	if(m < 0.0)
	{
		// Potential error value, displayed (negative)
		r = 0x0F, g = 0x0F, b = 0x0F;
	} 
	
	else if(m > 3.0)
	{
		// Potential error value, displayed (above vRef)
		r = 0xAA, g = 0xAA, b = 0xAA;
	}
	
	else
	{
		int rgbLeds = (int)(m * 8); // Multiplies the voltage value by a factor of 8, then converts to double
		
		b = 0xFF>>(24-rgbLeds); // There are 16 RGB LEDs available
		
		if(rgbLeds > 16)
		{
			g = 0xFF;
		}
		
		else
		{
			g = 0xFF>>(16-rgbLeds);
		}
		
		r = (rgbLeds > 8) ? 0xFF : 0xFF>>(8-rgbLeds);
	}
	
	// R
	SEG_PORT->BSRR = (1<<RLE);
	SEG_PORT->BSRR = ((1 << (SegA+16)) | (1 << (SegB+16)) | (1 << (SegC+16)) | (1 << (SegD+16)) | (1 << (SegE+16)) | (1 << (SegF+16)) | (1 << (SegG+16)) | (1 << (_DP+16)));
	SEG_PORT->BSRR = r << 2;
	SEG_PORT->BSRR = (1<<(RLE+16));
	
	// G
	SEG_PORT->BSRR = (1<<GLE);
	SEG_PORT->BSRR = ((1 << (SegA+16)) | (1 << (SegB+16)) | (1 << (SegC+16)) | (1 << (SegD+16)) | (1 << (SegE+16)) | (1 << (SegF+16)) | (1 << (SegG+16)) | (1 << (_DP+16)));
	SEG_PORT->BSRR = g << 2;
	SEG_PORT->BSRR = (1<<(GLE+16));
	
	// B
	SEG_PORT->BSRR = (1<<BLE);
	SEG_PORT->BSRR = ((1 << (SegA+16)) | (1 << (SegB+16)) | (1 << (SegC+16)) | (1 << (SegD+16)) | (1 << (SegE+16)) | (1 << (SegF+16)) | (1 << (SegG+16)) | (1 << (_DP+16)));
	SEG_PORT->BSRR = b << 2;
	SEG_PORT->BSRR = (1<<(BLE+16));
}

static const struct {const char letter, *code;} MorseMap[] =
{
	{ 'A', ".-" }, //A-Z letters
	{ 'B', "-..." },
	{ 'C', "-.-." },
	{ 'D', "-.." },
	{ 'E', "." },
	{ 'F', "..-." },
	{ 'G', "--." },
	{ 'H', "...." },
	{ 'I', ".." },
	{ 'J', ".---" },
	{ 'K', "-.-" },
	{ 'L', ".-.." },
	{ 'M', "--" },
	{ 'N', "-." },
	{ 'O', "---" },
	{ 'P', ".--." },
	{ 'Q', "--.-" },
	{ 'R', ".-." },
	{ 'S', "..." },
	{ 'T', "-" },
	{ 'U', "..-" },
	{ 'V', "...-" },
	{ 'W', ".--" },
	{ 'X', "-..-" },
	{ 'Y', "-.--" },
	{ 'Z', "--.." },
	
	{ ' ', "     " }, //Space
		
	{ '1', ".----" }, //0-9 numbers
	{ '2', "..---" },
	{ '3', "...--" },
	{ '4', "....-" },
	{ '5', "....." },
	{ '6', "-...." },
	{ '7', "--..." },
	{ '8', "---.." },
	{ '9', "----." },
	{ '0', "-----" },
		
	{ '.', ".-.-.-" }, //Special characters
	{ ',', "--..--" },
	{ '?', "..--.." },
	{ '!', "-.-.--" },
	{ ':', "---..." },
	{ ';', "-.-.-." },
	{ '(', "-.--." },
	{ ')', "-.--.-" },
	{ '"', ".-..-." },
	{ '@', ".--.-." },
	{ '&', ".-..." },
};

//Emit dots and dashes
unsigned int unitLength = 71;
void emitMorse(const char *morseString) {
  for(int i=0; morseString[i]; i++)
  {
    switch( morseString[i] )
    {
      case '.': //dot
				INT_PORT->BSRR = (1 << (BlueLED));
				INT_PORT->BSRR = (1 << (BlueLED+16));
        TIM3_wait_us(unitLength);
        INT_PORT->BSRR = (1 << (BlueLED+16));
        TIM3_wait_us(unitLength);
        break;

      case '-': //dash
        INT_PORT->BSRR = (1 << (BlueLED));
        TIM3_wait_us(unitLength * 3);
        INT_PORT->BSRR = (1 << (BlueLED+16));
        TIM3_wait_us(unitLength);
          
        break;

      case ' ': //space
        TIM3_wait_us(unitLength);
    }
  }
} 

//Emit a string of dots and dashes from chars
void doMorse(const char *string) 
{
  unsigned int i, j;
  
  for( i = 0; string[i]; ++i )
  {
    for(j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j)
    {
      if(string[i] == MorseMap[j].letter)
      {
        emitMorse(MorseMap[j].code);
        break;
      }
    }
    emitMorse("  ");
  }
}
