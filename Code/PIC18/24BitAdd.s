#include <xc.inc>
;psect code

;lowest bit
num_Al equ 0x22
num_Am equ 0x21
num_Ah equ 0x20

;middle bit
num_Bl equ 0x32
num_Bm equ 0x31
num_Bh equ 0x30
 
;high bit (carry)
num_Cl equ 0x52
num_Cm equ 0x51
num_Ch equ 0x50

;A=F191B5
movlw 0xB5
movwf num_Al, f, a
movlw 0x91
movwf num_Am, f, a
movlw 0xF1
movwf num_Ah, f, a

;B=07A204
movlw 0x04
movwf num_Bl, f, a
movlw 0xA2
movwf num_Bm, f, a
movlw 0x07
movwf num_Bh, f, a

;clear working reg
clrf WREG

;add low bytes
movwf num_Al, w, a
addwf num_Bl, w, a

;for high bit carry
movwf num_Cl, f, a

;add medium bytes
movwf num_Am, w, a
addwfc num_Bm, w, a

;for high bit carry
movwf num_Cm, f, a

;add high bytes
movwf num_Ah, w, a
addwfc num_Bh, w, a
movwf num_Ch, f, a

end