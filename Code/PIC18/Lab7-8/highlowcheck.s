config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
	
#include <xc.inc>
	goto start	
	
psect code
start: 
    ; set the directions
    setf    ADCON1, a   ; inputs are digital
    bcf	    TRISD, 1, a ; RD1 is output (connected to LED)
    bsf	    TRISB, 1, a ; RB1 is input (connected to pot)
	
loop:
    btfss   PORTB, 1, a ;skip if low
    bsf	    PORTD, 1, a	;turn on LED
    btfsc   PORTB, 1, a	;skip if high
    bcf	    PORTD, 1, a ;turn off LED
    bra	    loop

end


