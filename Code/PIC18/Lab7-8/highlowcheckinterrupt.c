/* 
 * File:   highlowcheckinterrupt.c
 * Author: samuel
 *
 * Created on July 13, 2022, 12:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#define _XTAL_FREQ 1000000
#define LED PORTDbits.RD1
//#define compar PORTBbits.RB1 //not used

void __interrupt() toggleLed(void);
/*
 * 
 */
int main() 
{
    ADCON1 = 0xff; //ADC is off
    
    TRISD = 0x00; //PORTD is output (Connected to LED)
    TRISB = 0xff; //PORTB is input (Connect to compar)
    
    
    INTCONbits.GIE = 1; //enable interrupt
    INTCON3bits.INT1IE = 1; //enable int1
    INTCON3bits.INT1IF = 0; //clear bit
    
    INTCON2bits.INTEDG1 = 0; //falling edge
    //INTCON2bits.INTEDG1 = 1; //rising edge
    
    return (EXIT_SUCCESS);
}

void __interrupt() toggleLED(void)
{    //ensuring ISR
    if(INTCON3bits.INT1IF){
        LED = ~LED;
        __delay_ms(500);
    }
    INTCON3bits.INT1IF = 0; //clear bit
    return;
}