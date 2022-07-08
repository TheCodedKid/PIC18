#include <xc.inc>
	goto start	
	
	; Lab 3 Prelab
	
	psect code
N   equ	0x00	; Number of X's
Z   equ 0x60	; Final answer is stored here
X   equ 0x50	; Starting point of Xi array


   
start:	; load location 0x50 to 0x57 with arbitrary numbers for testing
	; you should also use "Memory Fill" to test it with zeros, ones, or even random numbers
	lfsr	0, X
	movlw	1
	movwf	POSTINC0, a
	movlw	2
	movwf	POSTINC0, a
	movlw	3
	movwf	POSTINC0, a
	movlw	4
	movwf	POSTINC0, a
	movlw	5
	movwf	POSTINC0, a
	movlw	6
	movwf	POSTINC0, a
	movlw	7
	movwf	POSTINC0, a
	movlw	8
	movwf	POSTINC0, a
	
	movlw	8
	movwf	N, a ;set number of increments
	
	lfsr    1, X ;load whatever is in X to FSR1 

loop:
	call	sum8, 1 ;call sum subroutine
	decfsz	N, f, a ;decrement N until 0, then skips next instruction
	bra	loop	;loopback
	
		     ;Divide
	movlw	3
	movwf	N, a ;Dividing by 2 three times, utulizing N again because less memory usage 0-0

loop2:	call	divide, 1 ;calls divide subroutine
	decfsz	N, f, a ;decrements until 0
	bra	loop2 ;skips when 0
	bra	stop ;end of program, infinite loop hehe
    	
sum8:	
    movf    POSTINC1, w, a ;takes data at file reg FSR1 and loads to wreg
    addwf   Z, f, a ;adds the previously stored value of Z with whatever is in the wreg, then sets Z to result
    return  1 ;end of subroutine

divide:
    rrcf    Z, f, a ;rotates bits to the right by one, hence why remainder is removed because of this
    bcf	    STATUS, 0, a ;clears the carry bit
    return  1 ;end of subroutine

stop:	bra	stop
	
end
