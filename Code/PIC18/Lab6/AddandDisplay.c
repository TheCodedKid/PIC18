/* 
 * File:   AddandDisplay.c
 * Author: Samuel Raumin
 *
 * Created on July 5, 2022, 11:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

/*
 * 
 */
int main() 
{
char sevenseg[16] = {0x80, 0xF2, 0x48, 0x60, 0x32, 0x24, 0x04, 0xF0, 0x00, 0x20, 0x10, 0x06, 0x4E, 0x42, 0x0C, 0x1C};
    
    ADCON1 = 0xff; //ADC is off
    
    TRISD = 0x00; //PORTD is output (Connected to 7 Segment Display)
    TRISB = 0xff; //PORTB is input (Connect to switches)
    
    int firstnum = 0;
    int secondnum = 0;
    PORTD = 0x00;
    
    while(1)
    {
        firstnum = (PORTB & 0x07); //gets the first three bits
        secondnum = (PORTB >> 3);
        PORTD = sevenseg[(firstnum + secondnum)];
    }
    return (EXIT_SUCCESS);
}