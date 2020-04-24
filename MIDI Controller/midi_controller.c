#include <msp430.h>
#include <stdio.h>
#include "adc.h"
#include "usb.h"
#include "midi.h"

void ports_init(void)            //sets all ports as input with pull down resistors
{
    P1DIR = 0;                                // 1- set p1 to input (0)
    P1REN = 0xFF;                             // 2- 0xff to enable pull up/down resistors for all 8 bits of p1
    P1OUT = 0;                                // 3- pull down enable (0)

    P2DIR = 0;
    P2REN = 0xFF;
    P2OUT = 0;

    P3DIR = 0;
    P3REN = 0xFF;
    P3OUT = 0;

    P4DIR = 0;
    P4REN = 0xFF;
    P4OUT = 0;

    PJDIR = 0;
    PJREN = 0xFF;
    PJOUT = 0;

    P4SEL1 |= BIT2;                      // Configure P4.2 for A10
    P4SEL0 |= BIT2;

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Clear the LOCKLPM5 bit in the PM5CTL0 register
}

//stores most recent 7 bit amplitude for each input
//maybe make 2d array and store multiple
uint8_t prev_values[16];

int main(void)
{

    ports_init();
    adc_init();
    __delay_cycles(5000);


    while(1){
        uint8_t samp = adc_sample(1234);
        printf("%hu\n", samp);
        //printf("%d\n", samp*3627/4095); //converts to mV. what cast??
    }

    uint8_t curr = 0;
    uint8_t prev = 0;
    for(int i = 0; i < 16; i++){
        curr = adc_convert(adc_sample(i));
        prev = prev_values[i];
        usb_this_will_be_complicated_later(midi_generate(curr, prev));
        prev_values[i] = curr;
    }


}
