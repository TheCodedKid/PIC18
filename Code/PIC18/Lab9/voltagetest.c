/* 
 * File:   voltagetest.c
 * Author: samuel raumin
 *
 * First ADC example (using interrupt)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "config.h"


# define _XTAL_FREQ 1000000
void __interrupt() adc_sample(void);

volatile int num; // direct read from an0
volatile int value; // main value
volatile char final[10]; // final storage string for base ten number


int main() 
{
    TRISD = 0x00;
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // AN0 is connected to wipe of potentiometer
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0E;
    TRISAbits.RA0 = 1;

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0)

    // * Turn on A/D module
    ADCON0bits.ADON = 1;   
    
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0; // disable priority levels
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1;
    
    // 3 - Wait the required acquisition time (if required)
    // ---> using ACQT = 2 TAD, no need to manually specify a wait    
 
    while(1)
    {
        // 4- Start conversion: Set GO/DONE(bar) bit
        ADCON0bits.GO = 1;             
    }
    
    return 0;

}

void __interrupt() adc_sample(void)
{
    // test which interrupt called this interrupt service routine
    
    // ADC Interrupt
    if (PIR1bits.ADIF && PIE1bits.ADIE)
    {
        // 5 Wait for A/D conversion to complete by either
        // * Polling for the GO/Done bit to be cleared
        // * Waiting for the A/D interrupt
  
        // 6 - Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
        
        // reset the flag to avoid recursive interrupt calls
        PIR1bits.ADIF = 0;
        
        num = (ADRESH << 8) | ADRESL;
        
        value = (num / 204.6) * 10; // convert the value to the correct range, then bring it up one decimal place for the next step
        sprintf(final, "0x%d", value); //take the hex value, converts it to string
        PORTD = (int)strtol(final, NULL, 0); // //converts string from hex to base 10, then typecasts it to int, finalizing the output
        
    }
}
