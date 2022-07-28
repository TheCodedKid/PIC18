#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"


#define _XTAL_FREQ 1000000
#define SWITCH1 PORTDbits.RD3
#define SWITCH2 PORTDbits.RD4

void __interrupt() adc_sample(void);

volatile int Pot1; //raw values from pots
volatile int Pot2;
volatile long newPot1; //corrected range values
volatile long newPot2;
volatile long sub; //final values
volatile long add;
volatile long multi;

int main() 
{
    
    // Configure LCD Pins
    
    // Data pins connected to PORTC
    TRISC = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0x18;
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTC, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0D;
    TRISAbits.RA0 = 1; // connected to potentiometer 1
    TRISAbits.RA1 = 1; // connected to potentiometer 2

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0), starting with the potentiometer

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
        
        if (SWITCH1 == 1 && SWITCH2 == 0) //subtraction (equation on top line, result on bottom)
        {
            home(); //home cursor
            print("(");
            print_long(newPot1); //display Pot 1 value
            print(")-(");
            print_long(newPot2); //display Pot 2 value
            print(")");
            print("=");
            print("          "); //cheesy way to do this, but ensures that if the number decreases to the point of less digit representation compared to a previously computed value, it removes those extra "hanging" values
            setCursor(0,1); //sets cursor to the next row down (second row)
            print_long(sub);
            print("          ");
        }
        else if (SWITCH1 == 0 && SWITCH2 == 1) //addition
        {
            home();
            print("(");
            print_long(newPot1);
            print(")+(");
            print_long(newPot2);
            print(")");
            print("=");
            print("          ");
            setCursor(0,1);
            print_long(add);
            print("          ");
        }
        else if (SWITCH1 == 1 && SWITCH2 == 1) //multiplication
        {
            home();
            print("(");
            print_long(newPot1);
            print(")*(");
            print_long(newPot2);
            print(")");
            print("=");
            print("          ");
            setCursor(0,1);
            print_long(multi);
            print("          ");
        }
        else //clear screen, do nothing, cleans up both zero condition and makes it more efficient
        {
            clear();
        }           
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
        
        if (ADCON0bits.CHS == 0) // channel AN0 (pot 2)
        {
            Pot1 = (ADRESH << 8) | ADRESL; //pulls data from AN0
            newPot1 = Pot1 - 511; //set range
            ADCON0bits.CHS = 1; //rotate to next channel
        }
        else if (ADCON0bits.CHS == 1) // channel AN1 (pot 2))
        {
            Pot2 = (ADRESH << 8) | ADRESL; //pulls data from AN1
            newPot2 = Pot2 - 511; //set range
            ADCON0bits.CHS = 0; //rotate back to previous channel (looping)
        }
        
        sub = newPot1 - newPot2; //subtraction calculation
        add = newPot1 + newPot2; //addition
        multi = newPot1 * newPot2; //multiplication

    }
}