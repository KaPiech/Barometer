#pragma once

#include <stdio.h>
#include <avr/io.h>
#include "i2c.h"

#define BMP085_ADDR (0x77<<1) //0x77 default I2C address

//registers
#define BMP085_REGAC1				(0xAA)
#define BMP085_REGAC2				(0xAC)
#define BMP085_REGAC3				(0xAE)
#define BMP085_REGAC4				(0xB0)
#define BMP085_REGAC5				(0xB2)
#define BMP085_REGAC6				(0xB4)
#define BMP085_REGB1				(0xB6)
#define BMP085_REGB2				(0xB8)
#define BMP085_REGMB				(0xBA)
#define BMP085_REGMC				(0xBC)
#define BMP085_REGMD				(0xBE)
#define BMP085_REGCONTROL			(0xF4)
#define BMP085_REGCONTROLOUTPUT		(0xF6)
#define BMP085_REGREADTEMPERATURE	(0x2E)
#define BMP085_REGREADPRESSURE		(0x34)

//modes
#define BMP085_MODEULTRALOWPOWER	(0) //oversampling=0, internalsamples=1, maxconvtimepressure=4.5ms, avgcurrent=3uA, RMSnoise_hPA=0.06, RMSnoise_m=0.5
#define BMP085_MODESTANDARD			(1) //oversampling=1, internalsamples=2, maxconvtimepressure=7.5ms, avgcurrent=5uA, RMSnoise_hPA=0.05, RMSnoise_m=0.4
#define BMP085_MODEHIGHRES			(2) //oversampling=2, internalsamples=4, maxconvtimepressure=13.5ms, avgcurrent=7uA, RMSnoise_hPA=0.04, RMSnoise_m=0.3
#define BMP085_MODEULTRAHIGHRES		(3) //oversampling=3, internalsamples=8, maxconvtimepressure=25.5ms, avgcurrent=12uA, RMSnoise_hPA=0.03, RMSnoise_m=0.25

//setup parameters
#define BMP085_MODE BMP085_MODEULTRAHIGHRES		//define a mode
#define BMP085_UNITPAOFFSET					(0) //define a unit offset (pa)
#define BMP085_UNITMOFFSET					(0) //define a unit offset (m)

//functions
void bmp085_init();
int32_t bmp085_getpressure();
double bmp085_getaltitude();
double bmp085_gettemperature();