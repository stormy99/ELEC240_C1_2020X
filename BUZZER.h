#ifndef _BUZZER_H
#define _BUZZER_H

#include <stm32f4xx.h> // INCLUDE THE HEADER FILE FOR THIS MCU FAMILY
											 // this file contains the definitions for register addresses and values etc...
#include "TIMER.h"     // PWM for the buzzer

/* GPIO ports */
#define BZR_PORT GPIOB // GPIO Port for the Buzzer

/* GPIO pins */
#define BZR_PIN 13 // Buzzer pin on the GPIO

/* Musical Notes (microsecond periods) */
#define C0    61162
#define CS0   57737
#define D0    54496
#define DS0   51414
#define E0    48544
#define F0    45809
#define FS0   43252
#define G0    40816
#define GS0   38521
#define A0    36364
#define AS0   34317
#define B0    32394

#define C1    30581
#define CS1   28860
#define D1    27241
#define DS1   25714
#define E1    24272
#define F1    22910
#define FS1   21622
#define G1    20408
#define GS1   19264
#define A1    18182
#define AS1   17161
#define B1    16197

#define C2    15288
#define CS2   14430
#define D2    13620
#define DS2   12857
#define E2    12134
#define F2    11453
#define FS2   10811
#define G2    10204
#define GS2   9631
#define A2    9091
#define AS2   8581
#define B2    8099

#define C3    7645
#define CS3   7216
#define D3    6811
#define DS3   6428
#define E3    6068
#define F3    5727
#define FS3   5405
#define G3    5102
#define GS3   4816
#define A3    4545
#define AS3   4290
#define B3    4050

#define C4    3822
#define CS4   3608
#define D4    3405
#define DS4   3214
#define E4    3034
#define F4    2863
#define FS4   2703
#define G4    2551
#define GS4   2408
#define A4    2273
#define AS4   2145
#define B4    2025

#define C5    1911
#define CS5   1804
#define D5    1702
#define DS5   1607
#define E5    1517
#define F5    1432
#define FS5   1351
#define G5    1276
#define GS5   1204
#define A5    1137
#define AS5   1073
#define B5    1012

#define C6    956
#define CS6   902
#define D6    851
#define DS6   804
#define E6    758
#define F6    716
#define FS6   676
#define G6    638
#define GS6   602
#define A6    568
#define AS6   536
#define B6    506

#define C7    478
#define CS7   451
#define D7    426
#define DS7   402
#define E7    379
#define F7    358
#define FS7   338
#define G7    319
#define GS7   301
#define A7    284
#define AS7   268
#define B7    253

#define C8    239
#define CS8   225
#define D8    213
#define DS8   201
#define E8    190
#define F8    179
#define FS8   169
#define G8    159
#define GS8   150
#define A8    142
#define AS8   134
#define B8    127

#define QUIET 0

void init_BUZZER(void);
void playBuzzer(int time_period);
void playSong(void);

#endif 