#pragma once

#include <avr/io.h>
#include <stdbool.h>



void i2c_init(uint32_t clk);
bool i2c_start(uint8_t addr);
void i2c_start_wait(uint8_t addr);
void i2c_stop(void);
bool i2c_write(uint8_t data);
uint8_t i2c_readAck(void);
uint8_t i2c_readNak(void);
uint8_t i2c_read(bool ack);