#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "display.h"
#include "bmp085.h"

#define PRESSURE_AVG (100)

int main(void) {
	//init interrupt
	sei();
	
	//display init
	display_init();
	
	//init bmp085
	bmp085_init();
	
	uint32_t pressure[PRESSURE_AVG];
	uint32_t pressureFirst = bmp085_getpressure();
	
	for(uint8_t i=0; i < PRESSURE_AVG; i++) {
		pressure[i] = pressureFirst;
	}

	for(;;)
	{
		uint32_t pressureAvg = 0;
		
		for(int8_t i=PRESSURE_AVG-1; i >= 0; i--) {
			if(i < PRESSURE_AVG-1) {
				pressure[i+1] = pressure[i];
			}
			
			pressureAvg += pressure[i];
		}
		
		pressureAvg /= PRESSURE_AVG;
		pressure[0] = bmp085_getpressure();
		display_uint(pressureAvg);
	}
	
	return 0;
}
