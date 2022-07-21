config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	goto start	
	
	; Convert BCD into 7 Segment pattern
	; Start by copying the program from subroutine
	
	psect data  
lookup:	DB  0xfc, 0xfa, 0xf6, 0xee, 0xde, 0xbe

	psect code
SSEG	EQU 0x40    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
temp1	EQU 0x10
temp2	EQU 0x90

start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup
	movwf	TBLPTRL, f, a
	movlw	high lookup
	movwf	TBLPTRH, f, a
	movlw	low highword lookup
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	6
	movwf	I, f, a ; initialize counter with 6
	
	
loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, f, a;
	
	bnz	loop
	
	; set the I/O port directions
	setf	ADCON1, a   ; turn off the ADC
	clrf	TRISD, a    ; output connected to 7seg
	setf	TRISB, a    ; input connected to switches
	
; start the read/display
infiniteloop:
	
	movlw	0xff
	movwf	PORTD, a
	movf	PORTB, w, a
	andlw	0x03   ;lowest 2 bits
	bz	blank
	
	btfss	PORTB, 1, a
	bra	negativeToggle ;branch to toggle
	
	btfsc	PORTB, 0, a	;checking either branch 
	bra	counterclockwiseSet
	bra	clockwiseSet

; Display blank 
blank:
	movlw	0xff
	movwf	PORTD, a
	bra	infiniteloop
	
;displays center tick
negativeToggle:
	movlw	0xBF	;Hex code for lighting up G only on SSEG
	
	movwf	PORTD, a
	call	delay500ms
	movlw	0xFF
	movwf	PORTD, a
	call	delay500ms
	bra	infiniteloop
	
;set up the counter clockwise	
counterclockwiseSet:
	movlw	6
	movwf	temp1, a
	movlw	0xF9	;set one side of pattern
	movwf	PORTD, a
	call	delay500ms,0
	movf	temp1, w, a
    
;loop through counter clockwise
counterclockwiseLoop:
	call	bcd2sseg, 0
	movwf	PORTD, a
	call	delay500ms
	movlw	00000011B
	CPFSEQ	PORTD, 0
	bra	infiniteloop
	
	movlw	1
	subwf	temp1, f, a
	bz	counterclockwiseSet
	bra	counterclockwiseLoop
	
; set up the clockwise
clockwiseSet:
	movlw	6
	movwf	temp2, a
	movlw	1
	movwf	temp1, a
	movlw	0xF9 ; set one side of pattern
	movwf	PORTD, a
	clrf	temp1
	call	delay500ms, 0
	movf	temp1, w, a
	
;loop through clockwise 
clockwiseLoop:
	call	bcd2sseg, 0
	movwf	PORTD, a
	call	delay500ms
	movlw	00000010B
	CPFSEQ	PORTB, 0
	bra	infiniteloop
	
	movlw	1
	addwf	temp1, f, a
	movwf	temp1, w, a
	CPFSEQ	temp2, 0
	bra	clockwiseLoop
	bra	clockwiseSet
	
	
; convert a BCD pattern stored in 4 lsb of WREG into 7Seg
bcd2sseg: 
	lfsr	0, SSEG; move fsr0 pointer back to start of table
	movf	PLUSW0, w, a
	return 0; WREG will have the sseg pattern upon return

delay2550us:			   
	movlw	255
l1:	decf	WREG, w, a
	nop
	nop
	nop
	nop
	nop
	nop
	nop	;nop enough to get the delay just right
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

