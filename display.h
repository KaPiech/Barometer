#pragma once

#include <avr/io.h>

#define LED_DATA_PORT		PORTD
#define LED_DATA_DIR		DDRD
#define LED_DATA_SEG_A  ( 1 << 7 )
#define LED_DATA_SEG_B  ( 1 << 6 )
#define LED_DATA_SEG_C  ( 1 << 5 )
#define LED_DATA_SEG_D  ( 1 << 4 )
#define LED_DATA_SEG_E  ( 1 << 3 )
#define LED_DATA_SEG_F  ( 1 << 2 )
#define LED_DATA_SEG_G  ( 1 << 1 )

#define DIGIT_DATA_PORT		PORTC
#define DIGIT_DATA_DIR		DDRC
#define DIGIT_DATA_1 ( 1 << 3 )
#define DIGIT_DATA_2 ( 1 << 2 )
#define DIGIT_DATA_3 ( 1 << 1 )
#define DIGIT_DATA_4 ( 1 << 0 )
#define DIGIT_DATA_MASK ( DIGIT_DATA_1 | DIGIT_DATA_2 | DIGIT_DATA_3 | DIGIT_DATA_4 )

void display_init (void);
void display_uint(uint16_t val);