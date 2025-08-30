#ifndef __DAC8830_H
#define __DAC8830_H
#include "gpio.h"
#include "spi.h"

void dac8830_init(void);
void dac8830_send_byte(float data);

#endif













