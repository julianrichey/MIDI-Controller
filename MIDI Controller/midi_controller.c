#include <msp430.h>
#include <stdio.h>
#include "adc.h"
#include "usb.h"
#include "midi.h"

////////////////////////////////////////////////////////////////////////////////
//joe code
//#include <msp430.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
//#include "adc.h"
//#include "usb.h"
//#include "midi.h"

#define RX_BUFFER_LENGTH 100

static volatile uint8_t RXbuffer[RX_BUFFER_LENGTH];
static volatile uint8_t RXbufferIDX;

#define RX_BUFFER_LENGTH1 100

static volatile uint8_t RXbuffer1[RX_BUFFER_LENGTH1];
static volatile uint8_t RXbufferIDX1;

#define UART_TX_READY       (UCA0IFG & UCTXIFG)
#define UART_RX_READY       (UCA0IFG & UCRXIFG)
#define UART_TX_DONE        (UCA0IFG & UCTXCPTIFG)
#define UART_RESET_TX_DONE  (UCA0IFG &= ~UCTXCPTIFG)

//USCI_A1
#define UART_TX_READY_1       (UCA1IFG & UCTXIFG)
#define UART_RX_READY_1       (UCA1IFG & UCRXIFG)
#define UART_TX_DONE_1        (UCA1IFG & UCTXCPTIFG)
#define UART_RESET_TX_DONE_1  (UCA1IFG &= ~UCTXCPTIFG

void uartSetup(void)
{
    P2SEL1 |= (BIT5 | BIT6); // pin 2.5 as tx
    P2SEL0 &= ~(BIT5 | BIT6); // pin 2.6 as rx

    UCA1CTLW0 = UCSWRST; // reset
    UCA1CTLW0 |= UCSSEL__SMCLK;

    UCA1BR0 = 6;
    UCA1BR1 = 0x00;
    UCA1MCTLW |= UCOS16 | UCBRF_8;
    UCA1IE |= UCRXIE;
    UCA1IE |= UCTXIE;
    UCA1CTLW0 &= ~UCSWRST;
}

void uartSender(int value)
{
    while (!UART_TX_READY)
                ;
    UCA1TXBUF = value;

/*      while(!UART_TX_DONE)        // if I leave these lines commented out
            ;                       // then we will only send one single instance of value
        UART_RESET_TX_DONE;        // if I comment it out, we will send a constant stream of value
        */
}

void clockSetup(void)
{
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0x01;
}

#pragma vector=USCI_A1_VECTOR
__interrupt

void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE:
        break;
    case USCI_UART_UCRXIFG:

        RXbuffer1[RXbufferIDX1] = UCA1RXBUF;
        RXbufferIDX1 = (RXbufferIDX1 + 1) % RX_BUFFER_LENGTH1;
        LPM3_EXIT;
        break;
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}
//end joe code
////////////////////////////////////////////////////////////////////////////////



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
    //put the above code at the top of midi_controller.c
    //then in main, first call the ports_init(); function, then call clockSetup();
    //    then uartSetup();. Then in your while loop just call uartSender(samp)
    //    to send the sample value to your computer thru the uart cable.

    ports_init();
    clockSetup();
    uartSetup();
    adc_init();
    __delay_cycles(5000);

    while(1){
        uint16_t samp16 = adc_sample(1234);
        uint8_t samp = samp16 >> 4;
        printf("%u\n", samp);
        //printf("%u\n", samp*3627/4095); //converts to mV. what cast??
        uartSender(samp);
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
