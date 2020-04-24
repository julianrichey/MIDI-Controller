#ifndef midi_h
#define midi_h

#include <msp430.h>
#include <stdint.h>

//int is temp type
//also maybe need n parameters
int midi_generate(uint8_t curr, uint8_t prev);

#endif
