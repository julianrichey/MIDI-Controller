#ifndef ADC_H_
#define ADC_H_

#include <msp430.h>
#include <stdint.h>

void adc_init(void);
uint16_t adc_sample(int i);
uint8_t adc_convert(uint16_t val);


#endif
