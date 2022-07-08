config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	goto start	
	
	;Take Two 3 Bit values and computes the sum, then displays on a seven segment display
	
	psect data  
lookup:	DB  0x80, 0xF2, 0x48, 0x60, 0x32, 0x24, 0x04, 0xF0, 0x00, 0x20, 0x10, 0x06, 0x4E, 0x42, 0x0C, 0x1C	
	
	psect code
SSEG	EQU 0x41    ; 7 segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
num1	EQU 0x20    ; first Stored value
num2	EQU 0x21    ; second value stored
final	EQU 0x22    ; store final value
start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup
	movwf	TBLPTRL, f, a
	movlw	high lookup
	movwf	TBLPTRH, f, a
	movlw	low highword lookup
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	16
	movwf	I, f, a ; initialize counter with 15
loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, f, a;
	bnz	loop
	
	
	; set the I/O port directions
	setf	ADCON1, a   ; turn off the ADC
	clrf	TRISD, a    ; output connected to 7seg
	setf	TRISB, a    ; input connected to 4 switches
	
	; start the read/display
infiniteloop:
	movf	PORTB, w, a ; read value from switches
	andlw	0x07	    ; keep the lowest 3 bits only
	movwf	num1, a	    ; store first value
	movf	PORTB, w, a ; read value once again from switches
	movwf	num2, a	    ; loads WREG to num2
	rrncf	num2, f, a  ; rotate right three times, creating second number
	rrncf	num2, f, a
	rrncf	num2, a  ; final is stored in the wreg, since this will take less time to write/read
	andlw	0x07	    ; clear any other junk
	addwf	num1, a	    ; add values together
	call	bcd2sseg, 0 ; call seven segment conversion
	movwf	PORTD, a    ; loads to PORTD, which is the seven segment output
	bra	infiniteloop	;start back over
    
	; convert a BCD pattern stored in 4 lsb of WREG into 7Seg
bcd2sseg: 
	lfsr	0, SSEG; move fsr0 pointer back to start of table
	movf	PLUSW0, w, a
	return 0; WREG will have the sseg pattern upon return
	
end