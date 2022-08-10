#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"


#define _XTAL_FREQ 1000000
#define shutoffbutton PORTBbits.RB0
#define shutoffLED PORTBbits.RB1

void __interrupt(low_priority) adc_sample(void);
void __interrupt(high_priority) shutoff(void);

volatile int Pot1; //raw values from pots
volatile int Pot2;
volatile int counter; //counter value for reset
volatile int delay; //raw delay value
volatile _Bool flip = 0; //shutdown state toggle value for referencing, starts as false or off or 0
volatile int waitvalue = 0; //value to set motor speed

int main() 
{
    ADCON1 = 0x07; //only needed AN5 and AN6

    // Data pins connected to PORTA
    TRISA = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0x00;
    
    TRISB = 0x01; //RB0 as input and RB1 as output
    
    
    // PWM Frequency = 1 KHz
    // PWM Period = 1x10^-3 Second
    
    PR2 =  249; //initial config 249

    // setup timer 2
    // FOSC = 1 MHz
    T2CONbits.T2CKPS = 0b00; // Prescaler 1:1
    T2CONbits.TMR2ON = 1;
    

    // setup CCP2 in PWM mode
    TRISCbits.RC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;
    
    
    //----------------------------------------------------------------------
    // Configure LCD Pins
    
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTA, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module and Interrupts

    // * Configure analog pins, voltage reference and digital I/O 
    // Reference voltages are VSS and VDD
    //INTCON2bits.RBPU = 1; //enable pulled on RB1 and RB2

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 5; // Channel 0 (AN0), starting with the potentiometer

    // * Turn on A/D module
    ADCON0bits.ADON = 1;   
    
    
    INTCONbits.INT0E = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    INTCON2bits.INTEDG0 = 0; // falling edge
    
    
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 0;
    RCONbits.IPEN = 1; // enable priority levels
    INTCONbits.PEIE = 1; //enable peripheral interrupts
    INTCONbits.GIE = 1; // Global interrupt enable
    
    // setup timer0 
    T0CONbits.PSA = 0; // Prescaler is assigned
    T0CONbits.T0PS = 0x00; // 1:2 prescale value
    T0CONbits.T0CS = 0; // clock source is internal instruction cycle
    T0CONbits.T08BIT = 0; // operate in 16 bit mode now
    T0CONbits.TMR0ON = 1; // Turn on timer
    TMR0 = 64287;
    // setting up timer0 as an interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    INTCONbits.GIEH = 1; // enable all high priority
    INTCONbits.GIEL = 1; // enable all low priority
    
    //-------------------------------------------------------
    
    while(1)
    {
        // 4- Start conversion: Set GO/DONE(bar) bit
        ADCON0bits.GO = 1;
        
        home(); //homes cursor
        print("Max Speed: ");
        print_int(Pot1); //prints Pot1 value
        print("               "); //blanks for clearing screen
        setCursor(0,1);
        print("Delay: ");
        print_int(Pot2);
        print("               "); 
    }
}
void __interrupt(low_priority) adc_sample(void)
{   
    // ADC Interrupt
    if (PIR1bits.ADIF && PIE1bits.ADIE)
    {
        // reset the flag to avoid recursive interrupt calls
        PIR1bits.ADIF = 0;
        
        if (ADCON0bits.CHS == 5) // channel AN5 (pot 1)
        {
            Pot1 = (ADRESH << 8) | ADRESL;
            ADCON0bits.CHS = 6; //rotate to next channel
        }
        else if (ADCON0bits.CHS == 6) // channel AN6 (pot 2))
        {
            Pot2 = (ADRESH << 8) | ADRESL;
            ADCON0bits.CHS = 5; //rotate back to previous channel (looping)
            delay = Pot2; //sets delay
        }
    }    
        if (INTCONbits.TMR0IE && INTCONbits.TMR0IF)
        {
            INTCONbits.TMR0IF = 0; //reset flag for recursion
            TMR0 = 64287; //timer value for 10ms on a 1:2 prescaler
            CCPR2L = waitvalue >> 2; //bit shift for 8 bit system
            CCP2CONbits.DC2B = waitvalue & 0x03;
            counter += 1; //increments value every time the timer is called
            if (waitvalue >= Pot1){
                waitvalue = 0; // if the motor value goes above the listed speed, it corrects
            }
            if(counter >= delay){ // if reset counter and motor speed after delay has been reached
                counter = 0;
                waitvalue = 0;
            }
        }
}
void __interrupt(high_priority) shutoff(void){
    
    if (INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCONbits.INT0IF = 0;
            CCPR2L = 0; //zeros the motor
            print("   STOPPED!!!"); ///prints stop
            flip = ~ flip; //flips the state system from off to on
            shutoffLED = flip; //sets the current state value to the Red LED
    }
}