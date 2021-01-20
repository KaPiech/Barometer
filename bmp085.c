#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "bmp085.h"

//variables
int bmp085_regac1, bmp085_regac2, bmp085_regac3, bmp085_regb1, bmp085_regb2, bmp085_regmb, bmp085_regmc, bmp085_regmd;
unsigned int bmp085_regac4, bmp085_regac5, bmp085_regac6;
long bmp085_rawtemperature, bmp085_rawpressure;

void bmp085_writemem(uint8_t reg, uint8_t value) 
{
	i2c_start_wait(BMP085_ADDR);
	i2c_write(reg);
	i2c_write(value);
	i2c_stop();
}

void bmp085_readmem(uint8_t reg, uint8_t buff[], uint8_t bytes) 
{
	i2c_start_wait(BMP085_ADDR);
	i2c_write(reg);
	i2c_start(BMP085_ADDR | 1);
	
	for(uint8_t i=0; i<bytes; i++) 
	{
		if( i == bytes-1 )
			buff[i] = i2c_readNak();
		else
			buff[i] = i2c_readAck();
	}
	
	i2c_stop();
}

void bmp085_getcalibration() 
{
	uint8_t buff[2];
	memset(buff, 0, sizeof(buff));

	bmp085_readmem(BMP085_REGAC1, buff, 2);
	bmp085_regac1 = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGAC2, buff, 2);
	bmp085_regac2 = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGAC3, buff, 2);
	bmp085_regac3 = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGAC4, buff, 2);
	bmp085_regac4 = ((unsigned int)buff[0] <<8 | ((unsigned int)buff[1]));
	bmp085_readmem(BMP085_REGAC5, buff, 2);
	bmp085_regac5 = ((unsigned int)buff[0] <<8 | ((unsigned int)buff[1]));
	bmp085_readmem(BMP085_REGAC6, buff, 2);
	bmp085_regac6 = ((unsigned int)buff[0] <<8 | ((unsigned int)buff[1]));
	bmp085_readmem(BMP085_REGB1, buff, 2);
	bmp085_regb1 = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGB2, buff, 2);
	bmp085_regb2 = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGMB, buff, 2);
	bmp085_regmb = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGMC, buff, 2);
	bmp085_regmc = ((int)buff[0] <<8 | ((int)buff[1]));
	bmp085_readmem(BMP085_REGMD, buff, 2);
	bmp085_regmd = ((int)buff[0] <<8 | ((int)buff[1]));
}

void bmp085_getrawtemperature() 
{
	uint8_t buff[2];
	memset(buff, 0, sizeof(buff));
	long ut,x1,x2;

	//read raw temperature
	bmp085_writemem(BMP085_REGCONTROL, BMP085_REGREADTEMPERATURE);
	_delay_ms(5); // min. 4.5ms read Temp delay
	bmp085_readmem(BMP085_REGCONTROLOUTPUT, buff, 2);
	ut = ((long)buff[0] << 8 | ((long)buff[1])); //uncompensated temperature value

	//calculate raw temperature
	x1 = ((long)ut - bmp085_regac6) * bmp085_regac5 >> 15;
	x2 = ((long)bmp085_regmc << 11) / (x1 + bmp085_regmd);
	bmp085_rawtemperature = x1 + x2;
}

void bmp085_getrawpressure() 
{
	uint8_t buff[3];
	memset(buff, 0, sizeof(buff));
	long up,x1,x2,x3,b3,b6,p;
	unsigned long b4,b7;

	bmp085_getrawtemperature();

	//read raw pressure
	bmp085_writemem(BMP085_REGCONTROL, BMP085_REGREADPRESSURE+(BMP085_MODE << 6));
	_delay_ms(2 + (3<<BMP085_MODE));
	bmp085_readmem(BMP085_REGCONTROLOUTPUT, buff, 3);
	up = ((((long)buff[0] <<16) | ((long)buff[1] <<8) | ((long)buff[2])) >> (8-BMP085_MODE)); // uncompensated pressure value

	//calculate raw pressure
	b6 = bmp085_rawtemperature - 4000;
	x1 = (bmp085_regb2* (b6 * b6) >> 12) >> 11;
	x2 = (bmp085_regac2 * b6) >> 11;
	x3 = x1 + x2;
	b3 = (((((long)bmp085_regac1) * 4 + x3) << BMP085_MODE) + 2) >> 2;
	x1 = (bmp085_regac3 * b6) >> 13;
	x2 = (bmp085_regb1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (bmp085_regac4 * (uint32_t)(x3 + 32768)) >> 15;
	b7 = ((uint32_t)up - b3) * (50000 >> BMP085_MODE);
	p = b7 < 0x80000000 ? (b7 << 1) / b4 : (b7 / b4) << 1;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	bmp085_rawpressure = p + ((x1 + x2 + 3791) >> 4);
}

double bmp085_gettemperature()
{
	bmp085_getrawtemperature();
	double temperature = ((bmp085_rawtemperature + 8)>>4);
	temperature = temperature /10;
	return temperature;
}

int32_t bmp085_getpressure() 
{
	bmp085_getrawpressure();
	return bmp085_rawpressure + BMP085_UNITPAOFFSET;
}

double bmp085_getaltitude() 
{
	bmp085_getrawpressure();
	return ((1 - pow(bmp085_rawpressure/(double)101325, 0.1903 )) / 0.0000225577) + BMP085_UNITMOFFSET;
}

void bmp085_init() 
{
	i2c_init(1000);
	_delay_us(10);

	bmp085_getcalibration();
	bmp085_getrawtemperature();
}