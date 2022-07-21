#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdint.h>
#include "LiquidCrystal.h"

# define _XTAL_FREQ 1000000 
# define SWITCH_PLUS    PORTCbits.RC0
# define SWITCH_MINUS   PORTCbits.RC1
# define SWITCH_ZERO    PORTCbits.RC2

int main() 
{
    
    // ALL ANx input are digital
    ADCON1 = 0x0f;
    
    // Data pins connected to PORTA
    TRISA = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0x00;
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTA, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
       
    int delayTime = 50; // am lazy, set global value
    clear(); //clears the screen if anything is still on memory
    
    int n = 0; //counter value
    while (1)
    { 
        if (SWITCH_PLUS == 1) //if the switch or the button is high, specifically the plus one (or however long it is set for based on the delay time)
            {
            clear(); 
            n++;
            print_int(n);
            __delay_ms(delayTime);
            }
        if (SWITCH_MINUS == 1)
            {
            clear();
            n--;
            print_int(n);
            __delay_ms(delayTime);
            }
        if (SWITCH_ZERO == 1) //specifically for indicating a sign change
            {
            clear();
            n = 0;
            print_int(n);
            home();
            }
        else
            {
            home(); //stay at home, i.e. do nothing
            }
    }
    return 0;
}