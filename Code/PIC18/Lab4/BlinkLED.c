/* 
 * File:   BlinkLED.c
 * Author: Samuel Raumin
 *
 * Created on July 4, 2022, 1:04 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define LED1 PORTCbits.RC0
#define LED2 PORTDbits.RD0
#define SWITCH1 PORTCbits.RC1
#define SWITCH2 PORTDbits.RD1
/*
 * Switch 1 and LED 1 are connected to Port C, whereas Switch 2 and LED 2 are connected to Port D
 * The LEDs are connected via the cathode side to the pin
 * I am aware C allows you to run single line if conditions, but I like brackets so... hehe
 */
int main() 
{
    ADCON1 = 0xff; //Set ADC off
    TRISC = 0x02;
    TRISD = 0x02; //Set input and output bits for both PORTs
    
    while(1)
    {    
        //For Port C Logic
        if(SWITCH1 == 1) //if the switch is off
        {
            LED1 = 0; //set C LED On
        }
        else
        {
            LED1 = 1; //set C Led Off
        }
        
        
        //For Port D Logic
        if(SWITCH2 == 1) //if the switch is off
        {
            LED2 = 0; //set C LED On
        }
        else
        {
            LED2 = 1; //set C Led Off
        }
    }
    return (EXIT_SUCCESS);
}

