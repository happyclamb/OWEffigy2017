#ifndef __PROJECTDEFS_H__
#define __PROJECTDEFS_H__

#define INFO
//#define DEBUG
//#define TIMING

#ifdef INFO
 #define info_print(x)     Serial.print(x)
 #define info_println(x)   Serial.println(x)
#else
 #define info_print(x)
 #define info_println(x)
#endif

#ifdef DEBUG
 #define debug_print(x)     Serial.print(x)
 #define debug_println(x)   Serial.println(x)
#else
 #define debug_print(x)
 #define debug_println(x)
#endif

#ifdef TIMING
 #define timing_print(x)     Serial.print(x)
 #define timing_println(x)   Serial.println(x)
#else
 #define timing_print(x)
 #define timing_println(x)
 #endif

#define PATTERN_CHANGE_DELAY 250
#define COLOR_STEPS_IN_WHEEL 255

#define LEFT_DIAL_A0_PIN    0
#define RIGHT_DIAL_A1_PIN   1
#define TOGGLE_SWITCH_PIN   48

#define CH_01 0
#define CH_02 3
#define CH_03 6
#define CH_04 9
#define CH_05 12

#define CH_06 16
#define CH_07 19
#define CH_08 22
#define CH_09 25
#define CH_10 28

#define CH_11 32
#define CH_12 35
#define CH_13 38
#define CH_14 41
#define CH_15 44

#define CH_16 48
#define CH_17 51
#define CH_18 54
#define CH_19 57
#define CH_20 60

#define CH_21 64
#define CH_22 67
#define CH_23 70
#define CH_24 73
#define CH_25 76

#define CH_26 80
#define CH_27 83
#define CH_28 86
#define CH_29 89
#define CH_30 92

// timers:
// Pins 5 and 6: controlled by timer0
// Pins 9 and 10: controlled by timer1
// Pins 11 and 3: controlled by timer2

//////////////// 10DIGITAL PINS
// #define serialRX 0		// Used for serial debugging
// #define serialTX 1		// Used for serial debugging
/*
MEGA 2560

PWM.12	White			BLANK
PWM.11	Blue			XLAT
PWM.9	Brown			GSCLK

D.51 (MOSI)		Green	SIN
D.52 (SCLK)		Red		SCLK

D.48	ToggleSwitch
A.0		Left Dial
A.1		Right Dial

GND		Black
Vin		Yellow
*/



#endif // __PROJECTDEFS_H__
