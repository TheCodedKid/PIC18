/* 
 * File:   highlowcheck.c
 * Author: samuel
 *
 * Created on July 13, 2022, 11:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#define LED PORTDbits.RD1
#define compar PORTBbits.RB1
/*
 * 
 */
int main() 
{
    ADCON1 = 0xff; //ADC is off
    
    TRISD = 0x00; //PORTD is output (Connected to LED)
    TRISB = 0xff; //PORTB is input (Connect to compar)
    
    while(1)
    {
        if(compar == 1)//RB1 is on - high
        {
            LED = 0; //sets the LED pin to 
        }
        else
        {
            LED = 1; //set LED to be off
        }
    }
    return (EXIT_SUCCESS);
 }

