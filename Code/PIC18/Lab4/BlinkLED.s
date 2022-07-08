config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>
	
	goto start	
	
	;Two LEDs, two switches
	psect code
 
 ;RC0, RD0 == outputs
 ;RC1, RD1 == inputs
 
 start:
	setf ADCON1, a ; turn off ADC inputs
	movlw 0x02
	movwf TRISC, a
	movlw 0x02
	movwf TRISD, a
	clrf STATUS ;clear status
	
 cloop:
	movf PORTC, w, a
	andlw 0x02
	bz ledCoff
	bra ledCon
 dloop:
	movf PORTD, w, a
	andlw 0x02
	bz ledDoff
	bra ledDon
	
 ledCoff:
	bsf PORTC, 0, a ;turn off PortC LED (cathode connected to port)
	clrf STATUS
	bra dloop
    
 ledDoff:
	bsf PORTD, 0, a ;turn off PortD LED (cathode connected to port)
	clrf STATUS
	bra cloop
	
 ledCon:
	bcf PORTC, 0, a ;turn on PortC LED (cathode connected to port)
	clrf STATUS
	bra dloop

 ledDon:
	bcf PORTD, 0, a ;turn on PortD LED (cathode connected to port)
	clrf STATUS
	bra cloop


end