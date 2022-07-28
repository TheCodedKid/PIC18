# define _XTAL_FREQ 1000000
# define SWITCH1 PORTDbits.RD3
# define SWITCH2 PORTDbits.RD4
void __interrupt() adc_sample(void);

volatile int POT1;
volatile int POT2;
volatile int halfer1;
volatile int halfer2;
volatile int new_POT1;
volatile int new_POT2;
volatile int TOTAL1;
volatile int TOTAL2;
volatile int TOTAL3;
//volatile char dummy1[10];
//volatile char dummy2[10];
//volatile int test1;
//volatile int test2;
//volatile char s_num1[80];
//volatile char s_num2[80];

int main() 
{
    
    // Configure LCD Pins
    // Data pins connected to PORTB
    TRISB = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    //TRISD = 0x00;
    TRISDbits.RD0 = 0; //0x00;
    TRISDbits.RD1 = 0; //0x00;
    TRISDbits.RD2 = 0; //0x00;
    TRISDbits.RD3= 1; //0xff;
    TRISDbits.RD4 = 1; //0xff;
    
//    INTCON = 0x00;
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTB, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0D;
    TRISAbits.RA0 = 1; // connected to potentiometer
    TRISAbits.RA1 = 1; // connected to photo sensor

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
        
        // Display pot num on the first line
        /*home();
        print("POT: ");
        print_int(new_num_pot);
        print("       ");
        setCursor(0, 1);
        print("SEN: ");
        print_int(new_num_sensor);
        print("       ");
        */

          /* setCursor(0, 0);
            print("(");
            print_int(new_POT1);
            print(")*(");
            print_int(new_POT2);
            print(")=");
            print("       ");
           
            dummy = 0;
            int a = new_POT1;
            int b = new_POT2;
            while(a != 0)
            {
                dummy = dummy + b;
                a = a - 1;
            }
            TOTAL3 = dummy;
            
            setCursor(0, 1);
            print_int(TOTAL3);
            print("       ");
            __delay_ms(200);
            */
        if (SWITCH1 == 0 && SWITCH2 == 0) // switch = 0
        {
            clear();
        }
        else if (SWITCH1 == 1 && SWITCH2 == 0) // switch = 2 (sub)
        {
         
            setCursor(0, 0);
            print("(");
            print_int(new_POT1);
            print(")-(");
            print_int(new_POT2);
            print(")=");
            print("       ");
           
            setCursor(0, 1);
            print_int(TOTAL1);
            print("       ");
            
 
        }
        else if (SWITCH1 == 0 && SWITCH2 == 1) // switch = 1 (add)
        {

            setCursor(0, 0);
            print("(");
            print_int(new_POT1);
            print(")+(");
            print_int(new_POT2);
            print(")=");
            print("       ");
            
            setCursor(0, 1);
            print_int(TOTAL2);
            print("       ");

        }
        else if (SWITCH1 == 1 && SWITCH2 == 1) // switch = 3 (mult)
        {
            setCursor(0, 0);
            print("(");
            print_int(new_POT1);
            print(")*(");
            print_int(new_POT2);
            print(")=");
            print("       ");
            
            setCursor(0, 1);
            print_int(TOTAL3);
            print("       ");

        }
        else
        {
            clear();
        }
         

        // 7 - Go to Step 1 or Step 2 as required                
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
        
        if (ADCON0bits.CHS == 0) // channel AN0 (potentiometer)
        {
            POT1 = (ADRESH << 8) | ADRESL;
            halfer1 = 511;
            new_POT1 = POT1 - halfer1;
            ADCON0bits.CHS = 1;
        }
        else if (ADCON0bits.CHS == 1) // channel AN1 (photo sensor))
        {
            POT2 = (ADRESH << 8) | ADRESL;
            halfer2 = 511;
            new_POT2 = POT2 - halfer2;
            ADCON0bits.CHS = 0;
        }
        
        TOTAL1 = new_POT1 - new_POT2;
        TOTAL2 = new_POT1 + new_POT2;
        //sprintf(dummy1, "0x%d", POT1);
        //sprintf(dummy2, "0x%d", POT2);
        //test1 = (int)strtol(dummy1, NULL, 0); // * (int)strtol(dummy2, NULL, 0);
        //test2 = (int)strtol(dummy2, NULL, 0);
        /*
        int p1_ones = new_POT1 % 10;
        int p1_tenth = (new_POT1 /10) % 10;
        int p1_hundred = new_POT1 / 100;
        int p2_ones = new_POT2 % 10;
        int p2_tenth = (new_POT2 /10) % 10;
        int p2_hundred = new_POT2 / 100;
        int temp1 = p1_ones * p2_ones;
        int temp2 = (p1_tenth * p2_ones) + (p2_tenth * p1_ones);
        int temp3 = (p1_hundred * p2_ones) + (p1_tenth * p2_tenth) + (p2_hundred * p1_ones);
        int temp4 = (p1_hundred * p2_tenth) + (p2_hundred * p1_tenth);
        int temp5 = p1_hundred * p2_hundred;
        TOTAL3 = (temp5 * 10000) + (temp4 * 1000) + (temp3 * 100) + (temp2 * 10) + temp1;
        */
        
        /*sprintf(s_num1, "%d", POT1);
        sprintf(s_num2, "%d", POT2);
        
        int x = (int)(new_POT1);
        int y = (int)(new_POT2);
        int multi = 0;
        for (int i = 1; i <= y; i++)
        {
            multi = multi + x;
        }
        TOTAL3 = multi;
        */
        TOTAL3 = new_POT1 * new_POT2;

    }
}