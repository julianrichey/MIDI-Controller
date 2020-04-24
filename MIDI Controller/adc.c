#include "adc.h"

int pins[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//might have to configure adc12 every time bc different pins
//ADC12INCHx Can be modified only when ADC12ENC = 0
//or sequence channels together? looks like the max channels stored at once is 4?
//also probably gotta average readings together
//squeezing reduces resistance, increases voltage
void adc_init(void)
{
    ADC12CTL0 = ADC12ON | ADC12SHT0_0;          //ADC12 on, set sampling time
    ADC12CTL1 = ADC12SHP;                       // Use sampling timer
    //ADC12CTL2 = ADC12RES_0; //this would be optimal with precise vref+ and vref-
    ADC12MCTL0 = ADC12INCH_10 | ADC12VRSEL_0;   //currently configured to A10, P4.2. VCC max VSS min

    //ADC12CTL0 |= ADC12MSC;
    //ADC12CTL1 |= ADC12CONSEQ_1; //sequence of channels would be fastest

    ADC12CTL0 |= ADC12ENC;
}

//returns raw 12 bit value
uint16_t adc_sample(int i)
{
    while (ADC12CTL0 & ADC12SC);          //reset automatically
    ADC12CTL0 |= ADC12SC;
    while (!(ADC12IFGR0 & BIT0));
    uint16_t adcVal = ADC12MEM0;
    return adcVal;
}

//map range of 12 bit adc readings to 7 bit amplitudes
uint8_t adc_convert(uint16_t val)
{
    uint8_t amp = 0;
    return amp;
}
