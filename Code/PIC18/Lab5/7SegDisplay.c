/* 
 * File:   7SegDisplay.c
 * Author: Samuel Raumin
 *
 * Created on July 4, 2022, 3:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

/*
 * Going to be using a common anode display, although, I do have the logical pint def for the cathode as well in my notes
 * The ports were flipped as I was too lazy to flip them on the breadboard, so I just redid the numbers for the display
 */
int main() 
{
    char sevenseg[16] = {0x80, 0xF2, 0x48, 0x60, 0x32, 0x24, 0x04, 0xF0, 0x00, 0x20, 0x10, 0x06, 0x4E, 0x42, 0x0C, 0x1C};
    
    ADCON1 = 0xff; //ADC is off
    
    TRISD = 0x00; //PORTD is output (Connected to 7 Segment Display)
    TRISB = 0xff; //PORTB is input (Connect to switch)
    
    while(1)
    {
        PORTD = sevenseg[PORTB & 0x0F]; //bitwise and to convert back to decimal to be properly called for the array
    }
    return (EXIT_SUCCESS);
}

