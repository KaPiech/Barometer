#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

const static uint8_t SEGMENTS[] __ATTR_PROGMEM__ = {
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_E | LED_DATA_SEG_F ),									// 0
	( LED_DATA_SEG_B | LED_DATA_SEG_C ),																										// 1
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_D | LED_DATA_SEG_E | LED_DATA_SEG_G ),														// 2
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_G ),														// 3
	( LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_F | LED_DATA_SEG_G ),																		// 4
	( LED_DATA_SEG_A | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_F | LED_DATA_SEG_G  ),													// 5
	( LED_DATA_SEG_A | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_E | LED_DATA_SEG_F | LED_DATA_SEG_G ),									// 6
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_F ),																		// 7
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_F | LED_DATA_SEG_E | LED_DATA_SEG_F | LED_DATA_SEG_G ),	// 8
	( LED_DATA_SEG_A | LED_DATA_SEG_B | LED_DATA_SEG_C | LED_DATA_SEG_D | LED_DATA_SEG_F | LED_DATA_SEG_G )										// 9
};

const static uint8_t DIGITS[] __ATTR_PROGMEM__ = {
	DIGIT_DATA_1, DIGIT_DATA_2, DIGIT_DATA_3, DIGIT_DATA_4
};

volatile static uint8_t data[4];

void display_init(void)
{
	LED_DATA_DIR = 0xFF;	// all pins as outputs
	LED_DATA_PORT = 0x00;	// turn off all cathode outputs
	DIGIT_DATA_DIR |= ( DIGIT_DATA_1 | DIGIT_DATA_2 | DIGIT_DATA_3 | DIGIT_DATA_4 ); // 4 digits as output
	DIGIT_DATA_PORT &= ~DIGIT_DATA_MASK; // turn off every digit

	TCCR0A |= ( 1 << WGM01 );			// ctc mode
	TCCR0B |= ( 1 << CS02 | 1 << CS00 );// prescaller 1024
	TIMSK0 |= ( 1 << OCIE0A );			// enable interrupt
	OCR0A = 64;							// select frequency 60Hz
}

void display_uint(uint16_t val)
{
	data[0] = val % 10000 / 1000;
	data[1] = val % 1000 / 100;
	data[2] = val % 100 / 10;
	data[3] = val % 10;
}

ISR( TIMER0_COMPA_vect )
{
	static uint8_t counter = 0;

	DIGIT_DATA_PORT = (DIGIT_DATA_PORT & ~DIGIT_DATA_MASK) | pgm_read_byte( DIGITS+counter );
	LED_DATA_PORT = pgm_read_byte( SEGMENTS+data[counter] );

	if (counter )
		counter--;
	else
		counter = 3;
}