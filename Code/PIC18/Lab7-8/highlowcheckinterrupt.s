config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
	
#include <xc.inc>
    goto start	

	
psect intCodeHi, class = CODE, reloc = 2
    bcf	    INTCON, 1, a
    call    loop
    retfie  0	
	
psect code

start: 

    ;Set the directions
    setf    ADCON1, a   ; inputs are digital
    bcf	    TRISD, 1, a ; RD1 is output (connected to LED)
    bsf	    TRISB, 1, a ; RB1 is input (connected to pot)
    
    ; Setup interrupts
    movlw   0x90	
    movwf   INTCON, f, a    ; Enable global interrupt, enable INT0, reset INT0 Flag
	
loop:
    btfss   PORTB, 1, a ;skip if low
    bsf	    PORTD, 1, a	;turn on LED
    call    delay500ms
    btfsc   PORTB, 1, a	;skip if high
    bcf	    PORTD, 1, a ;turn off LED
    call    delay500ms
    bra	    loop

delay2550us:			    ; 2550 us delay
	movlw	255
l1:	decf	WREG, w, a
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bnz	l1
	return 1

delay500ms:			    
	movlw	100		    
	movwf	0x10, a
l2:	call	delay2550us
	decf	0x10, f, a
	bnz	l2
	return 1
end
    
    




