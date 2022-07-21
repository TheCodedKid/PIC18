/* 
 * File:   patterns.c
 * Author: samuel
 *
 * Created on July 8, 2022, 11:30 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define _XTAL_FREQ 1000000
/*
 * 
 */
int main() 
{
    //char sevenseg[16] = {0x80, 0xF2, 0x48, 0x60, 0x32, 0x24, 0x04, 0xF0, 0x00, 0x20, 0x10, 0x06, 0x4E, 0x42, 0x0C, 0x1C};
    //not needed, since I am writing hex for this manually
    char rotate[6] = {0xfc, 0xfa, 0xf6, 0xee, 0xde, 0xbe}; //starts with a, moves to b and so on, DP is always on
        
    ADCON1 = 0xff; //ADC is off
    
    TRISD = 0x00; //PORTD is output (Connected to 7 Segment Display)
    TRISB = 0xff; //PORTB is input (Connect to switches)
    int i = 0;
    
    while(1)   
    {
        if(PORTB == 0x00) //if 0
        {    
            PORTD = 0xff; //blank screen 
        }
        else if(PORTB == 0x01) //if 1, blink center
        { 
            PORTD = 0x7F; //set center character
            __delay_ms(500);
            PORTD = 0xff; //clear the display
            __delay_ms(500);
        }
        else if(PORTB == 0x02) //if 2, rotate clockwise
        {
              PORTD = rotate[i]; //read the value of i and relate that to the array created
              __delay_ms(500); //wait half a second
              if(i < 5)
                  i = i + 1;
              else
                  i = 0;
        }
        else if(PORTB == 0x03) //if 3, rotate counter clockwise
        {
            PORTD = rotate[i];
            __delay_ms(500);
            if(i > 0)
                i = i - 1;
            else
                i = 5;
        }
     }
    
    return (EXIT_SUCCESS);
}


